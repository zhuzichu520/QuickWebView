cmake_minimum_required(VERSION 3.16)

project(QuickWebView VERSION 0.1 LANGUAGES CXX)

option(QUICKWEBVIEW_BUILD_STATIC_LIB "Build QuickWebView static library." ON)
option(QUICKWEBVIEW_UNIVERSAL_BUILD_MACOS "Universal build QuickWebView for macOS." ON)

if(APPLE AND QUICKWEBVIEW_UNIVERSAL_BUILD_MACOS)
    set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64" CACHE STRING "" FORCE)
else()
    set(QUICKWEBVIEW_UNIVERSAL_BUILD_MACOS OFF)
endif()

message("---------------------------- QuickWebView ----------------------------")
message("Build QuickWebView static library: ${QUICKWEBVIEW_BUILD_STATIC_LIB}")
message("Universal build QuickWebView for macOS: ${QUICKWEBVIEW_BUILD_STATIC_LIB}")
message("------------------------------------------------------------------")

add_subdirectory(webview)
add_subdirectory(src)
add_subdirectory(example)
