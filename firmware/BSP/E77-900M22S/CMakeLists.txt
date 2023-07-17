cmake_minimum_required(VERSION 3.16)

set(SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/stm32wlxx_ebyte_radio.c
    ${CMAKE_CURRENT_SOURCE_DIR}/stm32wlxx_ebyte.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ADC/adc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ADC/adc_if.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Power/stm32_lpm_if.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Radio/radio_board_if.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Timer/rtc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Timer/timer_if.c
)

set(HEADER_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/ADC
    ${CMAKE_CURRENT_SOURCE_DIR}/Power
    ${CMAKE_CURRENT_SOURCE_DIR}/Radio
    ${CMAKE_CURRENT_SOURCE_DIR}/Timer
)

add_library(STM32WLxx_BSP STATIC ${SRCS})

target_include_directories(STM32WLxx_BSP PUBLIC ${HEADER_DIRS})

target_link_libraries(STM32WLxx_BSP PUBLIC
    STM32WLxx_Conf
    STM32WLxx_HAL_Driver
)