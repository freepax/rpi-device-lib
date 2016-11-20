include_directories(../include/)

# GPIO TIMER
set(GPIO_SOURCES
    gpio_timer.cpp
)

set(GPIO_HEADERS
    ../${INCLUDE}/gpio.h
)

add_executable(gpio_timer ${GPIO_SOURCES} ${GPIO_HEADERS})
add_executable(gpio_timer_static ${GPIO_SOURCES} ${GPIO_HEADERS})
target_link_libraries(gpio_timer gpio)
target_link_libraries(gpio_timer_static gpio_static)


# Altimeter
set(ALTIMETER_SOURCES
    altimeter.cpp
)

set(ALTIMETER_HEADERS
    ../${INCLUDE}/bmp180.h
)

set(ALTIMETER_LIB
    bmp180
    gpio
    ssd1306
    binary
)

set(ALTIMETER_STATIC_LIB
    bmp180_static
    ssd1306_static
    gpio_static
    binary_static
)

add_executable(altimeter ${ALTIMETER_SOURCES} ${ALTIMETER_HEADERS})
add_executable(altimeter_static ${ALTIMETER_SOURCES} ${ALTIMETER_HEADERS})
target_link_libraries(altimeter ${ALTIMETER_LIB})
target_link_libraries(altimeter_static ${ALTIMETER_STATIC_LIB})

# BaroLogger - Barometric logger
set(BAROLOGGER_SOURCES
    barologger.cpp
)

set(BAROLOGGER_HEADERS
    ../${INCLUDE}/bmp180.h
)

set(BAROLOGGER_LIB
    bmp180
    gpio
    ssd1306
    binary
)

set(BAROLOGGER_STATIC_LIB
    bmp180_static
    ssd1306_static
    gpio_static
    binary_static
)

add_executable(barologger ${BAROLOGGER_SOURCES} ${BAROLOGGER_HEADERS})
add_executable(barologger_static ${BAROLOGGER_SOURCES} ${BAROLOGGER_HEADERS})
target_link_libraries(barologger ${BAROLOGGER_LIB})
target_link_libraries(barologger_static ${BAROLOGGER_STATIC_LIB})

# install
set(EXECUTABLES
altimeter
altimeter_static
barologger
barologger_static
gpio_timer
gpio_timer_static
)

install(TARGETS ${EXECUTABLES} DESTINATION bin)
