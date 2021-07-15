QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17 console

requires(qtConfig(filedialog))

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    compress/huffman.cpp \
    lib/json.cpp \
#    lib/jsonnode.cpp \
    lib/xmlnode.cpp \
    lib/xmltree.cpp \
    test/compresstest.cpp \
    test/hashtest.cpp \
    test/xmltest.cpp \
    ui/codeeditor.cpp \
    ui/json_highlighter.cpp \
    main.cpp \
    ui/mainwidnow.cpp \
    ui/xml_highlighter.cpp

HEADERS += \
    compress/huffman.h \
    compress/hnode.h \
    lib/hashcode.h \
    lib/hashmap.h \
    lib/json.h \
#    lib/jsonnode.h \
    lib/mpair.h \
    lib/xmlnode.h \
    lib/xmltree.h \
    ui/codeeditor.h \
    ui/json_highlighter.h \
    ui/mainwindow.h \
    ui/xml_highlighter.h

RESOURCES = application.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
