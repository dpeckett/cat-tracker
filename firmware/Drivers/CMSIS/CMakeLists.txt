cmake_minimum_required(VERSION 3.16)

set(SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/Device/ST/STM32WLxx/Source/Templates/system_stm32wlxx.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Device/ST/STM32WLxx/Source/Templates/gcc/startup_stm32wle5xx.s
)

set(HEADER_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/Include
    ${CMAKE_CURRENT_SOURCE_DIR}/Device/ST/STM32WLxx/Include
    ${CMAKE_CURRENT_SOURCE_DIR}/
)

add_library(STM32WLxx_CMSIS_Driver STATIC ${SRCS})

target_include_directories(STM32WLxx_CMSIS_Driver PUBLIC ${HEADER_DIRS})

target_link_libraries(STM32WLxx_CMSIS_Driver PUBLIC
    STM32WLxx_Conf
    STM32WLxx_HAL_Driver
    #STM32WLxx_HAL_Driver_Headers
)