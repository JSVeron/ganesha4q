project(libqsfs)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
SET(CMAKE_BUILD_TYPE "Debug")


SET(libqsfs_LIB_SRCS
	main.c
	internal.c
	internal.h
	handle.c
	export.c
)

include_directories(${CUKE_INCLUDE_DIRS} ./src/)

add_library(libqsfs ${libqsfs_LIB_SRCS})

target_link_libraries(libqsfs  /root/Github/ganesha4q/src/libqsfs.a)

#if(GMOCK_FOUND)
    #add_executable(GTestBucketSteps src/GTestBucketSteps.cpp)
    
#endif()

