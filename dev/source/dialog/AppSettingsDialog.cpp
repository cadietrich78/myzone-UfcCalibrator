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

#include <qpushbutton.h>
#include <qboxlayout.h>
#include <qmessagebox.h>

#include <Logger.h>

#include "../StringResource.h"
#include "../viewmodel/UfcCalibratorViewModel.h"

#include "AppSettingsDialog.h"

CAppSettingsDialog::CAppSettingsDialog(QWidget *parent)
    : QDialog(parent)
{
    Create();

    try {
        m_contentsWidget = new QListWidget;

        m_contentsWidget->setViewMode(QListView::IconMode);
        m_contentsWidget->setIconSize(QSize(64, 64));

        // 91 IS THE SIZE OF THE ICON (64) PLUS THE SIZE OF THE SCROLLBAR
        m_contentsWidget->setMaximumWidth(91);

        m_contentsWidget->setMinimumHeight(360);
        m_contentsWidget->setMaximumHeight(560);

        m_pagesWidget = new QStackedWidget;

        m_appGuiSettingsGroupDialog = new CAppGuiSettingsGroupDialog();
        m_appGuiSettingsGroupDialog->CreateIcon(m_contentsWidget);

        m_pagesWidget->addWidget(m_appGuiSettingsGroupDialog);

        connect(m_contentsWidget, SIGNAL(currentItemChanged(QListWidgetItem *, QListWidgetItem *)), this, SLOT(ChangePage(QListWidgetItem *, QListWidgetItem *)));

        m_contentsWidget->setCurrentRow(0);

        QPushButton *cancelButton = new QPushButton(tr(UFC_STRING_RESOURCE_0134)),
            *okButton = new QPushButton(tr(UFC_STRING_RESOURCE_0133));

        connect(cancelButton, SIGNAL(clicked()), this, SLOT(close()));
        connect(okButton, SIGNAL(clicked()), this, SLOT(SaveSettings()));

        QHBoxLayout *horizontalLayout = new QHBoxLayout;

        horizontalLayout->addWidget(m_contentsWidget);
        horizontalLayout->addWidget(m_pagesWidget, 1);

        QHBoxLayout *buttonsLayout = new QHBoxLayout;
        buttonsLayout->addStretch(1);

        // THE EXACT VALUE TO STRETCH THIS BOX FOR DIALOG SIZE.
        buttonsLayout->addSpacing(48);

        buttonsLayout->addWidget(cancelButton);
        buttonsLayout->addWidget(okButton);

        QVBoxLayout *mainLayout = new QVBoxLayout;

        mainLayout->addLayout(horizontalLayout);

        mainLayout->addStretch(1);
        mainLayout->addSpacing(12);

        mainLayout->addLayout(buttonsLayout);

        setLayout(mainLayout);

        setMinimumWidth(500);
    }
    catch (...) {
        LOG_ERROR();

#define DELETE_WIDGET(WIDGET)\
    if (WIDGET)\
            {\
        delete WIDGET;\
        WIDGET = 0;\
            }

        DELETE_WIDGET(m_appGuiSettingsGroupDialog);
        DELETE_WIDGET(m_contentsWidget);
        DELETE_WIDGET(m_pagesWidget);
    }

    setWindowTitle(tr(UFC_STRING_RESOURCE_0135));
}

void CAppSettingsDialog::ChangePage(QListWidgetItem *current, QListWidgetItem *previous)
{
    if (!current)
        current = previous;

    m_pagesWidget->setCurrentIndex(m_contentsWidget->row(current));
}

void CAppSettingsDialog::SaveSettings()
{
    bool isRestartRequired = false;

    // GUI

    if (m_appGuiSettingsGroupDialog)
    {
        CUfcCalibratorViewModel::Instance().SetAttribute(GUI_EXPERT_MODE, m_appGuiSettingsGroupDialog->IsExpertModeEnabled());

        int guiTextSize = m_appGuiSettingsGroupDialog->GetGuiTextSize();
        
        int fieldLayoutLineThickness = m_appGuiSettingsGroupDialog->GetGuiFieldLayoutLineThickness();

        CUfcCalibratorViewModel::Instance().SetAttribute(GUI_FIELD_LAYOUT_LINE_THICKNESS_ITEM, fieldLayoutLineThickness);

        CUfcCalibratorViewModel::Instance().SetAttribute(GUI_TEXT_SIZE_ITEM, guiTextSize);

        CUfcCalibratorViewModel::Instance().SetAttribute(GUI_PLAY_KEY, m_appGuiSettingsGroupDialog->GetPlayKey());
        CUfcCalibratorViewModel::Instance().SetAttribute(GUI_PREVIOUS_FRAME_KEY, m_appGuiSettingsGroupDialog->GetPreviousFrameKey());
        CUfcCalibratorViewModel::Instance().SetAttribute(GUI_NEXT_FRAME_KEY, m_appGuiSettingsGroupDialog->GetNextFrameKey());

        CUfcCalibratorViewModel::Instance().SetAttribute(GUI_OCTAGON_SIZE, m_appGuiSettingsGroupDialog->GetOctagonSize());

        isRestartRequired |= m_appGuiSettingsGroupDialog->IsRestartRequired();
    }

    if (isRestartRequired)
    {
        QMessageBox::information(this, tr("Application settings"), tr("<p>When making changes to some application settings, "\
            "you need to restart the application for the changes to take effect."));
    }

    close();
}

void CAppSettingsDialog::Create()
{
    m_appGuiSettingsGroupDialog = 0;
    m_contentsWidget = 0;
    m_pagesWidget = 0;
}

