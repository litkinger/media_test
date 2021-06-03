cmake_minimum_required(VERSION 3.5)

project(ffmpeg_sdl_test LANGUAGES CXX)

add_subdirectory(logging)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_INCLUDE_DIRECTORIES_BEFORE ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -v -stdlib=libc++ -I/usr/local/include -L/usr/local/lib/ -I/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include -L/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/lib/")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 ")

set(Qt5_DIR "/Users/yaoping.zheng/Qt5.12.10/5.12.10/clang_64/lib/cmake/Qt5")

find_library(LIBAVCODEC libavcodec.dylib)
find_library(LIBAVFORMAT libavformat.dylib)
find_library(LIBAVUTIL libavutil.dylib)
find_library(LIBSWSCALE libswscale.dylib)
find_library(LIBSDL2 libSDL2.dylib)
find_library(ARP libapr-1.dylib libaprutil-1.dylib "../3rd/apr-1.7.0/.libs/")
find_library(LOG4CXX liblog4cxx.dylib)
find_package(Qt5 COMPONENTS Widgets REQUIRED)

include_directories(/usr/local/include)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_ROOT_PATH}/bin)

MARK_AS_ADVANCED(LIBAVCODEC LIBAVUTIL LIBSWSCALE LIBAVFORMAT)
set(extra_libs ${LIBAVCODEC} ${LIBAVUTIL} ${LIBSWSCALE} ${LIBAVFORMAT} ${LIBSDL2} ${ARP} ${LOG4CXX} )

#macro(ADD_FRAMEWORK appname fwname)
#find_library(FRAMEWORK_${fwname} NAMES ${fwname} PATHS ${CMAKE_OSX_SYSROOT}/System/Library PATH_SUFFIXES Frameworks NO_DEFAULT_PATH)
#if( ${FRAMEWORK_${fwname}} STREQUAL FRAMEWORK_${fwname}-NOTFOUND )
#    MESSAGE(ERROR ": Framework ${fwname} not found")
#else()
#    TARGET_LINK_LIBRARIES("${appname} ${FRAMEWORK_${fwname}}")
#    MESSAGE(STATUS ": Framework ${fwname} found at ${FRAMEWORK_${fwname}}")
#endif()
#endmacro(ADD_FRAMEWORK)


# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check http://doc.qt.io/qt-5/deployment-android.html for more information.
# They need to be set before the find_package(Qt5 ...) call.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()


#link_directories(${CMAKE_OSX_SYSROOT}/System/Library)

#if(ANDROID)
#  add_library(ffmpeg_sdl_test SHARED
#    main.cpp
#    video.h
#    video.cpp
#    audio.cpp
#    audio.h
#    avcommon.h
#    avcommon.cpp
#  )
#else()
  add_executable(ffmpeg_sdl_test
    main.cpp
    video.h
    video.cpp
    audio.cpp
    audio.h
    avcommon.h
    avcommon.cpp
  )
#endif()

#add_framework(ffmpeg_sdl_test VideoToolbox)

target_link_libraries(ffmpeg_sdl_test PRIVATE Qt5::Widgets ${extra_libs} logging)
