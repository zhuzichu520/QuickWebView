cmake_minimum_required(VERSION 3.16)

project(QuickWebView VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 COMPONENTS Quick REQUIRED)

set(QT_SDK_DIR "${Qt6_DIR}/../../..")
cmake_path(SET QT_SDK_DIR NORMALIZE ${QT_SDK_DIR})

set(SOURCE_FILES
    stdafx.h
    QuickWebViewImpl.h QuickWebViewImpl.cpp
)

set(QML_FILES
    QuickWebView.qml
)

if (QUICKWEBVIEW_BUILD_STATIC_LIB)
    set(QUICKWEBVIEW_LIB_TYPE "STATIC")
    set(QUICKWEBVIEW_QML_PLUGIN_DIRECTORY ${CMAKE_BINARY_DIR}/qt/qml/QuickWebView)
    set(PLUGIN_TARGET_NAME "")
else ()
    set(QUICKWEBVIEW_LIB_TYPE "SHARED")
    set(QUICKWEBVIEW_QML_PLUGIN_DIRECTORY ${QT_SDK_DIR}/qml/QuickWebView)
    set(PLUGIN_TARGET_NAME ${PROJECT_NAME})
endif ()

qt_add_library(${PROJECT_NAME} ${QUICKWEBVIEW_LIB_TYPE})

qt_add_qml_module(${PROJECT_NAME}
    URI "QuickWebView"
    VERSION "${PROJECT_VERSION}"
    TYPEINFO "plugins.qmltypes"
    DESIGNER_SUPPORTED
    PLUGIN_TARGET ${PLUGIN_TARGET_NAME}
    QML_FILES ${QML_FILES}
    SOURCES ${SOURCE_FILES}
    RESOURCE_PREFIX /qt/qml
    OUTPUT_DIRECTORY ${QUICKWEBVIEW_QML_PLUGIN_DIRECTORY}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Quick
    webview::headers
)

