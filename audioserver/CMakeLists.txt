INCLUDE_DIRECTORIES( . network protocol )
INCLUDE_DIRECTORIES( /usr/include/mysql )
INCLUDE_DIRECTORIES( ${SERVER_SOURCE_DIR}/share )
INCLUDE_DIRECTORIES(${SERVER_SOURCE_DIR}/thirdparty/tsf4g_lib/include)

LINK_DIRECTORIES( /usr/lib64/mysql )


FILE(GLOB_RECURSE SRC_LIST *.cpp)
AUX_SOURCE_DIRECTORY(${SERVER_SOURCE_DIR}/share SRC_LIST)
AUX_SOURCE_DIRECTORY(${SERVER_SOURCE_DIR}/share/util SRC_LIST)
AUX_SOURCE_DIRECTORY(${SERVER_SOURCE_DIR}/protocol/pb SRC_LIST)
					 
ADD_EXECUTABLE(audioserver ${SRC_LIST})
TARGET_LINK_LIBRARIES(audioserver common epoll util mysql pthread mysqlclient tcmalloc profiler protobuf z tsf4g scew expat voip_static curl)

set_target_properties(audioserver PROPERTIES COTIRE_CXX_PREFIX_HEADER_INIT "pch.h")
cotire(audioserver)
