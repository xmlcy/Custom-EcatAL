cmake_minimum_required(VERSION 3.9)
project(CustomEcatAL
    DESCRIPTION "Custom EtherCAT Application Layer"
    LANGUAGES C)

set(SRC_DIR ${PROJECT_SOURCE_DIR})

link_directories(${SRC_DIR}/lib)
include_directories(${SRC_DIR}/include/
                    ${SRC_DIR}/include/soem)

set(COM_SRC ${SRC_DIR}/customecatal.c
            ${SRC_DIR}/test.c)
add_executable(${PROJECT_NAME} ${COM_SRC})

target_link_libraries(${PROJECT_NAME} pthread soem)

