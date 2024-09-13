cmake_minimum_required(VERSION 3.16)

project(example VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(QML_IMPORT_PATH ${CMAKE_BINARY_DIR}/qt/qml CACHE STRING "" FORCE)

find_package(Qt6 COMPONENTS Quick REQUIRED)

qt_add_executable(${PROJECT_NAME}
    main.cpp
)

set(RESOURCES_FILES
    qtquickcontrols2.conf
)

set(QML_FILES
    main.qml
)

set(WEBRES_FILES
    webres/tailwindcss.js
    webres/test.html
)

qt_add_qml_module(${PROJECT_NAME}
    URI example
    VERSION 1.0
    TYPEINFO "plugins.qmltypes"
    RESOURCES ${RESOURCES_FILES}
    QML_FILES ${QML_FILES}
    RESOURCE_PREFIX /qt/qml
    OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/qt/qml/example
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Quick
    QuickWebView
)

if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(WEBRES_DIR ${CMAKE_CURRENT_BINARY_DIR}/example.app/Contents/MacOS/webres)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(WEBRES_DIR ${CMAKE_CURRENT_BINARY_DIR}/webres)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(WEBRES_DIR ${CMAKE_CURRENT_BINARY_DIR}/webres)
endif()


add_custom_target(${PROJECT_NAME}_webres
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/webres ${WEBRES_DIR}
    SOURCES ${WEBRES_FILES}
)

add_dependencies(${PROJECT_NAME} ${PROJECT_NAME}_webres)

if (QUICKWEBVIEW_BUILD_STATIC_LIB)
    target_link_libraries(
        ${PROJECT_NAME} PRIVATE
        QuickWebViewplugin
    )
target_compile_definitions(${PROJECT_NAME} PRIVATE
    QUICKWEBVIEW_BUILD_STATIC_LIB
)
endif()

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
