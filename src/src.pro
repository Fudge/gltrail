QT += opengl
CONFIG += debug

SOURCES += main.cpp \
           activity.cpp \
           element.cpp \
           window.cpp \
           glwidget.cpp \
           background_updater.cpp \
           inputs/ssh.cpp

HEADERS += input.h \
           element.h \
           activity.h \
           relation.h \
           window.h \
           glwidget.h \
           background_updater.h \
           inputs/ssh.h


TEMPLATE = app

CONFIG += warn_on \
          thread \
          qt \
          debug \
          opengl

TARGET = ../bin/gltrail

