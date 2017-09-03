QT  += core gui widgets
CONFIG += c++11

TARGET = simViewer
CONFIG += console gl_debug
CONFIG -= app_bundle

INCLUDEPATH += $$PWD/OpenGL
INCLUDEPATH += ./GLEW
TEMPLATE = app

unix:QMAKE_CXXFLAGS += -std=c++11

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS


# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    window.cpp \
    camera3d.cpp \
    input.cpp \
    transform3d.cpp \
    profiler.cpp \
    frameresult.cpp \
    debugdraw.cpp \
    OpenGL/openglerror.cpp \
    graph/pointmass.cpp \
    graph/spring.cpp \
    OpenGL/openglerror.cpp \
    camera3d.cpp \
    debugdraw.cpp \
    frameresult.cpp \
    input.cpp \
    main.cpp \
    profiler.cpp \
    transform3d.cpp \
    window.cpp \
    graph/pointmass.cpp \
    graph/spring.cpp \
    OpenGL/openglerror.cpp \
    camera3d.cpp \
    debugdraw.cpp \
    frameresult.cpp \
    input.cpp \
    main.cpp \
    profiler.cpp \
    transform3d.cpp \
    window.cpp \
    geometry.cpp \
    graph/massspringsystem.cpp \
    graph/pointmass.cpp \
    graph/spring.cpp \
    OpenGL/openglerror.cpp \
    graph/massspringsystem.cpp \
    graph/pointmass.cpp \
    graph/spring.cpp \
    OpenGL/openglerror.cpp \
    volume/VolumeData.cpp \
    camera3d.cpp \
    debugdraw.cpp \
    frameresult.cpp \
    geometry.cpp \
    input.cpp \
    main.cpp \
    profiler.cpp \
    transform3d.cpp \
    window.cpp \
    volume/octree.cpp \
    volume/volblockanalyzer.cpp \
    volume/volumeblock.cpp \
    graph/massspringsystem.cpp \
    graph/pointmass.cpp \
    graph/spring.cpp \
    OpenGL/openglerror.cpp \
    volume/octree.cpp \
    volume/volblockanalyzer.cpp \
    volume/volumeblock.cpp \
    volume/VolumeData.cpp \
    camera3d.cpp \
    debugdraw.cpp \
    frameresult.cpp \
    geometry.cpp \
    input.cpp \
    main.cpp \
    profiler.cpp \
    transform3d.cpp \
    window.cpp \

HEADERS += \
    window.h \
    camera3d.h \
    input.h \
    transform3d.h \
    vertex.h \
    profiler.h \
    frameresult.h \
    debugdraw.h \
    debugvertex.h \
    macros.h \
    OpenGL/openglerror.h \
    OpenGL/openglbuffer.h \
    OpenGL/openglshaderprogram.h \
    OpenGL/openglvertexarrayobject.h \
    OpenGL/openglcommon.h \
    OpenGL/openglmarkerscoped.h \
    OpenGL/openglfunctions.h \
    camera3d.h \
    debugdraw.h \
    debugvertex.h \
    frameresult.h \
    input.h \
    macros.h \
    profiler.h \
    transform3d.h \
    vertex.h \
    window.h \
    graph/pointmass.h \
    graph/spring.h \
    OpenGL/openglbuffer.h \
    OpenGL/openglcommon.h \
    OpenGL/openglerror.h \
    OpenGL/openglfunctions.h \
    OpenGL/openglmarkerscoped.h \
    OpenGL/openglshaderprogram.h \
    OpenGL/openglvertexarrayobject.h \
    camera3d.h \
    debugdraw.h \
    debugvertex.h \
    frameresult.h \
    input.h \
    macros.h \
    profiler.h \
    transform3d.h \
    vertex.h \
    window.h \
    graph/pointmass.h \
    graph/spring.h \
    OpenGL/openglbuffer.h \
    OpenGL/openglcommon.h \
    OpenGL/openglerror.h \
    OpenGL/openglfunctions.h \
    OpenGL/openglmarkerscoped.h \
    OpenGL/openglshaderprogram.h \
    OpenGL/openglvertexarrayobject.h \
    camera3d.h \
    debugdraw.h \
    debugvertex.h \
    frameresult.h \
    input.h \
    macros.h \
    profiler.h \
    transform3d.h \
    vertex.h \
    window.h \
    geometry.h \
    global.h \
    graph/massspringsystem.h \
    graph/pointmass.h \
    graph/spring.h \
    OpenGL/OpenGLBuffer \
    OpenGL/openglbuffer.h \
    OpenGL/OpenGLCommon \
    OpenGL/openglcommon.h \
    OpenGL/OpenGLError \
    OpenGL/openglerror.h \
    OpenGL/OpenGLFunctions \
    OpenGL/openglfunctions.h \
    OpenGL/OpenGLMarkerScoped \
    OpenGL/openglmarkerscoped.h \
    OpenGL/OpenGLShaderProgram \
    OpenGL/openglshaderprogram.h \
    OpenGL/OpenGLVertexArrayObject \
    OpenGL/openglvertexarrayobject.h \
    graph/massspringsystem.h \
    graph/pointmass.h \
    graph/spring.h \
    OpenGL/openglbuffer.h \
    OpenGL/openglcommon.h \
    OpenGL/openglerror.h \
    OpenGL/openglfunctions.h \
    OpenGL/openglmarkerscoped.h \
    OpenGL/openglshaderprogram.h \
    OpenGL/openglvertexarrayobject.h \
    volume/VolumeData.h \
    camera3d.h \
    debugdraw.h \
    debugvertex.h \
    frameresult.h \
    geometry.h \
    global.h \
    input.h \
    macros.h \
    profiler.h \
    transform3d.h \
    vertex.h \
    window.h \
    MyVectors.h \
    StdDefines.h \
    volume/octree.h \
    volume/volblockanalyzer.h \
    volume/volumeblock.h \
    graph/massspringsystem.h \
    graph/pointmass.h \
    graph/spring.h \
    OpenGL/openglbuffer.h \
    OpenGL/openglcommon.h \
    OpenGL/openglerror.h \
    OpenGL/openglfunctions.h \
    OpenGL/openglmarkerscoped.h \
    OpenGL/openglshaderprogram.h \
    OpenGL/openglvertexarrayobject.h \
    volume/octree.h \
    volume/volblockanalyzer.h \
    volume/volumeblock.h \
    volume/VolumeData.h \
    camera3d.h \
    debugdraw.h \
    debugvertex.h \
    frameresult.h \
    geometry.h \
    global.h \
    input.h \
    macros.h \
    MyVectors.h \
    profiler.h \
    StdDefines.h \
    transform3d.h \
    vertex.h \
    window.h

RESOURCES += \
    resources.qrc

CONFIG(debug,debug|release) {
  CONFIG(gl_debug) {
      DEFINES += GL_DEBUG
  }
}


