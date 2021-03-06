include_directories(../../include/)

set(PCA9685_SOURCE pca9685.cpp)
set(PCA9685_HEADERS ../../${INCLUDE}/pca9685.h ../../${INCLUDE}/firmware_i2c.h ../../${INCLUDE}/binary.h)

set (CMAKE_EXE_LINKER_FLAGS -march=armv7-a -mfloat-abi=hard -mfpu=neon -marm -mthumb-interwork)

add_library(pca9685 SHARED ${PCA9685_SOURCE} ${PCA9685_HEADERS})
add_library(pca9685_static STATIC ${PCA9685_SOURCE} ${PCA9685_HEADERS})

target_link_libraries(pca9685 firmware_i2c binary)
target_link_libraries(pca9685_static firmware_i2c_static binary_static)

set_target_properties(pca9685 PROPERTIES
  VERSION 1.0.0
  SOVERSION 1
)

install(TARGETS pca9685 DESTINATION lib)
install(TARGETS pca9685_static DESTINATION lib)
