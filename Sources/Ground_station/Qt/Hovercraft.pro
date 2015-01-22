TEMPLATE= app
QT += widgets network core

DEPENDPATH +=

win32{
INCLUDEPATH += -I"C:\Qt\5.4\mingw491_32\qjson-0.8.1\src"
LIBS+= -L"C:\Qt\5.4\mingw491_32\qjson-0.8.1\src"
}
unix{
LIBS += -L/usr/lib -lSDL
INCLUDEPATH += usr/include
}

HEADERS += mainwindow.h \
    sdl.h \
    libraries.h \
    joystick.h

SOURCES += mainwindow.cpp main.cpp \
    joystick.cpp

FORMS += mainwindow.ui

