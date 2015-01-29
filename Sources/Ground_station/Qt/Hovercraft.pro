TEMPLATE= app
QT += widgets network core gui

DEPENDPATH +=

win32{
INCLUDEPATH += -I"C:\Qt\5.4\mingw491_32\qjson-0.8.1\src"
#INCLUDEPATH += -L"C:\Qt\5.4\mingw491_32\SDL2-2.0.3\include"
#INCLUDEPATH += -L"C:\Qt\5.4\mingw491_32\SDL2-2-1.0.3\src"
INCLUDEPATH += -L"C:\Qt\5.4\mingw491_32\SDL2-2-1.0.3\include"

#LIBS+= -L"C:\Qt\5.4\mingw491_32\SDL2-2-1.0.3\src"
#LIBS+= -L"C:\Qt\5.4\mingw491_32\qjson-0.8.1\src"
LIBS+= -L"C:\Qt\5.4\mingw491_32\SDL2-2-1.0.3\lib\x86" -lSDL2
LIBS+= -L"C:\Qt\5.4\mingw491_32\SDL2-2-1.0.3\lib\x86" -lSDL2main
#LIBS+= -L"C:\Qt\5.4\mingw491_32\SDL2-2-1.0.3\lib\x86"
#LIBS+= -LC:\Users\Damien\Documents\PFE\build-Hovercraft-Desktop_Qt_5_4_0_MinGW_32bit-Debug\SDL2-2-1.0.3\lib\x64 -lSDL2 -lSDL2main
#LIBS+= -LC:\Users\Damien\Documents\PFE\build-Hovercraft-Desktop_Qt_5_4_0_MinGW_32bit-Debug\SDL2-2-1.0.3\lib\x86 -lSDL2 -lSDL2main
}
unix{
LIBS += -L/usr/lib -lSDL
INCLUDEPATH += usr/include
}

HEADERS += mainwindow.h \
    libraries.h \
    joystick.h \
    map.h

SOURCES += mainwindow.cpp \
    main.cpp \
    joystick.cpp \
    map.cpp


FORMS += mainwindow.ui

