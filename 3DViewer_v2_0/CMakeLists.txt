cmake_minimum_required(VERSION 3.5)

project(3DViewer_v2_0 VERSION 0.1 LANGUAGES CXX C)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets OpenGL OpenGLWidgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets OpenGL OpenGLWidgets)

set(PROJECT_SOURCES
        main.cc
        gif_lib/gif_lib.h
        gif_lib/gif_lib_private.h
        gif_lib/gif_hash.h
        gif_lib/egif_lib.c
        gif_lib/dgif_lib.c
        gif_lib/gif_err.c
        gif_lib/gif_font.c
        gif_lib/gif2rgb.c
        gif_lib/quantize.c
        gif_lib/gifalloc.c
        gif_lib/openbsd-reallocarray.c
        gif_lib/gif_hash.c
        view/view.cc
        view/view.h
        view/view.ui
        view/render.h
        view/render.cc
        view/memento/memento.cc
        view/memento/memento.h
        model/model.cc
        model/model.h
        model/data.h
        model/pars.cc
        model/pars.h
        model/command/command.cc
        model/command/command.h
        controller/controller.cc
        controller/controller.h
        resources/resources.qrc
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(3DViewer_v2_0
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET 3DViewer_v2_0 APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(3DViewer_v2_0 SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(3DViewer_v2_0
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(3DViewer_v2_0 PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::OpenGL Qt${QT_VERSION_MAJOR}::OpenGLWidgets)

set_target_properties(3DViewer_v2_0 PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(3DViewer_v2_0)
endif()
