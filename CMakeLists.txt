cmake_minimum_required(VERSION 3.25.1)

project(FovCalculator VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_PREFIX_PATH "$ENV{Qt6_DIR}")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(LIBS_DIR        "${CMAKE_CURRENT_SOURCE_DIR}/Libs")
set(QT_IMGUI_DIR    "${LIBS_DIR}/qtimgui")
set(EIGEN_DIR       "${LIBS_DIR}/Eigen")
set(INCLUDE_DIR     "${QT_IMGUI_DIR}/include" "${EIGEN_DIR}")
set(LIBS_DIR        "${QT_IMGUI_DIR}/lib")
set(LIBS
    debug imguid                optimized imgui
    debug qt_imgui_widgetsd     optimized qt_imgui_widgets
)

file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/Source/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.qrc")

find_package(Qt6 COMPONENTS Core Widgets Gui OpenGL OpenGLWidgets REQUIRED)

add_executable(FovCalculator ${SOURCES})

target_include_directories(FovCalculator PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Source" ${INCLUDE_DIR})

target_link_directories(FovCalculator PRIVATE ${LIBS_DIR})

target_link_libraries(FovCalculator 
    Qt6::Core 
    Qt6::Widgets 
    Qt6::OpenGL 
    Qt6::OpenGLWidgets 
    ${LIBS}
)

add_custom_command(TARGET FovCalculator
    POST_BUILD COMMAND
    Qt6::windeployqt
    --dir "$<TARGET_FILE_DIR:FovCalculator>"
    "$<TARGET_FILE_DIR:FovCalculator>/$<TARGET_FILE_NAME:FovCalculator>"
)
