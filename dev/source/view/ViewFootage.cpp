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

#include <iostream>

// Coment this line to link to glew.
//#include <GLee.h>

#if !defined(__glee_h_)
#include <GL/glew.h>
#endif // !defined(__glee_h_)

#include <QGLWidget>
#include <QFont>

#include <UnitConversion.h>
#include <GlHelper.h>

#include "../ObjectType.h"
#include "../viewmodel/UfcCalibratorViewModel.h"
#include "../StringResource.h"

#include "ViewFootage.h"

/**
*/
CViewFootage::CViewFootage()
{
    Create();
}

/**
*/
void CViewFootage::Draw(int /*mode*/)
{
    // TRICKY: (19-Nov-2016) NOW THE FOOTAGE IS ALWAYS ENABLED!

    boost::shared_ptr<CUfcCalibratorModel> ufcCalibratorModel = CUfcCalibratorViewModel::Instance().GetUfcCalibratorModel();

    if (!ufcCalibratorModel)
        return /*true*/;

    boost::shared_ptr<CFootage> footage = ufcCalibratorModel->GetFootage();

    if (!footage)
        return /*true*/;

    DrawVideo();

    DrawElapsedTime();
}

/**
*/
void CViewFootage::DrawVideo() const
{
    boost::shared_ptr<CUfcCalibratorModel> ufcCalibratorModel = CUfcCalibratorViewModel::Instance().GetUfcCalibratorModel();

    if (!ufcCalibratorModel)
        return /*true*/;

    boost::shared_ptr<CFootage> footage = ufcCalibratorModel->GetFootage();

    if (!footage.get())
        return /*true*/;

    boost::shared_ptr<CTexture> frameTexture = footage->GetFrameAsTexture();

    if (!frameTexture)
    {
        // Annoying message, there may be no video yet.
        //LOG_ERROR();

        return /*false*/;
    }

    GlHelper::PUSH_MATRIX_HELPER pushMatrixHelper(true);

    glDepthMask(false);

    glDisable(GL_DEPTH_TEST);

    frameTexture->Bind(0);

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
}

/**
*/
void CViewFootage::DrawElapsedTime() const
{
    if (!m_renderingContext)
        return /*false*/;

    boost::shared_ptr<CUfcCalibratorModel> ufcCalibratorModel = CUfcCalibratorViewModel::Instance().GetUfcCalibratorModel();

    if (!ufcCalibratorModel)
        return /*true*/;

    boost::shared_ptr<CFootage> footage = ufcCalibratorModel->GetFootage();

    if (!footage)
        return /*true*/;

    boost::shared_ptr<CMediaPlayerInterface> mediaPlayerInterface = footage->GetMediaPlayerInterface();

    if (mediaPlayerInterface)
    {
        double positionInSeconds = UnitConversion::Millisecond2Second((double)mediaPlayerInterface->GetPositionInMilliseconds()),
            durationInSeconds = UnitConversion::Millisecond2Second((double)mediaPlayerInterface->GetDurationInMilliseconds());

        if (!MyMath::IsZero(durationInSeconds))
        {
            std::string message = my::NumberToString(positionInSeconds, 3)
                + " of "
                + my::NumberToString(durationInSeconds, 3)
                + " seconds";

            int textSize = CUfcCalibratorViewModel::Instance().GetAttribute<int>(GUI_TEXT_SIZE_ITEM);

            HEALTH_CHECK(my::IsNull(textSize), /*false*/);

            // (BEGIN OF) TESTING: (25-Aug-2016) FONT-DEPENDENT SIZE
            QFont font;

            font.setFamily("Arial");
            font.setWeight(QFont::Bold);
            font.setPointSize(textSize);

            QFontMetrics fontMetrics(font);

            //// [0, width (GL window)) -> [0, 2]
            //double messageWidth = 2.0 * ((double)fontMetrics.width(message.c_str()) / ((QGLWidget*)m_renderingContext)->width()),
            //    messageHeight = 2.0 * ((double)fontMetrics.height() / ((QGLWidget*)m_renderingContext)->height());
            //// (END OF) TESTING: (25-Aug-2016) FONT-DEPENDENT SIZE

            // (BEGIN OF) TESTING: (??-???-????) DISABLED BECAUSE THERE ARE NO TIMESTAMPED VIDEOS YET
            //boost::shared_ptr<mlb::io::CGameplay> gameplay = ufcCalibratorModel->GetGameplay();

            //if (gameplay)
            //{
            //    my::CTimestamp timestamp(gameplay->GetStartTime());

            //    timestamp.AddSeconds(positionInSeconds);

            //    elapsedTime += " ("
            //        + my::NumberToString(timestamp.ToMillisecondsFromEpoch())
            //        + ")";
            //}
            // (END OF) TESTING: (??-???-????) DISABLED BECAUSE THERE ARE NO TIMESTAMPED VIDEOS YET

            glColor3ub(255, 255, 255);

            DrawString(-0.95, -0.95, textSize, message);
        }
    }
}

/**
*/
void CViewFootage::DrawString(double xScreenCoord, double yScreenCoord, int size, std::string text) const
{
    HEALTH_CHECK(m_renderingContext == 0, /*false*/);
    HEALTH_CHECK(text.empty(), /*false*/);

    int width = ((QGLWidget*)m_renderingContext)->width(),
        height = ((QGLWidget*)m_renderingContext)->height();

    QFont titleFont;

    titleFont.setFamily("Arial");
    titleFont.setWeight(QFont::Bold);
    titleFont.setPointSize(size);

    GLdouble x = 0.5 * (xScreenCoord + 1.0) * width,
        y = 0.5 * (yScreenCoord + 1.0) * height;

    ((QGLWidget*)m_renderingContext)->renderText((int)x, height - (int)y, text.c_str(), titleFont);
}

/**
*/
void CViewFootage::Create()
{
}

