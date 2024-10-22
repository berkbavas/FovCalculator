cmake_minimum_required(VERSION 3.25.1)
project(FovCalculator VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_PREFIX_PATH "$ENV{Qt6_DIR}")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(INCLUDE_DIR Libs/qtimgui/include
                Libs/Eigen)

set(LIBS_DIR    Libs/qtimgui/lib
                Libs/opencv-4.6.0/lib)

set(LIBS    debug   debug/imgui  
            debug   debug/qt_imgui_widgets                
            optimized release/imgui
            optimized release/qt_imgui_widgets)

file(GLOB_RECURSE SOURCES Source/*.cpp Source/*.h *.qrc)

find_package(Qt6 COMPONENTS Core Widgets Gui OpenGL OpenGLWidgets REQUIRED)

add_compile_options(/bigobj)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/Source" ${INCLUDE_DIR})

link_directories(${LIBS_DIR})

add_executable(FovCalculator ${SOURCES})

target_link_libraries(FovCalculator Qt6::Core Qt6::Widgets Qt6::OpenGL Qt6::OpenGLWidgets ${LIBS})

add_custom_command(TARGET FovCalculator
    POST_BUILD COMMAND ${CMAKE_COMMAND}
    -E copy_directory
    "${CMAKE_SOURCE_DIR}/Resources/"
    "$<TARGET_FILE_DIR:FovCalculator>/Resources/")

add_custom_command(TARGET FovCalculator
    POST_BUILD COMMAND
    Qt6::windeployqt
    --dir "$<TARGET_FILE_DIR:FovCalculator>"
    "$<TARGET_FILE_DIR:FovCalculator>/$<TARGET_FILE_NAME:FovCalculator>")
