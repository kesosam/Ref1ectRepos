#pragma once
class IEncoder
{
public:
    IEncoder();
    virtual ~IEncoder();
    virtual BOOL Encode(CString&) = 0;
    virtual BOOL Decode(CString&) = 0;
};

