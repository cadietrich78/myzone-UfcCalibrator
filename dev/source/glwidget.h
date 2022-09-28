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

#if !defined(GLWIDGET_INCLUDED)
#define GLWIDGET_INCLUDED

#include <QGLWidget>
#include <QtWidgets>

#include <boost/shared_ptr.hpp>

#include <PinholeCamera2.h>

#include "view/ViewFootage.h"
#include "view/ViewExtrinsicCalibration.h"
#include "view/ViewLandingPage.h"

class GLWidget 
	: public QGLWidget
{
    Q_OBJECT

public:
    GLWidget(QWidget *parent = (QWidget *)0);
    ~GLWidget();

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

    boost::shared_ptr<CPinholeCamera2> GetPinholeCamera() const;

    void SaveFrame(std::string fileName);

protected slots:

    void initializeGL();

    void UpdateSelectedMarker();
    void UpdateAllMarkers();

protected:
    void paintGL();

    void resizeGL(int width, int height);

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    void HighlightObject(int x, int y);
    void SelectObject(int x, int y);

	bool InitializePinholeCamera();

    void CreatePopupMenu();
    void UpdatePopupMenu();

    bool IsMarkerSelected() const;
    
    void Create();

private:
    QColor m_backgroundColor;

    float m_worldTranslation[3];

	QPoint m_mousePosition;

    CViewFootage m_footageViewer;
    CViewExtrinsicCalibration m_extrinsicCalibrationViewer;
    CViewLandingPage m_landingPageViewer;

    QWidget *m_parent;

    QMenu *m_popupMenu;

    QMenu *m_markerMenu;

    QActionGroup *m_markerScreenSizeActionGroup;
    QAction *m_markerScreenSizeSmallAction;
    QAction *m_markerScreenSizeMediumAction;
    QAction *m_markerScreenSizeLargeAction;

    QActionGroup *m_markerZoomFactorActionGroup;
    QAction *m_markerZoomFactorLowAction;
    QAction *m_markerZoomFactorMediumAction;
    QAction *m_markerZoomFactorHighAction;

    QActionGroup *m_markerScreenSpeedActionGroup;
    QAction *m_markerSpeedRegularAction;
    QAction *m_markerSpeedSlowAction;

    QMenu* m_allMarkersMenu;

    QAction* m_allMarkersScreenSizeSmallAction;
    QAction* m_allMarkersScreenSizeMediumAction;
    QAction* m_allMarkersScreenSizeLargeAction;

    QAction* m_allMarkersZoomFactorLowAction;
    QAction* m_allMarkersZoomFactorMediumAction;
    QAction* m_allMarkersZoomFactorHighAction;

    QAction* m_allMarkersSpeedRegularAction;
    QAction* m_allMarkersSpeedSlowAction;
};

#endif // #if !defined(GLWIDGET_INCLUDED)

