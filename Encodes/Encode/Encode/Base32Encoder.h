#pragma once
#include "IEncoder.h"

#define BASE32_DECODE_GROUP_MEMBER 8
#define BASE32_ENCODE_GROUP_MEMBER 5

class CBase32Encoder :
    public IEncoder
{
public:
    CBase32Encoder();
    virtual ~CBase32Encoder();

    BOOL Encode(CString& Source);
    BOOL Decode(CString& Source);
private:
    static char m_aryBase32Alphabet[32];
};

