cmake_minimum_required(VERSION 2.8.4)

PROJECT(SERVER)

set (CMAKE_MODULE_PATH ${SERVER_SOURCE_DIR}/CMake)

# CMAKE_POLICY(SET CMP0045 OLD)

#set_directory_properties(PROPERTIES COMPILE_DEFINITIONS_DEBUG "_DEBUG")

include(cotire)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++0x -fno-omit-frame-pointer ")
SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -D_DEBUG -DDEBUG -O0 -g  -Wno-unused -Wno-unused-function -Wno-unused-variable -Wno-invalid-offsetof -Wno-sign-compare ")
SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -D_DEBUG -DDEBUG -DDNA_LIVE_ENV -O1 -g  -Wno-unused -Wno-unused-function -Wno-unused-variable -Wno-invalid-offsetof -Wno-sign-compare ")

SET(EXECUTABLE_OUTPUT_PATH ${SERVER_BINARY_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${SERVER_SOURCE_DIR}/lib)


MESSAGE(STATUS "Server Bin path: ${SERVER_BINARY_DIR}")
MESSAGE(STATUS "Exe path: ${EXECUTABLE_OUTPUT_PATH}")
MESSAGE(STATUS "Src path: ${SERVER_SOURCE_DIR}")

INCLUDE_DIRECTORIES(${SERVER_SOURCE_DIR}/common/include )
INCLUDE_DIRECTORIES(${SERVER_SOURCE_DIR}/common/log )
INCLUDE_DIRECTORIES(${SERVER_SOURCE_DIR}/common/util )
INCLUDE_DIRECTORIES(${SERVER_SOURCE_DIR}/common/network/netproc )
INCLUDE_DIRECTORIES(${SERVER_SOURCE_DIR}/protocol )
#INCLUDE_DIRECTORIES(${SERVER_SOURCE_DIR}/thirdparty/protobuf-2.5.0/include )
INCLUDE_DIRECTORIES(/usr/local/include/ )
INCLUDE_DIRECTORIES(${SERVER_SOURCE_DIR}/thirdparty )
INCLUDE_DIRECTORIES(${SERVER_SOURCE_DIR}/thirdparty/apollo_voice_all/include )
INCLUDE_DIRECTORIES(${SERVER_SOURCE_DIR}/share/table )

LINK_DIRECTORIES( ${SERVER_SOURCE_DIR}/lib )

ADD_SUBDIRECTORY(dbserver)
ADD_SUBDIRECTORY(gateserver)
ADD_SUBDIRECTORY(loginserver)
ADD_SUBDIRECTORY(audioserver)
ADD_SUBDIRECTORY(gameserver)
ADD_SUBDIRECTORY(masterserver)
ADD_SUBDIRECTORY(controlserver)
ADD_SUBDIRECTORY(versionserver)
