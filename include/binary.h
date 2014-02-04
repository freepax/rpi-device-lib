#ifndef BINARY_H
#define BINARY_H

class Binary {
public:
    void printIntAsBinary(const char *text, unsigned int value);
    void printIntAsBinary(unsigned int value);
    void printShortAsBinary(unsigned short value);
    void printShortAsBinary(const char *text, unsigned short value);
    void printByteAsBinary(unsigned char value);
    void printByteAsBinary(const char *text, unsigned char value);
};

#endif /// BINARY_H
