QT += widgets opengl openglwidgets gui

CONFIG += c++20

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

HEADERS += \
    glwidget.h \
    mainwindow.h \
    terraingenerator.h

SOURCES += \
    glwidget.cpp \
    main.cpp \
    mainwindow.cpp \
    terraingenerator.cpp


# Default rules for deployment.
#qnx: target.path = /tmp/$${TARGET}/bin
#else: unix:!android: target.path = /opt/$${TARGET}/bin
#!isEmpty(target.path): INSTALLS += target

INCLUDEPATH += glm
DEPENDPATH += glm

DISTFILES += \
    fragment.frag \
    vertex.vert
