cmake_minimum_required(VERSION 3.16)

set(SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/stm32_radio_driver/lr_fhss_mac.c
    ${CMAKE_CURRENT_SOURCE_DIR}/stm32_radio_driver/radio_driver.c
    ${CMAKE_CURRENT_SOURCE_DIR}/stm32_radio_driver/radio_fw.c
    ${CMAKE_CURRENT_SOURCE_DIR}/stm32_radio_driver/radio.c
    ${CMAKE_CURRENT_SOURCE_DIR}/stm32_radio_driver/wl_lr_fhss.c
)

add_library(STM32WLxx_SUBGHZ_PHY STATIC ${SRCS})

target_include_directories(STM32WLxx_SUBGHZ_PHY PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(STM32WLxx_SUBGHZ_PHY PUBLIC
    STM32WLxx_Conf
    STM32_Utilities
    STM32WLxx_BSP
)