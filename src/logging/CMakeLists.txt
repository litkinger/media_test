cmake_minimum_required(VERSION 3.5)

project(logging LANGUAGES CXX)

#ADD_SUBDIRECTORY(lib)

find_library(LOG4CXX
    NAMES
    ${LIBPREFIX}log4cxx${LIBSUFFIX}
     PATHS
    ${LIBRARY_SEARCH_PATHS})

message(${LOG4CXX})

set(LIBRARY_OUTPUT_PATH ${PROJECT_ROOT_PATH}/lib)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

aux_source_directory(. srcs)

set(extra_libs ${LOG4CXX} )

ADD_LIBRARY(logging SHARED ${srcs})

ADD_LIBRARY(logging_static STATIC ${srcs})

#静态库名称
set_target_properties(logging_static PROPERTIES OUTPUT_NAME "logging")

#动态库是否生成
set_target_properties(logging PROPERTIES CLEAN_DIRECT_OUTPUT 1)

#静态库是否生成
set_target_properties(logging_static PROPERTIES CLEAN_DIRECT_OUTPUT 1)

#动态库 version
set_target_properties(logging PROPERTIES VERSION 1.0 SOVERSION 1)

#动态库依赖
target_link_libraries(logging PRIVATE ${extra_libs})




