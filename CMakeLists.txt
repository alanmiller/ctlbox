#set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${PICO_SDK_PATH}/cmake")
cmake_minimum_required(VERSION 3.25)

set(PICO_BOARD pico_w)
set(WIFI_PASSWORD $ENV{WIFI_PASSWORD})
set(WIFI_SSID $ENV{WIFI_SSID})

# Pico SDK (before project)
include($ENV{PICO_SDK_PATH}/pico_sdk_init.cmake)

# Project
project(ctlbox VERSION 1.0.2)

pico_sdk_init()

# Source
add_executable(${PROJECT_NAME} 
    main.c
    usb_descriptors.c
)
target_sources(${PROJECT_NAME} PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/main.c
        ${CMAKE_CURRENT_LIST_DIR}/usb_descriptors.c
        )

pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 1)

# For usb HID device
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR})

# Uncomment this line to enable fix for Errata RP2040-E5 (the fix requires use of GPIO 15)
#target_compile_definitions(${PROJECT_NAME} PUBLIC PICO_RP2040_USB_DEVICE_ENUMERATION_FIX=1)
target_link_libraries(${PROJECT_NAME} 
    PUBLIC
    pico_stdlib
    pico_cyw43_arch_none
    tinyusb_device)

# Binary
pico_set_program_version(ctlbox "${PROJECT_VERSION}")
pico_set_program_description(ctlbox "CtlBox firmware")
pico_set_binary_type(ctlbox copy_to_ram)
pico_add_extra_outputs(${PROJECT_NAME})

target_compile_definitions(${PROJECT_NAME} PRIVATE
  PICO_USB_DEVICE_ENUMERATION_DELAY_MS=2000
  PICO_USB_DEVICE_URL="https://github.com/alanmiller/ctlbox"
)
# Verbose compiler warnings
#if(MSVC)
#    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
#else()
#    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Werror)
#endif()
