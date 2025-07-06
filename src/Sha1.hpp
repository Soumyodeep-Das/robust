#ifndef SHA1_HPP
#define SHA1_HPP

#include <cstddef>

unsigned char *SHA1(const unsigned char *dataIn,
                    size_t dataLen,
                    unsigned char *dataOut);

#endif // SHA1_HPP