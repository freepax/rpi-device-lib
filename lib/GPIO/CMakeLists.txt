include_directories(../../include/)

set(GPIO_SOURCE gpio.cpp)
set(GPIO_HEADERS ../../${INCLUDE}/gpio.h)

add_library(gpio SHARED ${GPIO_SOURCE} ${GPIO_HEADERS})
add_library(gpio_static STATIC ${GPIO_SOURCE} ${GPIO_HEADERS})

install(TARGETS gpio DESTINATION lib)

