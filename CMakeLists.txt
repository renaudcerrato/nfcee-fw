CMAKE_MINIMUM_REQUIRED(VERSION 3.6)
INCLUDE(Environment.cmake)

PROJECT(nfcee)
ENABLE_LANGUAGE(ASM)

FIND_PACKAGE(ChibiOS 16 COMPONENTS rt hal adc can dac ext gpt i2c i2s icu mac mmc_spi pal pwm rtc sdc serial serial_usb spi st uart usb chprintf memstreams nullstreams evtimer shell syscalls REQUIRED)

INCLUDE_DIRECTORIES(
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${ChibiOS_INCLUDE_DIRS}
        config
        board
)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
ADD_DEFINITIONS(-DCORTEX_USE_FPU=TRUE)
SET(STM32_LINKER_SCRIPT ${ChibiOS_LINKER_SCRIPT})

SET(SOURCE_FILES main.c board/board.c board/board.h config/mcuconf.h config/halconf.h config/chconf.h)

ADD_EXECUTABLE(${CMAKE_PROJECT_NAME}.elf ${SOURCE_FILES}  ${ChibiOS_SOURCES})

TARGET_LINK_LIBRARIES(${CMAKE_PROJECT_NAME}.elf)

STM32_SET_TARGET_PROPERTIES(${CMAKE_PROJECT_NAME}.elf)
STM32_ADD_HEX_BIN_TARGETS(${CMAKE_PROJECT_NAME}.elf)
STM32_PRINT_SIZE_OF_TARGETS(${CMAKE_PROJECT_NAME}.elf)

