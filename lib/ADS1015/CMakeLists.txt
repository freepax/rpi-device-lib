include_directories(../../include/)

set(ADS1015_SOURCE ads1015.cpp)
set(ADS1015_HEADERS ../../${INCLUDE}/ads1015.h ../../${INCLUDE}/firmware_i2c.h  ../../${INCLUDE}/binary.h)

set (CMAKE_EXE_LINKER_FLAGS -march=armv7-a -mfloat-abi=hard -mfpu=neon -marm -mthumb-interwork) 

add_library(ads1015 SHARED ${ADS1015_SOURCE} ${ADS1015_HEADERS})
add_library(ads1015_static STATIC ${ADS1015_SOURCE} ${ADS1015_HEADERS})

target_link_libraries(ads1015 firmware_i2c binary)
target_link_libraries(ads1015_static firmware_i2c_static binary_static)

set_target_properties(ads1015 PROPERTIES
  VERSION 1.0.0
  SOVERSION 1
)

install(TARGETS ads1015 DESTINATION lib)
