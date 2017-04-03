SET(STM32_CHIP STM32F407VG)

IF(NOT CHIBIOS_ROOT)
  SET(CHIBIOS_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/external/ChibiOS")
ENDIF()

IF(NOT CMAKE_TOOLCHAIN_FILE)
  SET(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/external/stm32-cmake/cmake/gcc_stm32.cmake")
ENDIF()