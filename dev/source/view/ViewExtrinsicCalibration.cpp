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

// Coment this line to link to glew.
//#include <GLee.h>

#if !defined(__glee_h_)
#include <GL/glew.h>

#if defined(_WIN32)
#include <GL/wglew.h>
#else // defined(_WIN32)
#include <GL/glxew.h>
#endif // defined(_WIN32)
#endif // !defined(__glee_h_)

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <SelectionMechanism.h>
#include <SelectableObject.h>
#include <GLHelper.h>
#include <UnitConversion.h>
#include <PngWrapper.h>

// (BEGIN OF) BUG: (02-Dec-2015 Qt5 conflict with X11/Xlib.h
#if defined(__APPLE__)
#undef CursorShape
#undef Bool
#undef Status
#undef Unsorted    
#undef None        
#undef KeyPress   
#undef KeyRelease 
#undef FocusIn    
#undef FocusOut   
#undef FontChange 
#undef Expose    
#endif //#if defined(__APPLE__)
// (END OF) BUG: (02-Dec-2015 Qt5 conflict with X11/Xlib.h

#include <QGLWidget>
#include <QFont>

// (BEGIN OF) BUG: (02-Dec-2015 Qt5 conflict with X11/Xlib.h
#if defined(__APPLE__)
#if !defined(CursorShape)
#define CursorShape 0
#endif //#if !defined(CursorShape)
#if !defined(Bool)
#define Bool        int
#endif //#if !defined(Bool)
#if !defined(Status)
#define Status      int
#endif //#if !defined(Status)
#if !defined(Unsorted)
#define Unsorted    0
#endif //#if !defined(Unsorted)
#if !defined(None)
#define None        0L 
#endif //#if !defined(None)
#if !defined(KeyPress)
#define KeyPress    2
#endif //#if !defined(KeyPress)
#if !defined(KeyRelease)
#define KeyRelease  3
#endif //#if !defined(KeyRelease)
#if !defined(FocusIn)
#define FocusIn     9
#endif //#if !defined(FocusIn)
#if !defined(FocusOut)
#define FocusOut    10
#endif //#if !defined(FocusOut)
#if !defined(FontChange)
#define FontChange  255
#endif //#if !defined(FontChange)
#if !defined(Expose)
#define Expose      12
#endif //#if !defined(Expose)
#endif //#if defined(__APPLE__)
// (END OF) BUG: (02-Dec-2015 Qt5 conflict with X11/Xlib.h

#include "../ObjectType.h"
#include "../viewmodel/UfcCalibratorViewModel.h"
#include "../StringResource.h"

#include "ViewExtrinsicCalibration.h"

CViewExtrinsicCalibration::CViewExtrinsicCalibration()
{
    Create();
}

void CViewExtrinsicCalibration::Draw(int /*mode*/)
{
    if (!CUfcCalibratorViewModel::Instance().GetAttribute<bool>(EXTRINSIC_CALIBRATION_EDITING))
        return /*true*/;

    boost::shared_ptr<CUfcCalibratorModel> ufcCalibratorModel = CUfcCalibratorViewModel::Instance().GetUfcCalibratorModel();

    if (!ufcCalibratorModel)
        return /*true*/;

    boost::shared_ptr<CFootage> footage = ufcCalibratorModel->GetFootage();

    if (!footage)
        return /*true*/;

    // TRICKY: (19-Nov-2016) NOW THE FOOTAGE IS ALWAYS ENABLED!

    glMatrixMode(GL_PROJECTION);

    glPushMatrix();

    glMatrixMode(GL_MODELVIEW);

    glPushMatrix();

    boost::shared_ptr<CPinholeCamera2> pinholeCamera = footage->GetPinholeCamera();

    if (pinholeCamera)
    {
        pinholeCamera->ApplyTransform();

        // (BEGIN OF) TESTING: (18-Sep-2019) APPLY TRANSFORM!
        glMatrixMode(GL_PROJECTION);

        glLoadMatrixT(pinholeCamera->GetProjectionMatrix());

        glMatrixMode(GL_MODELVIEW);

        glLoadMatrixT(pinholeCamera->GetViewMatrix());
        // (END OF) TESTING: (18-Sep-2019) APPLY TRANSFORM!
    }

    // FIELD LAYOUT

    DrawFieldLayout();

    // TRICKY: (19-Nov-2016) NOW THE FOOTAGE IS ALWAYS ENABLED!

    glMatrixMode(GL_PROJECTION);

    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);

    glPopMatrix();

    DrawCalibrationQuality();

    // MARKERS

    std::vector<boost::shared_ptr<my::video::CMarker> > markerArray = footage->GetExtrinsicCalibrationMarkerArray();

    for (std::vector<boost::shared_ptr<my::video::CMarker> >::iterator markerIterator = markerArray.begin(); markerIterator != markerArray.end(); ++markerIterator)
    {
        if ((*markerIterator)->IsEnabled())
        {
            if (!CSelectionMechanism::Instance().IsEnabled())
                DrawMarker(*markerIterator);
            else
                DrawMask(*markerIterator);
        }
    }
}

void CViewExtrinsicCalibration::DrawMarker(boost::shared_ptr<my::video::CMarker> marker) const
{
    HEALTH_CHECK(!marker, /*false*/);

    boost::shared_ptr<CUfcCalibratorModel> ufcCalibratorModel = CUfcCalibratorViewModel::Instance().GetUfcCalibratorModel();

    if (!ufcCalibratorModel)
        return /*true*/;

    boost::shared_ptr<CFootage> footage = ufcCalibratorModel->GetFootage();

    if (!footage)
        return /*true*/;

    boost::shared_ptr<CTexture> texture = footage->GetFrameAsTexture();

    if (!texture)
        return /*true*/;

    // (SCREEN) POSITION WHERE THE MARKER SHOULD BE PROJECTED (2D)
    const double *_2dReferenceCoord = marker->GetScreenCoord();

    HEALTH_CHECK(!_2dReferenceCoord, /*false*/);

    int viewport[4] = { 0 };

    glGetIntegerv(GL_VIEWPORT, viewport);

    double ratio = (double)(viewport[3] - viewport[1])/(viewport[2] - viewport[0]);

    GlHelper::PUSH_MATRIX_HELPER pushMatrixHelper(true);

    glDisable(GL_DEPTH_TEST);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_BLEND);

    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

    glEnable(GL_LINE_SMOOTH);

    // [-1, 1] -> [0, 1]
    double s = 0.5 * (1.0 + _2dReferenceCoord[0]),
        // [-1, 1] -> [1, 0]
        t = 1.0 - 0.5 * (1.0 + _2dReferenceCoord[1]);

    double zoom = marker->GetZoomFactor(),
        size = marker->GetScreenSize();

    double sMin = s - ratio * zoom,
        sMax = s + ratio * zoom,
        tMin = t + zoom,
        tMax = t - zoom;

    // (BEGIN OF) DEBUG ONLY! (09-Mar-2016) RENDERING OF DEBUGGING DATA
    if (marker->IsTextured() &&
        marker->GetTexture())
    {
        texture = marker->GetTexture();

        sMin = 0.0;
        sMax = 1.0;
        tMin = 1.0;
        tMax = 0.0;
    }
    // (END OF) DEBUG ONLY! (09-Mar-2016) RENDERING OF DEBUGGING DATA

    if (texture)
        texture->Bind(0);

    // TEXTURE
    
    glColor3ub(255, 255, 255);

    glBegin(GL_QUADS);

    glTexCoord2d(sMin, tMin);
    glVertex2d(_2dReferenceCoord[0] - ratio * size, _2dReferenceCoord[1] - size);
    glTexCoord2d(sMax, tMin);
    glVertex2d(_2dReferenceCoord[0] + ratio * size, _2dReferenceCoord[1] - size);
    glTexCoord2d(sMax, tMax);
    glVertex2d(_2dReferenceCoord[0] + ratio * size, _2dReferenceCoord[1] + size);
    glTexCoord2d(sMin, tMax);
    glVertex2d(_2dReferenceCoord[0] - ratio * size, _2dReferenceCoord[1] + size);

    glEnd();

    if (texture)
        CTexture::DisableTextureUnit(0);

    // QUAD AROUND THE TEXTURE

    glColor3ub(255, 255, 255);

    glLineStipple(1, 0xf0f0);

    glEnable(GL_LINE_STIPPLE);

    glBegin(GL_LINE_LOOP);

    glVertex2d(_2dReferenceCoord[0] - ratio * size, _2dReferenceCoord[1] - size);
    glVertex2d(_2dReferenceCoord[0] + ratio * size, _2dReferenceCoord[1] - size);
    glVertex2d(_2dReferenceCoord[0] + ratio * size, _2dReferenceCoord[1] + size);
    glVertex2d(_2dReferenceCoord[0] - ratio * size, _2dReferenceCoord[1] + size);

    glEnd();

    glDisable(GL_LINE_STIPPLE);

    // CROSS INSIDE THE QUAD

    glColor3ub(255, 0, 0);

    glBegin(GL_LINES);

    glVertex2d(_2dReferenceCoord[0] - ratio * size, _2dReferenceCoord[1]);
    glVertex2d(_2dReferenceCoord[0] - 0.01, _2dReferenceCoord[1]);

    glVertex2d(_2dReferenceCoord[0] + 0.01, _2dReferenceCoord[1]);
    glVertex2d(_2dReferenceCoord[0] + ratio * size, _2dReferenceCoord[1]);

    glVertex2d(_2dReferenceCoord[0], _2dReferenceCoord[1] - size);
    glVertex2d(_2dReferenceCoord[0], _2dReferenceCoord[1] - 0.01);

    glVertex2d(_2dReferenceCoord[0], _2dReferenceCoord[1] + 0.01);
    glVertex2d(_2dReferenceCoord[0], _2dReferenceCoord[1] + size);

    glEnd();
    
    // ERROR

    const double *worldCoordError = marker->GetWorldCoordError();

    if (MyMath::IsValid(worldCoordError[0]) ||
        MyMath::IsValid(worldCoordError[1]) ||
        MyMath::IsValid(worldCoordError[2]))
    {
        double errorInInches = UnitConversion::Feet2Inch(MyMath::Magnitude3(worldCoordError)),
            errorInPct = 0.0;

        if (errorInInches < 1.0)
            errorInPct = 90.0 + ((errorInInches - 1.0) / (0.0 - 1.0)) * 10.0;
        else if (errorInInches < 12.0)
            errorInPct = 0.0 + ((errorInInches - 12.0) / (1.0 - 12.0)) * 90.0;

        std::string errorAsString = std::to_string((int)errorInPct) + "%";

        //// (BEGIN OF) ERROR IN PIXELS! (WINDOW COORD)
        //my::CVector3<double> windowCoordError = marker->GetWindowCoordError();

        //if (windowCoordError.IsValid())
        //{
        //    std::string windowErrorString = my::NumberToString(windowCoordError.Length(), 2);

        //    errorAsString += "/" + windowErrorString;
        //}
        //// (END OF) ERROR IN PIXELS! (WINDOW COORD)

        glColor3d(1.0, 1.0, 1.0);

        if (marker->GetName() != my::Null<std::string>())
            DrawString(_2dReferenceCoord[0] - 0.9 * ratio * size, _2dReferenceCoord[1] + 1.2 * size, errorAsString);

        // ERROR DIRECTION

        my::CVector3<double> windowCoordError = marker->GetWindowCoordError();

        if (windowCoordError.IsValid())
        {
            windowCoordError.Normalize();

            glLineWidth(4.0f);

            glColor3d(1.0, 0.0, 0.0);

            glBegin(GL_LINES);

            glVertex2d(_2dReferenceCoord[0] - 0.1 * size * windowCoordError.x(), _2dReferenceCoord[1] - 0.1 * size * windowCoordError.y());
            glVertex2d(_2dReferenceCoord[0] - 0.5 * size * windowCoordError.x(), _2dReferenceCoord[1] - 0.5 * size * windowCoordError.y());

            glEnd();

            glLineWidth(1.0f);
        }
    }

    // NAME

    glColor3ub(255, 255, 255);

    if (marker->GetName() != my::Null<std::string>())
        DrawString(_2dReferenceCoord[0] - 0.9 * ratio * size, _2dReferenceCoord[1] - 1.4 * size, marker->GetName());

    glDisable(GL_LINE_SMOOTH);

    glDisable(GL_BLEND);
}

void CViewExtrinsicCalibration::DrawMask(boost::shared_ptr<my::video::CMarker> marker) const
{
    HEALTH_CHECK(!marker.get(), /*false*/);

    boost::shared_ptr<CUfcCalibratorModel> ufcCalibratorModel = CUfcCalibratorViewModel::Instance().GetUfcCalibratorModel();

    if (!ufcCalibratorModel)
        return /*true*/;

    boost::shared_ptr<CFootage> footage = ufcCalibratorModel->GetFootage();

    if (!footage)
        return /*true*/;

    const double *screenCoord = marker->GetScreenCoord();

    HEALTH_CHECK(!screenCoord, /*false*/);

    int viewport[4] = {0};
    
    glGetIntegerv(GL_VIEWPORT, viewport);

    double ratio = (double)(viewport[3] - viewport[1]) / (viewport[2] - viewport[0]);

    glMatrixMode(GL_PROJECTION);

    glPushMatrix();

    glLoadIdentity();

    glMatrixMode(GL_MODELVIEW);

    glPushMatrix();

    glLoadIdentity();

    glDisable(GL_DEPTH_TEST);

    CSelectableObject(MARKER_ITEM, marker->GetId()).Load();

    double size = marker->GetScreenSize();

    glBegin(GL_QUADS);

    glVertex2f(screenCoord[0] - ratio * size, screenCoord[1] - size);
    glVertex2f(screenCoord[0] + ratio * size, screenCoord[1] - size);
    glVertex2f(screenCoord[0] + ratio * size, screenCoord[1] + size);
    glVertex2f(screenCoord[0] - ratio * size, screenCoord[1] + size);

    glEnd();

    glMatrixMode(GL_PROJECTION);

    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);

    glPopMatrix();
}

void CViewExtrinsicCalibration::DrawFieldLayout()
{
    boost::shared_ptr<CUfcCalibratorModel> ufcCalibratorModel = CUfcCalibratorViewModel::Instance().GetUfcCalibratorModel();

    if (!ufcCalibratorModel)
        return /*true*/;

    boost::shared_ptr<my::CSceneryLayout> sceneryLayout = ufcCalibratorModel->GetSceneryLayout();

    if (!sceneryLayout)
        return /*true*/;

    glEnable(GL_DEPTH_TEST);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_BLEND);

    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

    glEnable(GL_LINE_SMOOTH);

    glLineWidth((float)CUfcCalibratorViewModel::Instance().GetAttribute<int>(GUI_FIELD_LAYOUT_LINE_THICKNESS_ITEM));

    glColor3ub(255, 255, 0);

    GlHelper::Draw(sceneryLayout->GetLayoutMesh());

    glLineWidth(1.0f);

    glDisable(GL_LINE_SMOOTH);

    glDisable(GL_BLEND);

    glDisable(GL_DEPTH_TEST);
}

void CViewExtrinsicCalibration::DrawCalibrationQuality()
{
    boost::shared_ptr<CUfcCalibratorModel> ufcCalibratorModel = CUfcCalibratorViewModel::Instance().GetUfcCalibratorModel();

    if (!ufcCalibratorModel)
        return /*true*/;

    boost::shared_ptr<CFootage> footage = ufcCalibratorModel->GetFootage();

    if (!footage)
        return /*true*/;

    boost::shared_ptr<CPinholeCamera2> pinholeCamera = footage->GetPinholeCamera();

    if (!pinholeCamera)
        return /*true*/;

    boost::shared_ptr<CMarkerGroup> extrinsicCalibrationMarkerGroup = footage->GetExtrinsicCalibrationMarkerGroup();

    if (!extrinsicCalibrationMarkerGroup)
        return /*true*/;

    double fitness = extrinsicCalibrationMarkerGroup->GetFitness(pinholeCamera),
        fitnessInPct = 0.0;

    if (fitness < 0.2)
        fitnessInPct = 90.0 + ((fitness - 0.2) / (0.0 - 0.2)) * 10.0;
    else if (fitness < 12.0)
        fitnessInPct = 0.0 + ((fitness - 1.0) / (0.2 - 1.0)) * 90.0;

    std::string calibrationQualityAsString = std::to_string((int)fitnessInPct) + "%",
        octagonSizeAsString = std::to_string(CUfcCalibratorViewModel::Instance().GetAttribute<int>(GUI_OCTAGON_SIZE));

    int viewport[4] = { 0 };

    glGetIntegerv(GL_VIEWPORT, viewport);

    double ratio = (double)(viewport[3] - viewport[1]) / (viewport[2] - viewport[0]);

    GlHelper::PUSH_MATRIX_HELPER pushMatrixHelper(true);

    glDisable(GL_DEPTH_TEST);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_BLEND);

    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

    glEnable(GL_LINE_SMOOTH);

    glColor3d(1.0, 1.0, 1.0);

    DrawString(-0.975f, 0.95f, std::string(UFC_STRING_RESOURCE_0002) + ": " + calibrationQualityAsString + " (" + octagonSizeAsString + "' octagon)");

    glDisable(GL_LINE_SMOOTH);

    glDisable(GL_BLEND);
}

void CViewExtrinsicCalibration::DrawString(float x, float y, const std::string& text) const
{
    HEALTH_CHECK(m_renderingContext == 0, /*false*/);

    if (text.empty())
        return /*false*/;

    GlHelper::PUSH_MATRIX_HELPER pushMatrixHelper(true);

    // (BEGIN OF) BUG: (28-Sep-2022) THE TEXT IS NOT BEING RENDERED ON VM'S!
    //GLint currentUnpackAlignment = 4;

    //glGetIntegerv(GL_UNPACK_ALIGNMENT, &currentUnpackAlignment);    

    //glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

    //int width = ((QGLWidget*)m_renderingContext)->width(),
    //    height = ((QGLWidget*)m_renderingContext)->height();

    //QFont titleFont;

    //titleFont.setFamily(titleFont.defaultFamily());
    //titleFont.setWeight(80);

    //double winX = 0.5 * (x + 1.0) * width,
    //    winY = 0.5 * (y + 1.0) * height;

    //((QGLWidget*)m_renderingContext)->renderText((int)winX, height - (int)winY, text.c_str(), titleFont);

    //glPixelStorei(GL_UNPACK_ALIGNMENT, currentUnpackAlignment);

    //GlHelper::DrawText(x, y, 15, 0.0, GLUT_STROKE_ROMAN, text);

    int guiTextSize = CUfcCalibratorViewModel::Instance().GetAttribute<int>(GUI_TEXT_SIZE_ITEM);

    void* glutFont = GLUT_BITMAP_HELVETICA_10;

    switch (guiTextSize) {
    case 10:
        glutFont = GLUT_BITMAP_HELVETICA_10;
        break;
    case 12:
        glutFont = GLUT_BITMAP_HELVETICA_12;
        break;
    case 18:
        glutFont = GLUT_BITMAP_HELVETICA_18;
        break;
    default:
        LOG_ERROR();
    }

    GlHelper::DrawString(glutFont, x, y, 0.0, text);
    // (END OF) BUG: (28-Sep-2022) THE TEXT IS NOT BEING RENDERED ON VM'S!
}

/**
*/
void CViewExtrinsicCalibration::Create()
{
    m_currentVenueId = my::Null<int>();
    m_fieldLayout.reset();

    // (BEGIN OF) BUG: (28-Sep-2022) THE TEXT IS NOT BEING RENDERED ON VM'S!
    int argc = 0;

    char** argv = 0;

    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_ALPHA | GLUT_MULTISAMPLE);
    // (END OF) BUG: (28-Sep-2022) THE TEXT IS NOT BEING RENDERED ON VM'S!
}
