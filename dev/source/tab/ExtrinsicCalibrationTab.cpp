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

#include <Logger.h>

#include "../StringResource.h"
#include "../viewmodel/UfcCalibratorViewModel.h"

#include "ExtrinsicCalibrationTab.h"

CExtrinsicCalibrationTab::CExtrinsicCalibrationTab(QWidget * parent, Qt::WindowFlags flags)
    : CTabInterface(tr(UFC_STRING_RESOURCE_0021), parent, flags)
{
    Create();

    Initialize();
}

CExtrinsicCalibrationTab::~CExtrinsicCalibrationTab()
{
    Destroy();
}

OBJECT_TYPE CExtrinsicCalibrationTab::GetType() const
{
    return EXTRINSIC_CALIBRATION_EDITING;
}

// INITIALIZE WIDGET FROM FOOTAGE
void CExtrinsicCalibrationTab::Initialize()
{
    // BUG: (30-Mar-2017) THE TABS ARE RE-INITIALIZED FOR EACH PLAY. THE WIDGETS SHOULD BE CREATED WHEN A VALID PLAY IS AVAILABLE, AND NOT RE-CREATED DURING THE APPLICATION LIFETIME.
    if (m_isInitialized)
        return /*true*/;

    setFeatures(QDockWidget::DockWidgetMovable);
    setAllowedAreas(Qt::AllDockWidgetAreas);
    setAcceptDrops(false);

    // TESTING: (18-Nov-2015)
    setMinimumWidth(CUfcCalibratorViewModel::Instance().GetAttribute<int>(GUI_SETTINGS_DOCK_MINIMUM_SIZE));
    // TESTING: (18-Nov-2015)
    setMinimumHeight(CUfcCalibratorViewModel::Instance().GetAttribute<int>(GUI_SETTINGS_DOCK_MINIMUM_SIZE));

    boost::shared_ptr<CUfcCalibratorModel> ufcCalibratorModel = CUfcCalibratorViewModel::Instance().GetUfcCalibratorModel();

    if (!ufcCalibratorModel)
        return /*true*/;

    boost::shared_ptr<CFootage> footage = ufcCalibratorModel->GetFootage();

    // BUG: (22-Nov-2016) IT MAY BE CALLED WHEN NO FOOTAGE IS SET
    if (!footage)
        return /*true*/;

    connect(this, SIGNAL(visibilityChanged(bool)), this, SLOT(SettingsChanged(bool)));

    QBoxLayout* layout = 0;

    QBoxLayout::Direction layoutDirection = QBoxLayout::TopToBottom;

    // TRICKY: (07-Sep-2016) Qt::LeftDockWidgetArea (1), Qt::BottomDockWidgetArea (8), Qt::RightDockWidgetArea (2), Qt::TopDockWidgetArea (4)
    int extrinsicMarkerSettingsDockArea = Qt::LeftDockWidgetArea;

    if ((extrinsicMarkerSettingsDockArea == 8) ||
        (extrinsicMarkerSettingsDockArea == 4))
    {
        layoutDirection = QBoxLayout::LeftToRight;
    }

    try
    {
        layout = new QBoxLayout(layoutDirection);
    }
    catch (std::exception& e)
    {
        LOG_MESSAGE(e.what());

        return /*false*/;
    }

    QGroupBox* markerWidget = CreateMarkerWidget();

    if (markerWidget)
        layout->addWidget(markerWidget);

    QGroupBox* optimizationSettingsWidget = CreateOptimizationSettingsWidget();

    if (optimizationSettingsWidget)
        layout->addWidget(optimizationSettingsWidget);

    QGroupBox* domainSettingsWidget = CreateIterationSettingsWidget();

    if (domainSettingsWidget)
        layout->addWidget(domainSettingsWidget);

    if (CUfcCalibratorViewModel::Instance().GetAttribute<bool>(GUI_EXPERT_MODE))
    {
        QGroupBox* cameraWidget = CreateCameraWidget();

        if (cameraWidget)
            layout->addWidget(cameraWidget);
    }

    layout->addStretch(1);

    QWidget* emptyWidgetAsLayout = 0;

    try
    {
        emptyWidgetAsLayout = new QWidget;
    }
    catch (std::exception& e)
    {
        LOG_MESSAGE(e.what());

        return /*false*/;
    }

    emptyWidgetAsLayout->setLayout(layout);

    QScrollArea* scrollArea = new QScrollArea;

    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setWidget(emptyWidgetAsLayout);

    setWidget(scrollArea);

    // (BEGIN OF) TESTING: (23-Aug-2022) INITIAL POPULATION!
    std::string presetsDirectoryName = "./data/camera/";

    HEALTH_CHECK(!boost::filesystem::exists(presetsDirectoryName), /*false*/);
    HEALTH_CHECK(!boost::filesystem::is_directory(presetsDirectoryName), /*false*/);

    std::list<boost::filesystem::path> presetFileNameArray;

    try
    {
        std::copy(boost::filesystem::directory_iterator(presetsDirectoryName), boost::filesystem::directory_iterator(), back_inserter(presetFileNameArray));
    }
    catch (...)
    {
        LOG_ERROR();
    }

    boost::shared_ptr<CMarkerGroup> extrinsicCalibrationMarkerGroup = footage->GetExtrinsicCalibrationMarkerGroup();

    for (const auto& presetFileName : presetFileNameArray)
    {
        try
        {
            boost::shared_ptr<CPinholeCamera2> presetPinholeCamera(new CPinholeCamera2);

            if (presetPinholeCamera->FromFile(presetFileName.generic_string()))
                m_cameraCalibration.AddCandidate(presetPinholeCamera, extrinsicCalibrationMarkerGroup);
        }
        catch (std::exception& e)
        {
            LOG_MESSAGE(e.what());
        }
    }
    // (END OF) TESTING: (23-Aug-2022) INITIAL POPULATION!

    // BUG: (30-Mar-2017) THE TABS ARE RE-INITIALIZED FOR EACH PLAY. THE WIDGETS SHOULD BE CREATED WHEN A VALID PLAY IS AVAILABLE, AND NOT RE-CREATED DURING THE APPLICATION LIFETIME.
    m_isInitialized = true;
}

// SET WIDGET STATE FROM FOOTAGE
void CExtrinsicCalibrationTab::UpdateViewFromModel()
{
    // OPIMIZATION SETTINGS

    if (m_iterationCountSpinBox)
        m_iterationCountSpinBox->setDisabled(true);
    if (m_populationSizeSpinBox)
        m_populationSizeSpinBox->setDisabled(true);
    if (m_selectedIndividualsCountSpinBox)
        m_selectedIndividualsCountSpinBox->setDisabled(true);
    if (m_maintainingDiversityCheckBox)
        m_maintainingDiversityCheckBox->setDisabled(true);
    if (m_mutationThresholdSpinBox)
        m_mutationThresholdSpinBox->setDisabled(true);
    if (m_updateExtrinsicCalibrationButton)
        m_updateExtrinsicCalibrationButton->setDisabled(true);

    // ITERATION SETTINGS

    if (m_opticalCenterStepSpinBox)
        m_opticalCenterStepSpinBox->setDisabled(true);
    if (m_fieldOfViewStepSpinBox)
        m_fieldOfViewStepSpinBox->setDisabled(true);
    if (m_yawStepSpinBox)
        m_yawStepSpinBox->setDisabled(true);
    if (m_pitchStepSpinBox)
        m_pitchStepSpinBox->setDisabled(true);
    if (m_rollStepSpinBox)
        m_rollStepSpinBox->setDisabled(true);
    if (m_iterationPresetsComboBox)
        m_iterationPresetsComboBox->setDisabled(true);

    // CAMERA

    if (m_distanceFromOriginSpinBox)
        m_distanceFromOriginSpinBox->setDisabled(true);
    if (m_centerOfProjectionXSpinBox)
        m_centerOfProjectionXSpinBox->setDisabled(true);
    if (m_centerOfProjectionYSpinBox)
        m_centerOfProjectionYSpinBox->setDisabled(true);
    if (m_centerOfProjectionZSpinBox)
        m_centerOfProjectionZSpinBox->setDisabled(true);
    if (m_fieldOfViewSpinBox)
        m_fieldOfViewSpinBox->setDisabled(true);
    if (m_rollSpinBox)
        m_rollSpinBox->setDisabled(true);

    boost::shared_ptr<CUfcCalibratorModel> ufcCalibratorModel = CUfcCalibratorViewModel::Instance().GetUfcCalibratorModel();

    if (!ufcCalibratorModel)
        return /*true*/;

    boost::shared_ptr<CFootage> footage = ufcCalibratorModel->GetFootage();

    // BUG: (22-Nov-2016) IT MAY BE CALLED WHEN NO FOOTAGE IS SET
    if (!footage)
        return /*true*/;

    for (std::vector<QAbstractButton *>::iterator markerButtonIterator = m_markerButtonArray.begin(); markerButtonIterator != m_markerButtonArray.end(); ++markerButtonIterator)
    {
        HEALTH_CHECK(!(*markerButtonIterator)->property("name").isValid(), /*false*/);
        HEALTH_CHECK((*markerButtonIterator)->property("name").toString().isEmpty(), /*false*/);

        std::string name = (*markerButtonIterator)->property("name").toString().toLocal8Bit().constData();

        boost::shared_ptr<my::video::CMarker> marker = footage->GetMarker(name);

        if (marker)
            (*markerButtonIterator)->setChecked(marker->IsEnabled());
        else
            LOG_MESSAGE("Failed to update the settings of the marker '" + name + "'");
    }

    // (BEGIN OF) TESTING: (22-Nov-2016) VIEWPROJECTIONMATRIX-BASED CALIBRATION
    boost::shared_ptr<CPinholeCamera2> pinholeCamera = footage->GetPinholeCamera();

    HEALTH_CHECK(!pinholeCamera, /*false*/);

    // OPIMIZATION SETTINGS

    if (m_iterationCountSpinBox)
    {
        m_iterationCountSpinBox->setEnabled(true);
        m_iterationCountSpinBox->setValue(m_cameraCalibration.GetIterationCount());
    }

    if (m_populationSizeSpinBox)
    {
        m_populationSizeSpinBox->setEnabled(true);
        m_populationSizeSpinBox->setValue(m_cameraCalibration.GetPopulationSize());
    }

    if (m_selectedIndividualsCountSpinBox)
    {
        m_selectedIndividualsCountSpinBox->setEnabled(true);
        m_selectedIndividualsCountSpinBox->setValue(m_cameraCalibration.GetSelectedIndividualsCount());
    }

    if (m_maintainingDiversityCheckBox)
    {
        m_maintainingDiversityCheckBox->setEnabled(true);
        m_maintainingDiversityCheckBox->setChecked(m_cameraCalibration.IsMaintainingDiversity());
    }

    if (m_mutationThresholdSpinBox)
    {
        m_mutationThresholdSpinBox->setEnabled(true);
        m_mutationThresholdSpinBox->setValue(m_cameraCalibration.GetMutationThreshold());
    }

    if (m_updateExtrinsicCalibrationButton)
        m_updateExtrinsicCalibrationButton->setEnabled(true);

    // ITERATION SETTINGS

    if (m_opticalCenterStepSpinBox)
        m_opticalCenterStepSpinBox->setValue(m_cameraCalibration.GetParameterStep(CPinholeCameraCalibration::OPTICAL_CENTER_PARAMETER));

    if (m_fieldOfViewStepSpinBox)
        m_fieldOfViewStepSpinBox->setValue(m_cameraCalibration.GetParameterStep(CPinholeCameraCalibration::FIELD_OF_VIEW_PARAMETER));

    if (m_yawStepSpinBox)
        m_yawStepSpinBox->setValue(m_cameraCalibration.GetParameterStep(CPinholeCameraCalibration::YAW_PARAMETER));

    if (m_pitchStepSpinBox)
        m_pitchStepSpinBox->setValue(m_cameraCalibration.GetParameterStep(CPinholeCameraCalibration::PITCH_PARAMETER));

    if (m_rollStepSpinBox)
        m_rollStepSpinBox->setValue(m_cameraCalibration.GetParameterStep(CPinholeCameraCalibration::ROLL_PARAMETER));

    if (m_iterationPresetsComboBox)
    {
        m_iterationPresetsComboBox->setEnabled(true);

        if (m_iterationPresetsComboBox->currentData().toInt() == 2)
        {
            if (m_opticalCenterStepSpinBox)
                m_opticalCenterStepSpinBox->setEnabled(true);
            if (m_fieldOfViewStepSpinBox)
                m_fieldOfViewStepSpinBox->setEnabled(true);
            if (m_yawStepSpinBox)
                m_yawStepSpinBox->setEnabled(true);
            if (m_pitchStepSpinBox)
                m_pitchStepSpinBox->setEnabled(true);
            if (m_rollStepSpinBox)
                m_rollStepSpinBox->setEnabled(true);
        }
    }

    // CAMERA

    my::CVector3<double> opticalCenter = pinholeCamera->GetOpticalCenter();

    if (m_distanceFromOriginSpinBox)
    {
        // DISABLED AFTER CENTER OF PROJECTION WAS ADDED AS A USER-DEFINED PARAMETER
        //m_distanceFromOriginSpinBox->setEnabled(true);
        m_distanceFromOriginSpinBox->setValue(opticalCenter.Length());
    }

    if (m_centerOfProjectionXSpinBox)
    {
        //m_centerOfProjectionXSpinBox->setEnabled(true);
        m_centerOfProjectionXSpinBox->setValue(opticalCenter.x());
    }

    if (m_centerOfProjectionYSpinBox)
    {
        //m_centerOfProjectionYSpinBox->setEnabled(true);
        m_centerOfProjectionYSpinBox->setValue(opticalCenter.y());
    }

    if (m_centerOfProjectionZSpinBox)
    {
        //m_centerOfProjectionZSpinBox->setEnabled(true);
        m_centerOfProjectionZSpinBox->setValue(opticalCenter.z());
    }

    if (m_fieldOfViewSpinBox)
    {
        //m_fieldOfViewSpinBox->setEnabled(true);
        m_fieldOfViewSpinBox->setValue(pinholeCamera->GetFieldOfView());
    }

    if (m_rollSpinBox)
    {
        my::CVector3<double> upAxis = pinholeCamera->GetUpAxis();

        //m_rollSpinBox->setEnabled(true);
        m_rollSpinBox->setValue(upAxis.Angle(my::CVector3<double>(0, 0, 1)));
    }
}

void CExtrinsicCalibrationTab::UpdateModelFromView()
{
    SettingsChanged();
}

void CExtrinsicCalibrationTab::IterationSettingsChanged(const QString&)
{
    switch (m_iterationPresetsComboBox->currentData().toInt()) {
    // Coarse-grained settings
    case 0:
        m_opticalCenterStepSpinBox->setValue(5.0);
        m_opticalCenterStepSpinBox->setDisabled(true);

        m_fieldOfViewStepSpinBox->setValue(1.0);
        m_fieldOfViewStepSpinBox->setDisabled(true);

        m_yawStepSpinBox->setValue(1.0);
        m_yawStepSpinBox->setDisabled(true);

        m_pitchStepSpinBox->setValue(1.0);
        m_pitchStepSpinBox->setDisabled(true);

        m_rollStepSpinBox->setValue(0.1);
        m_rollStepSpinBox->setDisabled(true);
        break;

    // Fine-grained settings
    case 1:
        m_opticalCenterStepSpinBox->setValue(1.0);
        m_opticalCenterStepSpinBox->setDisabled(true);

        m_fieldOfViewStepSpinBox->setValue(0.05);
        m_fieldOfViewStepSpinBox->setDisabled(true);

        m_yawStepSpinBox->setValue(0.1);
        m_yawStepSpinBox->setDisabled(true);

        m_pitchStepSpinBox->setValue(0.1);
        m_pitchStepSpinBox->setDisabled(true);

        m_rollStepSpinBox->setValue(0.01);
        m_rollStepSpinBox->setDisabled(true);
        break;

    // User-defined settings
    case 2:
        m_opticalCenterStepSpinBox->setEnabled(true);
        m_fieldOfViewStepSpinBox->setEnabled(true);
        m_yawStepSpinBox->setEnabled(true);
        m_pitchStepSpinBox->setEnabled(true);
        m_rollStepSpinBox->setEnabled(true);
        break;
    }
}

// SET FOOTAGE FROM WIDGET STATE 
void CExtrinsicCalibrationTab::SettingsChanged()
{
    boost::shared_ptr<CUfcCalibratorModel> ufcCalibratorModel = CUfcCalibratorViewModel::Instance().GetUfcCalibratorModel();

    if (!ufcCalibratorModel)
        return /*true*/;

    boost::shared_ptr<CFootage> footage = ufcCalibratorModel->GetFootage();

    if (!footage)
        return /*true*/;

    for (std::vector<QAbstractButton *>::iterator markerButtonIterator = m_markerButtonArray.begin(); markerButtonIterator != m_markerButtonArray.end(); ++markerButtonIterator)
    {
        HEALTH_CHECK(!(*markerButtonIterator)->property("name").isValid(), /*false*/);
        HEALTH_CHECK((*markerButtonIterator)->property("name").toString().isEmpty(), /*false*/);

        std::string name = (*markerButtonIterator)->property("name").toString().toLocal8Bit().constData();

        boost::shared_ptr<my::video::CMarker> marker = footage->GetMarker(name);

        if (marker)
            marker->SetEnabled((*markerButtonIterator)->isChecked());
        else
            LOG_MESSAGE("Failed to update the settings of the marker '" + name + "'");
    }

    if (m_updateCallback)
        m_updateCallback();
}

void CExtrinsicCalibrationTab::SettingsChanged(bool)
{
    SettingsChanged();
}

void CExtrinsicCalibrationTab::SettingsChanged(int)
{
    SettingsChanged();
}

void CExtrinsicCalibrationTab::SettingsChanged(double)
{
    SettingsChanged();
}

void CExtrinsicCalibrationTab::UpdateExtrinsicCalibration()
{
    // OPIMIZATION SETTINGS

    if (m_iterationCountSpinBox)
        m_cameraCalibration.SetIterationCount(m_iterationCountSpinBox->value());
    if (m_populationSizeSpinBox)
        m_cameraCalibration.SetPopulationSize(m_populationSizeSpinBox->value());
    if (m_selectedIndividualsCountSpinBox)
        m_cameraCalibration.SetSelectedIndividualsCount(m_selectedIndividualsCountSpinBox->value());
    if (m_maintainingDiversityCheckBox)
        m_cameraCalibration.SetMaintainingDiversity(m_maintainingDiversityCheckBox->isChecked());
    if (m_mutationThresholdSpinBox)
        m_cameraCalibration.SetMutationThreshold(m_mutationThresholdSpinBox->value());

    // ITERATION SETTINGS

    if (m_opticalCenterStepSpinBox)
    {
        double opticalCenterStep = m_opticalCenterStepSpinBox->value();

        m_cameraCalibration.SetParameterEnabled(CPinholeCameraCalibration::OPTICAL_CENTER_PARAMETER, !MyMath::IsZero(opticalCenterStep));
        m_cameraCalibration.SetParameterStep(CPinholeCameraCalibration::OPTICAL_CENTER_PARAMETER, opticalCenterStep);
    }

    if (m_fieldOfViewStepSpinBox)
    {
        double fieldOfViewStep = m_fieldOfViewStepSpinBox->value();

        m_cameraCalibration.SetParameterEnabled(CPinholeCameraCalibration::FIELD_OF_VIEW_PARAMETER, !MyMath::IsZero(fieldOfViewStep));
        m_cameraCalibration.SetParameterStep(CPinholeCameraCalibration::FIELD_OF_VIEW_PARAMETER, fieldOfViewStep);
    }

    if (m_yawStepSpinBox)
    {
        double yawStep = m_yawStepSpinBox->value();

        m_cameraCalibration.SetParameterEnabled(CPinholeCameraCalibration::YAW_PARAMETER, !MyMath::IsZero(yawStep));
        m_cameraCalibration.SetParameterStep(CPinholeCameraCalibration::YAW_PARAMETER, yawStep);
    }

    if (m_pitchStepSpinBox)
    {
        double pitchStep = m_pitchStepSpinBox->value();

        m_cameraCalibration.SetParameterEnabled(CPinholeCameraCalibration::PITCH_PARAMETER, !MyMath::IsZero(pitchStep));
        m_cameraCalibration.SetParameterStep(CPinholeCameraCalibration::PITCH_PARAMETER, pitchStep);
    }

    if (m_rollStepSpinBox)
    {
        double rollStep = m_rollStepSpinBox->value();

        m_cameraCalibration.SetParameterEnabled(CPinholeCameraCalibration::ROLL_PARAMETER, !MyMath::IsZero(rollStep));
        m_cameraCalibration.SetParameterStep(CPinholeCameraCalibration::ROLL_PARAMETER, rollStep);
    }

    boost::shared_ptr<CUfcCalibratorModel> ufcCalibratorModel = CUfcCalibratorViewModel::Instance().GetUfcCalibratorModel();

    if (!ufcCalibratorModel)
        return /*true*/;

    boost::shared_ptr<CFootage> footage = ufcCalibratorModel->GetFootage();

    HEALTH_CHECK(!footage, /*false*/);

    // (BEGIN OF) TESTING: (22-Nov-2016) VIEWPROJECTIONMATRIX-BASED CALIBRATION
    boost::shared_ptr<CPinholeCamera2> pinholeCamera = footage->GetPinholeCamera();

    HEALTH_CHECK(!pinholeCamera, /*false*/);

    boost::shared_ptr<CMarkerGroup> extrinsicCalibrationMarkerGroup = footage->GetExtrinsicCalibrationMarkerGroup();

    if (extrinsicCalibrationMarkerGroup)
    {
        if (!m_cameraCalibration.Update(pinholeCamera, extrinsicCalibrationMarkerGroup))
        {
            LOG_ERROR();

            return /*false*/;
        }

        if (!footage->SetPinholeCamera(m_cameraCalibration.GetPinholeCamera()))
        {
            LOG_ERROR();

            return /*false*/;
        }

        UpdateViewFromModel();
    }

    // TRICKY: (28-Sep-2022) A DUMB WAY TO MAKE SURE (?) THE MAIN WINDOW WILL BE REFRESHED AFTER AN UPDATING OF THE UNDERLYING DATA! THE MAIN WINDOW IS WAITING FOR THIS NULL EVENT.
    parent()->eventFilter(0, 0);
}

QGroupBox* CExtrinsicCalibrationTab::CreateMarkerWidget()
{
    boost::shared_ptr<CUfcCalibratorModel> ufcCalibratorModel = CUfcCalibratorViewModel::Instance().GetUfcCalibratorModel();

    if (!ufcCalibratorModel)
        return 0;

    boost::shared_ptr<CFootage> footage = ufcCalibratorModel->GetFootage();

    if (!footage)
        return 0;

    std::vector<boost::shared_ptr<my::video::CMarker> > markerArray = footage->GetExtrinsicCalibrationMarkerArray();

    QGridLayout *widgetLayout = 0;

    try
    {
        widgetLayout = new QGridLayout();
    }
    catch (std::exception& e)
    {
        LOG_MESSAGE(e.what());

        return 0;
    }

    int rowIndex = 0,
        columnIndex = 0,
        columnCount = 4;

    // TRICKY: (07-Sep-2016) Qt::LeftDockWidgetArea (1), Qt::BottomDockWidgetArea (8), Qt::RightDockWidgetArea (2), Qt::TopDockWidgetArea (4)
    int extrinsicMarkerSettingsDockArea = Qt::LeftDockWidgetArea;

    if ((extrinsicMarkerSettingsDockArea == 8) ||
        (extrinsicMarkerSettingsDockArea == 4))
    {
        columnCount = 7;
    }

    for (std::vector<boost::shared_ptr<my::video::CMarker> >::iterator markerIterator = markerArray.begin(); markerIterator != markerArray.end(); ++markerIterator)
    {
        QAbstractButton * markerButton = 0;

        try
        {
            markerButton = new QPushButton;
        }
        catch (std::exception& e)
        {
            LOG_MESSAGE(e.what());

            return 0;
        }

        markerButton->setCheckable(true);

        std::string name = (*markerIterator)->GetName(),
            icon = (*markerIterator)->GetIcon();

        // TRICKY: (18-Nov-2015)
        markerButton->setProperty("name", name.c_str());

        // TRICKY: (18-Nov-2015)
        markerButton->setIcon(QIcon(QString(":/images/marker_%1.png").arg(icon.c_str())));

        markerButton->setIconSize(QSize(48, 48));

        connect(markerButton, SIGNAL(clicked()), this, SLOT(SettingsChanged()));

        widgetLayout->addWidget(markerButton, rowIndex, columnIndex);

        ++columnIndex;

        if (columnIndex == columnCount)
        {
            ++rowIndex;

            columnIndex = 0;
        }

        m_markerButtonArray.push_back(markerButton);
    }

    QGroupBox *markerWidget = 0;

    try
    {
        markerWidget = new QGroupBox(UFC_STRING_RESOURCE_0017);
    }
    catch (std::exception& e)
    {
        LOG_MESSAGE(e.what());

        return 0;
    }

    markerWidget->setLayout(widgetLayout);

    return markerWidget;
}

QGroupBox* CExtrinsicCalibrationTab::CreateOptimizationSettingsWidget()
{
    QGroupBox *widget = 0;

    try
    {
        m_iterationCountSpinBox = new QSpinBox;
        m_iterationCountSpinBox->setRange(1, 8192);
        m_iterationCountSpinBox->setSingleStep(1);
        m_iterationCountSpinBox->setAlignment(Qt::AlignRight);

        m_populationSizeSpinBox = new QSpinBox;
        m_populationSizeSpinBox->setRange(1, 8192);
        m_populationSizeSpinBox->setSingleStep(1);
        m_populationSizeSpinBox->setAlignment(Qt::AlignRight);

        m_selectedIndividualsCountSpinBox = new QSpinBox;
        m_selectedIndividualsCountSpinBox->setRange(1, 8192);
        m_selectedIndividualsCountSpinBox->setSingleStep(1);
        m_selectedIndividualsCountSpinBox->setAlignment(Qt::AlignRight);

        m_maintainingDiversityCheckBox = new QCheckBox;
        m_maintainingDiversityCheckBox->setChecked(true);

        m_mutationThresholdSpinBox = new QDoubleSpinBox;
        m_mutationThresholdSpinBox->setRange(0, 1);
        m_mutationThresholdSpinBox->setDecimals(2);
        m_mutationThresholdSpinBox->setSingleStep(0.01);
        m_mutationThresholdSpinBox->setAlignment(Qt::AlignRight);

        m_updateExtrinsicCalibrationButton = new QPushButton(tr(UFC_STRING_RESOURCE_0114));
        connect(m_updateExtrinsicCalibrationButton, SIGNAL(clicked()), this, SLOT(UpdateExtrinsicCalibration()));

        // LAYOUT 

        QGridLayout *widgetLayout = 0;

        widgetLayout = new QGridLayout();

        if (CUfcCalibratorViewModel::Instance().GetAttribute<bool>(GUI_EXPERT_MODE))
        {
            widgetLayout->addWidget(new QLabel(tr(UFC_STRING_RESOURCE_0289) + ":"), 0, 0);
            widgetLayout->addWidget(m_iterationCountSpinBox, 0, 1, Qt::AlignRight);
            widgetLayout->addWidget(new QLabel(tr(UFC_STRING_RESOURCE_0290) + ":"), 1, 0);
            widgetLayout->addWidget(m_populationSizeSpinBox, 1, 1, Qt::AlignRight);
            widgetLayout->addWidget(new QLabel(tr(UFC_STRING_RESOURCE_0291) + ":"), 2, 0);
            widgetLayout->addWidget(m_selectedIndividualsCountSpinBox, 2, 1, Qt::AlignRight);
            widgetLayout->addWidget(new QLabel(tr(UFC_STRING_RESOURCE_0292) + ":"), 3, 0);
            widgetLayout->addWidget(m_maintainingDiversityCheckBox, 3, 1, Qt::AlignRight);
            widgetLayout->addWidget(new QLabel(tr(UFC_STRING_RESOURCE_0293) + ":"), 4, 0);
            widgetLayout->addWidget(m_mutationThresholdSpinBox, 4, 1, Qt::AlignRight);
            //widgetLayout->addWidget(/**/, 7, 0);
            widgetLayout->addWidget(m_updateExtrinsicCalibrationButton, 7, 1);
        }
        else
        {
            //widgetLayout->addWidget(/**/, 0, 0);
            widgetLayout->addWidget(m_updateExtrinsicCalibrationButton, 0, 1);
        }

        widget = new QGroupBox(UFC_STRING_RESOURCE_0023);

        widget->setLayout(widgetLayout);
    }
    catch (...)
    {
        LOG_ERROR();

#define DELETE_WIDGET(WIDGET)\
    if (WIDGET)\
                {\
        delete WIDGET;\
        WIDGET = 0;\
                }

        DELETE_WIDGET(m_iterationCountSpinBox);
        DELETE_WIDGET(m_populationSizeSpinBox);
        DELETE_WIDGET(m_selectedIndividualsCountSpinBox);
        DELETE_WIDGET(m_maintainingDiversityCheckBox);
        DELETE_WIDGET(m_mutationThresholdSpinBox);
        DELETE_WIDGET(m_updateExtrinsicCalibrationButton);

        return 0;
    }

    return widget;
}

QGroupBox* CExtrinsicCalibrationTab::CreateIterationSettingsWidget()
{
    QGroupBox *widget = 0;

    try
    {
        m_opticalCenterStepSpinBox = new QDoubleSpinBox;
        m_opticalCenterStepSpinBox->setRange(0, 10);
        m_opticalCenterStepSpinBox->setDecimals(2);
        m_opticalCenterStepSpinBox->setSingleStep(0);
        m_opticalCenterStepSpinBox->setAlignment(Qt::AlignRight);
        // Maintenance settings
        m_opticalCenterStepSpinBox->setValue(0.0);
        m_opticalCenterStepSpinBox->setDisabled(true);

        m_fieldOfViewStepSpinBox = new QDoubleSpinBox;
        m_fieldOfViewStepSpinBox->setRange(0, 5);
        m_fieldOfViewStepSpinBox->setDecimals(3);
        m_fieldOfViewStepSpinBox->setSingleStep(0.001);
        m_fieldOfViewStepSpinBox->setAlignment(Qt::AlignRight);
        // Maintenance settings
        m_fieldOfViewStepSpinBox->setValue(0.05);
        m_fieldOfViewStepSpinBox->setDisabled(true);

        m_yawStepSpinBox = new QDoubleSpinBox;
        m_yawStepSpinBox->setRange(0, 1);
        m_yawStepSpinBox->setDecimals(3);
        m_yawStepSpinBox->setSingleStep(0.001);
        m_yawStepSpinBox->setAlignment(Qt::AlignRight);
        // Maintenance settings
        m_yawStepSpinBox->setValue(0.1);
        m_yawStepSpinBox->setDisabled(true);

        m_pitchStepSpinBox = new QDoubleSpinBox;
        m_pitchStepSpinBox->setRange(0, 1);
        m_pitchStepSpinBox->setDecimals(3);
        m_pitchStepSpinBox->setSingleStep(0.001);
        m_pitchStepSpinBox->setAlignment(Qt::AlignRight);
        // Maintenance settings
        m_pitchStepSpinBox->setValue(0.1);
        m_pitchStepSpinBox->setDisabled(true);

        m_rollStepSpinBox = new QDoubleSpinBox;
        m_rollStepSpinBox->setRange(0, 1);
        m_rollStepSpinBox->setDecimals(3);
        m_rollStepSpinBox->setSingleStep(0.001);
        m_rollStepSpinBox->setAlignment(Qt::AlignRight);
        // Maintenance settings
        m_rollStepSpinBox->setValue(0.01);
        m_rollStepSpinBox->setDisabled(true);

        m_iterationPresetsComboBox = new QComboBox();

        m_iterationPresetsComboBox->addItem(UFC_STRING_RESOURCE_0305, QVariant((int)0));
        m_iterationPresetsComboBox->addItem(UFC_STRING_RESOURCE_0306, QVariant((int)1));

        if (CUfcCalibratorViewModel::Instance().GetAttribute<bool>(GUI_EXPERT_MODE))
            m_iterationPresetsComboBox->addItem(UFC_STRING_RESOURCE_0307, QVariant((int)2));

        m_iterationPresetsComboBox->setCurrentText(UFC_STRING_RESOURCE_0305);

        connect(m_iterationPresetsComboBox, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(IterationSettingsChanged(const QString &)));

        // LAYOUT 

        QGridLayout *widgetLayout = 0;

        widgetLayout = new QGridLayout();

        if (CUfcCalibratorViewModel::Instance().GetAttribute<bool>(GUI_EXPERT_MODE))
        {
            widgetLayout->addWidget(new QLabel(tr(UFC_STRING_RESOURCE_0294) + ":"), 0, 0);
            widgetLayout->addWidget(m_opticalCenterStepSpinBox, 0, 1, Qt::AlignRight);
            widgetLayout->addWidget(new QLabel(tr(UFC_STRING_RESOURCE_0295) + ":"), 1, 0);
            widgetLayout->addWidget(m_fieldOfViewStepSpinBox, 1, 1, Qt::AlignRight);
            widgetLayout->addWidget(new QLabel(tr(UFC_STRING_RESOURCE_0296) + ":"), 2, 0);
            widgetLayout->addWidget(m_yawStepSpinBox, 2, 1, Qt::AlignRight);
            widgetLayout->addWidget(new QLabel(tr(UFC_STRING_RESOURCE_0297) + ":"), 3, 0);
            widgetLayout->addWidget(m_pitchStepSpinBox, 3, 1, Qt::AlignRight);
            widgetLayout->addWidget(new QLabel(tr(UFC_STRING_RESOURCE_0298) + ":"), 4, 0);
            widgetLayout->addWidget(m_rollStepSpinBox, 4, 1, Qt::AlignRight);
            widgetLayout->addWidget(new QLabel(tr(UFC_STRING_RESOURCE_0308) + ":"), 5, 0);
            widgetLayout->addWidget(m_iterationPresetsComboBox, 5, 1, Qt::AlignRight);
        }
        else
        {
            widgetLayout->addWidget(new QLabel(tr(UFC_STRING_RESOURCE_0308) + ":"), 0, 0);
            widgetLayout->addWidget(m_iterationPresetsComboBox, 0, 1, Qt::AlignRight);
        }

        widget = new QGroupBox(UFC_STRING_RESOURCE_0324);

        widget->setLayout(widgetLayout);
    }
    catch (...)
    {
        LOG_ERROR();

        DELETE_WIDGET(m_opticalCenterStepSpinBox);
        DELETE_WIDGET(m_fieldOfViewStepSpinBox);
        DELETE_WIDGET(m_yawStepSpinBox);
        DELETE_WIDGET(m_pitchStepSpinBox);
        DELETE_WIDGET(m_rollStepSpinBox);
        DELETE_WIDGET(m_iterationPresetsComboBox);

        return 0;
    }

    return widget;
}

QGroupBox* CExtrinsicCalibrationTab::CreateCameraWidget()
{
    QGroupBox *widget = 0;

    try
    {
        m_distanceFromOriginSpinBox = new QDoubleSpinBox;
        m_distanceFromOriginSpinBox->setRange(0, 8192);
        m_distanceFromOriginSpinBox->setDecimals(3);
        m_distanceFromOriginSpinBox->setSingleStep(0.001);
        m_distanceFromOriginSpinBox->setAlignment(Qt::AlignRight);
        m_distanceFromOriginSpinBox->setDisabled(true);

        m_centerOfProjectionXSpinBox = new QDoubleSpinBox;
        m_centerOfProjectionXSpinBox->setRange(-1000, 1000);
        m_centerOfProjectionXSpinBox->setDecimals(3);
        m_centerOfProjectionXSpinBox->setSingleStep(0.001);
        m_centerOfProjectionXSpinBox->setAlignment(Qt::AlignRight);
        m_centerOfProjectionXSpinBox->setDisabled(true);

        m_centerOfProjectionYSpinBox = new QDoubleSpinBox;
        m_centerOfProjectionYSpinBox->setRange(-1000, 1000);
        m_centerOfProjectionYSpinBox->setDecimals(3);
        m_centerOfProjectionYSpinBox->setSingleStep(0.001);
        m_centerOfProjectionYSpinBox->setAlignment(Qt::AlignRight);
        m_centerOfProjectionYSpinBox->setDisabled(true);

        m_centerOfProjectionZSpinBox = new QDoubleSpinBox;
        m_centerOfProjectionZSpinBox->setRange(-1000, 1000);
        m_centerOfProjectionZSpinBox->setDecimals(3);
        m_centerOfProjectionZSpinBox->setSingleStep(0.001);
        m_centerOfProjectionZSpinBox->setAlignment(Qt::AlignRight);
        m_centerOfProjectionZSpinBox->setDisabled(true);

        m_fieldOfViewSpinBox = new QDoubleSpinBox;
        m_fieldOfViewSpinBox->setRange(0, 60);
        m_fieldOfViewSpinBox->setDecimals(3);
        m_fieldOfViewSpinBox->setSingleStep(0.001);
        m_fieldOfViewSpinBox->setAlignment(Qt::AlignRight);
        m_fieldOfViewSpinBox->setDisabled(true);

        m_rollSpinBox = new QDoubleSpinBox;
        m_rollSpinBox->setRange(0, 60);
        m_rollSpinBox->setDecimals(3);
        m_rollSpinBox->setSingleStep(0.001);
        m_rollSpinBox->setAlignment(Qt::AlignRight);
        m_rollSpinBox->setDisabled(true);

        // LAYOUT 

        QGridLayout *widgetLayout = 0;

        widgetLayout = new QGridLayout();

        widgetLayout->addWidget(new QLabel(tr(UFC_STRING_RESOURCE_0301) + ":"), 0, 0);
        widgetLayout->addWidget(m_distanceFromOriginSpinBox, 0, 1, Qt::AlignRight);
        widgetLayout->addWidget(new QLabel(tr(UFC_STRING_RESOURCE_0304) + ":"), 1, 0);
        widgetLayout->addWidget(m_centerOfProjectionXSpinBox, 1, 1, Qt::AlignRight);
        widgetLayout->addWidget(new QLabel(tr(UFC_STRING_RESOURCE_0465) + ":"), 2, 0);
        widgetLayout->addWidget(m_centerOfProjectionYSpinBox, 2, 1, Qt::AlignRight);
        widgetLayout->addWidget(new QLabel(tr(UFC_STRING_RESOURCE_0466) + ":"), 3, 0);
        widgetLayout->addWidget(m_centerOfProjectionZSpinBox, 3, 1, Qt::AlignRight);
        widgetLayout->addWidget(new QLabel(tr(UFC_STRING_RESOURCE_0302) + ":"), 4, 0);
        widgetLayout->addWidget(m_fieldOfViewSpinBox, 4, 1, Qt::AlignRight);
        widgetLayout->addWidget(new QLabel(tr(UFC_STRING_RESOURCE_0303) + ":"), 5, 0);
        widgetLayout->addWidget(m_rollSpinBox, 5, 1, Qt::AlignRight);

        widget = new QGroupBox(UFC_STRING_RESOURCE_0325);

        widget->setLayout(widgetLayout);
    }
    catch (...)
    {
        LOG_ERROR();

        DELETE_WIDGET(m_distanceFromOriginSpinBox);
        DELETE_WIDGET(m_centerOfProjectionXSpinBox);
        DELETE_WIDGET(m_centerOfProjectionYSpinBox);
        DELETE_WIDGET(m_centerOfProjectionZSpinBox);
        DELETE_WIDGET(m_fieldOfViewSpinBox);
        DELETE_WIDGET(m_rollSpinBox);

        return 0;
    }

    return widget;
}

void CExtrinsicCalibrationTab::Create()
{
    m_isInitialized = false;
    m_markerButtonArray.clear();
    // OPIMIZATION SETTINGS
    m_iterationCountSpinBox = 0;
    m_populationSizeSpinBox = 0;
    m_selectedIndividualsCountSpinBox = 0;
    m_maintainingDiversityCheckBox = 0;
    m_mutationThresholdSpinBox = 0;
    m_updateExtrinsicCalibrationButton = 0;
    // ITERATION SETTINGS
    m_opticalCenterStepSpinBox = 0;
    m_fieldOfViewStepSpinBox = 0;
    m_yawStepSpinBox = 0;
    m_pitchStepSpinBox = 0;
    m_rollStepSpinBox = 0;
    m_iterationPresetsComboBox = 0;
    // CAMERA
    m_distanceFromOriginSpinBox = 0;
    m_centerOfProjectionXSpinBox = 0;
    m_centerOfProjectionYSpinBox = 0;
    m_centerOfProjectionZSpinBox = 0;    
    m_fieldOfViewSpinBox = 0;
    m_rollSpinBox = 0;
    // TESTING: (24-Nov-2016) TESTING THE USE OF A LOCAL CALIBRATION TOOL
    //m_cameraCalibration;
}

void CExtrinsicCalibrationTab::Destroy()
{
}
