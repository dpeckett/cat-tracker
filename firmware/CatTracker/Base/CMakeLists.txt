cmake_minimum_required(VERSION 3.16)

set(SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/subghz_radio.cc
)

add_library(CatTrackerBase STATIC ${SRCS})

target_include_directories(CatTrackerBase PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(CatTrackerBase PRIVATE 
    STM32WLxx_SUBGHZ_PHY 
)