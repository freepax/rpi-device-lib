#include <iostream>

/// binary headers
#include <binary.h>


void Binary::printIntAsBinary(unsigned int value)
{
    for (int i = 0; i < 32; i++) {
        if (value & 0x80000000)  std::cout << "1";
        else                     std::cout << "0";

        if (i == 3 || i == 11 || i == 19 || i == 27)
            std::cout << " ";
        if (i == 7 || i == 15 || i == 23 || i == 31)
            std::cout << "  ";
        value = value << 1;
    }
    std::cout << std::endl;
}


void Binary::printShortAsBinary(unsigned short value)
{
    for (int i = 0; i < 16; i++) {
        if (value & 0x8000)  std::cout << "1";
        else                 std::cout << "0";

        if (i == 3 || i == 11)
            std::cout << " ";
        if (i == 7)
            std::cout << "  ";
        value = value << 1;
    }
    std::cout << std::endl;
}


void Binary::printByteAsBinary(unsigned char value)
{
    for (int i = 0; i < 8; i++) {
        if (value & 0x80)  std::cout << "1";
        else                     std::cout << "0";

        if (i == 3)
            std::cout << " ";
        value = value << 1;
    }
    std::cout << std::endl;
}
