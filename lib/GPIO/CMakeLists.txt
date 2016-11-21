include_directories(../../include/)

set(GPIO_SOURCE gpio.cpp)
set(GPIO_HEADERS ../../${INCLUDE}/gpio.h)

set (CMAKE_EXE_LINKER_FLAGS -march=armv7-a -mfloat-abi=hard -mfpu=neon -marm -mthumb-interwork) 

add_library(gpioSHARED ${GPIO_SOURCE} ${GPIO_HEADERS})
add_library(gpio_static STATIC ${GPIO_SOURCE} ${GPIO_HEADERS})

set(gpio_link   ${CMAKE_INSTALL_PREFIX}/gpio.so)
set(gpio_target ${CMAKE_INSTALL_PREFIX}/gpio)

set_target_properties(gpio PROPERTIES
  VERSION 1.0.0
  SOVERSION 1
)

install(TARGETS gpio DESTINATION lib)

