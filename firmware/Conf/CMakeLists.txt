cmake_minimum_required(VERSION 3.16)

add_library(STM32WLxx_Conf INTERFACE)

target_include_directories(STM32WLxx_Conf INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(STM32WLxx_Conf INTERFACE
    STM32WLxx_HAL_Driver
    STM32WLxx_BSP
    STM32_Utilities
)