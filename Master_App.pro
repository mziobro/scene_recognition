QT += quick multimedia
CONFIG += c++11

SOURCES += \
        bag.cpp \
        calibrator.cpp \
        camerahandler.cpp \
        detector.cpp \
        main.cpp \
        videoimageprovider.cpp

RESOURCES += qml.qrc


#USING openCV version 4.0.1
INCLUDEPATH += C:\OPENCV\opencv\build\include
INCLUDEPATH += C:\OPENCV\opencv\build\include\opencv2



#build with QT. 5.9.8 mingw
LIBS += -LC:\OPENCV\opencv\build\x64\vc15\lib -lopencv_world401


LIBS += C:\OPENCV\opencv-build\bin\libopencv_core401.dll
LIBS += C:\OPENCV\opencv-build\bin\libopencv_highgui401.dll
LIBS += C:\OPENCV\opencv-build\bin\libopencv_imgcodecs401.dll
LIBS += C:\OPENCV\opencv-build\bin\libopencv_imgproc401.dll
LIBS += C:\OPENCV\opencv-build\bin\libopencv_features2d401.dll
LIBS += C:\OPENCV\opencv-build\bin\libopencv_calib3d401.dll
LIBS += C:\OPENCV\opencv-build\bin\libopencv_videoio401.dll
LIBS += C:\OPENCV\opencv-build\bin\libopencv_video401.dll
LIBS += C:\OPENCV\opencv-build\bin\libopencv_dnn401.dll


#libopencv_videoio

HEADERS += \
    bag.h \
    calibrator.h \
    camerahandler.h \
    detector.h \
    videoimageprovider.h
