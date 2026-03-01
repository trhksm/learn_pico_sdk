cmake_minimum_required(VERSION 3.13)
include(pico_sdk_import.cmake)

set(PROJ pio_blink)
project(${PROJ} C CXX ASM)

pico_sdk_init()

add_executable(${PROJ} main.c)

pico_generate_pio_header(${PROJ} ${CMAKE_CURRENT_LIST_DIR}/blink.pio)
target_link_libraries(${PROJ}
    pico_stdlib
    hardware_pio
    )

#stdio(printf)の出力先
pico_enable_stdio_usb(${PROJ} 1)
pico_enable_stdio_uart(${PROJ} 0)

pico_add_extra_outputs(${PROJ})
