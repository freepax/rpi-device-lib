include_directories(../../include/)

set(L3GD20_HEADERS ../../${INCLUDE}/l3gd20.h)
set(L3GD20_SOURCE l3gd20.cpp)

add_library(l3gd20 SHARED ${L3GD20_SOURCE} ${L3GD20_HEADERS})
add_library(l3gd20_static STATIC ${L3GD20_SOURCE} ${L3GD20_HEADERS})
install(TARGETS l3gd20 DESTINATION lib)
