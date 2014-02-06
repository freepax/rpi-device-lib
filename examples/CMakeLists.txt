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


# PCA9685
set(PCA9685_SOURCES
    ex_pca9685.cpp
)

set(PCA9685_HEADERS
    ../${INCLUDE}/pca9685.h
)

add_executable(ex_pca9685 ${PCA9685_SOURCES} ${PCA9685_HEADERS})
add_executable(ex_pca9685_static ${PCA9685_SOURCES} ${PCA9685_HEADERS})

link_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/PCA9685/)
target_link_libraries(ex_pca9685 pca9685)
target_link_libraries(ex_pca9685_static pca9685_static)


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

# SSD1306
set(SSD1306_SOURCES
    ex_ssd1306.cpp
)

set(SSD1306_HEADERS
    ../${INCLUDE}/ssd1306.h
)

set(SSD1306_LIB
    ssd1306
    binary
    gpio
    ads1015
)

set(SSD1306_STATIC_LIB
    ssd1306_static
    binary_static
    gpio_static
    ads1015_static
)

add_executable(ex_ssd1306 ${SSD1306_SOURCES} ${SSD1306_HEADERS})
add_executable(ex_ssd1306_static ${SSD1306_SOURCES} ${SSD1306_HEADERS})
target_link_libraries(ex_ssd1306 ${SSD1306_LIB})
target_link_libraries(ex_ssd1306_static ${SSD1306_STATIC_LIB})



# L3GD20
set(L3GD20_SOURCES
    ex_l3gd20.cpp
)

set(L3GD20_HEADERS
    ../${INCLUDE}/l3gd20.h
)

set(L3GD20_LIB
    l3gd20
    binary
    gpio
    ads1015
)

set(L3GD20_STATIC_LIB
    l3gd20_static
    binary_static
    gpio_static
    ads1015_static
)

add_executable(ex_l3gd20 ${L3GD20_SOURCES} ${L3GD20_HEADERS})
add_executable(ex_l3gd20_static ${L3GD20_SOURCES} ${L3GD20_HEADERS})
target_link_libraries(ex_l3gd20 ${L3GD20_LIB})
target_link_libraries(ex_l3gd20_static ${L3GD20_STATIC_LIB})

# BMP180
set(BMP180_SOURCES
    ex_bmp180.cpp
)

set(BMP180_HEADERS
    ../${INCLUDE}/bmp180.h
)

set(BMP180_LIB
    bmp180
    binary
)

set(BMP180_STATIC_LIB
    bmp180_static
    binary_static
)

add_executable(ex_bmp180 ${BMP180_SOURCES} ${BMP180_HEADERS})
add_executable(ex_bmp180_static ${BMP180_SOURCES} ${BMP180_HEADERS})
target_link_libraries(ex_bmp180 ${BMP180_LIB})
target_link_libraries(ex_bmp180_static ${BMP180_STATIC_LIB})

# install
set(EXECUTABLES
ex_binary
ex_binary_static
ex_gpio
ex_gpio_static
ex_ads1015
ex_ads1015_static
ex_pca9685
ex_pca9685_static
ex_mcp4725
ex_mcp4725_static
ex_mcp4725_eeprom
ex_mcp4725_eeprom_static
ex_mcp4725_read
ex_mcp4725_read_static
sinewave
sinewave_static
ex_ssd1306
ex_ssd1306_static
ex_l3gd20
ex_l3gd20_static
ex_bmp180
ex_bmp180_static
)

install(TARGETS ${EXECUTABLES} DESTINATION examples)
