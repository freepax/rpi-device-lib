include_directories(../../include/)

set(ADS1015_SOURCE ads1015.cpp)
set(ADS1015_HEADERS ../../${INCLUDE}/ads1015.h ../../${INCLUDE}/firmware_i2c.h  ../../${INCLUDE}/binary.h)

add_library(ads1015 SHARED ${ADS1015_SOURCE} ${ADS1015_HEADERS})
add_library(ads1015_static STATIC ${ADS1015_SOURCE} ${ADS1015_HEADERS})

target_link_libraries(ads1015 firmware_i2c binary)
target_link_libraries(ads1015_static firmware_i2c_static binary_static)

install(TARGETS ads1015 DESTINATION lib)
