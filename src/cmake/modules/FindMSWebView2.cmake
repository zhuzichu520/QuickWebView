if(DEFINED MSWebView2_ROOT)
    find_path(MSWebView2_INCLUDE_DIR WebView2.h
        PATHS
            "${MSWebView2_ROOT}/build/native"
            "${MSWebView2_ROOT}"
        PATH_SUFFIXES include
        NO_CMAKE_FIND_ROOT_PATH)
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(MSWebView2 REQUIRED_VARS MSWebView2_INCLUDE_DIR)

if(MSWebView2_FOUND)
    set(MSWebView2_ROOT_DIR "${MSWebView2_INCLUDE_DIR}/..")
    cmake_path(SET MSWebView2_ROOT_DIR NORMALIZE ${MSWebView2_ROOT_DIR})
    if(NOT TARGET MSWebView2::headers)
        add_library(MSWebView2::headers STATIC IMPORTED)
        set_target_properties(MSWebView2::headers PROPERTIES
            INTERFACE_INCLUDE_DIRECTORIES "${MSWebView2_INCLUDE_DIR}"
            IMPORTED_LOCATION "${MSWebView2_ROOT_DIR}/x64/WebView2LoaderStatic.lib"
        )
    endif()
endif()
