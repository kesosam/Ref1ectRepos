#pragma once
#include "IEncoder.h"

#include <openssl/md5.h>

#pragma comment(lib, "ssleay32.lib")
#pragma comment(lib, "libeay32.lib")

class CMD5Encoder
    :public IEncoder
{
public:
    CMD5Encoder();
    ~CMD5Encoder();
    virtual BOOL Encode(CString&);
    virtual BOOL Decode(CString&);
};

