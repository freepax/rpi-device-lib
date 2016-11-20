include_directories(../../include/)

set(GPIO_SOURCE gpio.cpp)
set(GPIO_HEADERS ../../${INCLUDE}/gpio.h)

set (CMAKE_EXE_LINKER_FLAGS -march=armv7-a -mfloat-abi=hard -mfpu=neon -marm -mthumb-interwork) 

add_library(gpio SHARED ${GPIO_SOURCE} ${GPIO_HEADERS})
add_library(gpio_static STATIC ${GPIO_SOURCE} ${GPIO_HEADERS})

install(TARGETS gpio DESTINATION lib)

