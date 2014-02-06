include_directories(../../include/)

set(MCP4725_SOURCE mcp4725.cpp)
set(MCP4725_HEADERS ../../${INCLUDE}/mcp4725.h ../../${INCLUDE}/firmware_i2c.h ../../${INCLUDE}/binary.h)

add_library(mcp4725 SHARED ${MCP4725_SOURCE} ${MCP4725_HEADERS})
add_library(mcp4725_static STATIC ${MCP4725_SOURCE} ${MCP4725_HEADERS})

target_link_libraries(mcp4725 firmware_i2c binary)
target_link_libraries(mcp4725_static firmware_i2c_static binary_static)

install(TARGETS mcp4725 DESTINATION lib)
