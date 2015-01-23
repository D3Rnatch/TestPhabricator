TEMPLATE= app
QT += widgets network core

DEPENDPATH +=
INCLUDEPATH += -I"C:\Qt\5.4\mingw491_32\qjson-0.8.1\src"

LIBS+= -L"C:\Qt\5.4\mingw491_32\qjson-0.8.1\src"


HEADERS += mainwindow.h \
    libraries.h

SOURCES += mainwindow.cpp main.cpp

FORMS += mainwindow.ui

