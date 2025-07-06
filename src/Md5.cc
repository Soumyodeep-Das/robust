#include "Md5.hpp"
#include <cmath>
#include <cstring>

const uint32_t Md5Encode::kA = 0x67452301;
const uint32_t Md5Encode::kB = 0xefcdab89;
const uint32_t Md5Encode::kC = 0x98badcfe;
const uint32_t Md5Encode::kD = 0x10325476;

const uint64_t Md5Encode::tiNumInteger = 4294967296;

#define BIT_OF_BYTE 8
#define BIT_OF_GROUP 512
#define SRC_DATA_LEN 64

// The FF, GG, HH, II macros or functions are assumed to be defined elsewhere,
// as they are used but not defined in the original source.

uint32_t Md5Encode::cycleMoveLeft(uint32_t srcNum, int bitNumToMove)
{
    uint32_t srcNum1 = srcNum;
    uint32_t srcNum2 = srcNum;
    if (0 >= bitNumToMove)
    {
        return srcNum;
    }
    return ((srcNum1 << bitNumToMove) | (srcNum2 >> (32 - bitNumToMove)));
}

uint32_t Md5Encode::fillData(const char *inDataPtr,
                             int dataByteLen,
                             char **outDataPtr)
{
    int bitNum = dataByteLen * BIT_OF_BYTE;
    int modBitNum = bitNum % BIT_OF_GROUP;
    int bitNeedFill = 0;
    if (modBitNum >= (BIT_OF_GROUP - SRC_DATA_LEN))
    {
        bitNeedFill = (BIT_OF_GROUP - modBitNum);
        bitNeedFill += (BIT_OF_GROUP - SRC_DATA_LEN);
    }
    else
    {
        bitNeedFill = (BIT_OF_GROUP - SRC_DATA_LEN) - modBitNum;
    }
    int allBit = bitNum + bitNeedFill;
    if (0 < bitNeedFill)
    {
        *outDataPtr =
            new char[allBit / BIT_OF_BYTE + SRC_DATA_LEN / BIT_OF_BYTE];
        memset(*outDataPtr,
               0,
               allBit / BIT_OF_BYTE + SRC_DATA_LEN / BIT_OF_BYTE);
        // copy data
        memcpy(*outDataPtr, inDataPtr, dataByteLen);
        // fill rest data
        unsigned char *tmp = reinterpret_cast<unsigned char *>(*outDataPtr);
        tmp += dataByteLen;
        // fill 1 and 0
        *tmp = 0x80;
        // fill origin data len
        unsigned long long *originNum =
            (unsigned long long *)((*outDataPtr) + ((allBit / BIT_OF_BYTE)));
        *originNum = dataByteLen * BIT_OF_BYTE;
    }
    return (allBit / BIT_OF_BYTE + SRC_DATA_LEN / BIT_OF_BYTE);
}

void Md5Encode::roundF(char *data512Ptr, ParamDynamic &param)
{
    uint32_t *M = reinterpret_cast<uint32_t *>(data512Ptr);
    int s[] = {7, 12, 17, 22};
    for (int i = 0; i < 16; ++i)
    {
        uint64_t ti = tiNumInteger * fabs(sin(i + 1));
        if (i % 4 == 0)
        {
            FF(param.ua_, param.ub_, param.uc_, param.ud_, M[i], s[i % 4], ti);
        }
        else if (i % 4 == 1)
        {
            FF(param.ud_, param.ua_, param.ub_, param.uc_, M[i], s[i % 4], ti);
        }
        else if (i % 4 == 2)
        {
            FF(param.uc_, param.ud_, param.ua_, param.ub_, M[i], s[i % 4], ti);
        }
        else if (i % 4 == 3)
        {
            FF(param.ub_, param.uc_, param.ud_, param.ua_, M[i], s[i % 4], ti);
        }
    }
}

void Md5Encode::roundG(char *data512Ptr, ParamDynamic &param)
{
    uint32_t *M = reinterpret_cast<uint32_t *>(data512Ptr);
    int s[] = {5, 9, 14, 20};
    for (int i = 0; i < 16; ++i)
    {
        auto sss = sin(i + 1 + 16);
        uint64_t ti = tiNumInteger * fabs(sss);
        int index = (i * 5 + 1) % 16;
        if (i % 4 == 0)
        {
            GG(param.ua_,
               param.ub_,
               param.uc_,
               param.ud_,
               M[index],
               s[i % 4],
               ti);
        }
        else if (i % 4 == 1)
        {
            GG(param.ud_,
               param.ua_,
               param.ub_,
               param.uc_,
               M[index],
               s[i % 4],
               ti);
        }
        else if (i % 4 == 2)
        {
            GG(param.uc_,
               param.ud_,
               param.ua_,
               param.ub_,
               M[index],
               s[i % 4],
               ti);
        }
        else if (i % 4 == 3)
        {
            GG(param.ub_,
               param.uc_,
               param.ud_,
               param.ua_,
               M[index],
               s[i % 4],
               ti);
        }
    }
}

void Md5Encode::roundH(char *data512Ptr, ParamDynamic &param)
{
    uint32_t *M = reinterpret_cast<uint32_t *>(data512Ptr);
    int s[] = {4, 11, 16, 23};
    for (int i = 0; i < 16; ++i)
    {
        uint64_t ti = tiNumInteger * fabs(sin(i + 1 + 32));
        int index = (i * 3 + 5) % 16;
        if (i % 4 == 0)
        {
            HH(param.ua_,
               param.ub_,
               param.uc_,
               param.ud_,
               M[index],
               s[i % 4],
               ti);
        }
        else if (i % 4 == 1)
        {
            HH(param.ud_,
               param.ua_,
               param.ub_,
               param.uc_,
               M[index],
               s[i % 4],
               ti);
        }
        else if (i % 4 == 2)
        {
            HH(param.uc_,
               param.ud_,
               param.ua_,
               param.ub_,
               M[index],
               s[i % 4],
               ti);
        }
        else if (i % 4 == 3)
        {
            HH(param.ub_,
               param.uc_,
               param.ud_,
               param.ua_,
               M[index],
               s[i % 4],
               ti);
        }
    }
}

void Md5Encode::roundI(char *data512Ptr, ParamDynamic &param)
{
    uint32_t *M = reinterpret_cast<uint32_t *>(data512Ptr);
    int s[] = {6, 10, 15, 21};
    for (int i = 0; i < 16; ++i)
    {
        uint64_t ti = tiNumInteger * fabs(sin(i + 1 + 48));
        int index = (i * 7 + 0) % 16;
        if (i % 4 == 0)
        {
            II(param.ua_,
               param.ub_,
               param.uc_,
               param.ud_,
               M[index],
               s[i % 4],
               ti);
        }
        else if (i % 4 == 1)
        {
            II(param.ud_,
               param.ua_,
               param.ub_,
               param.uc_,
               M[index],
               s[i % 4],
               ti);
        }
        else if (i % 4 == 2)
        {
            II(param.uc_,
               param.ud_,
               param.ua_,
               param.ub_,
               M[index],
               s[i % 4],
               ti);
        }
        else if (i % 4 == 3)
        {
            II(param.ub_,
               param.uc_,
               param.ud_,
               param.ua_,
               M[index],
               s[i % 4],
               ti);
        }
    }
}

void Md5Encode::rotationCalculate(char *data512Ptr, ParamDynamic &param)
{
    if (nullptr == data512Ptr)
    {
        return;
    }
    roundF(data512Ptr, param);
    roundG(data512Ptr, param);
    roundH(data512Ptr, param);
    roundI(data512Ptr, param);
    param.ua_ = param.va_last_ + param.ua_;
    param.ub_ = param.vb_last_ + param.ub_;
    param.uc_ = param.vc_last_ + param.uc_;
    param.ud_ = param.vd_last_ + param.ud_;

    param.va_last_ = param.ua_;
    param.vb_last_ = param.ub_;
    param.vc_last_ = param.uc_;
    param.vd_last_ = param.ud_;
}

std::string Md5Encode::getHexStr(uint32_t numStr)
{
    std::string hexstr = "";
    char szch[] = {'0',
                   '1',
                   '2',
                   '3',
                   '4',
                   '5',
                   '6',
                   '7',
                   '8',
                   '9',
                   'A',
                   'B',
                   'C',
                   'D',
                   'E',
                   'F'};
    unsigned char *tmptr = (unsigned char *)&numStr;
    int len = sizeof(numStr);
    for (int i = 0; i < len; ++i)
    {
        unsigned char ch = tmptr[i] & 0xF0;
        ch = ch >> 4;
        hexstr.append(1, szch[ch]);
        ch = tmptr[i] & 0x0F;
        hexstr.append(1, szch[ch]);
    }
    return hexstr;
}

std::string Md5Encode::encode(const char *data, const size_t dataLen)
{
    ParamDynamic param;
    param.ua_ = kA;
    param.ub_ = kB;
    param.uc_ = kC;
    param.ud_ = kD;
    param.va_last_ = kA;
    param.vb_last_ = kB;
    param.vc_last_ = kC;
    param.vd_last_ = kD;

    std::string result;
    char *outDataPtr = nullptr;
    int totalByte = fillData(data, dataLen, &outDataPtr);

    for (int i = 0; i < totalByte / (BIT_OF_GROUP / BIT_OF_BYTE); ++i)
    {
        char *dataBitOfGroup = outDataPtr;
        dataBitOfGroup += i * (BIT_OF_GROUP / BIT_OF_BYTE);
        rotationCalculate(dataBitOfGroup, param);
    }
    delete[] outDataPtr, outDataPtr = nullptr;
    result.append(getHexStr(param.ua_));
    result.append(getHexStr(param.ub_));
    result.append(getHexStr(param.uc_));
    result.append(getHexStr(param.ud_));
    return result;
}