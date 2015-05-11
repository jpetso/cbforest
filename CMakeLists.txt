cmake_minimum_required(VERSION 2.8)
project(CBForest)

if (${CMAKE_MAJOR_VERSION} GREATER 2)
    CMAKE_POLICY(SET CMP0042 NEW)
endif()

INCLUDE(CheckFunctionExists)
INCLUDE(CheckIncludeFileCXX)
INCLUDE(CheckIncludeFiles)
INCLUDE(CheckLibraryExists)
INCLUDE(CheckSymbolExists)
INCLUDE(CheckTypeSize)

set (SNAPPY_OPTION "Disable") # for ForestDB's CMakeLists.txt
add_subdirectory(vendor)

include_directories(BEFORE ${PROJECT_SOURCE_DIR})
include_directories(BEFORE ${FORESTDB_INCLUDE_DIRS})

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING
    "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif()

if (NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    #-Wunused-variable -Wall -pedantic -Wredundant-decls")
endif()

if (_MEMLEAK EQUAL 1)
    ADD_DEFINITIONS(-D_MEMLEAK_ENABLE=1)
elseif (_MEMLEAK EQUAL 2)
    ADD_DEFINITIONS(-D_MEMLEAK_ENABLE=1)
    ADD_DEFINITIONS(-D_CHK_MODIFY_AFTER_FREE=1)
endif()

# Uncomment the following define below to enable handle tracking on SIGABRT
# set(_TRACK_HANDLES 1)
#

if (_TRACK_HANDLES EQUAL 1)
    ADD_DEFINITIONS(-D_TRACE_HANDLES=1)
endif()

if ("${CMAKE_BUILD_TYPE}" STREQUAL "Release" OR "${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo")
    set(_CBFOREST_RELEASEISH_BUILD ON)
endif()

if ("${CMAKE_C_COMPILER_ID}" STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Qunused-arguments")
    set(_CBFOREST_UNIX_COMPILER ON)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(_CBFOREST_UNIX_COMPILER ON)
endif()

if (_CBFOREST_RELEASEISH_BUILD AND _CBFOREST_UNIX_COMPILER)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fomit-frame-pointer")
endif()

set(CBFOREST_SOURCES
    CBForest/Collatable.cc
    CBForest/Database.cc
    CBForest/DocEnumerator.cc
    CBForest/Document.cc
    CBForest/GeoIndex.cc
    CBForest/Geohash.cc
    CBForest/Index.cc
    CBForest/KeyStore.cc
    CBForest/MapReduceIndex.cc
    CBForest/RevID.cc
    CBForest/RevTree.cc
    CBForest/Tokenizer.cc
    CBForest/VersionedDocument.cc
    CBForest/slice.cc
    CBForest/varint.cc)

if (APPLE)
    LIST(APPEND CBFOREST_SOURCES
        CBForest/Collatable.mm
        CBForest/MapReduceDispatchIndexer.cc
        CBForest/VersionedDocument.mm
        CBForest/slice.mm)
endif()

add_library(cbforest ${CBFOREST_SOURCES})
target_link_libraries(cbforest snappy forestdb)

install(FILES
    CBForest/CBForest.hh
    CBForest/Collatable.hh
    CBForest/Database.hh
    CBForest/Document.hh
    CBForest/DocEnumerator.hh
    CBForest/Error.hh
    CBForest/GeoIndex.hh
    CBForest/Geohash.hh
    CBForest/Index.hh
    CBForest/KeyStore.hh
    CBForest/MapReduceIndex.hh
    CBForest/RevID.hh
    CBForest/RevTree.hh
    CBForest/Tokenizer.hh
    CBForest/VersionedDocument.hh
    CBForest/slice.hh
    DESTINATION include/CBForest)

if (APPLE)
    install(FILES
        CBForest/MapReduceDispatchIndexer.hh
        DESTINATION include/CBForest)
endif()

install(TARGETS cbforest
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)

#install(TARGETS cbforest_tests
#    RUNTIME DESTINATION bin)


# add top-level testing dir
#enable_testing()
#add_subdirectory("${PROJECT_SOURCE_DIR}/CBForest Tests")

