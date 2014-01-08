TEMPLATE = app
TARGET = cameracapture

QT += multimediawidgets core gui widgets

CONFIG += mobility
MOBILITY = multimedia

HEADERS = cameracapture.h \
          camera-capture-settings.h \
          cameracapture.h
          
SOURCES = main.cpp \
          cameracapture.cpp \    
          camera-capture-settings.cpp
          
FORMS += cameracapture.ui \
         settings.ui \
    

