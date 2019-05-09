cmake_minimum_required (VERSION 3.5.1)

project(utsuite C CXX)
#project(testcli  CXX)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

set(CMAKE_BUILD_TYPE DEBUG)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release")
endif()

message("CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE}")
IF(CMAKE_BUILD_TYPE MATCHES Debug)
  set(CMAKE_CXX_FLAGS_DEBUG "-O0")
  message("Debug build.")
ELSEIF(CMAKE_BUILD_TYPE MATCHES Release)
  set(CMAKE_CXX_FLAGS_RELEASE "-O2 -finline-limit=1000 -DNDEBUG")
  message("Release build.")
ELSE()
  message("Some other build type.")
ENDIF()

message(STATUS "PROJECT_BINARY_DIR="  ${PROJECT_BINARY_DIR} )
message(STATUS "PROJECT_SOURCE_DIR="  ${PROJECT_SOURCE_DIR} )
message(STATUS "CMAKE_SOURCE_DIR="  ${CMAKE_SOURCE_DIR} )
message(STATUS "CMAKE_MODULE_PATH="  ${CMAKE_MODULE_PATH} )


set(CXX_FLAGS
 -g
 -DCHECK_PTHREAD_RETURN_VALUE
 -D_FILE_OFFSET_BITS=64
 -Wall
 -Wextra
 -Wsign-compare
# -Werror
# -Wconversion
 -Wunused-variable
 -Wno-unused-parameter
# -Wold-style-cast
 -Woverloaded-virtual
 -Wpointer-arith
# -Wshadow
 -Wwrite-strings
 -march=native
 # -MMD
 -std=c++11
 -rdynamic
 -pthread
# -fPIC
 )

if(CMAKE_BUILD_BITS EQUAL 32)
  list(APPEND CXX_FLAGS "-m32")
endif()

string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CXX_FLAGS}")

set(CMAKE_CXX_COMPILER "g++")

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

ADD_DEFINITIONS(-DBOOST_ALL_DYN_LINK )

FIND_PACKAGE(Boost COMPONENTS  system  filesystem program_options thread log_setup log  REQUIRED)
if(NOT Boost_FOUND)
  message(FATAL_ERROR "Boost log  not found!")
endif(NOT Boost_FOUND)

find_package(ZLIB)

if(ZLIB_FOUND)
  message(STATUS "found zlib")
endif()


include_directories(${Boost_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR})

string(TOUPPER ${CMAKE_BUILD_TYPE} BUILD_TYPE)
message(STATUS "CXX_FLAGS = " ${CMAKE_CXX_FLAGS} " " ${CMAKE_CXX_FLAGS_${BUILD_TYPE}})

add_subdirectory(libqtutils)
#add_subdirectory(libdbadapter)
#add_subdirectory(testboost)
add_subdirectory(libwallet)
add_subdirectory(mysign)
add_subdirectory(uttools)
#add_subdirectory(claimtest)
add_subdirectory(walletd)
#add_subdirectory(tblmgrt) 
#add_subdirectory(filetodb) 
#add_subdirectory(tests) 
#add_subdirectory(ulordcenter) 
#add_subdirectory(unode) 
add_subdirectory(equihash) 




