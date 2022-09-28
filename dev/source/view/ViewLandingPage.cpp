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

#include <boost/filesystem.hpp>

// Coment this line to link to glew.
//#include <GLee.h>

#if !defined(__glee_h_)
#include <GL/glew.h>
#endif // !defined(__glee_h_)

#include <PngWrapper.h>

#include "../viewmodel/UfcCalibratorViewModel.h"
#include "../ObjectType.h"
#include "../StringResource.h"

#include "ViewLandingPage.h"

/**
*/
CViewLandingPage::CViewLandingPage()
{
    Create();
}

/**
*/
bool CViewLandingPage::Initialize()
{
    m_backgroundTexture = OpenTexture("./textures/LandingPage.png");

    if (!m_backgroundTexture)
    {
        LOG_ERROR();

        return false;
    }

    int fileIndex = 1;

    std::string fileNamePrefix = "./textures/LandingPageHelper",
        fileNameSuffix = ".png",
        fileIndexString = my::NumberToString(fileIndex);

    if (fileIndex < 10)
        fileIndexString = "0" + fileIndexString;

    std::string fileName = fileNamePrefix + fileIndexString + fileNameSuffix;

    while (boost::filesystem::exists(fileName))
    {
        boost::shared_ptr<CTexture> texture = OpenTexture(fileName);

        if (!texture)
        {
            LOG_ERROR();

            return false;
        }

        m_helperTextureArray.push_back(texture);

        ++fileIndex;

        fileIndexString = my::NumberToString(fileIndex);

        if (fileIndex < 10)
            fileIndexString = "0" + fileIndexString;

        fileName = fileNamePrefix + fileIndexString + fileNameSuffix;
    }

    return true;
}

/**
*/
void CViewLandingPage::Draw(int /*mode*/)
{
    boost::shared_ptr<CUfcCalibratorModel> ufcCalibratorModel = CUfcCalibratorViewModel::Instance().GetUfcCalibratorModel();

    if (!ufcCalibratorModel)
        return /*true*/;

    bool isFootageValid = false;

    boost::shared_ptr<CFootage> footage = ufcCalibratorModel->GetFootage();

    if (footage)
    {
        boost::shared_ptr<CMediaPlayerInterface> mediaPlayerInterface = footage->GetMediaPlayerInterface();

        if (mediaPlayerInterface &&
            mediaPlayerInterface->IsReady())
        {
            isFootageValid = true;
        }
    }

    // NO HELPERS ON 3D TAB?

    if (!isFootageValid)
        DrawBackground();

    // TODO: (11-Sep-2016) DISABLED UNTIL WE HAVE A BETTER WAY TO RENDER HINTS!
    //DrawHelper();
}

/**
*/
void CViewLandingPage::DrawBackground() const
{
    if (!m_backgroundTexture)
        return /*true*/;

    glMatrixMode(GL_PROJECTION);

    glPushMatrix();

    glLoadIdentity();

    glMatrixMode(GL_MODELVIEW);

    glPushMatrix();

    glLoadIdentity();

    glDepthMask(false);

    glDisable(GL_DEPTH_TEST);

    m_backgroundTexture->Bind(0);

    glColor4ub(255, 255, 255, 255);

    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 1.0f);
    glVertex2i(-1, -1);
    glTexCoord2f(1.0f, 1.0f);
    glVertex2i(1, -1);
    glTexCoord2f(1.0f, 0.0f);
    glVertex2i(1, 1);
    glTexCoord2f(0.0f, 0.0f);
    glVertex2i(-1, 1);

    glEnd();

    CTexture::DisableTextureUnit(0);

    glDepthMask(true);

    glMatrixMode(GL_PROJECTION);

    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);

    glPopMatrix();
}

/**
*/
void CViewLandingPage::DrawHelper() const
{
    if (!m_backgroundTexture)
        return /*true*/;

    bool isGameValid = false,
        isReady = false,
        isPlaying = false,
        //isStrikeZone = false,
        //isStrikeZoneMarkerUpdated = false,
        isCameraMarkerUpdated = false;

    boost::shared_ptr<CUfcCalibratorModel> ufcCalibratorModel = CUfcCalibratorViewModel::Instance().GetUfcCalibratorModel();

    if (ufcCalibratorModel)
    {
        boost::shared_ptr<CFootage> footage = ufcCalibratorModel->GetFootage();

        if (footage)
        {
            boost::shared_ptr<CMediaPlayerInterface> mediaPlayerInterface = footage->GetMediaPlayerInterface();

            if (mediaPlayerInterface)
            {
                if (mediaPlayerInterface->IsReady())
                    isReady = true;

                if (mediaPlayerInterface->GetState() != CPlayerInterface::STOPPED_STATE)
                    isPlaying = true;
            }
        }
    }

    boost::shared_ptr<CFootage> footage = ufcCalibratorModel->GetFootage();

    //if (footage)
    //{
    //    boost::shared_ptr<my::video::CMarker> marker = footage->GetMarker(UFC_STRING_RESOURCE_0046);

    //    if (marker)
    //        isStrikeZoneMarkerUpdated |= marker->HasChanged();

    //    marker = footage->GetMarker(UFC_STRING_RESOURCE_0047);

    //    if (marker)
    //        isStrikeZoneMarkerUpdated |= marker->HasChanged();

    //    marker = footage->GetMarker(UFC_STRING_RESOURCE_0034);

    //    if (marker)
    //        isCameraMarkerUpdated |= marker->HasChanged();

    //    marker = footage->GetMarker(UFC_STRING_RESOURCE_0035);

    //    if (marker)
    //        isCameraMarkerUpdated |= marker->HasChanged();
    //}

    // WAITING FOR GAME SELECTION?

    HEALTH_CHECK(m_helperTextureArray.size() < 1, /*false*/);
    
    if (!isGameValid)
        DrawHelper(m_helperTextureArray[0], -0.8f, 0.95f);

    // WAITING FOR PITCH SELECTION?

    HEALTH_CHECK(m_helperTextureArray.size() < 2, /*false*/);

    // (BEGIN OF) TESTING: (11-Jun-2016) DISABLED BY NOW
    //// (OPEN FROM FILE SUGGESTION)

    //HEALTH_CHECK(m_helperTextureArray.size() < 3, /*false*/);

    //DrawHelper(m_helperTextureArray[2], -0.95f, 0.4f);
    // (END OF) TESTING: (11-Jun-2016) DISABLED BY NOW

    // THE FOOTAGE IS VALID BUT THE "PLAY" BUTTON SHOULD BE PRESSED (ASYNCHRONOUS PLAYER)?

    HEALTH_CHECK(m_helperTextureArray.size() < 4, /*false*/);

    //// STRIKE ZONE INTERACTION (STRIKE ZONE VIEWER)

    //HEALTH_CHECK(m_helperTextureArray.size() < 5, /*false*/);

    //if (isGameValid &&
    //    isPitchValid &&
    //    isReady &&
    //    isStrikeZone &&
    //    !isStrikeZoneMarkerUpdated)
    //{
    //    DrawHelper(m_helperTextureArray[4], 0.0f, 0.95f);
    //}

    //// CAMERA INTERACTION (STRIKE ZONE VIEWER)

    //HEALTH_CHECK(m_helperTextureArray.size() < 6, /*false*/);

    //if (isGameValid &&
    //    isPitchValid &&
    //    isReady &&
    //    isStrikeZone &&
    //    !isCameraMarkerUpdated)
    //{
    //    DrawHelper(m_helperTextureArray[5], -0.95f, 0.4f);
    //}
}

/**
*/
void CViewLandingPage::DrawHelper(boost::shared_ptr<CTexture> texture, float horizontalPosition, float verticalPosition) const
{
    if (!m_backgroundTexture)
        return /*true*/;

    if (!texture)
        return /*true*/;

    float width = (float)m_backgroundTexture->GetWidth(),
        height = (float)m_backgroundTexture->GetHeight(),
        ratio = height / width;

    int viewport[4] = { 0 };

    glGetIntegerv(GL_VIEWPORT, viewport);

    float scale = 0.75f,
        viewportRatio = (float)(viewport[3] - viewport[1]) / (viewport[2] - viewport[0]);

    float textureHeight = scale * ((float)texture->GetHeight() / height),
        textureWidth = (viewportRatio / ratio) * scale * ((float)texture->GetWidth() / width);

    glMatrixMode(GL_PROJECTION);

    glPushMatrix();

    glLoadIdentity();

    glMatrixMode(GL_MODELVIEW);

    glPushMatrix();

    glLoadIdentity();

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_BLEND);

    glDisable(GL_DEPTH_TEST);

    texture->Bind(0);

    glColor4ub(255, 255, 255, 255);

    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f);
    glVertex2f(horizontalPosition, verticalPosition);
    glTexCoord2f(1.0f, 0.0f);
    glVertex2f(horizontalPosition + textureWidth, verticalPosition);
    glTexCoord2f(1.0f, 1.0f);
    glVertex2f(horizontalPosition + textureWidth, verticalPosition - textureHeight);
    glTexCoord2f(0.0f, 1.0f);
    glVertex2f(horizontalPosition, verticalPosition - textureHeight);

    glEnd();

    CTexture::DisableTextureUnit(0);

    glDisable(GL_BLEND);

    glMatrixMode(GL_PROJECTION);

    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);

    glPopMatrix();
}

/**
*/
boost::shared_ptr<CTexture> CViewLandingPage::OpenTexture(std::string fileName) const
{
    boost::shared_ptr<CTexture> emptyObject;

    CPngWrapper pngWrapper;

    if (!pngWrapper.Open(fileName))
    {
        LOG_ERROR();

        return emptyObject;
    }

    boost::shared_ptr<CTexture> texture(new CTexture);

    HEALTH_CHECK(!texture, emptyObject);

    if (!texture->Initialize(CTexture::TEXTURE_2D, CTexture::RGBA))
    {
        LOG_ERROR();

        return emptyObject;
    }

    unsigned int format = 0;

    switch (pngWrapper.GetChannelCount()) {
    case 3:
        format = CTexture::RGB;
        break;
    case 4:
        format = CTexture::RGBA;
        break;
    default:
        LOG_ERROR();

        return emptyObject;
    }

    if (!texture->SetImage(pngWrapper.GetWidth(), pngWrapper.GetHeight(), 0, format, CTexture::UNSIGNED_BYTE, pngWrapper.GetBuffer().get()))
    {
        LOG_ERROR();

        return emptyObject;
    }

    texture->SetParameter(CTexture::MIN_FILTER, CTexture::LINEAR);
    texture->SetParameter(CTexture::MAG_FILTER, CTexture::LINEAR);
    texture->SetParameter(CTexture::WRAP_S, CTexture::REPEAT);
    texture->SetParameter(CTexture::WRAP_T, CTexture::REPEAT);

    if (!texture->BuildMipmaps())
    {
        LOG_ERROR();

        return emptyObject;
    }

    return texture;
}

/**
*/
void CViewLandingPage::Create()
{
    m_backgroundTexture.reset();

    m_helperTextureArray.clear();
}

