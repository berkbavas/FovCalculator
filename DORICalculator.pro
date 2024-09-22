QT       += core gui opengl widgets

CONFIG += c++11

INCLUDEPATH += Libs \
                Source\

SOURCES += \
    Source/Core/Controller.cpp \
    Source/Core/Logic.cpp \
    Source/Gui/CentralWidget.cpp \
    Source/Gui/Handle.cpp \
    Source/Gui/Rectangle.cpp \
    Source/Gui/SideViewWidget.cpp \
    Source/Gui/TopViewWidget.cpp \
    Source/Main.cpp \
    Source/OpenGL/BasicShader.cpp \
    Source/OpenGL/Camera.cpp \
    Source/OpenGL/CustomNode.cpp \
    Source/OpenGL/CustomNodeData.cpp \
    Source/OpenGL/Helper.cpp \
    Source/OpenGL/Light.cpp \
    Source/OpenGL/Material.cpp \
    Source/OpenGL/ModelLoader.cpp \
    Source/OpenGL/Node.cpp \
    Source/OpenGL/NodeManager.cpp \
    Source/OpenGL/SimpleModel.cpp \
    Source/OpenGL/SimpleModelData.cpp \
    Source/OpenGL/Window3D.cpp

HEADERS += \
    Source/Core/Constants.h \
    Source/Core/Controller.h \
    Source/Core/Enums.h \
    Source/Core/Logic.h \
    Source/Core/Typedefs.h \
    Source/Gui/CentralWidget.h \
    Source/Gui/Handle.h \
    Source/Gui/Rectangle.h \
    Source/Gui/SideViewWidget.h \
    Source/Gui/TopViewWidget.h \
    Source/OpenGL/BasicShader.h \
    Source/OpenGL/Camera.h \
    Source/OpenGL/CustomNode.h \
    Source/OpenGL/CustomNodeData.h \
    Source/OpenGL/Helper.h \
    Source/OpenGL/Light.h \
    Source/OpenGL/Material.h \
    Source/OpenGL/ModelLoader.h \
    Source/OpenGL/Node.h \
    Source/OpenGL/NodeManager.h \
    Source/OpenGL/SimpleModel.h \
    Source/OpenGL/SimpleModelData.h \
    Source/OpenGL/Window3D.h

RESOURCES += \
    DORICalculator.qrc
