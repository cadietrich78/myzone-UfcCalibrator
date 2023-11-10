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

#if !defined(FOOTAGE_INCLUDE)
#define FOOTAGE_INCLUDE

#include <boost/utility.hpp>
#include <opencv2/highgui.hpp>

#include <Texture.h>
#include <Timestamp.h>
#include <TripodSceneryLayout.h>

#include "../calibration/PinholeCameraCalibration.h"
#include "../video/MediaPlayerInterface.h"

class CFootage
    : public CObject,
    public boost::noncopyable
{
public:
    CFootage();
    ~CFootage();

    std::string GetUrl() const;
    bool OpenUrl(std::string url);

    std::string GetName() const;
    void SetName(std::string name);
    
    my::CTimestamp GetTimestamp() const;
    void SetTimestamp(my::CTimestamp timestamp);

    boost::shared_ptr<CMediaPlayerInterface> GetMediaPlayerInterface();

    boost::shared_ptr<CTexture> GetFrameAsTexture();

    cv::Mat GetFrame();

    boost::shared_ptr<CPinholeCamera2> GetPinholeCamera();
    bool SetPinholeCamera(boost::shared_ptr<CPinholeCamera2> pinholeCamera);

    boost::shared_ptr<my::video::CMarker> GetMarker(const std::string& markerName);
    boost::shared_ptr<my::video::CMarker> GetMarker(int markerId);

    bool RemoveMarkerById(CObject::IdType id);

    bool MoveMarkerById(CObject::IdType markerId, double dx, double dy);

    bool AddExtrinsicCalibrationPointMarker(std::string name, std::string icon, my::CVector3<double> worldCoord, my::CVector2<double> screenCoord, my::video::CMarker::CONSTRAINT constraint = my::video::CMarker::UNCONSTRAINED, bool isEnabled = true);

    bool AddUserDefinedMarker(boost::shared_ptr<my::video::CMarker> marker);

    std::vector<boost::shared_ptr<my::video::CMarker> > GetExtrinsicCalibrationMarkerArray() const;
    std::vector<boost::shared_ptr<my::video::CMarker> > GetUserDefinedMarkerArray() const;

    boost::shared_ptr<CMarkerGroup> GetExtrinsicCalibrationMarkerGroup() const;

    bool IsValid() const;
    void SetIsValid(bool isValid);

private:
    bool InitializeStandByFrameTexture();

    bool UpdateFrameTexture();

    bool UpdateTexture(boost::shared_ptr<CTexture>& texture, int width, int height, unsigned int format, unsigned int type, boost::shared_array<unsigned char> buffer) const;

    void Create();
    void Destroy();

protected:
    std::string m_url;
    std::string m_name;

    my::CTimestamp m_timestamp;

    boost::shared_ptr<CMediaPlayerInterface> m_mediaPlayerInterface;

    boost::shared_ptr<CTexture> m_frameTexture;
    boost::shared_ptr<CTexture> m_standByFrameTexture;

    cv::Mat m_frame;

    boost::shared_ptr<CPinholeCamera2> m_pinholeCamera;

    boost::shared_ptr<CMarkerGroup> m_extrinsicCalibrationMarkerGroup;

    std::vector<boost::shared_ptr<my::video::CMarker> > m_userDefinedMarkerArray;

    my::int64 m_currentPositionInMilliseconds;

    bool m_isValid;
};

#endif // #if !defined(FOOTAGE_INCLUDE)
