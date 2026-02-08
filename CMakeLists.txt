# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.0)
set(toolchainVersion 13_3_Rel1)
set(picotoolVersion 2.1.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Pico VS Code extension (não mexer)
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()

set(PICO_BOARD pico_w CACHE STRING "Board type")

# Pico SDK
include(pico_sdk_import.cmake)

project(EstufaInteligente_AI C CXX ASM)

pico_sdk_init()

# Executável
add_executable(EstufaInteligente_AI
    EstufaInteligente_AI.c
    lib/bh1750_light_sensor.c
    lib/matriz.c
    lib/ssd1306.c
)

# Geração do header PIO (🔴 ponto crítico)
pico_generate_pio_header(
    EstufaInteligente_AI
    ${CMAKE_CURRENT_LIST_DIR}/lib/pio_matrix.pio
)

pico_set_program_name(EstufaInteligente_AI "EstufaInteligente_AI")
pico_set_program_version(EstufaInteligente_AI "0.1")

pico_enable_stdio_uart(EstufaInteligente_AI 1)
pico_enable_stdio_usb(EstufaInteligente_AI 1)

# Includes
target_include_directories(EstufaInteligente_AI PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/lib
)

# Bibliotecas
target_link_libraries(EstufaInteligente_AI
    pico_stdlib
    hardware_i2c
    hardware_pio
    hardware_clocks
    hardware_gpio
)

pico_add_extra_outputs(EstufaInteligente_AI)
