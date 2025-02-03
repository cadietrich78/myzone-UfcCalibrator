
# UfcCalibrator
A PoC of a calibration tool for the FightFlow project.

## API Dependencies (as of 2022/10)
All built on Microsoft Visual Studio 2019, version 16.9.5 (platform toolset **v142**, platform **x64**).

 - Qt 5.13.0 (https://www.qt.io/) 
 - Boost 1.72.0 (https://www.boost.org/)
 - Glew 2.1.0 (https://glew.sourceforge.net/) 
 - Png 1.6.37-10 
 - RapidJson (https://rapidjson.org/) 
 - Curl 7.46.0 (http://curl.haxx.se/) 
 - Curl CPP (https://github.com/JosephP91/curlcpp) 
 - FreeGLUT (https://freeglut.sourceforge.net/) 
 - OpenCV 4.5.4 (https://opencv.org/)

## Include Directories:

 - (QtDir)/include
 - (QtDir)/include/QtCore
 - (QtDir)/include/QtMultimedia
 - (QtDir)/include/QtWidgets
 - (QtDir)/include/QtMultimediaWidgets
 - (QtDir)/include/QtOpenGL
 - (QtDir)/include/QtGui
 - (QtDir)/include/QtNetwork
 - (QtDir)/include/QtWebEngineWidgets
 - (BoostDir)
 - (GlewDir)/include
 - (PngDir)/include
 - (RapidJsonDir)/include
 - (CurlDir)/include
 - (CurlCppDir)/include
 - (FreeGlutDir)/libs/freeglut/include
 - (OpenCvDir)/include
 - (OpenCvDir)/build-v142
 - (OpenCvDir)/build-v142/install/include
 - (OpenCvDir)/modules/core/include
 - (OpenCvDir)/modules/imgproc/include
 - (OpenCvDir)/modules/highgui/include
 - (OpenCvDir)/modules/imgcodecs/include
 - (OpenCvDir)/modules/videoio/include
 - (OpenCvDir)/modules/calib3d/include
 - (OpenCvDir)/modules/photo/include
 - (OpenCvDir)/modules/video/include

## Library Directories:

 - (QtDir)/lib
 - (BoostDir)/lib/v142/x64
 - (GlewDir)/lib/Release/x64
 - (PngDir)/lib
 - (CurlDir)/lib
 - (CurlCppDir)/build-v142/src/
 - (FreeGlutDir)/x64
 - (OpenCvDir)/build-v142/lib/
 - (OpenCvDir)/build-v142/3rdparty/lib/

## Preprocessor Definitions:
 - NOMINMAX

## Input Libraries:
 - **Debug**
	 - libcurl.lib
	 - curlcpp.lib
	 - libpng16d.lib
	 - glew32.lib
	 - Qt5OpenGLd.lib
	 - Qt5Guid.lib
	 - Qt5Cored.lib
	 - Qt5Multimediad.lib
	 - Qt5Widgetsd.lib
	 - Qt5MultimediaWidgetsd.lib
	 - Qt5Networkd.lib
	 - opencv_videoio454d.lib
	 - opencv_core454d.lib
	 - opencv_imgproc454d.lib
	 - opencv_video454d.lib
	 - opencv_imgcodecs454d.lib
	 - opencv_highgui454d.lib
	 - opencv_calib3d454d.lib
 - **Release**
	 - libcurl.lib
	 - curlcpp.lib
	 - libpng16.lib
	 - glew32.lib
	 - Qt5OpenGL.lib
	 - Qt5Gui.lib
	 - Qt5Core.lib
	 - Qt5Multimedia.lib
	 - Qt5Widgets.lib
	 - Qt5MultimediaWidgets.lib
	 - Qt5Network.lib
	 - opencv_videoio454.lib
	 - opencv_core454.lib
	 - opencv_imgproc454.lib
	 - opencv_video454.lib
	 - opencv_imgcodecs454.lib
	 - opencv_highgui454.lib
	 - opencv_calib3d454.lib

## Custom Build Steps:
The files listed below should be precompiled by Qt, and the resulting cpp's should be added to the project as well (these are the files at the "dev/build/v142/UfcCalibrator/UfcCalibrator/qt" folder):

 - AppGuiSettingsGroupDialog.h 
 - AppSettingsDialog.h
 - AppSettingsGroupDialog.h 
 - ExtrinsicCalibrationTab.h 
 - glwidget.h
 - mainwindow.h 
 - MyQTimer.h 
 - TabInterface.h 
 - application.qrc
