TEMPLATE = app
TARGET = qt5-cam-demo

QMAKE_LFLAGS += -L/usr/lib/arm-linux-gnueabihf/

QT += multimedia multimediawidgets core gui widgets

CONFIG += mobility
MOBILITY += multimedia

HEADERS = cameracapture.h \
          camera-capture-settings.h \
          cameracapture.h
          
SOURCES = main.cpp \
          cameracapture.cpp \    
          camera-capture-settings.cpp
          
FORMS += cameracapture.ui \
         settings.ui \
    

OTHER_FILES += \
    deploy.sh \
    camtest.desktop \
    run.sh

