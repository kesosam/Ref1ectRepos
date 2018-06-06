#pragma once

#include "IEncoder.h"

#define BASE64_DECODE_GROUP_MEMBER 4
#define BASE64_ENCODE_GROUP_MEMBER 3
class CBase64Encoder :
    public IEncoder
{
public:
    CBase64Encoder();
    ~CBase64Encoder();
    BOOL Encode(CString& Source);
    BOOL Decode(CString& Source);
private:
    static char m_aryBase64Alphabet[64];
};

