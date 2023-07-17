cmake_minimum_required(VERSION 3.16)
project(CatTracker C CXX ASM)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR ARM)

set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
set(CMAKE_AR arm-none-eabi-ar)
set(CMAKE_NM arm-none-eabi-nm)
set(CMAKE_RANLIB arm-none-eabi-ranlib)

set(CMAKE_C_FLAGS "-g -mcpu=cortex-m4 -mthumb -mfloat-abi=soft")
set(CMAKE_CXX_FLAGS "-g -mcpu=cortex-m4 -mthumb -mfloat-abi=soft")
set(CMAKE_ASM_FLAGS "-g -mcpu=cortex-m4 -mthumb -mfloat-abi=soft")
set(CMAKE_EXE_LINKER_FLAGS "-T ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/CMSIS/Device/ST/STM32WLxx/Source/Templates/gcc/linker/STM32WLE5XX_FLASH.ld -Wl,-Map=CatBeHome.map -Wl,--gc-sections -static -Wl,--start-group -lc -lm -Wl,--end-group")

add_compile_definitions(-DUSE_HAL_DRIVER -DSTM32WLE5xx -DCORE_CM4 -DUSE_BSP_DRIVER)

add_subdirectory(Conf)
add_subdirectory(BSP/E77-900M22S)
add_subdirectory(Drivers/CMSIS)
add_subdirectory(Drivers/STM32WLxx_HAL_Driver)
add_subdirectory(Utilities)
add_subdirectory(Middlewares/Third_Party/SubGHz_Phy)
add_subdirectory(System)
add_subdirectory(CatTracker)