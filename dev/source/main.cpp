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

#include <boost/filesystem.hpp>

#include <QApplication>

#include <Logger.h>

#include "StringResource.h"

#include "mainwindow.h"

class CApplicationWithExceptHandling
	: public QApplication
{
public:
	CApplicationWithExceptHandling(int &argc, char *argv[])
		: QApplication(argc, argv)
	{
	}

	bool notify(QObject *receiver_, QEvent *event_)
	{
		try
		{
			return QApplication::notify(receiver_, event_);
		}
		catch (std::exception &e)
		{
			LOG_MESSAGE(e.what());
		}

		return false;
	}
};

int main(int argc, char *argv[])
{
    //Q_INIT_RESOURCE(application);

	CApplicationWithExceptHandling app(argc, argv);

    app.setOrganizationName(QString(UFC_STRING_RESOURCE_0129));
    app.setApplicationName(QString(UFC_STRING_RESOURCE_0130));
    app.setApplicationVersion(QString(UFC_STRING_RESOURCE_0131));

    // TRICKY: (25-Oct-2016) DON'T MAKE IT A RESOURCE, IT'S TOO BIG
    QPixmap pixmap("./textures/UfcCalibratorSplashScreen.png");

    QSplashScreen *splashScreen = new QSplashScreen(pixmap.scaled(800, 540, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));

    splashScreen->show();

    // (BEGIN OF) TESTING: (26-Nov-2015) 
    QFont font;

    font.setFamily(font.defaultFamily());

    app.setFont(font);
    // (END OF) TESTING: (26-Nov-2015) 

    MainWindow mainWindow;

#if defined(Q_OS_SYMBIAN)
    mainWindow.showMaximized();
#else
    mainWindow.show();
#endif

    splashScreen->finish(&mainWindow);

    return app.exec();
}

