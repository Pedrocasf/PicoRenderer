# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.1)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.1.1)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
cmake_minimum_required(VERSION 3.12)
set(PICO_BOARD vgaboard_2_plus_w CACHE STRING "Board type")
set(PICO_PLATFORM rp2350)
set(PICO_EXTRAS_FETCH_FROM_GIT 1)
# Pull in PICO SDK (must be before project)
include(pico_sdk_import.cmake)

# We also need PICO EXTRAS
include(pico_extras_import.cmake)

project(PicoRenderer C CXX)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(PicoRenderer
        main.c
        faces.c
        matrix.c
        texture.raw.c
        tank.uvs.bin.c
        tank.txc.bin.c
        tank.faces.bin.c
        tank.vertices.bin.c
        gourad.c
        )
target_link_libraries(PicoRenderer PRIVATE
        pico_stdlib
        pico_multicore
        pico_scanvideo_dpi
        pico_runtime
        )
pico_add_extra_outputs(PicoRenderer)

pico_enable_stdio_uart(PicoRenderer 0)
pico_enable_stdio_usb(PicoRenderer 1)