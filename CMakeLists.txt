project(GTestBucketSteps)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
SET(CMAKE_BUILD_TYPE "Debug")

set(CUKE_GTEST_LIBRARIES GTest::GTest)

include_directories(${CUKE_INCLUDE_DIRS} ./src/)

#add_library(libqs src/libqingstor.a)

#if(GMOCK_FOUND)
    add_executable(GTestBucketSteps src/GTestBucketSteps.cpp)
    target_link_libraries(GTestBucketSteps  /root/Github/ganesha4q/src/libqingstor.so /root/Github/ganesha4q/src/libcucumber-cpp.a /root/Github/ganesha4q/src/libgtest.a /root/Github/ganesha4q/src/libboost_regex.so
 /root/Github/ganesha4q/src/libboost_system.so /root/Github/ganesha4q/src/libboost_filesystem.so /root/Github/ganesha4q/src/libboost_program_options.so)
#endif()

