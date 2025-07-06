#ifndef MD5_HPP
#define MD5_HPP

#include <cstdint>
#include <string>

class Md5Encode
{
public:
    static const uint32_t kA;
    static const uint32_t kB;
    static const uint32_t kC;
    static const uint32_t kD;
    static const uint64_t tiNumInteger;

    struct ParamDynamic
    {
        uint32_t ua_;
        uint32_t ub_;
        uint32_t uc_;
        uint32_t ud_;
        uint32_t va_last_;
        uint32_t vb_last_;
        uint32_t vc_last_;
        uint32_t vd_last_;
    };

    static uint32_t cycleMoveLeft(uint32_t srcNum, int bitNumToMove);
    static uint32_t fillData(const char *inDataPtr, int dataByteLen, char **outDataPtr);
    static void roundF(char *data512Ptr, ParamDynamic &param);
    static void roundG(char *data512Ptr, ParamDynamic &param);
    static void roundH(char *data512Ptr, ParamDynamic &param);
    static void roundI(char *data512Ptr, ParamDynamic &param);
    static void rotationCalculate(char *data512Ptr, ParamDynamic &param);
    static std::string getHexStr(uint32_t numStr);
    static std::string encode(const char *data, const size_t dataLen);
};

#endif // MD5_HPP