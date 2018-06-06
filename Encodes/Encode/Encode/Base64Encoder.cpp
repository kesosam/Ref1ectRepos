#include "stdafx.h"
#include "Base64Encoder.h"

//Base64编码表
char CBase64Encoder::m_aryBase64Alphabet[64] = {
                        'A','B','C','D','E','F','G','H',
                        'I','J','K','L','M','N','O','P',
                        'Q','R','S','T','U','V','W','X',
                        'Y','Z','a','b','c','d','e','f',
                        'g','h','i','j','k','l','m','n',
                        'o','p','q','r','s','t','u','v',
                        'w','x','y','z','0','1','2','3',
                        '4','5','6','7','8','9','+','/'};

CBase64Encoder::CBase64Encoder()
{
}


CBase64Encoder::~CBase64Encoder()
{
}

/*编码*/
BOOL CBase64Encoder::Encode(CString& szSource)
{
    if (szSource.IsEmpty())
    {
        return FALSE;
    } 

    int nLength = szSource.GetLength();
    int nTail = 0;

    if (nLength < BASE64_ENCODE_GROUP_MEMBER) //计算编码的字符串长度是否为3的倍数，不够的结尾加=
    {
        nTail = BASE64_ENCODE_GROUP_MEMBER - nLength;
    }
    else
    {
        nTail = nLength % BASE64_ENCODE_GROUP_MEMBER;
    }

    nLength += nTail;
    TCHAR * arySrc = new TCHAR[nLength]();
    CopyMemory(arySrc, (void*)szSource.GetString(), (szSource.GetLength()* sizeof(TCHAR)));
    szSource.Empty();

    /*3个字符为一组进行编码*/
    for (int nCount = 0; nCount < nLength / BASE64_ENCODE_GROUP_MEMBER; nCount++)
    {
        TCHAR aryBuffer[BASE64_ENCODE_GROUP_MEMBER] = { 0 };
        CopyMemory(aryBuffer, &arySrc[nCount * BASE64_ENCODE_GROUP_MEMBER], BASE64_ENCODE_GROUP_MEMBER*sizeof(TCHAR));

        int nTrans = (aryBuffer[0] >> 2) & 0x3f; //第一个字符左移两位，只取后六位，其他位置零
        szSource.AppendChar(m_aryBase64Alphabet[nTrans]); //查表，将对应的字符加入传出的字符串


        nTrans = 0;
        nTrans = ((aryBuffer[0] << 4) & 0x30) + ((aryBuffer[1] >> 4) & 0xf);//第一个字符剩下的两位与第二个字符的前4位相加，得到第二个编码
        szSource.AppendChar(m_aryBase64Alphabet[nTrans]);

        if (nCount != nLength / BASE64_ENCODE_GROUP_MEMBER - 1 || nTail != 2) //如果结尾差两个字符满一组就要补两个=
        {
            nTrans = 0;
            nTrans = ((aryBuffer[1] << 2) & 0x3c) + ((aryBuffer[2] >> 6) & 0x3); //第二个字符剩下的四个字符与第三个字符的前两字符组成第三个编码
            szSource.AppendChar(m_aryBase64Alphabet[nTrans]);
        }
        else
        {
            szSource.AppendChar(_T('='));
        }

        if (nCount != nLength / BASE64_ENCODE_GROUP_MEMBER - 1 || nTail == 0) //如果结尾差一个字符满一组就要补一个=
        {
            nTrans = 0;
            nTrans = aryBuffer[2] & 0x3f;
            szSource.AppendChar(m_aryBase64Alphabet[nTrans]); //第三个字符剩下的六位作为最后一个编码
        }
        else
        {
            szSource.AppendChar(_T('='));
        }
    }

    if (arySrc != NULL)
    {
        delete [] arySrc;
        arySrc = NULL;
    }
    return TRUE;
}

/*解码*/
BOOL CBase64Encoder::Decode(CString& szSource)
{
    int nLength = szSource.GetLength();

    /*由于Base64的编码规则，Base64的编码必须为4的倍数*/
    if (nLength % 4)
    {
        return FALSE;
    }

    int nTail = 0;
    int nCheck = szSource.ReverseFind('=');

    /*判断结尾有几个=*/
    if (nCheck != -1)
    {
        nCheck = szSource.Find(_T("=="));
        if (nCheck != -1)
        {
            nTail = 2;
        }
        else
        {
            nTail = 1;
        }
    }

    TCHAR * arySrc = new TCHAR[nLength]();
    int * aryCode = new int[nLength]();
    CopyMemory(arySrc, szSource.GetString(), (nLength * sizeof(TCHAR)));
    szSource.Empty();

    /*查表，将编码翻译成数值*/
    for (int nCount = 0; nCount < nLength; nCount++)
    {
        for (int nCount1 = 0; nCount1 < 64; nCount1++)
        {
            if (arySrc[nCount] == m_aryBase64Alphabet[nCount1])
            {
                aryCode[nCount] = nCount1;
            }
        }
    }

    if (arySrc != NULL)
    {
        delete [] arySrc;
        arySrc = NULL;
    }

    /*四个编码作为一组进行解码*/
    for (int nCount = 0; nCount < nLength / BASE64_DECODE_GROUP_MEMBER; nCount++)
    {
        int aryBuffer[BASE64_DECODE_GROUP_MEMBER] = { 0 };
        CopyMemory(aryBuffer, &aryCode[nCount * BASE64_DECODE_GROUP_MEMBER], BASE64_DECODE_GROUP_MEMBER*sizeof(int));

        TCHAR chTrans = ((aryBuffer[0] << 2) & 0xfc) + ((aryBuffer[1] >> 4) & 0x3); //第一个编码左移两位，加上第二个编码的前两位，组成第一个字符
        szSource.AppendChar(chTrans);

        if (nCount != nLength / BASE64_DECODE_GROUP_MEMBER - 1 || nTail != 2) //如果不是最后一组，或者结尾没有两个等号就继续
        {
            chTrans = 0;
            chTrans = ((aryBuffer[1] << 4) & 0xf0) + ((aryBuffer[2] >> 2) & 0xf); //第二个编码剩下的四位加上第三个编码的前四位组成第二个字符
            szSource.AppendChar(chTrans);

            if (nCount != nLength / BASE64_DECODE_GROUP_MEMBER - 1 || nTail != 1)//如果不是最后一组，或者结尾没有一个等号就继续
            {
                chTrans = 0;
                chTrans = ((aryBuffer[2] << 6) & 0xc0) + (aryBuffer[3] & 0x3f); //第三个编码剩下的两位与最后一个编码组成最后一个字符
                szSource.AppendChar(chTrans);
            }
        }
    }

    if (aryCode != NULL)
    {
        delete aryCode;
        aryCode = NULL;
    }
    return TRUE;

}