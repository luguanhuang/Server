INCLUDE_DIRECTORIES( . network protocol )
#LINK_DIRECTORIES(${LIBRARY_OUTPUT_PATH})
INCLUDE_DIRECTORIES(${SERVER_SOURCE_DIR}/share )
INCLUDE_DIRECTORIES(${SERVER_SOURCE_DIR}/thirdparty/kcp )



FILE(GLOB_RECURSE SRC_LIST *.cpp)
AUX_SOURCE_DIRECTORY(${SERVER_SOURCE_DIR}/protocol/pb SRC_LIST)
AUX_SOURCE_DIRECTORY(${SERVER_SOURCE_DIR}/share/version SRC_LIST)
AUX_SOURCE_DIRECTORY(${SERVER_SOURCE_DIR}/share/profile SRC_LIST)



ADD_EXECUTABLE(gateserver ${SRC_LIST})
TARGET_LINK_LIBRARIES(gateserver common epoll util kcp pthread tcmalloc profiler protobuf z)



set_target_properties(gateserver PROPERTIES COTIRE_CXX_PREFIX_HEADER_INIT "pch.h")
cotire(gateserver)
