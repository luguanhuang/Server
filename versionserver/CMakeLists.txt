INCLUDE_DIRECTORIES( . network protocol connection )
INCLUDE_DIRECTORIES(${SERVER_SOURCE_DIR}/share)
INCLUDE_DIRECTORIES(${SERVER_SOURCE_DIR}/thirdparty/tinyxml2 )
INCLUDE_DIRECTORIES(${SERVER_SOURCE_DIR}/thirdparty/tsf4g_lib/include)


FILE(GLOB_RECURSE SRC_LIST *.cpp)
AUX_SOURCE_DIRECTORY(${SERVER_SOURCE_DIR}/protocol/pb SRC_LIST)
AUX_SOURCE_DIRECTORY(${SERVER_SOURCE_DIR}/thirdparty/tinyxml2 SRC_LIST)
AUX_SOURCE_DIRECTORY(${SERVER_SOURCE_DIR}/share/version SRC_LIST)

LIST(APPEND SRC_LIST ${SERVER_SOURCE_DIR}/share/util/jsonutil.cpp
					 ${SERVER_SOURCE_DIR}/share/util/UtilEncode.cpp
					 ${SERVER_SOURCE_DIR}/share/util/XRandom.cpp
					 )


ADD_EXECUTABLE(versionserver ${SRC_LIST})
TARGET_LINK_LIBRARIES(versionserver common epoll util pthread tcmalloc
		profiler protobuf z curl tsf4g scew expat)

set_target_properties(versionserver PROPERTIES COTIRE_CXX_PREFIX_HEADER_INIT "pch.h")
cotire(versionserver)
