cmake_minimum_required(VERSION 3.4.1)

add_library(
        fluid-lib

        SHARED

        jni.cpp
        quad.cpp
        Fluid.cpp)

find_library(
        log-lib

        log)

target_link_libraries(
        fluid-lib

        android
        ${log-lib}
        EGL
        GLESv2)