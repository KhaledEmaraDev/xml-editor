QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    codeeditor.cpp \
    highlighter.cpp \
    lib/xmlnode.cpp \
    lib/xmltree.cpp \
    main.cpp \
    mainwidnow.cpp \
    test/hashtest.cpp \
    test/xmltest.cpp

HEADERS += \
    codeeditor.h \
    highlighter.h \
    lib/hashcode.h \
    lib/hashmap.h \
    lib/mpair.h \
    lib/xmlnode.h \
    lib/xmltree.h \
    mainwindow.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
