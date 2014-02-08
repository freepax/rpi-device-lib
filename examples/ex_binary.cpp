#include <iostream>

#include <string.h>
#include <stdlib.h>

/// binary headers
#include <binary.h>


int main(int argc, char **argv)
{
    Binary binary;

    /// check if we got som input
    if (argc > 1) {
        if (strlen(argv[1]) == 3)
            binary.printByteAsBinary("Binary argument", atoi(argv[1]));
        else
            binary.printIntAsBinary("Binary argument ", atoi(argv[1]));
    }

    std::cout << "Testing printIntAsBinary" << std::endl;
    binary.printIntAsBinary(0x44556677);
    binary.printIntAsBinary(0x8899aabb);
    binary.printIntAsBinary(0xccddeeff);
    binary.printIntAsBinary(0x0f0f0f0f);
    binary.printIntAsBinary(0xf0f0f0f0);

    std::cout << "Testing printShortAsBinary" << std::endl;
    binary.printShortAsBinary(0x0011);
    binary.printShortAsBinary(0x2233);
    binary.printShortAsBinary(0x4455);
    binary.printShortAsBinary(0x6677);
    binary.printShortAsBinary(0x8899);
    binary.printShortAsBinary(0xaabb);
    binary.printShortAsBinary(0xccdd);
    binary.printShortAsBinary(0xeeff);
    binary.printShortAsBinary(0x0f0f);
    binary.printShortAsBinary(0xf0f0);

    std::cout << "Testing printByteAsBinary" << std::endl;
    binary.printByteAsBinary(0x01);
    binary.printByteAsBinary(0x23);
    binary.printByteAsBinary(0x45);
    binary.printByteAsBinary(0x67);
    binary.printByteAsBinary(0x89);
    binary.printByteAsBinary(0xab);
    binary.printByteAsBinary(0xcd);
    binary.printByteAsBinary(0xef);
    binary.printByteAsBinary(0xff);
    binary.printByteAsBinary(0x00);
    binary.printByteAsBinary(0x48);

    return 0;
}
