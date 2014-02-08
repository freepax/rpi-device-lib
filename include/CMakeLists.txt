set(HEADERS 
    mcp4725.h
    bmp180.h
    ads1015.h
    l3gd20.h
    ssd1306.h
    gpio.h
    sinewave.h
    firmware_i2c.h
    pca9685.h
    arial_bold.h
)

install(FILES ${HEADERS} DESTINATION include)
