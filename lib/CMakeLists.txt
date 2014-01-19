include_directories(../include/)

set(FIRMWARE_SOURCE firmware_i2c.cpp)
add_library(firmware_i2c SHARED ${FIRMWARE_SOURCE})
add_library(firmware_i2c_static STATIC ${FIRMWARE_SOURCE})


set(BINARY_SOURCE binary.cpp)
add_library(binary SHARED ${BINARY_SOURCE})
add_library(binary_static STATIC ${BINARY_SOURCE})


install(TARGETS binary DESTINATION lib)
install(TARGETS firmware_i2c DESTINATION lib)


add_subdirectory(ADS1015)
add_subdirectory(GPIO)
add_subdirectory(MCP4725)
add_subdirectory(L3GD20)
