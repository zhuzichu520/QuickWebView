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
    webres/test.in.html
    webres/tailwindcss.in.js
)

set(QML_FILES
    main.qml
)

configure_file(webres/test.in.html ${CMAKE_CURRENT_BINARY_DIR}/webres/test.html COPYONLY)
configure_file(webres/tailwindcss.in.js ${CMAKE_CURRENT_BINARY_DIR}/webres/tailwindcss.js COPYONLY)

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
