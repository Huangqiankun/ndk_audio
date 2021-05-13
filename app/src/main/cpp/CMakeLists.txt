cmake_minimum_required(VERSION 3.4.1)

file(GLOB allCpp *.cpp)

include_directories(${CMAKE_SOURCE_DIR}/include)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L${CMAKE_SOURCE_DIR}/${CMAKE_ANDROID_ARCH_ABI}")

add_library(
        native-lib
        SHARED
        # native-lib.cpp
        ${allCpp}
)

find_library(
        log-lib
        log)

target_link_libraries(
        native-lib
        # 此坑不想去踩了，直接可以忽略顺序，为所欲为的填写即可
        -Wl,--start-group
        avcodec avfilter avformat avutil swresample swscale
        -Wl,--end-group
        z
        ${log-lib}
        OpenSLES
        android
        OpenSLES
)