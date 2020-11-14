QT += quick multimedia
CONFIG += c++11

SOURCES += \
        bag.cpp \
        camerhandler.cpp \
        customvideoreader.cpp \
        customvideowriter.cpp \
        detector.cpp \
        main.cpp

RESOURCES += qml.qrc


#USING openCV version 4.0.1
INCLUDEPATH += C:\OPENCV\opencv\build\include
INCLUDEPATH += C:\OPENCV\opencv\build\include\opencv2


#LIBS += -LC:\OPENCV\opencv\build\x64\vc15\lib -lopencv_world401d
LIBS += -LC:\OPENCV\opencv\build\x64\vc15\lib -lopencv_world401
#LIBS += -LC:\OPENCV\opencv\build\x64\vc15\bin -lopencv_world401d
#LIBS += -LC:\OPENCV\opencv\build\x64\vc15\bin -lopencv_world401

LIBS += C:\OPENCV\opencv-build\bin\libopencv_core401.dll
LIBS += C:\OPENCV\opencv-build\bin\libopencv_highgui401.dll
LIBS += C:\OPENCV\opencv-build\bin\libopencv_imgcodecs401.dll
LIBS += C:\OPENCV\opencv-build\bin\libopencv_imgproc401.dll
LIBS += C:\OPENCV\opencv-build\bin\libopencv_features2d401.dll
LIBS += C:\OPENCV\opencv-build\bin\libopencv_calib3d401.dll
LIBS += C:\OPENCV\opencv-build\bin\libopencv_videoio401.dll
LIBS += C:\OPENCV\opencv-build\bin\libopencv_video401.dll
LIBS += C:\OPENCV\opencv-build\bin\libopencv_dnn401.dll


libopencv_videoio

HEADERS += \
    bag.h \
    camerhandler.h \
    customvideoreader.h \
    customvideowriter.h \
    detector.h
