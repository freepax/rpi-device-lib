include_directories(../../include/)

set(BMP180_HEADERS ../../${INCLUDE}/bmp180.h ../../${INCLUDE}/firmware_i2c.h)
set(BMP180_SOURCE bmp180.cpp)
set(BMP180_LIB firmware_i2c binary binary)
set(BMP180_STATIC_LIB firmware_i2c binary_static binary_static)

set (CMAKE_EXE_LINKER_FLAGS -march=armv7-a -mfloat-abi=hard -mfpu=neon -marm -mthumb-interwork)

add_library(bmp180 SHARED ${BMP180_SOURCE} ${BMP180_HEADERS})
add_library(bmp180_static STATIC ${BMP180_SOURCE} ${BMP180_HEADERS})

target_link_libraries(bmp180 ${BMP180_LIB})
target_link_libraries(bmp180_static ${BMP180_STATIC_LIB})
install(TARGETS bmp180 DESTINATION lib)
