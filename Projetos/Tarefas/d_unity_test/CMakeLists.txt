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
# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# == Pico SDK Initialization ==
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

set(PICO_BOARD pico_w CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK
include(pico_sdk_import.cmake)

# == Diretórios de Include ==
include_directories(src)
include_directories(unity)
include_directories(tests)

project(d_unity_test C CXX ASM)

# Inicializar Pico SDK
pico_sdk_init()

# ------------------------------------------------------------------------------------
# 1. Programa Principal: temperature_program (firmware para a Pico)
# ------------------------------------------------------------------------------------

add_executable(temperature_program
    src/main.c
    src/temperature.c
)

# Configurar output do programa
pico_set_program_name(temperature_program "temperature_program")
pico_set_program_version(temperature_program "0.1")

# Habilitar saída via UART (e desabilitar USB para serial)
pico_enable_stdio_uart(temperature_program 1)
pico_enable_stdio_usb(temperature_program 0)

# Incluir bibliotecas da Pico SDK
target_link_libraries(temperature_program
    pico_stdlib
    hardware_adc
)

# Incluir diretórios
target_include_directories(temperature_program PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
)

# Gerar .uf2, .bin etc
pico_add_extra_outputs(temperature_program)

# ------------------------------------------------------------------------------------
# 2. Programa de Testes: temperature_tests (rodar no PC)
# ------------------------------------------------------------------------------------

add_executable(temperature_tests
    tests/main.c
    tests/test_temperature.c
    src/temperature.c
    unity/unity.c
)

# Diretórios para o programa de testes
target_include_directories(temperature_tests PRIVATE
    src
    unity
    tests
)

# NÃO linkar pico_stdlib, hardware_adc no teste,
# porque vamos rodar no PC, sem a placa

# Opcional: habilitar testes automáticos (ctest)
enable_testing()
add_test(NAME TemperatureUnitTests COMMAND temperature_tests)
