cmake_minimum_required(VERSION 3.16)

set(SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/syscalls.c
    ${CMAKE_CURRENT_SOURCE_DIR}/sysmem.c
)

add_library(STM32_System STATIC ${SRCS})