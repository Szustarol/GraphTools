QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets



CONFIG += c++11 file_copies

COPIES += shaders

shaders.files = $$files(graph_gl_scene/*.glsl)
shaders.path = $$OUT_PWD/graph_gl_scene

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    graph_gl_scene/graph_node.cpp \
    graph_gl_scene/graph_window_data.cpp \
    main.cpp \
    mainwindow.cpp \
    qtgl_graph_window.cpp

HEADERS += \
    graph_gl_scene/graph_node.h \
    graph_gl_scene/graph_window_data.h \
    mainwindow.h \
    qtgl_graph_window.h

FORMS += \
    mainwindow.ui

TRANSLATIONS += \
    GraphTools_pl_PL.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    graph_gl_scene/fragment_shader.glsl \
    graph_gl_scene/vertex_shader.glsl
