QT += opengl network
CONFIG += debug

SOURCES += main.cpp \
           activity.cpp \
           element.cpp \
           window.cpp \
           glwidget.cpp \
           background_updater.cpp \
           input.cpp \
           inputs/ssh.cpp \
           inputs/digg.cpp \ 
           inputs/twitter.cpp

HEADERS += input.h \
           element.h \
           activity.h \
           relation.h \
           window.h \
           glwidget.h \
           background_updater.h \
           inputs/ssh.h \
           inputs/digg.h \
           inputs/twitter.h 

TEMPLATE = app

CONFIG += warn_on \
          thread \
          qt \

TARGET = ../bin/gltrail

