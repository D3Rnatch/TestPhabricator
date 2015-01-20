TEMPLATE= app
QT += widgets network core

DEPENDPATH +=
INCLUDEPATH += -I"C:\Qt\5.4\mingw491_32\qjson-0.8.1\src"

LIBS+= -L"C:\Qt\5.4\mingw491_32\qjson-0.8.1\src"


HEADERS += mainwindow.h \
    sdl.h \
    libraries.h \
    log.h

SOURCES += mainwindow.cpp main.cpp \
    log.cpp

FORMS += mainwindow.ui

