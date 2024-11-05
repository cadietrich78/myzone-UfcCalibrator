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

#include <future>
#include <set>

#include <boost/lexical_cast.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/filesystem.hpp>
#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/calib3d.hpp>

#include <Logger.h>
#include <FileHelper.h>
#include <UnitConversion.h>
#include <CalibratedPinholeCamera.h>

#include "StringResource.h"
#include "ObjectType.h"
#include "viewmodel/UfcCalibratorViewModel.h"
#include "tab/ExtrinsicCalibrationTab.h"
#include "dialog/AppSettingsDialog.h"
#include "UfcCalibratorConfig.h"

#include "mainwindow.h"

MainWindow::MainWindow()
{
    Create();

    // Set initial properties of the window.
    ReadWindowSettings();

    CreateActions();
    CreateMenus();
    CreateTabs();
    CreateStatusBar();

    CreateCentralWidget();

    setWindowTitle(tr(UFC_STRING_RESOURCE_0001));

    setWindowIcon(QIcon(":/images/stat_o_rama_icon.png"));

    // If set to true, then the top toolbar area is replaced with a Carbon HIToolbar or a Cocoa NSToolbar (depending on whether Qt was built with Carbon or Cocoa). All toolbars in the top toolbar area and any toolbars added afterwards are moved to that. (http://doc.qt.io/qt-4.8/qmainwindow.html#unifiedTitleAndToolBarOnMac-prop)
    setUnifiedTitleAndToolBarOnMac(true);

    // TRICKY: (26-Jul-2016) http://stackoverflow.com/questions/26368659/qwidget-how-to-receive-keypressevent-inside-child-widgets
    // Sometimes an object needs to look at, and possibly intercept, the events that are delivered to another object. For example, dialogs commonly want to filter key presses for some widgets; for example, to modify Return-key handling.
    // The QObject::installEventFilter() function enables this by setting up an event filter, causing a nominated filter object to receive the events for a target object in its QObject::eventFilter() function. An event filter gets to process events before the target object does, allowing it to inspect and discard the events as required. An existing event filter can be removed using the QObject::removeEventFilter() function.
    qApp->installEventFilter(this);
}

void MainWindow::Repaint()
{
    UpdateCentralWidget();
}

void MainWindow::Refresh()
{
    Repaint();

    // (BEGIN OF) TRICKY: (07-Nov-2016) A DUMB MAILBOX TO OVERCOME THE PROBLEM OF MESSAGES THAT COME FROM DIFFERENT THREADS
    if (m_messageQueue)
    {
        std::string message = m_messageQueue->PopMessage();

        if (!my::IsNull(message))
            statusBar()->showMessage(tr(message.c_str()));

        message = m_messageQueue->PopMessageDialog();

        if (!my::IsNull(message))
        {
            // (BEGIN OF) TESTING: (21-Mar-2017) SOUND FEEDBACK
            if (!m_messageSoundEffect)
                m_messageSoundEffect.reset(new QSound("./sounds/notify.wav"));
            
            if (m_messageSoundEffect)
                m_messageSoundEffect->play();
            // (END OF) TESTING: (21-Mar-2017) SOUND FEEDBACK

            QMessageBox::information(this, tr("Information"), tr(message.c_str()));
        }
    }
    // (END OF) TRICKY: (07-Nov-2016) A DUMB MAILBOX TO OVERCOME THE PROBLEM OF MESSAGES THAT COME FROM DIFFERENT THREADS

    UpdateActions();
}

// TRICKY: (26-Jul-2016) http://stackoverflow.com/questions/26368659/qwidget-how-to-receive-keypressevent-inside-child-widgets
// Sometimes an object needs to look at, and possibly intercept, the events that are delivered to another object. For example, dialogs commonly want to filter key presses for some widgets; for example, to modify Return-key handling.
// The QObject::installEventFilter() function enables this by setting up an event filter, causing a nominated filter object to receive the events for a target object in its QObject::eventFilter() function. An event filter gets to process events before the target object does, allowing it to inspect and discard the events as required. An existing event filter can be removed using the QObject::removeEventFilter() function.
bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    // TRICKY: (28-Sep-2022) A DUMB WAY TO MAKE SURE (?) THE MAIN WINDOW WILL BE REFRESHED AFTER AN UPDATING OF THE UNDERLYING DATA!
    if (!obj &&
        !event)
    {
        Repaint();

        return true;
    }
    else if (event->type() == QEvent::KeyRelease)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

        int playKey = CUfcCalibratorViewModel::Instance().GetAttribute<int>(GUI_PLAY_KEY),
            previousFrameKey = CUfcCalibratorViewModel::Instance().GetAttribute<int>(GUI_PREVIOUS_FRAME_KEY),
            nextFrameKey = CUfcCalibratorViewModel::Instance().GetAttribute<int>(GUI_NEXT_FRAME_KEY);

        if (my::IsNull(playKey))
            playKey = Qt::Key_M;
        if (my::IsNull(previousFrameKey))
            previousFrameKey = Qt::Key_Comma;
        if (my::IsNull(nextFrameKey))
            nextFrameKey = Qt::Key_Period;
        
        std::set<int> keyOfInterestSet;

        keyOfInterestSet.insert(playKey);
        keyOfInterestSet.insert(previousFrameKey);
        keyOfInterestSet.insert(nextFrameKey);
        // HELP SCREEN
        keyOfInterestSet.insert(Qt::Key_F1);

        // (BEGIN OF) TESTING: (??-Aug-2016) TESTING WIDGETS (IT DOESN'T WORK)
        //// TRICKY: (26-Jul-2016) MAIN VIEWER (GL VIEWER)
        //if ((obj == this) ||
        //    // TRICKY: (26-Jul-2016) PLAY VIEWER
        //    (obj == m_playViewerTreeView) ||
        //    // TRICKY: (26-Jul-2016) SETTINGS (STRIKE ZONE)
        //    (obj->isWidgetType() && (((QWidget *)obj)->windowTitle().toStdString() == UFC_STRING_RESOURCE_0045)) ||
        //    // TRICKY: (26-Jul-2016) SETTINGS (EXTRINSIC MARKERS)
        //    (obj->isWidgetType() && (((QWidget *)obj)->windowTitle().toStdString() == UFC_STRING_RESOURCE_0021)))
        // (END OF) TESTING: (??-Aug-2016) TESTING WIDGETS (IT DOESN'T WORK)
        // TESTING: (11-Sep-2016) TESTING KEYS INSTEAD OF WIDGETS
        if ((keyOfInterestSet.find(keyEvent->key()) != keyOfInterestSet.end()) &&
            // BUG: (21-Mar-2017) WAIT FOR DATA
            CUfcCalibratorViewModel::Instance().GetUfcCalibratorModel()->GetFootage())
        {
            //// DEBUG ONLY! (26-Jul-2016)
            //qDebug() << "MainWindow::eventFilter: " << keyEvent->key() << " from " << obj;

            if (keyEvent->key() == playKey)
                PlaybackPlayButton();
            else if (keyEvent->key() == previousFrameKey)
                PlaybackPreviousFrame();
            else if (keyEvent->key() == nextFrameKey)
                PlaybackNextFrameButton();
            // DEBUG ONLY! (03-Nov-2016)
            else if (keyEvent->key() == Qt::Key_F1)
                ShowCheatSheet();
            else
            {
                for (std::vector<CTabInterface *>::iterator tabInterfaceIterator = m_tabInterfaceArray.begin(); tabInterfaceIterator != m_tabInterfaceArray.end(); ++tabInterfaceIterator)
                {
                    if (IsTabVisible(*tabInterfaceIterator) &&
                        // BUG: (03-Oct-2016) DO NOT SEND THE EVENT BACK TO THE WIDGET FROM WHERE IT CAME
                        !(obj->isWidgetType() && (((QWidget *)obj)->windowTitle() == (*tabInterfaceIterator)->windowTitle())))
                    {
                        // (BEGIN OF) TRICKY: (03-Oct-2016) THIS IS THE WAY IT SHOULD BE IMPLEMENTED - EVENTS SHOULD BE POSTED TO THE MAINWINDOW CHILDREN; HOWEVER, SINCE ALL EVENTS ARE BEING CAPTURED BY THE MAINWINDOW, THE POSTED EVENTS RETURNS TO THIS VERY METHOD (A RECURSION). GIVEN THAT, WE'LL IMPLEMENT THE BRUTE FORCE, ND SEND THE EVENT TO THE CHILD WIDGET DIRECTLY.
                        //// (BEGIN OF) BUG: (14-Jun-2016) The event must be allocated on the heap since the post event queue will take ownership of the event and delete it once it has been posted. It is not safe to access the event after it has been posted.
                        //QKeyEvent *eventCopy = new QKeyEvent(QKeyEvent::KeyRelease, keyEvent->key(), keyEvent->modifiers());

                        //qApp->postEvent((QWidget*)(*tabInterfaceIterator), (QEvent *)eventCopy);
                        //// (END OF) BUG: (14-Jun-2016) The event must be allocated on the heap since the post event queue will take ownership of the event and delete it once it has been posted. It is not safe to access the event after it has been posted.

                        QKeyEvent eventCopy(QKeyEvent::KeyRelease, keyEvent->key(), keyEvent->modifiers(), keyEvent->text());

                        (*tabInterfaceIterator)->keyReleaseEvent(&eventCopy);
                        // (END OF) TRICKY: (03-Oct-2016) THIS IS THE WAY IT SHOULD BE IMPLEMENTED - EVENTS SHOULD BE POSTED TO THE MAINWINDOW CHILDREN; HOWEVER, SINCE ALL EVENTS ARE BEING CAPTURED BY THE MAINWINDOW, THE POSTED EVENTS RETURNS TO THIS VERY METHOD (A RECURSION). GIVEN THAT, WE'LL IMPLEMENT THE BRUTE FORCE, ND SEND THE EVENT TO THE CHILD WIDGET DIRECTLY.
                    }
                }
            }

            Repaint();

            return true;
        }
    }

    return QObject::eventFilter(obj, event);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    // BUG: (15-Jul-2017) IT SHOULD BE THE LAST ONE TO BE CALLED, BUT NOT FROM THE CLASS DESTRUCTOR, SINCE IT'S A SINGLETON AND MAY BE DEAD BEFORE OTHER SIGLETONS, LIKE LOGGER
    CUfcCalibratorViewModel::Instance().SaveUserSettings();

    CUfcCalibratorViewModel::Instance().RegisterUpdateCallback();

    WriteWindowSettings();

    event->accept();
}

void MainWindow::RefreshContents()
{
    // BUG: (29-Mar-2017) GUI (QT) OPERATIONS SHOULD BE CARRIED ON IN THE CURRENT THREAD
    //// TESTING: (10-Mar-2017) ASYNCHRONOUS CALL
    //std::async(std::launch::async, &MainWindow::RefreshPlayViewerContents, this);
    //RefreshPlayViewerContents();

    // BUG: (29-Mar-2017) GUI (QT) OPERATIONS SHOULD BE CARRIED ON IN THE CURRENT THREAD
    //// TESTING: (10-Mar-2017) ASYNCHRONOUS CALL
    //std::async(std::launch::async, &MainWindow::UpdateDatabaseActions, this);
    //UpdateDatabaseActions();
}

void MainWindow::OpenFrameFromDisk()
{
    QFileDialog frameFileOpenDialog(this, UFC_STRING_RESOURCE_0312, NULL);

    frameFileOpenDialog.setFileMode(QFileDialog::ExistingFiles);

    QString frameNameFilter = tr(UFC_STRING_RESOURCE_0061) + tr(";;")
        + UFC_STRING_RESOURCE_0089 + tr(";;");

    frameFileOpenDialog.setNameFilter(frameNameFilter);
    frameFileOpenDialog.setDirectory(GetCurrentDirectory());

    if (!frameFileOpenDialog.exec())
    {
        LOG_ERROR();

        return /*false*/;
    }

    QStringList frameFileNameArray = frameFileOpenDialog.selectedFiles();

    std::vector<std::string> frameFormattedFileNameArray;

    for (int fileNameIndex = 0; fileNameIndex < frameFileNameArray.size(); ++fileNameIndex)
    {
        if (frameFileNameArray.at(fileNameIndex).isEmpty() ||
            frameFileNameArray.at(fileNameIndex).toLocal8Bit().isEmpty())
        {
            LOG_ERROR();

            return /*false*/;
        }

        std::string formattedFileName = frameFileNameArray.at(fileNameIndex).toLocal8Bit().constData();

        frameFormattedFileNameArray.push_back(formattedFileName);
    }

    if (frameFormattedFileNameArray.size() != 1)
    {
        LOG_ERROR();

        statusBar()->showMessage(UFC_STRING_RESOURCE_0313, 9000);

        QApplication::restoreOverrideCursor();

        return /*false*/;
    }

    std::string frameUrl = frameFormattedFileNameArray[0];

    statusBar()->showMessage(tr(UFC_STRING_RESOURCE_0148), 9000);

    // (BEGIN OF) REMOVE DISTORTION!
    QFileDialog calibrationFileOpenDialog(this, UFC_STRING_RESOURCE_0554, NULL);

    calibrationFileOpenDialog.setFileMode(QFileDialog::ExistingFiles);

    QString calibrationNameFilter = tr(UFC_STRING_RESOURCE_0031) + tr(";;");

    calibrationFileOpenDialog.setNameFilter(calibrationNameFilter);
    calibrationFileOpenDialog.setDirectory(GetCurrentDirectory());

    if (!calibrationFileOpenDialog.exec())
    {
        LOG_ERROR();

        return /*false*/;
    }

    QStringList calibrationFileNameArray = calibrationFileOpenDialog.selectedFiles();

    std::vector<std::string> calibrationFormattedFileNameArray;

    for (int fileNameIndex = 0; fileNameIndex < calibrationFileNameArray.size(); ++fileNameIndex)
    {
        if (calibrationFileNameArray.at(fileNameIndex).isEmpty() ||
            calibrationFileNameArray.at(fileNameIndex).toLocal8Bit().isEmpty())
        {
            LOG_ERROR();

            return /*false*/;
        }

        std::string formattedFileName = calibrationFileNameArray.at(fileNameIndex).toLocal8Bit().constData();

        calibrationFormattedFileNameArray.push_back(formattedFileName);
    }

    if (calibrationFormattedFileNameArray.size() == 1)
    {
        m_intrinsicCalibrationFileName = calibrationFormattedFileNameArray.front();

        CCalibratedPinholeCamera calibratedPinholeCamera;

        if (!calibratedPinholeCamera.FromOpenCvFile(m_intrinsicCalibrationFileName))
        {
            LOG_ERROR();

            return /*false*/;
        }

        cv::Mat frame = cv::imread(frameUrl);

        if (frame.empty())
        {
            LOG_ERROR();

            return /*false*/;
        }

        cv::Mat frameToBeDisplayed(frame.size(), frame.type(), cv::Scalar(0, 0, 0));

        cv::undistort(frame, frameToBeDisplayed, calibratedPinholeCamera.GetCameraMatrix(), calibratedPinholeCamera.GetDistortionCoefficientArray());

        frameUrl = "data/temporary/" + my::GetFileName(frameUrl) + ".UNDISTORTED.png";

        if (!cv::imwrite(frameUrl, frameToBeDisplayed))
        {
            LOG_ERROR();

            return /*false*/;
        }
    }
    // (END OF) REMOVE DISTORTION!

    bool createPlay = true;

    boost::shared_ptr<CUfcCalibratorModel> ufcCalibratorModel = CUfcCalibratorViewModel::Instance().GetUfcCalibratorModel();

    if (ufcCalibratorModel)
    {
        boost::shared_ptr<CFootage> footage = ufcCalibratorModel->GetFootage();

        if (footage)
        {
            int reply = QMessageBox::warning(this, UFC_STRING_RESOURCE_0013, UFC_STRING_RESOURCE_0014, QMessageBox::Yes, QMessageBox::No);

            createPlay = reply == QMessageBox::No;
        }
    }

    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

    if (!CUfcCalibratorViewModel::Instance().OpenFromUrl(frameUrl, createPlay))
    {
        LOG_ERROR();

        statusBar()->showMessage(UFC_STRING_RESOURCE_0313, 9000);

        QApplication::restoreOverrideCursor();

        return /*false*/;
    }

    UpdatePlaybackWidget();

    InitializeTabs();

    SetCurrentDirectory(my::GetDirectory(frameFormattedFileNameArray[0]).c_str());

    Repaint();

    statusBar()->showMessage(UFC_STRING_RESOURCE_0149, 9000);

    UpdateActions();

    QApplication::restoreOverrideCursor();
}

void MainWindow::OpenFrameFromWebcam()
{
    bool isValidInput = false;

    int webcamIndex = QInputDialog::getInt(this, UFC_STRING_RESOURCE_0024, UFC_STRING_RESOURCE_0025, 0, 0, 8, 1, &isValidInput);

    if (!isValidInput)
        return /*true*/;

    cv::VideoCapture videoCapture;

    videoCapture.open(webcamIndex, cv::CAP_DSHOW);

    if (!videoCapture.isOpened())
    {
        LOG_ERROR();

        return /*false*/;
    }

    if (!videoCapture.set(cv::CAP_PROP_FRAME_WIDTH, 640) ||
        !videoCapture.set(cv::CAP_PROP_FRAME_HEIGHT, 480))
    {
        LOG_ERROR();

        return /*false*/;
    }

    cv::Mat frame;

    std::string frameUrl = "data/temporary/webcam_" + std::to_string(webcamIndex) + ".png";

    if (videoCapture.read(frame))
    {
        if (frame.empty())
        {
            LOG_ERROR();

            return /*false*/;
        }
    }

    // (BEGIN OF) REMOVE DISTORTION!
    QFileDialog fileOpenDialog(this, UFC_STRING_RESOURCE_0312, NULL);

    fileOpenDialog.setFileMode(QFileDialog::ExistingFiles);

    QString nameFilter = tr(UFC_STRING_RESOURCE_0031) + tr(";;");

    fileOpenDialog.setNameFilter(nameFilter);
    fileOpenDialog.setDirectory(GetCurrentDirectory());

    if (!fileOpenDialog.exec())
        return /*false*/;

    QStringList fileNameArray = fileOpenDialog.selectedFiles();

    std::vector<std::string> formattedFileNameArray;

    for (int fileNameIndex = 0; fileNameIndex < fileNameArray.size(); ++fileNameIndex)
    {
        if (fileNameArray.at(fileNameIndex).isEmpty() ||
            fileNameArray.at(fileNameIndex).toLocal8Bit().isEmpty())
        {
            LOG_ERROR();

            return /*false*/;
        }

        std::string formattedFileName = fileNameArray.at(fileNameIndex).toLocal8Bit().constData();

        formattedFileNameArray.push_back(formattedFileName);
    }

    if (formattedFileNameArray.size() != 1)
    {
        LOG_ERROR();

        return /*false*/;
    }

    m_intrinsicCalibrationFileName = formattedFileNameArray.front();

    CCalibratedPinholeCamera calibratedPinholeCamera;

    if (!calibratedPinholeCamera.FromOpenCvFile(m_intrinsicCalibrationFileName))
    {
        LOG_ERROR();

        return /*false*/;
    }

    cv::Mat frameToBeDisplayed(frame.size(), frame.type(), cv::Scalar(0, 0, 0));

    cv::undistort(frame, frameToBeDisplayed, calibratedPinholeCamera.GetCameraMatrix(), calibratedPinholeCamera.GetDistortionCoefficientArray());

    if (!cv::imwrite(frameUrl, frameToBeDisplayed))
    {
        LOG_ERROR();

        return /*false*/;
    }

    if (videoCapture.isOpened())
        videoCapture.release();
    // (END OF) REMOVE DISTORTION!

    bool createPlay = true;

    boost::shared_ptr<CUfcCalibratorModel> ufcCalibratorModel = CUfcCalibratorViewModel::Instance().GetUfcCalibratorModel();

    if (ufcCalibratorModel)
    {
        boost::shared_ptr<CFootage> footage = ufcCalibratorModel->GetFootage();

        if (footage)
        {
            int reply = QMessageBox::warning(this, UFC_STRING_RESOURCE_0013, UFC_STRING_RESOURCE_0014, QMessageBox::Yes, QMessageBox::No);

            createPlay = reply == QMessageBox::No;
        }
    }

    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

    if (!CUfcCalibratorViewModel::Instance().OpenFromUrl(frameUrl, createPlay))
    {
        statusBar()->showMessage(UFC_STRING_RESOURCE_0313, 9000);

        QApplication::restoreOverrideCursor();

        LOG_ERROR();

        return /*false*/;
    }

    UpdatePlaybackWidget();

    InitializeTabs();

    Repaint();

    statusBar()->showMessage(UFC_STRING_RESOURCE_0149, 9000);

    UpdateActions();

    QApplication::restoreOverrideCursor();
}

void MainWindow::LoadExtrinsicSettings()
{
    QFileDialog fileOpenDialog(this, UFC_STRING_RESOURCE_0312, NULL);

    fileOpenDialog.setFileMode(QFileDialog::ExistingFiles);

    QString nameFilter = tr(UFC_STRING_RESOURCE_0031) + tr(";;");

    fileOpenDialog.setNameFilter(nameFilter);
    fileOpenDialog.setDirectory(GetCurrentDirectory());

    if (!fileOpenDialog.exec())
        return /*false*/;

    QStringList fileNameArray = fileOpenDialog.selectedFiles();

    std::vector<std::string> formattedFileNameArray;

    for (int fileNameIndex = 0; fileNameIndex < fileNameArray.size(); ++fileNameIndex)
    {
        if (fileNameArray.at(fileNameIndex).isEmpty() ||
            fileNameArray.at(fileNameIndex).toLocal8Bit().isEmpty())
        {
            LOG_ERROR();

            return /*false*/;
        }

        std::string formattedFileName = fileNameArray.at(fileNameIndex).toLocal8Bit().constData();

        formattedFileNameArray.push_back(formattedFileName);
    }

    if (formattedFileNameArray.size() != 1)
    {
        LOG_ERROR();

        return /*false*/;
    }

    std::string extrinsicCalibrationFileName = formattedFileNameArray.front();

    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

    if (!CUfcCalibratorViewModel::Instance().LoadExtrinsicSettings(extrinsicCalibrationFileName))
    {
        statusBar()->showMessage(UFC_STRING_RESOURCE_0313, 9000);

        QApplication::restoreOverrideCursor();

        LOG_ERROR();

        return /*false*/;
    }

    //UpdatePlaybackWidget();

    //InitializeTabs();

    Repaint();

    statusBar()->showMessage(UFC_STRING_RESOURCE_0149, 9000);

    //UpdateActions();

    QApplication::restoreOverrideCursor();
}

void MainWindow::LoadCameraSettings()
{
    QFileDialog fileOpenDialog(this, UFC_STRING_RESOURCE_0312, NULL);

    fileOpenDialog.setFileMode(QFileDialog::ExistingFiles);

    QString nameFilter = tr(UFC_STRING_RESOURCE_0031) + tr(";;");

    fileOpenDialog.setNameFilter(nameFilter);
    fileOpenDialog.setDirectory(GetCurrentDirectory());

    if (!fileOpenDialog.exec())
        return /*false*/;

    QStringList fileNameArray = fileOpenDialog.selectedFiles();

    std::vector<std::string> formattedFileNameArray;

    for (int fileNameIndex = 0; fileNameIndex < fileNameArray.size(); ++fileNameIndex)
    {
        if (fileNameArray.at(fileNameIndex).isEmpty() ||
            fileNameArray.at(fileNameIndex).toLocal8Bit().isEmpty())
        {
            LOG_ERROR();

            return /*false*/;
        }

        std::string formattedFileName = fileNameArray.at(fileNameIndex).toLocal8Bit().constData();

        formattedFileNameArray.push_back(formattedFileName);
    }

    if (formattedFileNameArray.size() != 1)
    {
        LOG_ERROR();

        return /*false*/;
    }

    std::string cameraSettingsFileName = formattedFileNameArray.front();

    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

    if (!CUfcCalibratorViewModel::Instance().LoadCameraSettings(cameraSettingsFileName))
    {
        statusBar()->showMessage(UFC_STRING_RESOURCE_0313, 9000);

        QApplication::restoreOverrideCursor();

        LOG_ERROR();

        return /*false*/;
    }

    //UpdatePlaybackWidget();

    //InitializeTabs();

    Repaint();

    statusBar()->showMessage(UFC_STRING_RESOURCE_0149, 9000);

    //UpdateActions();

    QApplication::restoreOverrideCursor();
}

void MainWindow::SaveFightFlowCalibration()
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

    QFileDialog fileExportDialog;

    QString nameFilter = tr(UFC_STRING_RESOURCE_0031) + tr(";;"),
        cameraFileName = fileExportDialog.getSaveFileName(this, UFC_STRING_RESOURCE_0020, (my::AddTrailingSlash(GetCurrentDirectory().toStdString()) + UFC_STRING_RESOURCE_0022).c_str(), nameFilter);

    if (cameraFileName.isEmpty())
        return /*true*/;

    std::string feedbackMessage = UFC_STRING_RESOURCE_0026;

    // INTERNAL CALIBRAITON ONLY!
    CCalibratedPinholeCamera partiallyCalibratedPinholeCamera;

    if (!partiallyCalibratedPinholeCamera.FromOpenCvFile(m_intrinsicCalibrationFileName))
    {
        LOG_ERROR();

        return /*false*/;
    }

    CCalibratedPinholeCamera calibratedPinholeCamera(*pinholeCamera, partiallyCalibratedPinholeCamera.GetCameraMatrix(), partiallyCalibratedPinholeCamera.GetDistortionCoefficientArray());

    bool okCLicked;

    int cameraIndex = QInputDialog::getInt(this, tr("Camera Settings"), tr("Index:"), 1, 1, 24, 1, &okCLicked);

    if (okCLicked)
        calibratedPinholeCamera.SetIndex(cameraIndex);

    if (!calibratedPinholeCamera.ToPinholeCameraFile(cameraFileName.toStdString()))
    {
        LOG_ERROR();

        return /*false*/;
    }

    //m_glWidget->SaveFrame(my::GetDirectory(cameraFileName.toStdString()) + "calibrator_view.png");

    statusBar()->showMessage(feedbackMessage.c_str(), 9000);
}

// 1. UPDATE GAME
// 2. UPDATE AUDITING ACTIONS
// 3. UPDATE PLAY VIEWER
void MainWindow::CloseGameCallback()
{
    InitializeTabs();
}

void MainWindow::EditSettings()
{
    CAppSettingsDialog appSettingsDialog(this);

    appSettingsDialog.exec();
}

void MainWindow::ClearCalibration()
{
    boost::shared_ptr<CUfcCalibratorModel> ufcCalibratorModel = CUfcCalibratorViewModel::Instance().GetUfcCalibratorModel();

    if (!ufcCalibratorModel)
        return /*true*/;

    for (std::vector<CTabInterface*>::const_iterator tabInterfaceIterator = m_tabInterfaceArray.begin(); tabInterfaceIterator != m_tabInterfaceArray.end(); ++tabInterfaceIterator)
    {
        CUfcCalibratorViewModel::Instance().SetAttribute((*tabInterfaceIterator)->GetType(), IsTabVisible(*tabInterfaceIterator));

        (*tabInterfaceIterator)->UpdateViewFromModel();
    }

    Repaint();
}

// TESTING: (24-Feb-2016) PLAYBACK CONTROL - WORKS FOR BOTH 3D AND FOOTAGE VIEW
void MainWindow::PlaybackPreviousFrame()
{
    boost::shared_ptr<CPlayerInterface> playerInterface = CUfcCalibratorViewModel::Instance().GetPlayerInterface();

    HEALTH_CHECK(!playerInterface, /*false*/);

    // BACK FROM IDLE
    m_idleTimer.Stop();

    playerInterface->PreviousFrame();

    PlaybackRefresh();

    if (playerInterface->GetState() != CMediaPlayerInterface::PLAYING_STATE)
        m_idleTimer.Start();
}

void MainWindow::PlaybackPlayButton()
{
    boost::shared_ptr<CPlayerInterface> playerInterface = CUfcCalibratorViewModel::Instance().GetPlayerInterface();

    HEALTH_CHECK(!playerInterface, /*false*/);

    // BACK FROM IDLE
    m_idleTimer.Stop();

    playerInterface->TogglePlay();

    Repaint();

    if (playerInterface->GetState() != CMediaPlayerInterface::PLAYING_STATE)
        m_idleTimer.Start();
}

void MainWindow::PlaybackNextFrameButton()
{
    boost::shared_ptr<CPlayerInterface> playerInterface = CUfcCalibratorViewModel::Instance().GetPlayerInterface();

    HEALTH_CHECK(!playerInterface, /*false*/);

    // BACK FROM IDLE
    m_idleTimer.Stop();

    playerInterface->NextFrame();

    PlaybackRefresh();

    if (playerInterface->GetState() != CMediaPlayerInterface::PLAYING_STATE)
        m_idleTimer.Start();
}

void MainWindow::PlaybackPositionSlider(int position)
{
    boost::shared_ptr<CPlayerInterface> playerInterface = CUfcCalibratorViewModel::Instance().GetPlayerInterface();

    HEALTH_CHECK(!playerInterface, /*false*/);

    // BACK FROM IDLE
    m_idleTimer.Stop();

    playerInterface->SetPositionInMilliseconds(m_playbackPositionSlider->value());

    Repaint();

    if (playerInterface->GetState() != CMediaPlayerInterface::PLAYING_STATE)
        m_idleTimer.Start();
}

void MainWindow::PlaybackRefresh()
{
    // UPDATE CONTROL AND STATE

    boost::shared_ptr<CPlayerInterface> playerInterface = CUfcCalibratorViewModel::Instance().GetPlayerInterface();

    HEALTH_CHECK(!playerInterface, /*false*/);

    switch (playerInterface->GetState())
    {
    case CMediaPlayerInterface::STOPPED_STATE:
    {
        m_playbackPlayButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));

        my::int64 positionInMilliseconds = playerInterface->GetPositionInMilliseconds(),
            durationInMilliseconds = playerInterface->GetDurationInMilliseconds();

        m_playbackPreviousFrameButton->setEnabled(positionInMilliseconds > 0);
        m_playbackNextFrameButton->setEnabled(positionInMilliseconds < durationInMilliseconds);
    }
    break;

    case CMediaPlayerInterface::PAUSED_STATE:
        m_playbackPlayButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));

        m_playbackPreviousFrameButton->setEnabled(true);
        m_playbackNextFrameButton->setEnabled(true);
        break;

    case CMediaPlayerInterface::PLAYING_STATE:
        m_playbackPlayButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));

        m_playbackPreviousFrameButton->setEnabled(false);
        m_playbackNextFrameButton->setEnabled(false);
        break;
    }

    // TRICKY: (09-Oct-2015) INITIAZE SLIDER (ASYNCHRONOUS PLAYER)
    if (m_playbackPositionSlider->maximum() == 0)
    {
        my::int64 durationInMilliseconds = playerInterface->GetDurationInMilliseconds();

        if (durationInMilliseconds != 0)
        {
            m_playbackPositionSlider->blockSignals(true);

            m_playbackPositionSlider->setRange(0, durationInMilliseconds);

            m_playbackPositionSlider->setEnabled(true);

            m_playbackPositionSlider->blockSignals(false);
        }
    }
    // TRICKY: (09-Oct-2015) UPDATE SLIDER (ASYNCHRONOUS)
    else
    {
        m_playbackPositionSlider->blockSignals(true);

        m_playbackPositionSlider->setValue(playerInterface->GetPositionInMilliseconds());

        m_playbackPositionSlider->blockSignals(false);
    }

    Repaint();
}

// TRICKY: (22-Mar-2017) CHANGED THE CURRENT FRAME, IDLE AGAIN
void MainWindow::IdleCallback()
{
}

// TRICKY: (22-Mar-2017) BACK FROM IDLE, ABOUT TO CHANGE THE CURRENT FRAME
void MainWindow::BackFromIdleCallback()
{
}

void MainWindow::ShowCheatSheet()
{
    HEALTH_CHECK(!boost::filesystem::is_regular_file("./textures/UfcCalibratorCheatSheet.png"), /*false*/);

    QPixmap pixmap("./textures/UfcCalibratorCheatSheet.png");

    m_helpScreen.setPixmap(pixmap.scaled(size() * 0.75, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));

    m_helpScreen.show();
}

void MainWindow::ShowLog()
{
    boost::shared_ptr<QDialog> dialog;
    std::vector<std::string> messageArray;
    QTextEdit *textEdit = 0;
    QVBoxLayout *mainLayout = 0;        

    try
    {
        dialog.reset(new QDialog(this));
    }
    catch (std::exception& e)
    {
        LOG_MESSAGE(e.what());

        return /*false*/;
    }

    dialog->setMinimumWidth(800);
    dialog->setMinimumHeight(480);

    //CLogger::Instance().PrintMessages(messageArray);

    try
    {
        textEdit = new QTextEdit();
    }
    catch (std::exception& e)
    {
        LOG_MESSAGE(e.what());

        return /*false*/;
    }

	for (std::vector<std::string>::const_iterator message = messageArray.begin(); message != messageArray.end(); ++message)
    {
        // DEBUG ONLY! (11-Mar-2015) Helper for error reporting.
        if (message->find(" LOADING GAME:") != std::string::npos)
        {
            textEdit->setTextColor(QColor(255, 255, 255));
            textEdit->setTextBackgroundColor(QColor(255, 0, 0));
        }
        // DEBUG ONLY! (11-Mar-2015) Helper for error reporting.
        else if (message->find(" LOADING PLAY:") != std::string::npos)
        {
            textEdit->setTextColor(QColor(255, 0, 0));
            textEdit->setTextBackgroundColor(QColor(0, 255, 0));
        }
        // Highlight error messages.
        else if (message->find("error has occurred") != std::string::npos)
        {
            textEdit->setTextColor(QColor(255, 0, 0));
            textEdit->setTextBackgroundColor(QColor(255, 255, 0));
        }
        else
        {
            textEdit->setTextColor(QColor(0, 0, 0));
            textEdit->setTextBackgroundColor(QColor(255, 255, 255));
        }

        textEdit->append(tr(message->c_str()));
    }

    textEdit->setReadOnly(true);

    try
    {
        mainLayout = new QVBoxLayout;
    }
    catch (std::exception& e)
    {
        LOG_MESSAGE(e.what());

        return /*false*/;
    }

    mainLayout->addWidget(textEdit);

    dialog->setLayout(mainLayout);

    dialog->exec();
}

void MainWindow::CurrentTabChanged()
{
    for (std::vector<CTabInterface *>::const_iterator tabInterfaceIterator = m_tabInterfaceArray.begin(); tabInterfaceIterator != m_tabInterfaceArray.end(); ++tabInterfaceIterator)
    {
        CUfcCalibratorViewModel::Instance().SetAttribute((*tabInterfaceIterator)->GetType(), IsTabVisible(*tabInterfaceIterator));
    
        (*tabInterfaceIterator)->UpdateViewFromModel();
    }

    UpdatePlaybackWidget();
}

void MainWindow::CurrentTabChanged(bool)
{
    CurrentTabChanged();
}

/**
*/
void MainWindow::CreateActions()
{
    try
    {
        m_exitAction = new QAction(QIcon(":/images/door_out.png"), tr(UFC_STRING_RESOURCE_0005), this);
    }
    catch (std::exception& e)
    {
        LOG_MESSAGE(e.what());

        return /*false*/;
    }

    m_exitAction->setShortcuts(QKeySequence::Quit);
    m_exitAction->setStatusTip(tr(UFC_STRING_RESOURCE_0006));
    connect(m_exitAction, SIGNAL(triggered()), this, SLOT(close()));

    // EDIT

    try
    {
        m_settingsAction = new QAction(QIcon(":/images/settings.png"), tr(UFC_STRING_RESOURCE_0138), this);
    }
    catch (std::exception& e)
    {
        LOG_MESSAGE(e.what());

        return /*false*/;
    }

    connect(m_settingsAction, SIGNAL(triggered()), this, SLOT(EditSettings()));

    // HELP
    
    try
    {
        m_showCheatSheetAction = new QAction(tr(UFC_STRING_RESOURCE_0253), this);
    }
    catch (std::exception& e)
    {
        LOG_MESSAGE(e.what());

        return /*false*/;
    }

    m_showCheatSheetAction->setStatusTip(tr(UFC_STRING_RESOURCE_0254));
    connect(m_showCheatSheetAction, SIGNAL(triggered()), this, SLOT(ShowCheatSheet()));

    try
    {
        m_showLogAction = new QAction(tr(UFC_STRING_RESOURCE_0007), this);
    }
    catch (std::exception& e)
    {
        LOG_MESSAGE(e.what());

        return /*false*/;
    }

    m_showLogAction->setStatusTip(tr(UFC_STRING_RESOURCE_0008));
    connect(m_showLogAction, SIGNAL(triggered()), this, SLOT(ShowLog()));

    // IO

    try
    {
        m_openFrameFromDiskAction = new QAction(tr(UFC_STRING_RESOURCE_0311), this);
    }
    catch (std::exception& e)
    {
        LOG_MESSAGE(e.what());

        return /*false*/;
    }

    connect(m_openFrameFromDiskAction, SIGNAL(triggered()), this, SLOT(OpenFrameFromDisk()));

    try
    {
        m_openFrameFromWebcamAction = new QAction(tr(UFC_STRING_RESOURCE_0314), this);
    }
    catch (std::exception& e)
    {
        LOG_MESSAGE(e.what());

        return /*false*/;
    }

    connect(m_openFrameFromWebcamAction, SIGNAL(triggered()), this, SLOT(OpenFrameFromWebcam()));

    try
    {
        m_loadExtrinsicSettingsAction = new QAction(tr("Extrinsic settings..."), this);
    }
    catch (std::exception& e)
    {
        LOG_MESSAGE(e.what());

        return /*false*/;
    }

    connect(m_loadExtrinsicSettingsAction, SIGNAL(triggered()), this, SLOT(LoadExtrinsicSettings()));

    try
    {
        m_loadCameraSettingsAction = new QAction(tr("Camera settings..."), this);
    }
    catch (std::exception& e)
    {
        LOG_MESSAGE(e.what());

        return /*false*/;
    }

    connect(m_loadCameraSettingsAction, SIGNAL(triggered()), this, SLOT(LoadCameraSettings()));

    try
    {
        m_saveFightFlowCalibrationAction = new QAction(tr(UFC_STRING_RESOURCE_0004), this);
    }
    catch (std::exception& e)
    {
        LOG_MESSAGE(e.what());

        return /*false*/;
    }

    connect(m_saveFightFlowCalibrationAction, SIGNAL(triggered()), this, SLOT(SaveFightFlowCalibration()));

    try
    {
        m_clearCalibrationAction = new QAction(tr(UFC_STRING_RESOURCE_0030), this);
    }
    catch (std::exception& e)
    {
        LOG_MESSAGE(e.what());

        return /*false*/;
    }

    connect(m_clearCalibrationAction, SIGNAL(triggered()), this, SLOT(ClearCalibration()));
}

void MainWindow::CreateMenus()
{
    // FILE

    try
    {
        m_fileMenu = menuBar()->addMenu(tr(UFC_STRING_RESOURCE_0009));
    }
    catch (std::exception& e)
    {
        LOG_MESSAGE(e.what());

        return /*false*/;
    }

    QMenu *fileOpenMenu = 0;

    try
    {
        fileOpenMenu = m_fileMenu->addMenu(tr(UFC_STRING_RESOURCE_0310));
    }
    catch (std::exception& e)
    {
        LOG_MESSAGE(e.what());

        return /*false*/;
    }

    fileOpenMenu->addAction(m_openFrameFromDiskAction);
    fileOpenMenu->addAction(m_openFrameFromWebcamAction);

    fileOpenMenu->addSeparator();

    fileOpenMenu->addAction(m_loadExtrinsicSettingsAction);
    fileOpenMenu->addAction(m_loadCameraSettingsAction);

    m_fileMenu->addSeparator();

    QMenu* fileExportMenu = 0;

    try
    {
        fileExportMenu = m_fileMenu->addMenu(tr(UFC_STRING_RESOURCE_0012));
    }
    catch (std::exception& e)
    {
        LOG_MESSAGE(e.what());

        return /*false*/;
    }

    fileExportMenu->addAction(m_saveFightFlowCalibrationAction);

    m_fileMenu->addSeparator();

    m_fileMenu->addAction(m_exitAction);

    // EDIT

    m_editMenu = menuBar()->addMenu(tr(UFC_STRING_RESOURCE_0087));

    m_editMenu->addAction(m_settingsAction);

    m_editMenu->addSeparator();

    m_editMenu->addAction(m_clearCalibrationAction);

    // HELP

    m_helpMenu = menuBar()->addMenu(tr(UFC_STRING_RESOURCE_0010));

    m_helpMenu->addAction(m_showCheatSheetAction);

    m_helpMenu->addSeparator();

    // HELP - DEBUGGING

    QMenu *helpDebuggingMenu = 0;

    try
    {
        helpDebuggingMenu = m_helpMenu->addMenu(tr(UFC_STRING_RESOURCE_0011));
    }
    catch (std::exception& e)
    {
        LOG_MESSAGE(e.what());

        return /*false*/;
    }
    
    m_helpMenu->addSeparator();

    helpDebuggingMenu->addAction(m_showLogAction);

    UpdateActions();
}

void MainWindow::CreateTabs()
{
    setDockOptions(QMainWindow::AllowTabbedDocks | QMainWindow::VerticalTabs);

    // EXTRINSIC CALIBRATION

    CExtrinsicCalibrationTab *extrinsicCalibrationTab = 0;

    try
    {
        extrinsicCalibrationTab = new CExtrinsicCalibrationTab(this);
    }
    catch (std::exception& e)
    {
        LOG_MESSAGE(e.what());

        return /*false*/;
    }

    extrinsicCalibrationTab->RegisterUpdateCallback(boost::bind(&MainWindow::Repaint, this));

    connect(extrinsicCalibrationTab, SIGNAL(visibilityChanged(bool)), this, SLOT(CurrentTabChanged(bool)));

    // 1ST TAB IN THE DOCK?
    if (m_tabInterfaceArray.empty())
        // TRICKY: (07-Sep-2016) Qt::LeftDockWidgetArea (1), Qt::BottomDockWidgetArea (8), Qt::RightDockWidgetArea (2), Qt::TopDockWidgetArea (4)
        addDockWidget(Qt::LeftDockWidgetArea, extrinsicCalibrationTab);
    else
        tabifyDockWidget(m_tabInterfaceArray.back(), extrinsicCalibrationTab);

    m_tabInterfaceArray.push_back(extrinsicCalibrationTab);

    // BRUTE-FORCE INITIALIZATION

    CurrentTabChanged();
}

void MainWindow::CreateStatusBar()
{
    statusBar()->showMessage(tr(UFC_STRING_RESOURCE_0251), 10000);
}

void MainWindow::CreateCentralWidget()
{
    // PLAYBACK BUTTONS

    try
    {
        m_playbackPreviousFrameButton = new QPushButton;
    }
    catch (std::exception& e)
    {
        LOG_MESSAGE(e.what());

        return /*false*/;
    }

    m_playbackPreviousFrameButton->setDisabled(true);
    m_playbackPreviousFrameButton->setIcon(style()->standardIcon(QStyle::SP_MediaSkipBackward));

    connect(m_playbackPreviousFrameButton, SIGNAL(clicked()), this, SLOT(PlaybackPreviousFrame()));

    try
    {
        m_playbackPlayButton = new QPushButton;
    }
    catch (std::exception& e)
    {
        LOG_MESSAGE(e.what());

        return /*false*/;
    }

    m_playbackPlayButton->setDisabled(true);
    m_playbackPlayButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));

    connect(m_playbackPlayButton, SIGNAL(clicked()), this, SLOT(PlaybackPlayButton()));

    try
    {
        m_playbackNextFrameButton = new QPushButton;
    }
    catch (std::exception& e)
    {
        LOG_MESSAGE(e.what());

        return /*false*/;
    }

    m_playbackNextFrameButton->setDisabled(true);
    m_playbackNextFrameButton->setIcon(style()->standardIcon(QStyle::SP_MediaSkipForward));

    connect(m_playbackNextFrameButton, SIGNAL(clicked()), this, SLOT(PlaybackNextFrameButton()));

    m_playbackPositionSlider = new QSlider(Qt::Horizontal);

    m_playbackPositionSlider->setSingleStep(10);
    m_playbackPositionSlider->setTickInterval(10);
    m_playbackPositionSlider->setRange(0, 0);
    // If tracking is enabled (the default), the slider emits the valueChanged() signal while the slider is being dragged. If tracking is disabled, the slider emits the valueChanged() signal only when the user releases the slider.
    m_playbackPositionSlider->setTracking(false);
    m_playbackPositionSlider->setDisabled(true);

    connect(m_playbackPositionSlider, SIGNAL(valueChanged(int)), this, SLOT(PlaybackPositionSlider(int)));

    QBoxLayout *playbackLayout = new QHBoxLayout;

    playbackLayout->setMargin(0);
    playbackLayout->addWidget(m_playbackPreviousFrameButton);
    playbackLayout->addWidget(m_playbackPlayButton);
    playbackLayout->addWidget(m_playbackNextFrameButton);
    playbackLayout->addWidget(m_playbackPositionSlider);

    m_idleTimer.SetInterval(500);
    m_idleTimer.SetSingleShot(true);

    connect(&m_idleTimer, SIGNAL(timeout()), this, SLOT(IdleCallback()));
    connect(&m_idleTimer, SIGNAL(StopSignal()), this, SLOT(BackFromIdleCallback()));

    QBoxLayout *centralWidgetLayout = new QVBoxLayout;

    try
    {
        m_glWidget = new GLWidget(this);
    }
    catch (std::exception& e)
    {
        LOG_MESSAGE(e.what());

        return /*false*/;
    }

    centralWidgetLayout->addWidget(m_glWidget);
    centralWidgetLayout->addLayout(playbackLayout);

    QWidget *emptyWidget = 0;

    try
    {
        emptyWidget = new QWidget;
    }
    catch (std::exception& e)
    {
        LOG_MESSAGE(e.what());

        return /*false*/;
    }

    emptyWidget->setLayout(centralWidgetLayout);

    setCentralWidget(emptyWidget);
}

void MainWindow::UpdateCentralWidget()
{
    // BUG: (09-Oct-2015) Find a better way for it.
    //m_glWidget->update(); // DOESN'T WORK
    //centralWidget()->update(); // DOESN'T WORK
    QList<GLWidget *> widgetArray;
    widgetArray = findChildren<GLWidget *>();
    if (!widgetArray.isEmpty())
        widgetArray.front()->update();
}

void MainWindow::UpdatePlaybackWidget()
{
    boost::shared_ptr<CUfcCalibratorModel> ufcCalibratorModel = CUfcCalibratorViewModel::Instance().GetUfcCalibratorModel();

    if (!ufcCalibratorModel)
        return /*true*/;

    boost::shared_ptr<CFootage> footage = ufcCalibratorModel->GetFootage();

    if (!footage)
        return /*true*/;

    boost::shared_ptr<CPlayerInterface> playerInterface = CUfcCalibratorViewModel::Instance().GetPlayerInterface();

    // TRICKY: (09-Oct-2015)
    m_playbackPositionSlider->setRange(0, 0);

    // BACK FROM IDLE
    m_idleTimer.Stop();

    if (playerInterface &&
        playerInterface->IsPlayable())
    {
        my::int64 positionInMilliseconds = playerInterface->GetPositionInMilliseconds(),
            durationInMilliseconds = playerInterface->GetDurationInMilliseconds();

        m_playbackPreviousFrameButton->setEnabled(positionInMilliseconds > 0);
        m_playbackPlayButton->setEnabled(true);
        m_playbackNextFrameButton->setEnabled(positionInMilliseconds < durationInMilliseconds);

        m_playbackPositionSlider->setRange(0, durationInMilliseconds);
        m_playbackPositionSlider->setValue(positionInMilliseconds);
        m_playbackPositionSlider->setEnabled(true);
        m_playbackPositionSlider->setSingleStep(playerInterface->GetMillisecondsByFrame());
        m_playbackPositionSlider->setTickInterval(playerInterface->GetMillisecondsByFrame());

        // Asynchronous?
        if (durationInMilliseconds == 0)
        {
            m_playbackPreviousFrameButton->setDisabled(true);
            m_playbackPositionSlider->setDisabled(true);
            m_playbackNextFrameButton->setDisabled(true);
        }

        // (BEGIN OF) TESTING: (25-Feb-2016) ONCE THE PLAYBACK IS ENABLED, THE CALLBACK SHOULD BE REGISTERED
        boost::shared_ptr<CPlayerInterface> playerInterface = CUfcCalibratorViewModel::Instance().GetPlayerInterface();

        if (playerInterface)
            playerInterface->RegisterUpdateCallback(boost::bind(&MainWindow::PlaybackRefresh, this));
        // (END OF) TESTING: (25-Feb-2016) ONCE THE PLAYBACK IS ENABLED, THE CALLBACK SHOULD BE REGISTERED

        if (playerInterface->GetState() != CMediaPlayerInterface::PLAYING_STATE)
            m_idleTimer.Start();
    }
    // Nothing to playback.
    else
    {
        m_playbackPreviousFrameButton->setDisabled(true);
        m_playbackPlayButton->setDisabled(true);
        m_playbackNextFrameButton->setDisabled(true);
        m_playbackPositionSlider->setDisabled(true);
    }
}

void MainWindow::InitializeTabs()
{
    for (std::vector<CTabInterface *>::iterator tabInterfaceIterator = m_tabInterfaceArray.begin(); tabInterfaceIterator != m_tabInterfaceArray.end(); ++tabInterfaceIterator)
    {
        (*tabInterfaceIterator)->Initialize();

        (*tabInterfaceIterator)->UpdateViewFromModel();

        // TRICKY: (22-Nov-2015) VIEW PARAMETERS THAT ARE NOT PART OF THE MODEL!
        (*tabInterfaceIterator)->UpdateModelFromView();
    }
}

// TODO: (20-Nov-2015) QT APPROACH FOR THAT?
bool MainWindow::IsTabVisible(const QDockWidget* dockWidget) const
{
    // TRICKY: (23-Nov-2015) http://stackoverflow.com/questions/22230042/how-to-detect-if-a-tabified-qdockwidget-is-the-active-tab
    return !dockWidget->visibleRegion().isEmpty();
}

void MainWindow::ReadWindowSettings()
{
    QSettings settings("Calibrator", tr(UFC_STRING_RESOURCE_0001));

    // MAIN WINDOW

    QPoint pos = settings.value("position", QPoint(200, 200)).toPoint();
    QSize size = settings.value("size", QSize(400, 400)).toSize();

    resize(size);
    move(pos);

    // TABS AND WIDGETS

    QString currentDirectory = settings.value("current directory", QApplication::applicationDirPath()).toString();

    SetCurrentDirectory(currentDirectory);
}

void MainWindow::WriteWindowSettings()
{
    QSettings settings("Calibrator", tr(UFC_STRING_RESOURCE_0001));

    // MAIN WINDOW

    settings.setValue("position", pos());
    settings.setValue("size", size());

    // TABS AND WIDGETS

    settings.setValue("current directory", GetCurrentDirectory());

    //QList<QDockWidget *> dockWidgetArray = findChildren<QDockWidget *>();

    //for (QList<QDockWidget *>::const_iterator dockWidgetIterator = dockWidgetArray.begin(); dockWidgetIterator != dockWidgetArray.end(); ++dockWidgetIterator)
    //{
    //    // PLAY VIEWER
    //    if ((*dockWidgetIterator)->windowTitle().toStdString() == /*SOME WIDGET NAME*/)
    //    {
    //        //
    //    }
    //}
}

void MainWindow::SetCurrentDirectory(const QString &directoryName)
{
    m_currentDirectoryName = directoryName;

    setWindowModified(false);
}

QString MainWindow::GetCurrentDirectory() const
{
    return m_currentDirectoryName;
}

void MainWindow::UpdateActions() const
{
    boost::shared_ptr<CUfcCalibratorModel> ufcCalibratorModel = CUfcCalibratorViewModel::Instance().GetUfcCalibratorModel();

    if (!ufcCalibratorModel)
        return /*true*/;

    if (!my::IsNull(ufcCalibratorModel->GetAuditingId()))
    {
        m_loadExtrinsicSettingsAction->setEnabled(true);
        m_loadCameraSettingsAction->setEnabled(true);
    }
    else
    {
        m_loadExtrinsicSettingsAction->setDisabled(true);
        m_loadCameraSettingsAction->setDisabled(true);
    }
}

void MainWindow::Create()
{
    m_settingsFileName = QApplication::applicationDirPath().left(1) + ":/appsettings.ini";
    m_currentDirectoryName.clear();
    m_fileMenu = 0;
    m_editMenu = 0;
    m_inputDataMenu = 0;
    m_auditingMenu = 0;
    m_helpMenu = 0;
    m_fileToolBar = 0;
    m_openFrameFromDiskAction = 0;
    m_openFrameFromWebcamAction = 0;
    m_loadExtrinsicSettingsAction = 0;
    m_loadCameraSettingsAction = 0;
    m_saveFightFlowCalibrationAction = 0;
    m_clearCalibrationAction = 0;
    m_exitAction = 0;
    m_settingsAction = 0;
    m_showCheatSheetAction = 0;
    m_showLogAction = 0;
    m_playbackPreviousFrameButton = 0;
    m_playbackPlayButton = 0;
    m_playbackNextFrameButton = 0;
    m_playbackPositionSlider = 0;
    m_glWidget = 0;
    m_tabInterfaceArray.clear();

    try
    {
        m_messageQueue.reset(new CMessageQueue);
    }
    catch (std::exception& e)
    {
        LOG_MESSAGE(e.what());
    }

    // m_helpScreen
    // m_guiRefreshTimer
    m_intrinsicCalibrationFileName = my::Null<std::string>();

    // DEPRECATED: (23-Oct-2016) REPLACE BY DEPENDENCY INJECTION
    CUfcCalibratorViewModel::Instance().SetMessageQueue(m_messageQueue);

    CUfcCalibratorViewModel::Instance().RegisterUpdateCallback(boost::bind(&MainWindow::Repaint, this));
}

