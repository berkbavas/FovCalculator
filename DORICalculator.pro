QT       += core gui opengl widgets

CONFIG += c++11

INCLUDEPATH += libs \
                src\

SOURCES += \
    src/Core/Controller.cpp \
    src/Core/Logic.cpp \
    src/Gui/CentralWidget.cpp \
    src/Gui/Handle.cpp \
    src/Gui/Rectangle.cpp \
    src/Gui/SideViewWidget.cpp \
    src/Gui/TopViewWidget.cpp \
    src/Main.cpp \
    src/OpenGL/BasicShader.cpp \
    src/OpenGL/Camera.cpp \
    src/OpenGL/CustomNode.cpp \
    src/OpenGL/CustomNodeData.cpp \
    src/OpenGL/Helper.cpp \
    src/OpenGL/Light.cpp \
    src/OpenGL/Material.cpp \
    src/OpenGL/ModelLoader.cpp \
    src/OpenGL/Node.cpp \
    src/OpenGL/NodeManager.cpp \
    src/OpenGL/SimpleModel.cpp \
    src/OpenGL/SimpleModelData.cpp \
    src/OpenGL/Window3D.cpp

HEADERS += \
    src/Core/Constants.h \
    src/Core/Controller.h \
    src/Core/Enums.h \
    src/Core/Logic.h \
    src/Core/Typedefs.h \
    src/Gui/CentralWidget.h \
    src/Gui/Handle.h \
    src/Gui/Rectangle.h \
    src/Gui/SideViewWidget.h \
    src/Gui/TopViewWidget.h \
    src/OpenGL/BasicShader.h \
    src/OpenGL/Camera.h \
    src/OpenGL/CustomNode.h \
    src/OpenGL/CustomNodeData.h \
    src/OpenGL/Helper.h \
    src/OpenGL/Light.h \
    src/OpenGL/Material.h \
    src/OpenGL/ModelLoader.h \
    src/OpenGL/Node.h \
    src/OpenGL/NodeManager.h \
    src/OpenGL/SimpleModel.h \
    src/OpenGL/SimpleModelData.h \
    src/OpenGL/Window3D.h

RESOURCES += \
    DORICalculator.qrc
