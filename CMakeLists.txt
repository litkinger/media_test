cmake_minimum_required(VERSION 3.5)

project(media_test LANGUAGES CXX)
SET(PROJECT_ROOT_PATH "${CMAKE_SOURCE_DIR}/build-media_test-Desktop_Qt_5_12_10_clang_64bit-Debug")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_INCLUDE_DIRECTORIES_BEFORE ON)

if (CMAKE_HOST_WIN32)
    set(WINDOWS 1)
    set(LIBSUFFIX .lib)
    set(LIBPREFIX )
    set(LIBRARY_SEARCH_PATHS
        C:/msys64/usr/src/src/third_party/ffmpeg/x64
        C:/msys64/usr/src/src/third_party/sdl/x64
        D:/workspace/opensource/SDL2/lib/x64
        D:/workspace/mycode/media_test/3rd/apache-log4cxx-0.10.0/projects/x64/Debug
        D:/workspace/mycode/media_test/3rd/apr-util/LibD
    )
    include_directories(C:/msys64/usr/local/ffmpeg/include)
    include_directories(C:/msys64/usr/src/src/third_party/sdl/include)
    include_directories(src)
    include_directories(D:/workspace/mycode/media_test/3rd/apache-log4cxx-0.10.0/src/main/include)
elseif(CMAKE_HOST_APPLE)
    set(MACOS 1)
    set(LIBSUFFIX .dylib)
    set(LIBPREFIX lib)
    set(LIBRARY_SEARCH_PATHS
        ../3rd/apr-1.7.0/.libs/
        /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/lib/
        /usr/local/lib/
        /usr/local/apr/lib
    )
    include_directories(/usr/local/include)
elseif(CMAKE_HOST_UNIX)
    set(LINUX 1)
    set(LIBSUFFIX .so)
    set(LIBPREFIX lib)
endif()

if(WINDOWS)
    set(Qt5_DIR "C:/Qt/Qt5.14.0/5.14.0/msvc2017_64/lib/cmake/Qt5")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I C:/msys64/usr/local/ffmpeg/include -I D:/workspace/mycode/media_test/3rd/apache-log4cxx-0.10.0/src/main/include -IC:/msys64/usr/src/src/third_party/sdl/include -IC:/msys64/usr/src/ffmpeg-4.3.2")
elseif(LINUX)

elseif(MACOS)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 ")
    set(Qt5_DIR "/Users/yaoping.zheng/Qt5.12.10/5.12.10/clang_64/lib/cmake/Qt5")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -v -stdlib=libc++ -I/usr/local/include -L/usr/local/lib/ -I/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include -L/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/lib/")
endif()


#SET(PROJECT_ROOT_PATH "${CMAKE_SOURCE_DIR}/build-ffmpeg_sdl_test-Desktop_Qt_5_12_10_clang_64bit-Debug")
#MESSAGE( STATUS "this var key = ${PROJECT_ROOT_PATH}.")

add_subdirectory(src)
#add_subdirectory(test)

