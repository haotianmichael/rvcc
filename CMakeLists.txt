cmake_minimum_required(VERSION 3.2)
PROJECT(release)

add_definitions(-std=c++11)
INCLUDE_DIRECTORIES(include)
AUX_SOURCE_DIRECTORY(src DIR_SRCS)

SET(TEST_RELEASE
    ${DIR_SRCS}
    )

ADD_EXECUTABLE(${PROJECT_NAME} ${TEST_RELEASE})

