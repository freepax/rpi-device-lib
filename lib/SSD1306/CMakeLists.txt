include_directories(../../include/)

set(SSD1306_SOURCE ssd1306.cpp)
set(SSD1306_HEADERS ../../${INCLUDE}/ssd1306.h ../../${INCLUDE}/firmware_i2c.h ../../${INCLUDE}/binary.h)

add_library(ssd1306 SHARED ${SSD1306_SOURCE} ${SSD1306_HEADERS})
add_library(ssd1306_static STATIC ${SSD1306_SOURCE} ${SSD1306_HEADERS})

target_link_libraries(ssd1306 firmware_i2c binary)
target_link_libraries(ssd1306_static firmware_i2c_static binary_static)

install(TARGETS ssd1306 DESTINATION lib)
