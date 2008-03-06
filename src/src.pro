QT += opengl
CONFIG += debug

SOURCES += main.cpp \
          activity.cpp \
           element.cpp \
           window.cpp \
           glwidget.cpp \
           background_reader.cpp \
           host.cpp

TEMPLATE = app

CONFIG += warn_on \
          thread \
          qt \
          debug \
          opengl

TARGET = ../bin/gltrail

HEADERS += element.h \
           activity.h \
           relation.h \
           window.h \
           glwidget.h \
           background_reader.h \
           host.h

