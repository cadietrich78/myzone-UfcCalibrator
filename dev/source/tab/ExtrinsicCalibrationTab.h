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

#if !defined(EXTRINSIC_CALIBRATION_TAB_INCLUDED)
#define EXTRINSIC_CALIBRATION_TAB_INCLUDED

#include "../calibration/PinholeCameraCalibration.h"

#include "TabInterface.h"

class CExtrinsicCalibrationTab
    : public CTabInterface
{
    Q_OBJECT

public:
    CExtrinsicCalibrationTab(QWidget * parent = 0, Qt::WindowFlags flags = 0);

    ~CExtrinsicCalibrationTab();

    OBJECT_TYPE GetType() const;

    void Initialize();

    void UpdateViewFromModel();
    void UpdateModelFromView();

protected slots:
    void IterationSettingsChanged(const QString&);

    void SettingsChanged();
    void SettingsChanged(bool);
    void SettingsChanged(int);
    void SettingsChanged(double);

    void ResetExtrinsicCalibration();
    void UpdateExtrinsicCalibration();

private:
    QGroupBox* CreateMarkerWidget();
    QGroupBox* CreateOptimizationSettingsWidget();
    QGroupBox* CreateIterationSettingsWidget();
    QGroupBox* CreateCameraWidget();

    void Create();
    void Destroy();

protected:
     bool m_isInitialized;

    std::vector<QAbstractButton *> m_markerButtonArray;

    QSpinBox *m_iterationCountSpinBox;
    QSpinBox *m_populationSizeSpinBox;
    QSpinBox *m_selectedIndividualsCountSpinBox;
    QCheckBox *m_maintainingDiversityCheckBox;
    QDoubleSpinBox *m_mutationThresholdSpinBox;
    QAbstractButton* m_updateExtrinsicCalibrationButton;

    QDoubleSpinBox *m_opticalCenterStepSpinBox;
    QDoubleSpinBox *m_fieldOfViewStepSpinBox;
    QDoubleSpinBox *m_yawStepSpinBox;
    QDoubleSpinBox *m_pitchStepSpinBox;
    QDoubleSpinBox *m_rollStepSpinBox;
    QComboBox *m_iterationPresetsComboBox;

    QDoubleSpinBox *m_distanceFromOriginSpinBox;
    QDoubleSpinBox *m_centerOfProjectionXSpinBox;
    QDoubleSpinBox *m_centerOfProjectionYSpinBox;
    QDoubleSpinBox *m_centerOfProjectionZSpinBox;
    QDoubleSpinBox *m_fieldOfViewSpinBox;
    QDoubleSpinBox *m_rollSpinBox;

    CPinholeCameraCalibration m_cameraCalibration;
};

#endif //#if !defined(EXTRINSIC_CALIBRATION_TAB_INCLUDED)

