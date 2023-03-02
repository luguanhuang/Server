INCLUDE_DIRECTORIES( . network protocol )
INCLUDE_DIRECTORIES( /usr/include/mysql )
INCLUDE_DIRECTORIES(${SERVER_SOURCE_DIR}/share )
INCLUDE_DIRECTORIES(${SERVER_SOURCE_DIR}/thirdparty/tinyxml2 )
INCLUDE_DIRECTORIES(${SERVER_SOURCE_DIR}/thirdparty/tsf4g_lib/include)

LINK_DIRECTORIES( /usr/lib64/mysql )


FILE(GLOB_RECURSE SRC_LIST *.cpp)
AUX_SOURCE_DIRECTORY(${SERVER_SOURCE_DIR}/protocol/pb SRC_LIST)
AUX_SOURCE_DIRECTORY(${SERVER_SOURCE_DIR}/thirdparty/tinyxml2 SRC_LIST)
AUX_SOURCE_DIRECTORY(${SERVER_SOURCE_DIR}/share SRC_LIST)
AUX_SOURCE_DIRECTORY(${SERVER_SOURCE_DIR}/share/idip SRC_LIST)
AUX_SOURCE_DIRECTORY(${SERVER_SOURCE_DIR}/share/version SRC_LIST)
AUX_SOURCE_DIRECTORY(${SERVER_SOURCE_DIR}/share/profile SRC_LIST)

LIST(APPEND SRC_LIST ${SERVER_SOURCE_DIR}/share/util/jsonutil.cpp
					 ${SERVER_SOURCE_DIR}/share/util/UtilEncode.cpp
					 ${SERVER_SOURCE_DIR}/share/util/XRandom.cpp
					 ${SERVER_SOURCE_DIR}/share/util/XCommon.cpp
					 ${SERVER_SOURCE_DIR}/share/util/gametime.cpp
					 ${SERVER_SOURCE_DIR}/share/appid.cpp
					 )


ADD_EXECUTABLE(loginserver ${SRC_LIST})
TARGET_LINK_LIBRARIES(loginserver common epoll util pthread mysqlclient tcmalloc
		profiler protobuf z curl tsf4g scew expat)

set_target_properties(loginserver PROPERTIES COTIRE_CXX_PREFIX_HEADER_INIT "pch.h")
cotire(loginserver)
