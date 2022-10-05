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

#if !defined(MAIN_WINDOW_INCLUDED)
#define MAIN_WINDOW_INCLUDED

#include <QMainWindow>
#include <QtWidgets/QtWidgets>
#include <QSound>

#include <Common.h>

#include "glwidget.h"
#include "MyQTimer.h"
#include "viewmodel/MessageQueue.h"

#include "tab/TabInterface.h"

class QResizeableTreeView
    : public QTreeView
{
    Q_OBJECT

public:
    QResizeableTreeView(QWidget *parent = 0)
        : QTreeView(parent),
        m_width(my::Null<int>()),
        m_height(my::Null<int>())
    {
    }

    QSize sizeHint() const
    {
        QSize size(width(), height());

        if (!my::IsNull(m_width) &&
            !my::IsNull(m_height))
        {
            size.setWidth(m_width);
            size.setHeight(m_height);
        }
        return size;
    }

    void SetSize(int width, int height)
    {
        m_width = width;
        m_height = height;
    }

public:
    int m_width;
    int m_height;
};

class MainWindow 
    : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

public slots:
    void Repaint();

    void Refresh();

protected:
    bool eventFilter(QObject *obj, QEvent *event);

    void closeEvent(QCloseEvent *event);

private slots:
    void RefreshContents();
    
    void OpenVideoFrame();
    void SaveFightFlowCalibration();

    void CloseGameCallback();

    void EditSettings();

    void ClearCalibration();

    void PlaybackPreviousFrame();
    void PlaybackPlayButton();
    void PlaybackNextFrameButton();

    void PlaybackPositionSlider(int position);

    void PlaybackRefresh();

    void IdleCallback();
    void BackFromIdleCallback();
    
    void ShowCheatSheet();
    void ShowLog();

    void CurrentTabChanged();
    void CurrentTabChanged(bool);

private:
    void CreateActions();
    void CreateMenus();
    void CreateTabs();
    void CreateStatusBar();

    void CreateCentralWidget();

    void UpdateCentralWidget();
    void UpdatePlaybackWidget();

private:
    void InitializeTabs();

    bool IsTabVisible(const QDockWidget* dockWidget) const;

    void ReadWindowSettings();
    void WriteWindowSettings();

    void SetCurrentDirectory(const QString &directoryName);
    QString GetCurrentDirectory() const;

    void UpdateActions() const;

    void Create();

protected:
    QString m_settingsFileName;

    QString m_currentDirectoryName;

    QMenu *m_fileMenu;
    QMenu *m_editMenu;
    QMenu *m_inputDataMenu;
    QMenu *m_auditingMenu;
    QMenu *m_helpMenu;

    QToolBar *m_fileToolBar;

    QAction* m_openVideoFrameAction;
    QAction* m_saveFightFlowCalibrationAction;

    QAction* m_clearCalibrationAction;

    QAction *m_exitAction;

    QAction *m_settingsAction;

    QAction *m_showCheatSheetAction;
    QAction *m_showLogAction;

    QAbstractButton *m_playbackPreviousFrameButton;
    QAbstractButton *m_playbackPlayButton;
    QAbstractButton *m_playbackNextFrameButton;

    QSlider *m_playbackPositionSlider;

    CMyQTimer m_idleTimer;

    GLWidget *m_glWidget;

    std::vector<CTabInterface *> m_tabInterfaceArray;

    boost::shared_ptr<CMessageQueue> m_messageQueue;

    QSplashScreen m_helpScreen;

    QTimer m_guiRefreshTimer;

    boost::shared_ptr<QSound> m_messageSoundEffect;

    QMenu *m_playContextMenu;
};

#endif //#if !defined(MAIN_WINDOW_INCLUDED)

