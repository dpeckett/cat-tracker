cmake_minimum_required(VERSION 3.16)

file(GLOB_RECURSE SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/Src/*.c
)

list(FILTER SRCS EXCLUDE REGEX "_template\\.c$")

add_library(STM32WLxx_HAL_Driver STATIC ${SRCS})

target_include_directories(STM32WLxx_HAL_Driver PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Inc
)

target_link_libraries(STM32WLxx_HAL_Driver PUBLIC
    STM32WLxx_CMSIS_Driver
    STM32WLxx_Conf
)

#add_library(STM32WLxx_HAL_Driver_Headers INTERFACE)

#target_include_directories(STM32WLxx_HAL_Driver_Headers INTERFACE
#    ${CMAKE_CURRENT_SOURCE_DIR}/Conf
#    ${CMAKE_CURRENT_SOURCE_DIR}/Inc
#)