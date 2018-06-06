#include "stdafx.h"
#include "MD5Encoder.h"

CMD5Encoder::CMD5Encoder()
{
}


CMD5Encoder::~CMD5Encoder()
{
}

/*±àÂë*/
BOOL CMD5Encoder::Encode(CString& szSource)
{ 
    MD5_CTX md5_ctx;
    MD5_Init(&md5_ctx);
    UCHAR md[16] = { 0 };
    CStringA strTemp;

    int nLength = szSource.GetLength();
    char* szBuffer = new char(nLength + 1);
    strTemp = CW2A(szSource.GetString());
    
    MD5_Update(&md5_ctx, strTemp.GetString(), nLength);
    MD5_Final(md, &md5_ctx);
    szSource.Empty();

    for (int nCount = 0; nCount < 16; nCount++)
    {
        strTemp.Format("%02x", md[nCount]);
        szSource += strTemp;
    }
    return TRUE;
}

/*½âÂë*/
BOOL CMD5Encoder::Decode(CString& szSource)
{

    return TRUE;
}