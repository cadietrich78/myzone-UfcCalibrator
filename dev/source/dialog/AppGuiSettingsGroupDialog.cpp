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

#include <QGroupBox>
#include <QLabel>
#include <QGridLayout>
#include <QScrollArea>

#include <Logger.h>

#include "../StringResource.h"
#include "../viewmodel/UfcCalibratorViewModel.h"

#include "AppGuiSettingsGroupDialog.h"

CAppGuiSettingsGroupDialog::CAppGuiSettingsGroupDialog(QWidget *parent)
    : CAppSettingsGroupDialog(parent)
{
    Create();

    try {
        // WINDOWS SETTINGS GROUP /////////////////////////////////////////////

        QGroupBox *windowsSettingsGroup = new QGroupBox(tr(UFC_STRING_RESOURCE_0152));

        m_windowExpertModeCheckBox = new QCheckBox;
        m_windowExpertModeCheckBox->setChecked(CUfcCalibratorViewModel::Instance().GetAttribute<bool>(GUI_EXPERT_MODE));
        connect(m_windowExpertModeCheckBox, SIGNAL(stateChanged(int)), this, SLOT(ExpertModeChanged(int)));

        // DOMAIN-SPECIFIC SETTING PLACEHOLDER!
        //m_octagonSizeComboBox = CreateOctagonSizeComboBox();

        // STRIKE ZONE GROUP //////////////////////////////////////////////////

        QGroupBox *guiSettingsGroup = new QGroupBox(tr(UFC_STRING_RESOURCE_0141));

        // TEXT SIZE

        m_guiTextSizeComboBox = new QComboBox();

        m_guiTextSizeComboBox->addItem("10", QVariant(10));
        m_guiTextSizeComboBox->addItem("12", QVariant(12));
        m_guiTextSizeComboBox->addItem("18", QVariant(18));

        int guiTextSize = CUfcCalibratorViewModel::Instance().GetAttribute<int>(GUI_TEXT_SIZE_ITEM);

        if (my::IsNull(guiTextSize))
            guiTextSize = 10;

        m_guiTextSizeComboBox->setCurrentText(std::to_string(guiTextSize).c_str());

        // FIELD LAYOUT LINE THICKNESS

        m_guiFieldLayoutLineThicknessSpinBox = new QSpinBox;

        m_guiFieldLayoutLineThicknessSpinBox->setRange(1, 8);
        m_guiFieldLayoutLineThicknessSpinBox->setSingleStep(1);

        int fieldLayoutLineThickness = CUfcCalibratorViewModel::Instance().GetAttribute<int>(GUI_FIELD_LAYOUT_LINE_THICKNESS_ITEM);

        if (my::IsNull(fieldLayoutLineThickness))
            fieldLayoutLineThickness = 1;

        m_guiFieldLayoutLineThicknessSpinBox->setValue(fieldLayoutLineThickness);

        QGridLayout* windowSettingsLayout = new QGridLayout;

        windowSettingsLayout->addWidget(new QLabel(tr(UFC_STRING_RESOURCE_0003) + tr(":")), 0, 0);
        windowSettingsLayout->addWidget(m_windowExpertModeCheckBox, 0, 1, Qt::AlignRight);

        // DOMAIN-SPECIFIC SETTING PLACEHOLDER!
        //windowSettingsLayout->addWidget(new QLabel(tr(UFC_STRING_RESOURCE_0028) + tr(":")), 1, 0);
        //windowSettingsLayout->addWidget(m_octagonSizeComboBox, 1, 1, Qt::AlignRight);

        windowsSettingsGroup->setLayout(windowSettingsLayout);

        // STRIKE ZONE LAYOUT

        QGridLayout *guiSettingsLayout = new QGridLayout;

        guiSettingsLayout->addWidget(new QLabel(tr(UFC_STRING_RESOURCE_0142) + tr(":")), 0, 0);
        guiSettingsLayout->addWidget(m_guiTextSizeComboBox, 0, 1, Qt::AlignRight);
        guiSettingsLayout->addWidget(new QLabel(tr(UFC_STRING_RESOURCE_0193) + tr(":")), 1, 0);
        guiSettingsLayout->addWidget(m_guiFieldLayoutLineThicknessSpinBox, 1, 1, Qt::AlignRight);

        guiSettingsGroup->setLayout(guiSettingsLayout);

        // KEY ASSIGNMENT

        QGroupBox *keyAssignmentGroup = new QGroupBox(tr(UFC_STRING_RESOURCE_0537));

        m_playKeyComboBox = CreateKeyAssignmentComboBox(GUI_PLAY_KEY);
        m_previousFrameKeyComboBox = CreateKeyAssignmentComboBox(GUI_PREVIOUS_FRAME_KEY);
        m_nextFrameKeyComboBox = CreateKeyAssignmentComboBox(GUI_NEXT_FRAME_KEY);

        QGridLayout *keyAssignmentLayout = new QGridLayout;

        keyAssignmentLayout->addWidget(new QLabel(tr(UFC_STRING_RESOURCE_0540) + tr(":")), 0, 0);
        keyAssignmentLayout->addWidget(m_playKeyComboBox, 0, 1, Qt::AlignRight);
        keyAssignmentLayout->addWidget(new QLabel(tr(UFC_STRING_RESOURCE_0541) + tr(":")), 1, 0);
        keyAssignmentLayout->addWidget(m_previousFrameKeyComboBox, 1, 1, Qt::AlignRight);
        keyAssignmentLayout->addWidget(new QLabel(tr(UFC_STRING_RESOURCE_0542) + tr(":")), 2, 0);
        keyAssignmentLayout->addWidget(m_nextFrameKeyComboBox, 2, 1, Qt::AlignRight);
        
        keyAssignmentGroup->setLayout(keyAssignmentLayout);
        
        // MAIN LAYOUT

        // (BEGIN OF) TESTING: (08-Sep-2016) http://www.qtcentre.org/threads/42635-Add-QScrollArea-to-QDialog
        QScrollArea *dialogScrollArea = new QScrollArea(this);

        //dialogScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        dialogScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        
        QWidget *dialogViewport = new QWidget(this);

        dialogScrollArea->setWidget(dialogViewport);

        dialogScrollArea->setWidgetResizable(true);

        QVBoxLayout *verticalLayout = new QVBoxLayout(dialogViewport);

        dialogViewport->setLayout(verticalLayout);

        verticalLayout->addWidget(windowsSettingsGroup);
        verticalLayout->addWidget(guiSettingsGroup);
        verticalLayout->addWidget(keyAssignmentGroup);

        QHBoxLayout *dialogLayout = new QHBoxLayout(this);

        dialogLayout->addWidget(dialogScrollArea);

        setLayout(dialogLayout);
        // (END OF) TESTING: (08-Sep-2016) http://www.qtcentre.org/threads/42635-Add-QScrollArea-to-QDialog
    }
    catch (...) {
        LOG_ERROR();

#define DELETE_WIDGET(WIDGET)\
    if (WIDGET)\
        {\
        delete WIDGET;\
        WIDGET = 0;\
        }

        DELETE_WIDGET(m_windowExpertModeCheckBox);
        // DOMAIN-SPECIFIC SETTING PLACEHOLDER!
        //DELETE_WIDGET(m_octagonSizeComboBox);
        DELETE_WIDGET(m_guiTextSizeComboBox);
        DELETE_WIDGET(m_guiFieldLayoutLineThicknessSpinBox);
        DELETE_WIDGET(m_playKeyComboBox);
        DELETE_WIDGET(m_previousFrameKeyComboBox);
        DELETE_WIDGET(m_nextFrameKeyComboBox);
    }

    m_isRestartRequired = false;
}

QListWidgetItem *CAppGuiSettingsGroupDialog::CreateIcon(QListWidget *parent) const
{
    QListWidgetItem *guiItem = 0;

    try 
    {
        guiItem = new QListWidgetItem(parent);

        guiItem->setIcon(QIcon(":/images/personal.png"));
        guiItem->setText(tr("GUI"));
        guiItem->setTextAlignment(Qt::AlignHCenter);
        guiItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    }
    catch (...)
    {
        LOG_ERROR();

        guiItem = 0;
    }

    return guiItem;
}

bool CAppGuiSettingsGroupDialog::IsExpertModeEnabled() const
{
    HEALTH_CHECK(!m_windowExpertModeCheckBox, false);

    return m_windowExpertModeCheckBox->isChecked();
}

int CAppGuiSettingsGroupDialog::GetGuiTextSize() const
{
    HEALTH_CHECK(!m_guiTextSizeComboBox, my::Null<int>());

    return m_guiTextSizeComboBox->currentData().toInt();
}

void CAppGuiSettingsGroupDialog::SetGuiTextSize(int guiTextSize)
{
    HEALTH_CHECK(!m_guiTextSizeComboBox, /*false*/);

    m_guiTextSizeComboBox->setCurrentText(std::to_string(guiTextSize).c_str());
}

int CAppGuiSettingsGroupDialog::GetGuiFieldLayoutLineThickness() const
{
    HEALTH_CHECK(!m_guiFieldLayoutLineThicknessSpinBox, my::Null<int>());

    return m_guiFieldLayoutLineThicknessSpinBox->value();
}

int CAppGuiSettingsGroupDialog::GetPlayKey() const
{
    HEALTH_CHECK(!m_playKeyComboBox, my::Null<int>());

    return m_playKeyComboBox->currentData().toInt();
}

int CAppGuiSettingsGroupDialog::GetPreviousFrameKey() const
{
    HEALTH_CHECK(!m_previousFrameKeyComboBox, my::Null<int>());

    return m_previousFrameKeyComboBox->currentData().toInt();
}

int CAppGuiSettingsGroupDialog::GetNextFrameKey() const
{
    HEALTH_CHECK(!m_nextFrameKeyComboBox, my::Null<int>());

    return m_nextFrameKeyComboBox->currentData().toInt();
}

// DOMAIN-SPECIFIC SETTING PLACEHOLDER!
//int CAppGuiSettingsGroupDialog::GetOctagonSize() const
//{
//    HEALTH_CHECK(!m_octagonSizeComboBox, my::Null<int>());
//
//    return m_octagonSizeComboBox->currentData().toInt();
//}

bool CAppGuiSettingsGroupDialog::IsRestartRequired() const
{
    return m_isRestartRequired;
}

// DOMAIN-SPECIFIC SETTING PLACEHOLDER!
//void CAppGuiSettingsGroupDialog::OctagonSizeChanged(const QString&)
//{
//    UpdateRestartRequired(true);
//}

void CAppGuiSettingsGroupDialog::ExpertModeChanged(int state)
{
    UpdateRestartRequired(true);
}

void CAppGuiSettingsGroupDialog::KeyAssignmentChanged(const QString &text)
{
    // TODO: (31-Aug-2017) CHECK OVERLAPPINGS
}

void CAppGuiSettingsGroupDialog::UpdateRestartRequired(bool)
{
    m_isRestartRequired = true;
}

QComboBox *CAppGuiSettingsGroupDialog::CreateKeyAssignmentComboBox(OBJECT_TYPE keyAssignmentId) const
{
    int keyAssignment = CUfcCalibratorViewModel::Instance().GetAttribute<int>(keyAssignmentId);

    HEALTH_CHECK(my::IsNull(keyAssignment), 0);
        
    QComboBox *keyAssignmentComboBox = new QComboBox();

    // TRICKY: (31-Aug-2017) FOLLOWING QT CONVENTIONS
    std::map<int, std::string> keyIndexToNameMap;

    keyIndexToNameMap[0x01000012] = UFC_STRING_RESOURCE_0487;
    keyIndexToNameMap[0x01000013] = UFC_STRING_RESOURCE_0488;
    keyIndexToNameMap[0x01000014] = UFC_STRING_RESOURCE_0489;
    keyIndexToNameMap[0x01000015] = UFC_STRING_RESOURCE_0490;
    keyIndexToNameMap[0x20] = UFC_STRING_RESOURCE_0491;
    keyIndexToNameMap[0x2b] = UFC_STRING_RESOURCE_0492;
    keyIndexToNameMap[0x2c] = UFC_STRING_RESOURCE_0493;
    keyIndexToNameMap[0x2d] = UFC_STRING_RESOURCE_0494;
    keyIndexToNameMap[0x2e] = UFC_STRING_RESOURCE_0495;
    keyIndexToNameMap[0x2f] = UFC_STRING_RESOURCE_0496;
    keyIndexToNameMap[0x30] = UFC_STRING_RESOURCE_0497;
    keyIndexToNameMap[0x31] = UFC_STRING_RESOURCE_0498;
    keyIndexToNameMap[0x32] = UFC_STRING_RESOURCE_0499;
    keyIndexToNameMap[0x33] = UFC_STRING_RESOURCE_0500;
    keyIndexToNameMap[0x34] = UFC_STRING_RESOURCE_0501;
    keyIndexToNameMap[0x35] = UFC_STRING_RESOURCE_0502;
    keyIndexToNameMap[0x36] = UFC_STRING_RESOURCE_0503;
    keyIndexToNameMap[0x37] = UFC_STRING_RESOURCE_0504;
    keyIndexToNameMap[0x38] = UFC_STRING_RESOURCE_0505;
    keyIndexToNameMap[0x39] = UFC_STRING_RESOURCE_0506;
    keyIndexToNameMap[0x3b] = UFC_STRING_RESOURCE_0507;
    keyIndexToNameMap[0x41] = UFC_STRING_RESOURCE_0508;
    keyIndexToNameMap[0x42] = UFC_STRING_RESOURCE_0509;
    keyIndexToNameMap[0x43] = UFC_STRING_RESOURCE_0510;
    keyIndexToNameMap[0x44] = UFC_STRING_RESOURCE_0511;
    keyIndexToNameMap[0x45] = UFC_STRING_RESOURCE_0512;
    keyIndexToNameMap[0x46] = UFC_STRING_RESOURCE_0513;
    keyIndexToNameMap[0x47] = UFC_STRING_RESOURCE_0514;
    keyIndexToNameMap[0x48] = UFC_STRING_RESOURCE_0515;
    keyIndexToNameMap[0x49] = UFC_STRING_RESOURCE_0516;
    keyIndexToNameMap[0x4a] = UFC_STRING_RESOURCE_0517;
    keyIndexToNameMap[0x4b] = UFC_STRING_RESOURCE_0518;
    keyIndexToNameMap[0x4c] = UFC_STRING_RESOURCE_0519;
    keyIndexToNameMap[0x4d] = UFC_STRING_RESOURCE_0520;
    keyIndexToNameMap[0x4e] = UFC_STRING_RESOURCE_0521;
    keyIndexToNameMap[0x4f] = UFC_STRING_RESOURCE_0522;
    keyIndexToNameMap[0x50] = UFC_STRING_RESOURCE_0523;
    keyIndexToNameMap[0x51] = UFC_STRING_RESOURCE_0524;
    keyIndexToNameMap[0x52] = UFC_STRING_RESOURCE_0525;
    keyIndexToNameMap[0x53] = UFC_STRING_RESOURCE_0526;
    keyIndexToNameMap[0x54] = UFC_STRING_RESOURCE_0527;
    keyIndexToNameMap[0x55] = UFC_STRING_RESOURCE_0528;
    keyIndexToNameMap[0x56] = UFC_STRING_RESOURCE_0529;
    keyIndexToNameMap[0x57] = UFC_STRING_RESOURCE_0530;
    keyIndexToNameMap[0x58] = UFC_STRING_RESOURCE_0531;
    keyIndexToNameMap[0x59] = UFC_STRING_RESOURCE_0532;
    keyIndexToNameMap[0x5a] = UFC_STRING_RESOURCE_0533;
    keyIndexToNameMap[0x5b] = UFC_STRING_RESOURCE_0534;
    keyIndexToNameMap[0x5c] = UFC_STRING_RESOURCE_0535;
    keyIndexToNameMap[0x5d] = UFC_STRING_RESOURCE_0536;
    keyIndexToNameMap[0x3d] = UFC_STRING_RESOURCE_0553;

    for (std::map<int, std::string>::const_iterator keyIndexToNameIterator = keyIndexToNameMap.begin(); keyIndexToNameIterator != keyIndexToNameMap.end(); ++keyIndexToNameIterator)
    {
        keyAssignmentComboBox->addItem(tr(keyIndexToNameIterator->second.c_str()), QVariant(keyIndexToNameIterator->first));
    }

    if (keyIndexToNameMap.find(keyAssignment) == keyIndexToNameMap.end())
    {
        LOG_ERROR();

        return 0;
    }

    keyAssignmentComboBox->setCurrentText(tr(keyIndexToNameMap[keyAssignment].c_str()));

    connect(keyAssignmentComboBox, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(KeyAssignmentChanged(const QString &)));

    return keyAssignmentComboBox;
}

// DOMAIN-SPECIFIC SETTING PLACEHOLDER!
//QComboBox* CAppGuiSettingsGroupDialog::CreateOctagonSizeComboBox() const
//{
//    QComboBox* octagonSizeComboBox = new QComboBox();
//
//    octagonSizeComboBox->addItem("25", QVariant(25));
//    octagonSizeComboBox->addItem("30", QVariant(30));
//
//    int octagonSize = CUfcCalibratorViewModel::Instance().GetAttribute<int>(GUI_OCTAGON_SIZE);
//
//    octagonSizeComboBox->setCurrentText(std::to_string(octagonSize).c_str());
//
//    connect(octagonSizeComboBox, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(OctagonSizeChanged(const QString&)));
//
//    return octagonSizeComboBox;
//}

void CAppGuiSettingsGroupDialog::Create()
{
    m_guiTextSizeComboBox = 0;
    m_guiFieldLayoutLineThicknessSpinBox = 0;
    m_playKeyComboBox = 0;
    m_previousFrameKeyComboBox = 0;
    m_nextFrameKeyComboBox = 0;
    m_isRestartRequired = false;
}

