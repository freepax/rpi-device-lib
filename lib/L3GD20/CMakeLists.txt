include_directories(../../include/)

set(L3GD20_HEADERS ../../${INCLUDE}/l3gd20.h)
set(L3GD20_SOURCE l3gd20.cpp)

set (CMAKE_EXE_LINKER_FLAGS -march=armv7-a -mfloat-abi=hard -mfpu=neon -marm -mthumb-interwork)

add_library(l3gd20 SHARED ${L3GD20_SOURCE} ${L3GD20_HEADERS})
add_library(l3gd20_static STATIC ${L3GD20_SOURCE} ${L3GD20_HEADERS})

set_target_properties(l3gd20 PROPERTIES
  VERSION 1.0.0
  SOVERSION 1
)

install(TARGETS l3gd20 DESTINATION lib)
