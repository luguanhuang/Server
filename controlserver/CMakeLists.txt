INCLUDE_DIRECTORIES( . network protocol )
#LINK_DIRECTORIES(${LIBRARY_OUTPUT_PATH})
INCLUDE_DIRECTORIES(${SERVER_SOURCE_DIR}/share )
INCLUDE_DIRECTORIES(${SERVER_SOURCE_DIR}/thirdparty/tsf4g_lib/include)


LINK_DIRECTORIES( /usr/lib64/mysql )


FILE(GLOB_RECURSE SRC_LIST *.cpp)
AUX_SOURCE_DIRECTORY(${SERVER_SOURCE_DIR}/protocol/pb SRC_LIST)
AUX_SOURCE_DIRECTORY(${SERVER_SOURCE_DIR}/share/util SRC_LIST)
AUX_SOURCE_DIRECTORY(${SERVER_SOURCE_DIR}/share/table SRC_LIST)
AUX_SOURCE_DIRECTORY(${SERVER_SOURCE_DIR}/share/sharememory SRC_LIST)
AUX_SOURCE_DIRECTORY(${SERVER_SOURCE_DIR}/share/profile SRC_LIST)
AUX_SOURCE_DIRECTORY(${SERVER_SOURCE_DIR}/share/version SRC_LIST)
AUX_SOURCE_DIRECTORY(${SERVER_SOURCE_DIR}/share/idip SRC_LIST)

LIST(APPEND SRC_LIST ${SERVER_SOURCE_DIR}/share/DBInfo.cpp
					 ${SERVER_SOURCE_DIR}/share/functionopenbase.cpp)



ADD_EXECUTABLE(controlserver ${SRC_LIST})
TARGET_LINK_LIBRARIES(controlserver common epoll util pthread mysql mysqlclient tcmalloc profiler protobuf z tsf4g scew expat curl)



set_target_properties(controlserver PROPERTIES COTIRE_CXX_PREFIX_HEADER_INIT "pch.h")
cotire(controlserver)
