cmake_minimum_required(VERSION 3.16)

set(SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/stm32wlxx_it.cc
)

add_executable(Tracker ${SRCS})

target_link_libraries(Tracker PRIVATE
    CatTrackerBase
    STM32WLxx_SUBGHZ_PHY 
    STM32_Utilities
    STM32WLxx_BSP
    STM32WLxx_CMSIS_Driver 
    STM32WLxx_HAL_Driver
    gcc c m 
    STM32_System)