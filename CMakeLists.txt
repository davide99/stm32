cmake_minimum_required(VERSION 3.0)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_C_COMPILER_ID GNU)
set(CMAKE_CXX_COMPILER_ID GNU)
set(CMAKE_C_COMPILER_FORCED TRUE)
set(CMAKE_CXX_COMPILER_FORCED TRUE)


project(stm32 C CXX ASM)

set(LINKER_SCRIPT "stm32.ld")
set(CPU_OPTIONS -mthumb -mcpu=cortex-m3 -mfloat-abi=softfp)

set(CMAKE_C_FLAGS_DEBUG "-g -Og")
set(CMAKE_C_FLAGS_RELEASE "-Os -flto")
set(CMAKE_CXX_FLAGS_DEBUG "-g -Og")
set(CMAKE_CXX_FLAGS_RELEASE "-Os -flto")
set(CMAKE_ASM_FLAGS_DEBUG  "-c -O0")
set(CMAKE_ASM_FLAGS_RELEASE  "-c -Os")

add_compile_options(
        ${CPU_OPTIONS}
        $<$<COMPILE_LANGUAGE:C>:-std=c11>
        $<$<COMPILE_LANGUAGE:CXX>:-std=c++17>
        $<$<COMPILE_LANGUAGE:CXX>:-fno-exceptions>
        $<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>
        -Wall
        -fno-common
        -ffunction-sections
        -fdata-sections
)

include_directories(
        ${CMAKE_SOURCE_DIR}
)

link_directories(
        ${CMAKE_SOURCE_DIR}
)

add_executable(${PROJECT_NAME}
        vector_table.s
        core.s
        core.c
        main.cpp
        lib/Serial.h lib/Serial.cpp
        lib/GPIO.h lib/GPIO.cpp
        lib/common.h
        lib/Utils.cpp lib/Utils.h
        lib/Interrupts.cpp lib/Interrupts.h
        lib/SPI.cpp lib/SPI.h
        lib/MFRC522.cpp lib/MFRC522.h
        lib/SD.cpp lib/SD.h lib/Clock.cpp lib/Clock.h lib/Timer2.cpp lib/Timer2.h)

target_link_libraries(${PROJECT_NAME}
        ${CPU_OPTIONS}
        -T${LINKER_SCRIPT}
        --specs=nosys.specs
        --static
        -Wl,--gc-sections
        -nostdlib
        -lgcc
        -lc
        -Wall
)

set_property(TARGET ${PROJECT_NAME} PROPERTY LINK_DEPENDS ${CMAKE_SOURCE_DIR}/${LINKER_SCRIPT})

add_custom_command(OUTPUT ${PROJECT_NAME}.ihex
        DEPENDS ${PROJECT_NAME}
        COMMAND ${CMAKE_OBJCOPY} -Oihex ${PROJECT_NAME} ${PROJECT_NAME}.ihex
)

add_custom_command(OUTPUT ${PROJECT_NAME}.bin
        DEPENDS ${PROJECT_NAME}
        COMMAND ${CMAKE_OBJCOPY} -Obinary ${PROJECT_NAME} ${PROJECT_NAME}.bin
)

add_custom_command(OUTPUT ${PROJECT_NAME}.lst
        DEPENDS ${PROJECT_NAME}
        COMMAND ${CMAKE_OBJDUMP} -S ${PROJECT_NAME} > ${PROJECT_NAME}.lst
)

add_custom_command(OUTPUT ${PROJECT_NAME}.sym
        DEPENDS ${PROJECT_NAME}
        COMMAND ${CMAKE_NM} -C -l -n -S ${PROJECT_NAME} > ${PROJECT_NAME}.sym
)


add_custom_target(ihex
        DEPENDS ${PROJECT_NAME}.ihex
)

add_custom_target(bin
        DEPENDS ${PROJECT_NAME}.bin
)

add_custom_target(lst
        DEPENDS ${PROJECT_NAME}.lst
)

add_custom_target(sym
        DEPENDS ${PROJECT_NAME}.sym
)

add_custom_target(flash
        st-flash --format ihex write ${PROJECT_NAME}.ihex
        DEPENDS ${PROJECT_NAME}.ihex
)