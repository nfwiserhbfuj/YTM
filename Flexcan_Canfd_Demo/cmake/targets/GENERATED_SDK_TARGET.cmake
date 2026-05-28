cmake_minimum_required(VERSION 3.16)



set(sources
    ${PROJ_DIR}/platform/drivers/src/clock/YTM32B1Mx/clock_YTM32B1Mx.c
    ${PROJ_DIR}/platform/drivers/src/pins/pins_driver.c
    ${PROJ_DIR}/platform/drivers/src/pins/pins_port_hw_access.c
    ${PROJ_DIR}/platform/drivers/src/interrupt/interrupt_manager.c
    ${PROJ_DIR}/platform/drivers/src/dma/dma_driver.c
    ${PROJ_DIR}/platform/drivers/src/dma/dma_hw_access.c
    ${PROJ_DIR}/platform/drivers/src/dma/dma_irq.c
    ${PROJ_DIR}/platform/drivers/src/linflexd/linflexd_uart_driver.c
    ${PROJ_DIR}/platform/drivers/src/linflexd/linflexd_uart_irq.c
    ${PROJ_DIR}/platform/drivers/src/flexcan/flexcan_driver.c
    ${PROJ_DIR}/platform/drivers/src/flexcan/flexcan_hw_access.c
    ${PROJ_DIR}/platform/drivers/src/flexcan/flexcan_irq.c
    ${PROJ_DIR}/platform/devices/YTM32B1ME0/startup/system_YTM32B1ME0.c
    ${PROJ_DIR}/rtos/osif/osif_baremetal.c
)
set(includes
    ${PROJ_DIR}/platform/drivers/src/clock/YTM32B1Mx
    ${PROJ_DIR}/platform/drivers/src/pins
    ${PROJ_DIR}/platform/drivers/src/dma
    ${PROJ_DIR}/platform/drivers/src/linflexd
    ${PROJ_DIR}/platform/drivers/src/flexcan
    ${PROJ_DIR}/platform/drivers/inc
    ${PROJ_DIR}/platform/devices/common
    ${PROJ_DIR}/platform/devices
    ${PROJ_DIR}/platform/devices/YTM32B1ME0/include
    ${PROJ_DIR}/platform/devices/YTM32B1ME0/startup
    ${PROJ_DIR}/CMSIS/Core/Include
    ${PROJ_DIR}/rtos/osif
)
set(priIncludes
)

add_library(GENERATED_SDK_TARGET STATIC ${sources})

target_include_directories(GENERATED_SDK_TARGET PUBLIC ${includes})


target_include_directories(GENERATED_SDK_TARGET PRIVATE ${priIncludes})
configcore(GENERATED_SDK_TARGET ${CMAKE_SOURCE_DIR})

target_compile_definitions(GENERATED_SDK_TARGET PUBLIC
    YTM32B1ME0
    CPU_YTM32B1ME0
)
target_compile_options(GENERATED_SDK_TARGET PUBLIC
    -fdiagnostics-color=always
)



target_link_libraries(GENERATED_SDK_TARGET
    GENERATED_CONFIG_TARGET
)
