cmake_minimum_required(VERSION 3.5)

project(ffmpeg_sdl_test LANGUAGES CXX)

add_subdirectory(logging)

message(${LIBPREFIX}avcodec${LIBSUFFIX})

find_library(LIBAVCODEC
    NAMES
    ${LIBPREFIX}avcodec${LIBSUFFIX}
     PATHS
    ${LIBRARY_SEARCH_PATHS})
find_library(LIBAVFORMAT
    NAMES
    ${LIBPREFIX}avformat${LIBSUFFIX}
     PATHS
    ${LIBRARY_SEARCH_PATHS})
find_library(LIBAVUTIL
    NAMES
    ${LIBPREFIX}avutil${LIBSUFFIX}
     PATHS
    ${LIBRARY_SEARCH_PATHS})
find_library(LIBSWSCALE
    NAMES
    ${LIBPREFIX}swscale${LIBSUFFIX}
     PATHS
    ${LIBRARY_SEARCH_PATHS})


if(WINDOWS)
    find_library(ARP
        NAMES
        libaprutil${LIBSUFFIX}
         PATHS
        ${LIBRARY_SEARCH_PATHS})
    find_library(LIBSDL2
        NAMES
        ${LIBPREFIX}SDL2${LIBSUFFIX} ${LIBPREFIX}SDL2main${LIBSUFFIX}
         PATHS
        ${LIBRARY_SEARCH_PATHS})
elseif(MACOS)
    find_library(ARP
        NAMES
        libapr-1.dylib libaprutil-1.dylib
        PATHS
        ${LIBRARY_SEARCH_PATHS})
    find_library(LIBSDL2
        NAMES
        ${LIBPREFIX}SDL2${LIBSUFFIX}
         PATHS
        ${LIBRARY_SEARCH_PATHS})
endif()



find_library(LOG4CXX
    NAMES
    ${LIBPREFIX}log4cxx${LIBSUFFIX}
     PATHS
    ${LIBRARY_SEARCH_PATHS})


find_package(Qt5 COMPONENTS Widgets REQUIRED)

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
