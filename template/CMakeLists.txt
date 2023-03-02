INCLUDE_DIRECTORIES( . network protocol )
#LINK_DIRECTORIES(${LIBRARY_OUTPUT_PATH})



FILE(GLOB_RECURSE SRC_LIST *.cpp)
AUX_SOURCE_DIRECTORY(${SERVER_SOURCE_DIR}/protocol/pb SRC_LIST)



ADD_EXECUTABLE(myserver ${SRC_LIST})
TARGET_LINK_LIBRARIES(myserver network pthread tcmalloc profiler protobuf)



set_target_properties(myserver PROPERTIES COTIRE_CXX_PREFIX_HEADER_INIT "pch.h")
cotire(myserver)
