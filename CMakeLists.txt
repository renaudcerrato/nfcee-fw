CMAKE_MINIMUM_REQUIRED(VERSION 3.0)
INCLUDE(Environment.cmake)

PROJECT(nfcee)
ENABLE_LANGUAGE(ASM)

FIND_PACKAGE(ChibiOS 16 COMPONENTS rt hal ext gpt icu serial spi st usb serial_usb chprintf memstreams nullstreams evtimer syscalls REQUIRED)
FIND_PROGRAM(OPENOCD openocd)

INCLUDE_DIRECTORIES(
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${ChibiOS_INCLUDE_DIRS}
        config
        board
        drivers/trf797x/include
        drivers/gpio
        drivers/gpio/stm32/v2/
        libs/trace/
        libs/nfc/include/
)

SET(SOURCE_FILES
        main.c
        usbcfg.c
        nfc.c
        board/board.c
        drivers/gpio/stm32/v2/gpio_lld.c
        drivers/trf797x/trf797x_lld.c
        drivers/trf797x/trf797x.c
        drivers/trf797x/trf797x_initiator.c
        libs/trace/trace.c
        libs/nfc/nfc.c
        libs/nfc/iso14443.c
        libs/nfc/iso14443a.c
)

ADD_DEFINITIONS(-DCORTEX_USE_FPU=TRUE)
ADD_DEFINITIONS(-DUSB_CDC_TRACE)

SET(STM32_LINKER_SCRIPT ${ChibiOS_LINKER_SCRIPT})

###
#    TARGET
###
SET(TARGET ${CMAKE_PROJECT_NAME}.elf)

IF(EXECUTABLE_OUTPUT_PATH)
    SET(TARGET_ABSOLUTE_DIR ${EXECUTABLE_OUTPUT_PATH})
    SET(TARGET_FILENAME "${EXECUTABLE_OUTPUT_PATH}/${TARGET}")
ELSE()
    SET(TARGET_ABSOLUTE_DIR ${CMAKE_CURRENT_BINARY_DIR})
    SET(TARGET_FILENAME "${TARGET}")
ENDIF()

ADD_EXECUTABLE(${TARGET} ${SOURCE_FILES} ${ChibiOS_SOURCES})
ADD_EXECUTABLE(elf ALIAS ${TARGET})
TARGET_LINK_LIBRARIES(${TARGET})

ADD_CUSTOM_TARGET(hex DEPENDS ${TARGET} COMMAND ${CMAKE_OBJCOPY} -Oihex ${TARGET_FILENAME} ${CMAKE_PROJECT_NAME}.hex)
ADD_CUSTOM_TARGET(bin DEPENDS ${TARGET} COMMAND ${CMAKE_OBJCOPY} -Obinary ${TARGET_FILENAME} ${CMAKE_PROJECT_NAME}.bin)
ADD_CUSTOM_TARGET(map ALL DEPENDS ${TARGET} COMMAND ${CMAKE_OBJDUMP} -t ${TARGET_FILENAME} > ${CMAKE_PROJECT_NAME}.map)

IF(OPENOCD)
    ADD_CUSTOM_TARGET(flash DEPENDS ${TARGET}
            WORKING_DIRECTORY ${TARGET_ABSOLUTE_DIR}
            COMMAND ${OPENOCD} -f ${CMAKE_CURRENT_SOURCE_DIR}/tools/openocd.cfg -c "program ${TARGET} reset exit")
ENDIF()

STM32_SET_TARGET_PROPERTIES(${TARGET})
STM32_PRINT_SIZE_OF_TARGETS(${TARGET})


