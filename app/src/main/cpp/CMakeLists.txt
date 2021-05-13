
cmake_minimum_required(VERSION 3.4.1)

file(GLOB allCpp *.cpp)

add_library(
        native-lib

        SHARED

        ${allCpp})


include_directories(${CMAKE_SOURCE_DIR}/include)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L${CMAKE_SOURCE_DIR}/${CMAKE_ANDROID_ARCH_ABI}")


find_library(
        log-lib

        log)


target_link_libraries(
        native-lib

        -Wl,--start-group

        avcodec avfilter avformat avutil swresample swscale
        -Wl,--end-group
        z
        android


        ${log-lib})