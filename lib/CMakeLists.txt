include_directories(../include/)

set(FIRMWARE_SOURCE firmware_i2c.cpp)
add_library(firmware_i2c SHARED ${FIRMWARE_SOURCE})
add_library(firmware_i2c_static STATIC ${FIRMWARE_SOURCE})
set_target_properties(firmware_i2c PROPERTIES
  VERSION 1.0.0
  SOVERSION 1
)

set(BINARY_SOURCE binary.cpp)
add_library(binary SHARED ${BINARY_SOURCE})
add_library(binary_static STATIC ${BINARY_SOURCE})
set_target_properties(binary PROPERTIES
  VERSION 1.0.0
  SOVERSION 1
)

set (CMAKE_EXE_LINKER_FLAGS -march=armv7-a -mfloat-abi=hard -mfpu=neon -marm -mthumb-interwork) 

target_link_libraries(firmware_i2c binary)
target_link_libraries(firmware_i2c_static binary_static)


install(TARGETS binary DESTINATION lib)
install(TARGETS firmware_i2c DESTINATION lib)

add_subdirectory(GPIO)
add_subdirectory(ADS1015)
add_subdirectory(BMP180)
add_subdirectory(MCP4725)
add_subdirectory(L3GD20)
add_subdirectory(PCA9685)
add_subdirectory(SSD1306)
