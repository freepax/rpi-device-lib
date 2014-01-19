include_directories(../include/)

# binary
set(BINARY_SOURCES
    ex_binary.cpp
)

set(BINARY_HEADERS
    ../${INCLUDE}/binary.h
)

add_executable(ex_binary ${BINARY_SOURCES} ${BINARY_HEADERS})
add_executable(ex_binary_static ${BINARY_SOURCES} ${BINARY_HEADERS})

target_link_libraries(ex_binary binary)
target_link_libraries(ex_binary_static binary_static)


# GPIO
set(GPIO_SOURCES
    ex_gpio.cpp
)

set(GPIO_HEADERS
    ../${INCLUDE}/gpio.h
)

add_executable(ex_gpio ${GPIO_SOURCES} ${GPIO_HEADERS})
add_executable(ex_gpio_static ${GPIO_SOURCES} ${GPIO_HEADERS})
target_link_libraries(ex_gpio gpio)
target_link_libraries(ex_gpio_static gpio_static)

#LINK_DIRECTORIES(${CMAKE_BINARY_DIR}/lib/MCP4725)


# ADS1015
set(ADS1015_SOURCES
    ex_ads1015.cpp
)

set(ADS1015_HEADERS
    ../${INCLUDE}/ads1015.h
)

add_executable(ex_ads1015 ${ADS1015_SOURCES} ${ADS1015_HEADERS})
add_executable(ex_ads1015_static ${ADS1015_SOURCES} ${ADS1015_HEADERS})

link_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/ADS1015/)
target_link_libraries(ex_ads1015 ads1015)
target_link_libraries(ex_ads1015_static ads1015_static)


# MCP4725
set(MCP4725_SOURCES
    ex_mcp4725.cpp
)

set(MCP4725_LIB
    mcp4725
    binary
)

set(MCP4725_STATIC_LIB
    mcp4725_static
    binary_static
)

set(MCP4725_HEADERS
 ../${INCLUDE}/mcp4725.h
)

link_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/MCP4725/)

add_executable(ex_mcp4725 ${MCP4725_SOURCES})
add_executable(ex_mcp4725_static ${MCP4725_SOURCES} ${MCP4725_HEADERS})
target_link_libraries(ex_mcp4725 ${MCP4725_LIB})
target_link_libraries(ex_mcp4725_static ${MCP4725_STATIC_LIB})

set(MCP4725_EEPROM_SOURCES
    ex_mcp4725_eeprom.cpp
)

add_executable(ex_mcp4725_eeprom ${MCP4725_EEPROM_SOURCES})
add_executable(ex_mcp4725_eeprom_static ${MCP4725_EEPROM_SOURCES} ${MCP4725_HEADERS})
target_link_libraries(ex_mcp4725_eeprom ${MCP4725_LIB})
target_link_libraries(ex_mcp4725_eeprom_static ${MCP4725_STATIC_LIB})

set(MCP4725_READ_SOURCES
    ex_mcp4725_read.cpp
)

add_executable(ex_mcp4725_read ${MCP4725_READ_SOURCES})
add_executable(ex_mcp4725_read_static ${MCP4725_READ_SOURCES} ${MCP4725_HEADERS})
target_link_libraries(ex_mcp4725_read ${MCP4725_LIB})
target_link_libraries(ex_mcp4725_read_static ${MCP4725_STATIC_LIB})


# sinewave
set(SINEWAVE_SOURCES
    sinewave.cpp
)

set(SINEWAVE_LIB
    mcp4725
)

set(SINEWAVE_STATIC_LIB
    mcp4725_static
)

set(SINEWAVE_HEADERS
    ${MCP4725_HEADERS}
    ../${INCLUDE}/sinewave.h
)

add_executable(sinewave ${SINEWAVE_SOURCES} ${SINEWAVE_HEADERS})
add_executable(sinewave_static ${SINEWAVE_SOURCES} ${SINEWAVE_HEADERS})

link_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/MCP4725/)
target_link_libraries(sinewave ${SINEWAVE_LIB})
target_link_libraries(sinewave_static ${SINEWAVE_STATIC_LIB})

# install
set(EXECUTABLES
ex_binary
ex_binary_static
ex_gpio
ex_gpio_static
ex_ads1015
ex_ads1015_static
ex_mcp4725
ex_mcp4725_static
ex_mcp4725_eeprom
ex_mcp4725_eeprom_static
ex_mcp4725_read
ex_mcp4725_read_static
sinewave
sinewave_static
)

install(TARGETS ${EXECUTABLES} DESTINATION examples)
