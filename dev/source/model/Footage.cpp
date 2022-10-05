/*
* COPYRIGHT NOTICE, DISCLAIMER, and LICENSE:
*
* 
* For the purposes of this copyright and license, "Contributing Authors"
* is defined as the following set of individuals:
*
*    Carlos Augusto Dietrich (cadietrich@gmail.com)
*
* This library is supplied "AS IS".  The Contributing Authors disclaim 
* all warranties, expressed or implied, including, without limitation, 
* the warranties of merchantability and of fitness for any purpose. 
* The Contributing Authors assume no liability for direct, indirect, 
* incidental, special, exemplary, or consequential damages, which may 
* result from the use of the this library, even if advised of the 
* possibility of such damage.
*
* Permission is hereby granted to use, copy, modify, and distribute this
* source code, or portions hereof, for any purpose, without fee, subject
* to the following restrictions:
*
* 1. The origin of this source code must not be misrepresented.
*
* 2. Altered versions must be plainly marked as such and must not be 
*    misrepresented as being the original source.
*
* 3. This Copyright notice may not be removed or altered from any source 
*    or altered source distribution.
*
* The Contributing Authors specifically permit, without fee, and 
* encourage the use of this source code as a component in commercial 
* products. If you use this source code in a product, acknowledgment 
* is not required but would be appreciated.
*
* 
* "Software is a process, it's never finished, it's always evolving. 
* That's its nature. We know our software sucks. But it's shipping! 
* Next time we'll do better, but even then it will be shitty. 
* The only software that's perfect is one you're dreaming about. 
* Real software crashes, loses data, is hard to learn and hard to use. 
* But it's a process. We'll make it less shitty. Just watch!"
*/

#include <map>

#if !defined(__GNUC__)
#include <windows.h>
#endif // !defined(__GNUC__)

#if !defined(__glee_h_)
#include <GL/glew.h>

#if defined(_WIN32)
#include <GL/wglew.h>
#else // defined(_WIN32)
#if defined(__APPLE__)
#include <GL/glew.h>
//#else
//#include <GL/glxew.h>
#endif
#endif // defined(_WIN32)
#endif // !defined(__glee_h_)

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#endif

#include <Common.h>
#include <PngWrapper.h>
//// DEBUG ONLY! (04-Oct-2016) POOR PERFORMANCE OF THE TOOL
//#include <Timer.h>
#include <GLHelper.h>

#include "../video/ImageMediaPlayer.h"
#include "../calibration/UfcMarkerArray.h"

#include "Footage.h"

CFootage::CFootage()
    : CObject(),
    m_timestamp(my::Null<my::CTimestamp>())
{
    Create();
}

CFootage::~CFootage()
{
    Destroy();
}

std::string CFootage::GetUrl() const
{
    return m_url;
}

bool CFootage::OpenUrl(std::string url)
{
    HEALTH_CHECK(url.empty(), false);

    m_url = url;

    SetName(my::GetFileName(m_url));

    std::string extension = my::GetFileExtension(m_url);

    if ((extension == "png") ||
        (extension == "jpg"))
    {
        m_mediaPlayerInterface.reset(new CImageMediaPlayer);

        HEALTH_CHECK(!m_mediaPlayerInterface, false);

        if (!m_mediaPlayerInterface->OpenUrl(m_url))
        {
            LOG_ERROR();

            return false;
        }

        // (BEGIN OF) BUG: (29-Sep-2022) MAKE SURE THE TEXTURE IS UPDATED!
        m_frameTexture.reset();

        m_currentPositionInMilliseconds = my::Null<my::int64>();
        // (END OF) BUG: (29-Sep-2022) MAKE SURE THE TEXTURE IS UPDATED!
    }
    else
    {
        LOG_ERROR();

        return false;
    }

    return true;
}

std::string CFootage::GetName() const
{
    return m_name;
}

void CFootage::SetName(std::string name)
{
    m_name = name;
}

my::CTimestamp CFootage::GetTimestamp() const
{
    return m_timestamp;
}

void CFootage::SetTimestamp(my::CTimestamp timestamp)
{
    m_timestamp = timestamp;
}

boost::shared_ptr<CMediaPlayerInterface> CFootage::GetMediaPlayerInterface()
{
    return m_mediaPlayerInterface;
}

// BUG: (21-Feb-2017) THIS METHOD MAKES CALLS TO THE GRAPHICS LIBRARY - IT CAN BE CALLED ONLY BY THE MAIN THREAD
boost::shared_ptr<CTexture> CFootage::GetFrameAsTexture()
{
    if (!UpdateFrameTexture())
    {
        // Annoying message (asynchronous media player?)
        //LOG_ERROR();

        return m_standByFrameTexture;
    }

    return m_frameTexture;
}

// BUG: (21-Feb-2017) THIS METHOD MAKES CALLS TO THE GRAPHICS LIBRARY - IT CAN BE CALLED ONLY BY THE MAIN THREAD
boost::shared_ptr<my::image::CImage> CFootage::GetFrameImage()
{
    boost::shared_ptr<my::image::CImage> emptyObject;

    if (!m_frameImage)
        return emptyObject;

    return m_frameImage;
}

boost::shared_ptr<CPinholeCamera2> CFootage::GetPinholeCamera()
{
    return m_pinholeCamera;
}

bool CFootage::SetPinholeCamera(boost::shared_ptr<CPinholeCamera2> pinholeCamera)
{
    m_pinholeCamera.reset(new CPinholeCamera2(*pinholeCamera));

    HEALTH_CHECK(!m_pinholeCamera, false);

    return true;
}

boost::shared_ptr<my::video::CMarker> CFootage::GetMarker(const std::string& markerName)
{
    std::vector<boost::shared_ptr<my::video::CMarker> > markerArray;

    // EXTRINSIC CALIBRATION MARKERS
    if (m_extrinsicCalibrationMarkerGroup)
    {
        std::vector<boost::shared_ptr<my::video::CMarker> > extrinsicMarkerArray = m_extrinsicCalibrationMarkerGroup->GetMarkerArray();

        markerArray.insert(markerArray.end(), extrinsicMarkerArray.begin(), extrinsicMarkerArray.end());
    }

    // USER-DEFINED MARKERS
    markerArray.insert(markerArray.end(), m_userDefinedMarkerArray.begin(), m_userDefinedMarkerArray.end());

    for (std::vector<boost::shared_ptr<my::video::CMarker> >::const_iterator markerIterator = markerArray.begin(); markerIterator != markerArray.end(); ++markerIterator)
    {
        if ((*markerIterator)->GetName() == markerName)
            return (*markerIterator);
    }

    return boost::shared_ptr<my::video::CMarker>();
}

boost::shared_ptr<my::video::CMarker> CFootage::GetMarker(int markerId)
{
    std::vector<boost::shared_ptr<my::video::CMarker> > markerArray;

    // EXTRINSIC CALIBRATION MARKERS
    if (m_extrinsicCalibrationMarkerGroup)
    {
        std::vector<boost::shared_ptr<my::video::CMarker> > extrinsicMarkerArray = m_extrinsicCalibrationMarkerGroup->GetMarkerArray();

        markerArray.insert(markerArray.end(), extrinsicMarkerArray.begin(), extrinsicMarkerArray.end());
    }

    // USER-DEFINED MARKERS
    markerArray.insert(markerArray.end(), m_userDefinedMarkerArray.begin(), m_userDefinedMarkerArray.end());

    for (std::vector<boost::shared_ptr<my::video::CMarker> >::const_iterator markerIterator = markerArray.begin(); markerIterator != markerArray.end(); ++markerIterator)
    {
        if ((*markerIterator)->GetId() == markerId)
            return (*markerIterator);
    }

    return boost::shared_ptr<my::video::CMarker>();
}

// INTRINSIC MARKERS AND USER-DEFINED MARKERS (STRIKE ZONE (TOP AND BOTTOM) AND BALL MARKERS
bool CFootage::RemoveMarkerById(CObject::IdType id)
{
    // TODO: (??-???-????) ADD EXTRINSIC CALIBRATION MARKERS?

    // USER-DEFINED MARKERS
    std::vector<boost::shared_ptr<my::video::CMarker> >::const_iterator userDefinedMarkerIterator = m_userDefinedMarkerArray.begin();
    
    while (userDefinedMarkerIterator != m_userDefinedMarkerArray.end())
    {
        if ((*userDefinedMarkerIterator)->GetId() == id)
            userDefinedMarkerIterator = m_userDefinedMarkerArray.erase(userDefinedMarkerIterator);
        else
            ++userDefinedMarkerIterator;
    }

    return true;
}

bool CFootage::MoveMarkerById(CObject::IdType markerId, double dx, double dy)
{ 
	boost::shared_ptr<my::video::CMarker> marker = GetMarker(markerId);

    if (marker)
    {
        double _2dMarkerCoordPointer[3] = { 0 };
        
        MyMath::Assign3(marker->GetScreenCoord(), _2dMarkerCoordPointer);

        // TRICKY: (24-Nov-2015) MARKERS MAY HAVE NO SCREEN COORDINATES UNTIL THE USER CHANGE THEM (THE SCREEN COORDINATES WOULD BE COMPUTED FROM THE 3D COORDINATES UNTIL THAT). 

        if (my::IsNull(_2dMarkerCoordPointer[0]) ||
            my::IsNull(_2dMarkerCoordPointer[1]))
        {
            HEALTH_CHECK(!m_pinholeCamera, false);

            const double *_3dMarkerCoord = marker->GetWorldCoord();

            HEALTH_CHECK(!_3dMarkerCoord, false);

            my::CVector3<double> _2dMarkerCoord = m_pinholeCamera->ProjectVertex(my::CVector3<double>(_3dMarkerCoord[0], _3dMarkerCoord[1], _3dMarkerCoord[2]));

            _2dMarkerCoordPointer[0] = _2dMarkerCoord.x();
            _2dMarkerCoordPointer[1] = _2dMarkerCoord.y();
            _2dMarkerCoordPointer[2] = _2dMarkerCoord.z();

            const int *viewport = m_pinholeCamera->GetViewport();

            HEALTH_CHECK(!viewport, false);

            _2dMarkerCoordPointer[0] = 2.0 * ((double)(_2dMarkerCoordPointer[0] - viewport[0]) / (viewport[2] - viewport[0])) - 1.0;
            _2dMarkerCoordPointer[1] = 2.0 * ((double)(_2dMarkerCoordPointer[1] - viewport[1]) / (viewport[3] - viewport[1])) - 1.0;
        }

        marker->SetScreenCoord(_2dMarkerCoordPointer[0] + dx, _2dMarkerCoordPointer[1] + dy);

        return true;
    }

    return false;
}

// TODO: (??-Dec-2016) SHORTCUT TO ADD AN EXTRINSIC CALIBRATION MARKER, SHOULD BE REVIEWED
bool CFootage::AddExtrinsicCalibrationPointMarker(std::string name, std::string icon, my::CVector3<double> worldCoord, my::CVector2<double> screenCoord, my::video::CMarker::CONSTRAINT constraint, bool isEnabled)
{
    LOG_ERROR();

    return false;

    //if (!m_extrinsicCalibrationMarkerGroup)
    //{
    //    LOG_ERROR();

    //    return false;
    //}

    //boost::shared_ptr<CCenterFieldMarkerGroup> centerFieldMarkerGroup = boost::dynamic_pointer_cast<CCenterFieldMarkerGroup>(m_extrinsicCalibrationMarkerGroup);

    //if (!centerFieldMarkerGroup)
    //{
    //    LOG_ERROR();

    //    return false;
    //}

    //if (!centerFieldMarkerGroup->AddPointMarker(name, icon, worldCoord, screenCoord, constraint, isEnabled))
    //{
    //    LOG_ERROR();

    //    return false;
    //}

    //return true;
}

bool CFootage::AddUserDefinedMarker(boost::shared_ptr<my::video::CMarker> marker)
{
    HEALTH_CHECK(!marker, false);

    m_userDefinedMarkerArray.push_back(marker);

    return true;
}

bool CFootage::Reset()
{
    try
    {
        m_extrinsicCalibrationMarkerGroup.reset(new CUfcMarkerArray);
    }
    catch (std::exception& e)
    {
        LOG_MESSAGE(e.what());

        return false;
    }

    return true;
}

std::vector<boost::shared_ptr<my::video::CMarker> > CFootage::GetExtrinsicCalibrationMarkerArray() const
{
    HEALTH_CHECK(!m_extrinsicCalibrationMarkerGroup, std::vector<boost::shared_ptr<my::video::CMarker> >());

    return m_extrinsicCalibrationMarkerGroup->GetMarkerArray();
}

std::vector<boost::shared_ptr<my::video::CMarker> > CFootage::GetUserDefinedMarkerArray() const
{
    return m_userDefinedMarkerArray;
}

boost::shared_ptr<CMarkerGroup> CFootage::GetExtrinsicCalibrationMarkerGroup() const
{
    return m_extrinsicCalibrationMarkerGroup;
}

bool CFootage::IsValid() const
{
    return m_mediaPlayerInterface &&
        m_isValid;
}

void CFootage::SetIsValid(bool isValid)
{
    m_isValid = isValid;
}

bool CFootage::InitializeStandByFrameTexture()
{
    CPngWrapper pngWrapper;

    if (!pngWrapper.Open("./textures/NoData.png"))
    {
        LOG_ERROR();

        return false;
    }

    if (pngWrapper.GetChannelCount() != 3)
    {
        LOG_MESSAGE("Unsupported number of channels (" + boost::lexical_cast<std::string>(pngWrapper.GetChannelCount()) + ").");

        return false;
    }

    if (!UpdateTexture(m_standByFrameTexture, pngWrapper.GetWidth(), pngWrapper.GetHeight(), CTexture::RGB, CTexture::UNSIGNED_BYTE, pngWrapper.GetBuffer()))
    {
        LOG_ERROR();

        return false;
    }

    return true;
}

bool CFootage::UpdateFrameTexture()
{
    // BUG: (01-Jul-2016) 
    HEALTH_CHECK(!m_mediaPlayerInterface, false);

    if (m_currentPositionInMilliseconds == m_mediaPlayerInterface->GetPositionInMilliseconds())
        return true;

    boost::shared_array<unsigned char> fileBuffer;
    int width = 0,
        height = 0;
    unsigned int format,
        type;

    if (!m_mediaPlayerInterface->GetFrame(fileBuffer, width, height, format, type))
    {
        // ANNOYING MESSAGE (ASYNCHRONOUS MEDIA PLAYERS WAIT FOR THE 'PLAY' SIGNAL, SO THE FRAME MAY BE NOT AVAILABLE BEFORE THAT)
        // ANNOYING MESSAGE (THE ASYNCHRONOUS LOADING OF VIDEO FRAMES MAY RESULT ON FRAMES THAT ARE NOT AVAILABLE RIGHT AFTER OPENING THE VIDEO)
        //LOG_ERROR();

        return false;
    }

    m_currentPositionInMilliseconds = m_mediaPlayerInterface->GetPositionInMilliseconds();
    
    // (BEGIN OF) TRICKY: (21-Nov-2015) DEFERRED PINHOLE CAMERA INITIALIZATION - THE FRAME DIMENSIONS WHERE UNKNOWN UNTIL NOW
    if (m_pinholeCamera &&
        !m_frameTexture)
    {
        const int *viewport = m_pinholeCamera->GetViewport();

        HEALTH_CHECK(!viewport, false);

        int width = m_mediaPlayerInterface->GetWidth(),
            height = m_mediaPlayerInterface->GetHeight();

        if ((width != my::Null<int>()) &&
            (height != my::Null<int>()) &&
            (width != (viewport[2] - viewport[0])) &&
            (height != (viewport[3] - viewport[1])))
        {
            m_pinholeCamera->SetViewport(0, 0, width, height);
        }
    }
    // (END OF) TRICKY: (21-Nov-2015) DEFERRED PINHOLE CAMERA INITIALIZATION - THE FRAME DIMENSIONS WHERE UNKNOWN UNTIL NOW

    if (!UpdateTexture(m_frameTexture, width, height, format, type, fileBuffer))
	{
		LOG_ERROR();
        
		return false;
	}

    if (!m_frameImage)
    {
        m_frameImage.reset(new my::image::CImage);

        HEALTH_CHECK(!m_frameImage, false);
    }

    int channelCount = 0;

    if (format == CTexture::RGB)
        channelCount = 3;
    else if (format == CTexture::RGBA)
        channelCount = 4;

    // UPDATE ACTIVE MARKERS
    if (!m_frameImage->Set(width, height, channelCount, fileBuffer))
    {
        LOG_ERROR();

        return false;
    }

    m_extrinsicCalibrationMarkerGroup->SetImage(m_frameImage);

    return true;
}

// BUG: (21-Feb-2017) THIS METHOD MAKES CALLS TO THE GRAPHICS LIBRARY - IT CAN BE CALLED ONLY BY THE MAIN THREAD
bool CFootage::UpdateTexture(boost::shared_ptr<CTexture>& texture, int width, int height, unsigned int format, unsigned int type, boost::shared_array<unsigned char> buffer) const
{
    if (!texture)
    {
        texture.reset(new CTexture);

        HEALTH_CHECK(!texture, false);

        // BUG: (21-Feb-2017) ?????
        if (!GlHelper::IsGlOk())
        {
            // Annoying message
            //LOG_ERROR();

            return false;
        }

        if (!texture->Initialize(CTexture::TEXTURE_2D, CTexture::RGBA))
	    {
		    LOG_ERROR();

		    return false;
	    }

        if (!texture->SetImage(4096, 2048, 0, CTexture::LUMINANCE, CTexture::UNSIGNED_BYTE, 0))
	    {
		    LOG_ERROR();

		    return false;
	    }

        texture->SetParameter(CTexture::MIN_FILTER, CTexture::LINEAR);
        texture->SetParameter(CTexture::MAG_FILTER, CTexture::LINEAR);
        texture->SetParameter(CTexture::WRAP_S, CTexture::REPEAT);
        texture->SetParameter(CTexture::WRAP_T, CTexture::REPEAT);
    }

    if (!texture->SetImage(width, height, 0, format, type, buffer.get()))
	{
		LOG_ERROR();

		return false;
	}

    return true;
}

/**
*/
void CFootage::Create()
{
    // TODO: (03-Jan-2016) REVIEW
    m_url.clear();
    m_name.clear();
    // INITIALIZED AT THE CONSTRUCTOR
    // m_timestamp
    m_mediaPlayerInterface.reset();
    m_frameTexture.reset();
    m_pinholeCamera.reset();

    if (!Reset())
    {
        LOG_ERROR();

        return /*false*/;
    }

    if (!InitializeStandByFrameTexture())
    {
        LOG_ERROR();

        return /*false*/;
    }

    m_userDefinedMarkerArray.clear();
    m_currentPositionInMilliseconds = my::Null<my::int64>();

    m_isValid = true;
}

/**
*/
void CFootage::Destroy()
{
    m_url = my::Null<std::string>();
    m_name = my::Null<std::string>();
    m_timestamp = my::Null<my::CTimestamp>();
    m_mediaPlayerInterface.reset();
    m_frameTexture.reset();
    m_standByFrameTexture.reset();
    m_frameImage.reset();
    m_pinholeCamera.reset();
    m_extrinsicCalibrationMarkerGroup.reset();
    m_userDefinedMarkerArray.clear();
    m_currentPositionInMilliseconds = my::Null<my::int64>();
    m_isValid = false;
}

