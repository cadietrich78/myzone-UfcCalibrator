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
#endif // !defined(__glee_h_)

#include <Compass.h>
#include <SelectableObject.h>
#include <SelectionMechanism.h>
// TODO: (26-Oct-2015) DEPRECATED
#include <BoundingBox.h>

#include "ObjectType.h"
#include "viewmodel/UfcCalibratorViewModel.h"
#include "StringResource.h"

#include "glwidget.h"

#include <GLHelper.h>

/**
*/
GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(QGLFormat(QGL::Rgba | QGL::DoubleBuffer | QGL::DepthBuffer | QGL::SampleBuffers), parent),
    m_parent(parent)
{
    if (!format().rgba())
        LOG_MESSAGE(UFC_STRING_RESOURCE_0110);
    if (!format().doubleBuffer())
        LOG_MESSAGE(UFC_STRING_RESOURCE_0111);
    if (!format().depth())
        LOG_MESSAGE(UFC_STRING_RESOURCE_0112);
    if (!format().sampleBuffers())
        LOG_MESSAGE(UFC_STRING_RESOURCE_0113);

    m_footageViewer.SetRenderingContext(this);
    m_extrinsicCalibrationViewer.SetRenderingContext(this);
    m_landingPageViewer.SetRenderingContext(this);

    setMouseTracking(true);
}

/**
*/
GLWidget::~GLWidget()
{
}

/**
*/
QSize GLWidget::minimumSizeHint() const
{
    return QSize(320, 200);
}

/**
*/
QSize GLWidget::sizeHint() const
{
    return QSize(640, 480);
}

// TRICKY: (11-Oct-2015) BUILD A PINHOLE CAMERA FROM THE CURRENT VIEW-PROJECTION TRANSFORM.
boost::shared_ptr<CPinholeCamera2> GLWidget::GetPinholeCamera() const
{
    glMatrixMode(GL_PROJECTION);

    glPushMatrix();

    glLoadIdentity();

    glMatrixMode(GL_MODELVIEW);

    glPushMatrix();

    glLoadIdentity();

    boost::shared_ptr<CPinholeCamera2> emptyObject;
    
    boost::shared_ptr<CPinholeCamera2> pinholeCamera = CUfcCalibratorViewModel::Instance().GetPinholeCamera();

    HEALTH_CHECK(!pinholeCamera, emptyObject)

	pinholeCamera->ApplyTransform();

    // (BEGIN OF) TESTING: (18-Sep-2019) APPLY TRANSFORM!
    glMatrixMode(GL_PROJECTION);

    glLoadMatrixT(pinholeCamera->GetProjectionMatrix());

    glMatrixMode(GL_MODELVIEW);

    glLoadMatrixT(pinholeCamera->GetViewMatrix());
    // (END OF) TESTING: (18-Sep-2019) APPLY TRANSFORM!

    int viewport[4] = { 0 };
    double modelViewMatrix[16] = { 0 },
        projectionMatrix[16] = { 0 };

    glGetIntegerv(GL_VIEWPORT, viewport);

	glGetDoublev(GL_PROJECTION_MATRIX, projectionMatrix);
    glGetDoublev(GL_MODELVIEW_MATRIX, modelViewMatrix);

    glMatrixMode(GL_PROJECTION);

    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);

    glPushMatrix();

    boost::shared_ptr<CPinholeCamera2> currentPinholeCamera(new CPinholeCamera2);

    HEALTH_CHECK(!currentPinholeCamera, emptyObject);
    
    currentPinholeCamera->SetTrackballMode(pinholeCamera->GetTrackballMode());
    currentPinholeCamera->SetCameraType(pinholeCamera->GetCameraType());
    currentPinholeCamera->SetFieldOfView(pinholeCamera->GetFieldOfView());
    currentPinholeCamera->SetNearPlaneDepth(1.0);
    currentPinholeCamera->SetFarPlaneDepth(1000.0);

    // BUG: (21-Nov-2015) FOOTAGE VIEWPORT INSTEAD OF VIEWER VIEWPORT
    boost::shared_ptr<CUfcCalibratorModel> ufcCalibratorModel = CUfcCalibratorViewModel::Instance().GetUfcCalibratorModel();

    if (!ufcCalibratorModel)
        return emptyObject;

    boost::shared_ptr<CFootage> footage = ufcCalibratorModel->GetFootage();

    if (footage)
    {
        boost::shared_ptr<CMediaPlayerInterface> mediaPlayerInterface = footage->GetMediaPlayerInterface();

        if (mediaPlayerInterface)
        {
            int width = mediaPlayerInterface->GetWidth(),
                height = mediaPlayerInterface->GetHeight();

            if ((width != my::Null<int>()) &&
                (height != my::Null<int>()))
            {
                viewport[0] = 0;
                viewport[1] = 0;
                viewport[2] = width;
                viewport[3] = height;
            }
        }
    }

    currentPinholeCamera->SetViewport(viewport[0], viewport[1], viewport[2], viewport[3]);

    currentPinholeCamera->SetViewMatrix(modelViewMatrix);
    currentPinholeCamera->SetProjectionMatrix(projectionMatrix);

    return currentPinholeCamera;
}

/**
*/
void GLWidget::SaveFrame(std::string fileName)
{
    QImage image = grabFrameBuffer(false);

    if (my::GetFileExtension(fileName) != "png")
        fileName += ".png";

    if (!image.save(fileName.c_str(), "png"))
    {
        LOG_ERROR();

        return /*false*/;
    }
}

/**
*/
void GLWidget::initializeGL()
{
    qglClearColor(m_backgroundColor);

	if (glewInit() != GLEW_OK) 
    {
        LOG_ERROR();

        return /*false*/;
    }

	Create();

    CreatePopupMenu();

    // DEBUG ONLY! (01-Oct-2015)
    std::string openGLInfo = std::string("GL_VENDOR: ")
        + (char *)glGetString(GL_VENDOR)
        + "\nGL_RENDERER: "
        + (char *)glGetString(GL_RENDERER)
        + "\nGL_VERSION: "
        + (char *)glGetString(GL_VERSION)
        + "\nGL_SHADING_LANGUAGE_VERSION: "
        + (char *)glGetString(GL_SHADING_LANGUAGE_VERSION);

    // DEBUG ONLY! (01-Oct-2015)
    LOG_MESSAGE(openGLInfo);

    int redBits = 0,
        greenBits = 0,
        blueBits = 0,
        alphaBits = 0,
        stencilBits = 0;

    // DEBUG ONLY! (01-Oct-2015)
    glGetIntegerv(GL_RED_BITS, &redBits);
    glGetIntegerv(GL_GREEN_BITS, &greenBits);
    glGetIntegerv(GL_BLUE_BITS, &blueBits);
    glGetIntegerv(GL_ALPHA_BITS, &alphaBits);
    glGetIntegerv(GL_STENCIL_BITS, &stencilBits);

    // DEBUG ONLY! (01-Oct-2015)
    openGLInfo = "RED BITS: "
        + my::NumberToString(redBits)
        + "\nGREEN BITS: "
        + my::NumberToString(greenBits)
        + "\nBLUE BITS: "
        + my::NumberToString(blueBits)
        + "\nALPHA BITS: "
        + my::NumberToString(alphaBits)
        + "\nSTENCIL BITS: "
        + my::NumberToString(stencilBits);

    // DEBUG ONLY! (01-Oct-2015)
    LOG_MESSAGE(openGLInfo);
    
    if (!GlHelper::IsGlOk())
	{
		LOG_ERROR();
		
		return;
	}
}

/**
*/
void GLWidget::UpdateSelectedMarker()
{
    if (IsMarkerSelected())
    {
        int selectedObjectId = CSelectionMechanism::Instance().GetSelectedObjectId();

        HEALTH_CHECK(selectedObjectId == CObject::INVALID_ID, /*false*/);

        boost::shared_ptr<CUfcCalibratorModel> ufcCalibratorModel = CUfcCalibratorViewModel::Instance().GetUfcCalibratorModel();

        HEALTH_CHECK(!ufcCalibratorModel, /*false*/);

        boost::shared_ptr<CFootage> footage = ufcCalibratorModel->GetFootage();

        HEALTH_CHECK(!footage, /*false*/);

        boost::shared_ptr<my::video::CMarker> marker = footage->GetMarker(selectedObjectId);

        HEALTH_CHECK(!marker, /*false*/);

        if (m_markerScreenSizeSmallAction->isChecked())
            marker->SetScreenSize(0.1);
        else if (m_markerScreenSizeMediumAction->isChecked())
            marker->SetScreenSize(0.15);
        else if (m_markerScreenSizeLargeAction->isChecked())
            marker->SetScreenSize(0.225);

        if (m_markerZoomFactorLowAction->isChecked())
            marker->SetZoomFactor(0.055); 
        else if (m_markerZoomFactorMediumAction->isChecked())
            marker->SetZoomFactor(0.04);
        else if (m_markerZoomFactorHighAction->isChecked())
            marker->SetZoomFactor(0.025);

        if (m_markerSpeedRegularAction->isChecked())
            marker->SetScreenSpeed(1.0);
        else if (m_markerSpeedSlowAction->isChecked())
            marker->SetScreenSpeed(0.2);
    }
}

void GLWidget::UpdateAllMarkers()
{
    QObject* senderObject = sender();

    boost::shared_ptr<CUfcCalibratorModel> ufcCalibratorModel = CUfcCalibratorViewModel::Instance().GetUfcCalibratorModel();

    HEALTH_CHECK(!ufcCalibratorModel, /*false*/);

    boost::shared_ptr<CFootage> footage = ufcCalibratorModel->GetFootage();

    HEALTH_CHECK(!footage, /*false*/);

    std::vector<boost::shared_ptr<my::video::CMarker> > markerArray = footage->GetExtrinsicCalibrationMarkerArray();

    for (auto& marker : markerArray)
    {
        if (marker->IsEnabled())
        {
            if (m_allMarkersScreenSizeSmallAction == senderObject)
                marker->SetScreenSize(0.1);
            else if (m_allMarkersScreenSizeMediumAction == senderObject)
                marker->SetScreenSize(0.15);
            else if (m_allMarkersScreenSizeLargeAction == senderObject)
                marker->SetScreenSize(0.225);

            if (m_allMarkersZoomFactorLowAction == senderObject)
                marker->SetZoomFactor(0.055);
            else if (m_allMarkersZoomFactorMediumAction == senderObject)
                marker->SetZoomFactor(0.04);
            else if (m_allMarkersZoomFactorHighAction == senderObject)
                marker->SetZoomFactor(0.025);

            if (m_allMarkersSpeedRegularAction == senderObject)
                marker->SetScreenSpeed(1.0);
            else if (m_allMarkersSpeedSlowAction == senderObject)
                marker->SetScreenSpeed(0.2);
        }
    }
}

/**
*/
void GLWidget::paintGL()
{
    // BUG: (??-???-????) http://lists.apple.com/archives/mac-opengl/2012/Jul/msg00038.html
    if (!GlHelper::IsGlOk())
	{
		LOG_MESSAGE("Unknown or untracked OpenGL error");

#if defined(__APPLE)
        while ((error = glGetError()) != GL_NO_ERROR)
            ;
#endif        
	}

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    boost::shared_ptr<CPinholeCamera2> pinholeCamera = CUfcCalibratorViewModel::Instance().GetPinholeCamera();

    HEALTH_CHECK(!pinholeCamera, /*false*/);
    
    pinholeCamera->ApplyTransform();

    // (BEGIN OF) TESTING: (18-Sep-2019) APPLY TRANSFORM!
    glMatrixMode(GL_PROJECTION);

    glLoadMatrixT(pinholeCamera->GetProjectionMatrix());

    glMatrixMode(GL_MODELVIEW);

    glLoadMatrixT(pinholeCamera->GetViewMatrix());
    // (END OF) TESTING: (18-Sep-2019) APPLY TRANSFORM!

    CCompass background;

    background.SetGroundColor(60.0f/255.0f, 100.0f/255.0f, 40.0f/255.0f);
    //background.SetHorizonColor(0.75f*(165.0f/255.0f), 0.75f*(191.0f/255.0f), 0.75f*(221.0f/255.0f));
    //background.SetSkyColor(0.15f*(165.0f/255.0f), 0.15f*(191.0f/255.0f), 0.15f*(221.0f/255.0f));

    //background.Draw(pinholeCamera);    

    m_footageViewer.Draw(0);
    m_extrinsicCalibrationViewer.Draw(0);
    // THE LANDING PAGE OVERRIDES ALL OTHER VIEWS
    m_landingPageViewer.Draw(0);
}

/**
*/
void GLWidget::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);

    boost::shared_ptr<CPinholeCamera2> pinholeCamera = CUfcCalibratorViewModel::Instance().GetPinholeCamera();

    HEALTH_CHECK(!pinholeCamera, /*false*/);
    
    pinholeCamera->SetViewport(0, 0, width, height);
}

/**
*/
void GLWidget::mousePressEvent(QMouseEvent *event)
{
    //TRACE_FUNCTION(my::CTrace::MUST_HAVE, "(buttons): %i (x): %i (y): %i", event->buttons(), event->x(), event->y());

    m_mousePosition = event->pos();

    SelectObject(event->x(), height() - event->y());

    if (event->buttons() & Qt::LeftButton)
    {
        // Nothing by now.
	}
	else if (event->buttons() & Qt::RightButton)
	{
        boost::shared_ptr<CUfcCalibratorModel> ufcCalibratorModel = CUfcCalibratorViewModel::Instance().GetUfcCalibratorModel();

        if (ufcCalibratorModel)
        {
            boost::shared_ptr<CFootage> footage = ufcCalibratorModel->GetFootage();

            if (footage)
            {
                UpdatePopupMenu();

                m_popupMenu->exec(event->globalPos());
            }
        }
    }

    m_mousePosition = event->pos();

	updateGL();
}

/**
*/
void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    boost::shared_ptr<CPinholeCamera2> pinholeCamera = CUfcCalibratorViewModel::Instance().GetPinholeCamera();

    HEALTH_CHECK(!pinholeCamera, /*false*/);

    int dx = event->x() - m_mousePosition.x(),
        dy = event->y() - m_mousePosition.y();

    HighlightObject(event->x(), height() - event->y());

    int selectedObjectId = CSelectionMechanism::Instance().GetSelectedObjectId();

    if (event->buttons() & Qt::LeftButton)
    {
        // IFF NO OBJECT IS SELECTED, ROTATE THE CAMERA
        if (selectedObjectId == CObject::INVALID_ID)
        {
            pinholeCamera->Pitch(0.05 * dy);
            pinholeCamera->Roll(0.05 * dx);
        }
        // IFF AN OBJECT IS SELECTED, MOVE IT
        else
        {
            // ONLY MARKERS

            if (CUfcCalibratorViewModel::Instance().GetAttribute<bool>(EXTRINSIC_CALIBRATION_EDITING))
            {
                boost::shared_ptr<CUfcCalibratorModel> ufcCalibratorModel = CUfcCalibratorViewModel::Instance().GetUfcCalibratorModel();

                if (!ufcCalibratorModel)
                    return /*true*/;

                boost::shared_ptr<CFootage> footage = ufcCalibratorModel->GetFootage();

                HEALTH_CHECK(!footage, /*false*/);

                // NORMALIZING TO [-1, 1]

                boost::shared_ptr<my::video::CMarker> marker = footage->GetMarker(selectedObjectId);

                HEALTH_CHECK(!marker, /*false*/);

                double speed = marker->GetScreenSpeed();

                double s = speed * (2.0 * (double)dx / width()),
                    t = speed * (2.0 * (double)dy / height());

                t = -t;

                footage->MoveMarkerById(selectedObjectId, s, t);
            }
        }
    }
#if QT_VERSION > 0x040603
    else if (event->buttons() & Qt::MiddleButton)
#else //#if QT_VERSION > 0x040603
    else if (event->buttons() & Qt::MidButton)
#endif //#if QT_VERSION > 0x040603
    {
		pinholeCamera->MoveSide(0.1 * dx);
		pinholeCamera->MoveUp(0.1 * (-dy));
    }
    else if (event->buttons() & Qt::RightButton)
    {
        // IFF NO OBJECT IS SELECTED, ZOOM OUT
        if (selectedObjectId == CObject::INVALID_ID)
        {
            pinholeCamera->MoveFront(0.1 * dy);
        }
    }

    m_mousePosition = event->pos();

	updateGL();
}

/**
*/
void GLWidget::mouseReleaseEvent(QMouseEvent *event)
{
	CSelectionMechanism::Instance().Clear();

	updateGL();
}

/**
*/
void GLWidget::HighlightObject(int x, int y)
{
    //boost::shared_ptr<CPinholeCamera2> pinholeCamera = CUfcCalibratorViewModel::Instance().GetPinholeCamera();

    //HEALTH_CHECK(!pinholeCamera, /*false*/);

    //GLubyte color[4] = { 0 };
    //int objectBitset = 0;

    //CSelectionMechanism::Instance().Enable();

    //CSelectableObject::ObjectBitsetToColor(INVALID_OBJECT_TYPE, color, color + 1, color + 2, color + 3);

    //glClearColor((float)color[0] / 255.0f, (float)color[1] / 255.0f, (float)color[2] / 255.0f, (float)color[3] / 255.0f);

    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //pinholeCamera->ApplyTransform();

    //// ???

    //glReadPixels(x, y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, color);

    //objectBitset = CSelectableObject::ColorToObjectBitset(color[0], color[1], color[2], color[3]);

    //CSelectionMechanism::Instance().Disable();

    //CSelectionMechanism::Instance().SetHighlightedObjectBitset(objectBitset);

    //// DEBUG ONLY! (06-Nov-2015)
    ////switch (CSelectionMechanism::Instance().GetSelectedObjectType()) {
    ////case ?????:
    ////    std::cout << "?????" << " - " << CSelectionMechanism::Instance().GetSelectedObjectId() << std::endl;
    ////    break;
    ////}
}

/**
*/
void GLWidget::SelectObject(int x, int y)
{
    boost::shared_ptr<CPinholeCamera2> pinholeCamera = CUfcCalibratorViewModel::Instance().GetPinholeCamera();

    HEALTH_CHECK(!pinholeCamera, /*false*/);

    GLubyte color[4] = { 0 };
    int objectBitset = 0;

    CSelectionMechanism::Instance().Enable();

    CSelectableObject::ObjectBitsetToColor(INVALID_OBJECT_TYPE, color, color + 1, color + 2, color + 3);

    glClearColor((float)color[0] / 255.0f, (float)color[1] / 255.0f, (float)color[2] / 255.0f, (float)color[3] / 255.0f);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    pinholeCamera->ApplyTransform();

    // (BEGIN OF) TESTING: (18-Sep-2019) APPLY TRANSFORM!
    glMatrixMode(GL_PROJECTION);

    glLoadMatrixT(pinholeCamera->GetProjectionMatrix());

    glMatrixMode(GL_MODELVIEW);

    glLoadMatrixT(pinholeCamera->GetViewMatrix());
    // (END OF) TESTING: (18-Sep-2019) APPLY TRANSFORM!

    m_extrinsicCalibrationViewer.Draw(0);

    glReadPixels(x, y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, color);

    objectBitset = CSelectableObject::ColorToObjectBitset(color[0], color[1], color[2], color[3]);

    CSelectionMechanism::Instance().Disable();

    CSelectionMechanism::Instance().SetSelectedObjectBitset(objectBitset);

    // DEBUG ONLY! (06-Nov-2015)
    //switch (CSelectionMechanism::Instance().GetSelectedObjectType()) {
    //case ?????:
    //    std::cout << "?????" << " - " << CSelectionMechanism::Instance().GetSelectedObjectId() << std::endl;
    //    break;
    //}
}

/**
*/
bool GLWidget::InitializePinholeCamera()
{
    boost::shared_ptr<CPinholeCamera2> pinholeCamera(new CPinholeCamera2());

	if (!pinholeCamera)
	{
		LOG_ERROR();

		return false;
	}

    double opticalCenter[3] = { 0.0, -150.0, 50.0 },
        center[3] = { 0, 60, 0 },
        up[3] = { 0.0, 0.0, 1.0 };

    if (!pinholeCamera->Create(opticalCenter, center, up, 30.0, 1.0, 1000.0, width(), height()))
    {
        LOG_ERROR();

        return false;
    }

	pinholeCamera->SetTrackballMode(CPinholeCamera2::AROUND_Z);

    CUfcCalibratorViewModel::Instance().SetPinholeCamera(pinholeCamera);

    return true;
}

/**
*/
void GLWidget::CreatePopupMenu()
{
    try
    {
        m_popupMenu = new QMenu(this);

        // SELECTED MARKER

        m_markerMenu = m_popupMenu->addMenu(QIcon(":/images/marker.png"), tr(UFC_STRING_RESOURCE_0017));

        m_markerScreenSizeSmallAction = new QAction(tr(UFC_STRING_RESOURCE_0209), this);
        m_markerScreenSizeSmallAction->setCheckable(true);
        m_markerScreenSizeSmallAction->setChecked(false);
        connect(m_markerScreenSizeSmallAction, SIGNAL(triggered()), this, SLOT(UpdateSelectedMarker()));

        m_markerScreenSizeMediumAction = new QAction(tr(UFC_STRING_RESOURCE_0210), this);
        m_markerScreenSizeMediumAction->setCheckable(true);
        m_markerScreenSizeMediumAction->setChecked(true);
        connect(m_markerScreenSizeMediumAction, SIGNAL(triggered()), this, SLOT(UpdateSelectedMarker()));

        m_markerScreenSizeLargeAction = new QAction(tr(UFC_STRING_RESOURCE_0211), this);
        m_markerScreenSizeLargeAction->setCheckable(true);
        m_markerScreenSizeLargeAction->setChecked(false);
        connect(m_markerScreenSizeLargeAction, SIGNAL(triggered()), this, SLOT(UpdateSelectedMarker()));

        m_markerScreenSizeActionGroup = new QActionGroup(this);
        m_markerScreenSizeActionGroup->addAction(m_markerScreenSizeSmallAction);
        m_markerScreenSizeActionGroup->addAction(m_markerScreenSizeMediumAction);
        m_markerScreenSizeActionGroup->addAction(m_markerScreenSizeLargeAction);

        m_markerZoomFactorLowAction = new QAction(tr(UFC_STRING_RESOURCE_0212), this);
        m_markerZoomFactorLowAction->setCheckable(true);
        m_markerZoomFactorLowAction->setChecked(false);
        connect(m_markerZoomFactorLowAction, SIGNAL(triggered()), this, SLOT(UpdateSelectedMarker()));

        m_markerZoomFactorMediumAction = new QAction(tr(UFC_STRING_RESOURCE_0213), this);
        m_markerZoomFactorMediumAction->setCheckable(true);
        m_markerZoomFactorMediumAction->setChecked(false);
        connect(m_markerZoomFactorMediumAction, SIGNAL(triggered()), this, SLOT(UpdateSelectedMarker()));

        m_markerZoomFactorHighAction = new QAction(tr(UFC_STRING_RESOURCE_0214), this);
        m_markerZoomFactorHighAction->setCheckable(true);
        m_markerZoomFactorHighAction->setChecked(false);
        connect(m_markerZoomFactorHighAction, SIGNAL(triggered()), this, SLOT(UpdateSelectedMarker()));

        m_markerZoomFactorActionGroup = new QActionGroup(this);
        m_markerZoomFactorActionGroup->addAction(m_markerZoomFactorLowAction);
        m_markerZoomFactorActionGroup->addAction(m_markerZoomFactorMediumAction);
        m_markerZoomFactorActionGroup->addAction(m_markerZoomFactorHighAction);

        m_markerSpeedRegularAction = new QAction(tr(UFC_STRING_RESOURCE_0164), this);
        m_markerSpeedRegularAction->setCheckable(true);
        m_markerSpeedRegularAction->setChecked(true);
        connect(m_markerSpeedRegularAction, SIGNAL(triggered()), this, SLOT(UpdateSelectedMarker()));

        m_markerSpeedSlowAction = new QAction(tr(UFC_STRING_RESOURCE_0165), this);
        m_markerSpeedSlowAction->setCheckable(true);
        m_markerSpeedSlowAction->setChecked(false);
        connect(m_markerSpeedSlowAction, SIGNAL(triggered()), this, SLOT(UpdateSelectedMarker()));

        m_markerScreenSpeedActionGroup = new QActionGroup(this);
        m_markerScreenSpeedActionGroup->addAction(m_markerSpeedRegularAction);
        m_markerScreenSpeedActionGroup->addAction(m_markerSpeedSlowAction);

        QMenu *markerSizeMenu = m_markerMenu->addMenu(tr(UFC_STRING_RESOURCE_0018));

        markerSizeMenu->addAction(m_markerScreenSizeSmallAction);
        markerSizeMenu->addAction(m_markerScreenSizeMediumAction);
        markerSizeMenu->addAction(m_markerScreenSizeLargeAction);

        QMenu *markerZoomMenu = m_markerMenu->addMenu(tr(UFC_STRING_RESOURCE_0019));

        markerZoomMenu->addAction(m_markerZoomFactorLowAction);
        markerZoomMenu->addAction(m_markerZoomFactorMediumAction);
        markerZoomMenu->addAction(m_markerZoomFactorHighAction);

        QMenu *markerSpeedMenu = m_markerMenu->addMenu(tr(UFC_STRING_RESOURCE_0163));

        markerSpeedMenu->addAction(m_markerSpeedRegularAction);
        markerSpeedMenu->addAction(m_markerSpeedSlowAction);

        // ALL MARKERS

        m_allMarkersMenu = m_popupMenu->addMenu(tr(UFC_STRING_RESOURCE_0029));

        m_allMarkersScreenSizeSmallAction = new QAction(tr(UFC_STRING_RESOURCE_0209), this);
        m_allMarkersScreenSizeSmallAction->setCheckable(false);
        connect(m_allMarkersScreenSizeSmallAction, SIGNAL(triggered()), this, SLOT(UpdateAllMarkers()));

        m_allMarkersScreenSizeMediumAction = new QAction(tr(UFC_STRING_RESOURCE_0210), this);
        m_allMarkersScreenSizeMediumAction->setCheckable(false);
        connect(m_allMarkersScreenSizeMediumAction, SIGNAL(triggered()), this, SLOT(UpdateAllMarkers()));

        m_allMarkersScreenSizeLargeAction = new QAction(tr(UFC_STRING_RESOURCE_0211), this);
        m_allMarkersScreenSizeLargeAction->setCheckable(false);
        connect(m_allMarkersScreenSizeLargeAction, SIGNAL(triggered()), this, SLOT(UpdateAllMarkers()));

        m_allMarkersZoomFactorLowAction = new QAction(tr(UFC_STRING_RESOURCE_0212), this);
        m_allMarkersZoomFactorLowAction->setCheckable(false);
        connect(m_allMarkersZoomFactorLowAction, SIGNAL(triggered()), this, SLOT(UpdateAllMarkers()));

        m_allMarkersZoomFactorMediumAction = new QAction(tr(UFC_STRING_RESOURCE_0213), this);
        m_allMarkersZoomFactorMediumAction->setCheckable(false);
        connect(m_allMarkersZoomFactorMediumAction, SIGNAL(triggered()), this, SLOT(UpdateAllMarkers()));

        m_allMarkersZoomFactorHighAction = new QAction(tr(UFC_STRING_RESOURCE_0214), this);
        m_allMarkersZoomFactorHighAction->setCheckable(false);
        connect(m_allMarkersZoomFactorHighAction, SIGNAL(triggered()), this, SLOT(UpdateAllMarkers()));

        m_allMarkersSpeedRegularAction = new QAction(tr(UFC_STRING_RESOURCE_0164), this);
        m_allMarkersSpeedRegularAction->setCheckable(false);
        connect(m_allMarkersSpeedRegularAction, SIGNAL(triggered()), this, SLOT(UpdateAllMarkers()));

        m_allMarkersSpeedSlowAction = new QAction(tr(UFC_STRING_RESOURCE_0165), this);
        m_allMarkersSpeedSlowAction->setCheckable(false);
        connect(m_allMarkersSpeedSlowAction, SIGNAL(triggered()), this, SLOT(UpdateAllMarkers()));

        QMenu* allMarkersSizeMenu = m_allMarkersMenu->addMenu(tr(UFC_STRING_RESOURCE_0018));

        allMarkersSizeMenu->addAction(m_allMarkersScreenSizeSmallAction);
        allMarkersSizeMenu->addAction(m_allMarkersScreenSizeMediumAction);
        allMarkersSizeMenu->addAction(m_allMarkersScreenSizeLargeAction);

        QMenu* allMarkersZoomMenu = m_allMarkersMenu->addMenu(tr(UFC_STRING_RESOURCE_0019));

        allMarkersZoomMenu->addAction(m_allMarkersZoomFactorLowAction);
        allMarkersZoomMenu->addAction(m_allMarkersZoomFactorMediumAction);
        allMarkersZoomMenu->addAction(m_allMarkersZoomFactorHighAction);

        QMenu* allMarkersSpeedMenu = m_allMarkersMenu->addMenu(tr(UFC_STRING_RESOURCE_0163));

        allMarkersSpeedMenu->addAction(m_allMarkersSpeedRegularAction);
        allMarkersSpeedMenu->addAction(m_allMarkersSpeedSlowAction);
    }
    catch (...)
    {
        m_popupMenu = 0;
        m_markerMenu = 0;
        m_markerScreenSizeActionGroup = 0;
        m_markerScreenSizeSmallAction = 0;
        m_markerScreenSizeMediumAction = 0;
        m_markerScreenSizeLargeAction = 0;
        m_markerZoomFactorActionGroup = 0;
        m_markerZoomFactorLowAction = 0;
        m_markerZoomFactorMediumAction = 0;
        m_markerZoomFactorHighAction = 0;
        m_markerScreenSpeedActionGroup = 0;
        m_markerSpeedRegularAction = 0;
        m_markerSpeedSlowAction = 0;
        m_allMarkersMenu = 0;
        m_allMarkersScreenSizeSmallAction = 0;
        m_allMarkersScreenSizeMediumAction = 0;
        m_allMarkersScreenSizeLargeAction = 0;
        m_allMarkersZoomFactorLowAction = 0;
        m_allMarkersZoomFactorMediumAction = 0;
        m_allMarkersZoomFactorHighAction = 0;
        m_allMarkersSpeedRegularAction = 0;
        m_allMarkersSpeedSlowAction = 0;

        LOG_ERROR();
    }
}

/**
*/
void GLWidget::UpdatePopupMenu()
{
    HEALTH_CHECK(!m_markerMenu, /*false*/);
    HEALTH_CHECK(!m_markerScreenSizeSmallAction, /*false*/);
    HEALTH_CHECK(!m_markerScreenSizeMediumAction, /*false*/);
    HEALTH_CHECK(!m_markerScreenSizeLargeAction, /*false*/);
    HEALTH_CHECK(!m_markerZoomFactorLowAction, /*false*/);
    HEALTH_CHECK(!m_markerZoomFactorMediumAction, /*false*/);
    HEALTH_CHECK(!m_markerZoomFactorHighAction, /*false*/);
    HEALTH_CHECK(!m_markerSpeedRegularAction, /*false*/);
    HEALTH_CHECK(!m_markerSpeedSlowAction, /*false*/);

    // HELPER TO UPDATE ACTIONS THAT ARE CONNECTED TO SLOTS
#define ACTION_SET_CHECKED(ACTION, STATE) \
    if (ACTION) \
    { \
        ACTION->blockSignals(true); \
        ACTION->setChecked(STATE); \
        ACTION->blockSignals(false); \
    }

    // MARKERS (ALL MARKERS)

    if (IsMarkerSelected())
    {
        m_markerMenu->setEnabled(true);

        int selectedObjectId = CSelectionMechanism::Instance().GetSelectedObjectId();

        HEALTH_CHECK(selectedObjectId == CObject::INVALID_ID, /*false*/);

        boost::shared_ptr<CUfcCalibratorModel> ufcCalibratorModel = CUfcCalibratorViewModel::Instance().GetUfcCalibratorModel();

        HEALTH_CHECK(!ufcCalibratorModel, /*false*/);

        boost::shared_ptr<CFootage> footage = ufcCalibratorModel->GetFootage();

        HEALTH_CHECK(!footage, /*false*/);

        boost::shared_ptr<my::video::CMarker> marker = footage->GetMarker(selectedObjectId);

        HEALTH_CHECK(!marker, /*false*/);

        double screenSize = marker->GetScreenSize();

        if (MyMath::IsClose(screenSize, 0.1, MyMath::Epsilon<double>()))
            m_markerScreenSizeSmallAction->setChecked(true);
        else if (MyMath::IsClose(screenSize, 0.15, MyMath::Epsilon<double>()))
            m_markerScreenSizeMediumAction->setChecked(true);
        else //if (MyMath::IsClose(screenSize, 0.225, MyMath::Epsilon<double>()))
            m_markerScreenSizeLargeAction->setChecked(true);

        double zoomFactor = marker->GetZoomFactor();

        if (MyMath::IsClose(zoomFactor, 0.055, MyMath::Epsilon<double>()))
            m_markerZoomFactorLowAction->setChecked(true);
        else if (MyMath::IsClose(zoomFactor, 0.04, MyMath::Epsilon<double>()))
            m_markerZoomFactorMediumAction->setChecked(true);
        else //if (MyMath::IsClose(zoomFactor, 0.025, MyMath::Epsilon<double>()))
            m_markerZoomFactorHighAction->setChecked(true);

        if (MyMath::IsClose(marker->GetScreenSpeed(), 1.0, MyMath::Epsilon<double>()))
            m_markerSpeedRegularAction->setChecked(true);
        else
            m_markerSpeedSlowAction->setChecked(true);
    }
    else
    {
        m_markerMenu->setDisabled(true);
    }
}

// Helper for the initialization of the marker menu.
bool GLWidget::IsMarkerSelected() const
{
    int selectedObjectId = CSelectionMechanism::Instance().GetSelectedObjectId();

    if (selectedObjectId == CObject::INVALID_ID)
        return false;

    boost::shared_ptr<CUfcCalibratorModel> ufcCalibratorModel = CUfcCalibratorViewModel::Instance().GetUfcCalibratorModel();

    if (!ufcCalibratorModel)
        return false;

    boost::shared_ptr<CFootage> footage = ufcCalibratorModel->GetFootage();

    if (!footage)
        return false;

    boost::shared_ptr<my::video::CMarker> marker = footage->GetMarker(selectedObjectId);

    if (!marker)
        return false;

    return true;
}

/**
*/
void GLWidget::Create()
{
    m_backgroundColor = QColor::fromRgbF(0, 0, 0);

    InitializePinholeCamera();

    MyMath::Assign3(0.0f, 0.0f, 0.0f, m_worldTranslation);
    m_mousePosition = QPoint(-1, -1);
    //m_footageViewer
    //m_extrinsicCalibrationViewer

    if (!m_landingPageViewer.Initialize())
    {
        LOG_ERROR();

        return /*false*/;
    }

    //m_guiBoxViewer

    // BUG: (12-Oct-2015) 'parent()' and 'parentWidget()' fail to hold a pointer to the QGLWidget parent.
    //m_parent = 0;
    // POPUP MENU
    m_popupMenu = 0;
    // MARKERS (ALL MARKERS)
    m_markerMenu = 0;
    m_markerScreenSizeActionGroup = 0;
    m_markerScreenSizeSmallAction = 0;
    m_markerScreenSizeMediumAction = 0;
    m_markerScreenSizeLargeAction = 0;
    m_markerZoomFactorActionGroup = 0;
    m_markerZoomFactorLowAction = 0;
    m_markerZoomFactorMediumAction = 0;
    m_markerZoomFactorHighAction = 0;
    m_markerScreenSpeedActionGroup = 0;
    m_markerSpeedRegularAction = 0;
    m_markerSpeedSlowAction = 0;
    m_allMarkersMenu = 0;
}

