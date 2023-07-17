cmake_minimum_required(VERSION 3.16)

set(SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/lpm/stm32_lpm.c
    ${CMAKE_CURRENT_SOURCE_DIR}/misc/stm32_mem.c
    ${CMAKE_CURRENT_SOURCE_DIR}/misc/stm32_systime.c
    ${CMAKE_CURRENT_SOURCE_DIR}/misc/stm32_tiny_sscanf.c
    ${CMAKE_CURRENT_SOURCE_DIR}/misc/stm32_tiny_vsnprintf.c
    ${CMAKE_CURRENT_SOURCE_DIR}/sequencer/stm32_seq.c
    ${CMAKE_CURRENT_SOURCE_DIR}/timer/stm32_timer.c
)

set(HEADER_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/lpm
    ${CMAKE_CURRENT_SOURCE_DIR}/misc
    ${CMAKE_CURRENT_SOURCE_DIR}/sequencer
    ${CMAKE_CURRENT_SOURCE_DIR}/timer
)

add_library(STM32_Utilities STATIC ${SRCS})

target_include_directories(STM32_Utilities PUBLIC ${HEADER_DIRS})

target_link_libraries(STM32_Utilities PUBLIC
    STM32WLxx_CMSIS_Driver
    STM32WLxx_Conf
)