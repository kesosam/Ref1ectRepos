#include "stdafx.h"
#include "Base32Encoder.h"

//Base64编码表
char CBase32Encoder::m_aryBase32Alphabet[32] = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
    'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
    'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
    'Y', 'Z', '2', '3', '4', '5', '6', '7' };

CBase32Encoder::CBase32Encoder()
{
}


CBase32Encoder::~CBase32Encoder()
{
}

/*编码*/
BOOL CBase32Encoder::Encode(CString& szSource)
{
    if (szSource.IsEmpty())
    {
        return FALSE;
    }

    int nLength = szSource.GetLength();
    int nTail = 0;

    if (nLength < BASE32_ENCODE_GROUP_MEMBER) //计算编码的字符串长度是否为5的倍数，不够的结尾加=
    {
        nTail = BASE32_ENCODE_GROUP_MEMBER - nLength;
    }
    else
    {
        nTail = nLength % BASE32_ENCODE_GROUP_MEMBER;

        if (nTail != 0)
        {
            nTail = BASE32_ENCODE_GROUP_MEMBER - nTail;
        }
    }

    nLength += nTail;
    TCHAR * arySrc = new TCHAR[nLength]();
    CopyMemory(arySrc, (void*)szSource.GetString(), (szSource.GetLength()* sizeof(TCHAR)));
    szSource.Empty();

    /*5个字符为一组进行编码*/
    for (int nCount = 0; nCount < nLength / BASE32_ENCODE_GROUP_MEMBER; nCount++)
    {
        TCHAR aryBuffer[BASE32_ENCODE_GROUP_MEMBER] = { 0 };
        CopyMemory(aryBuffer, &arySrc[nCount * BASE32_ENCODE_GROUP_MEMBER], BASE32_ENCODE_GROUP_MEMBER*sizeof(TCHAR));

        int nTrans = (aryBuffer[0] >> 3) & 0x1f; //第一个字符右移三位，只取后五位，其他位置零
        szSource.AppendChar(m_aryBase32Alphabet[nTrans]); //查表，将对应的字符加入传出的字符串


        nTrans = 0;
        nTrans = ((aryBuffer[0] << 2) & 0x1c) + ((aryBuffer[1] >> 6) & 0x3);//第一个字符剩下的三位与第二个字符的前两位相加，得到第二个编码
        szSource.AppendChar(m_aryBase32Alphabet[nTrans]);

        if (nCount != nLength / BASE32_ENCODE_GROUP_MEMBER - 1 || nTail < 4) //如果结尾差四个字符满一组就要补六个=
        {
            nTrans = 0;
            nTrans = (aryBuffer[1] >> 1) & 0x1f;                      //第二个字符剩下的六位右移一位，得到第三个编码
            szSource.AppendChar(m_aryBase32Alphabet[nTrans]);        //查表，将对应的字符加入传出的字符串

            nTrans = 0;
            nTrans = ((aryBuffer[1] << 4) & 0x10) + ((aryBuffer[2] >> 4) & 0xf); //第二个字符剩下的一位与第三个字符的前四位组成第四个编码
            szSource.AppendChar(m_aryBase32Alphabet[nTrans]);
        }
        else
        {
            szSource.Append(_T("=="));
        }

        if (nCount != nLength / BASE32_ENCODE_GROUP_MEMBER - 1 || nTail < 3) //如果结尾差三个字符满一组就要补一个四个=
        {
            nTrans = 0;
            nTrans = ((aryBuffer[2] << 1) & 0x1e) + ((aryBuffer[3] >> 7) & 0x1);//第三个字符剩下的四位和第四个字符的前一位组成第五个编码
            szSource.AppendChar(m_aryBase32Alphabet[nTrans]);        //查表，将对应的字符加入传出的字符串

            
        }
        else
        {
            szSource.AppendChar(_T('='));
        }

        if (nCount != nLength / BASE32_ENCODE_GROUP_MEMBER - 1 || nTail < 2) //如果结尾差两个字符满一组就要补一个三个=
        {
            nTrans = 0;
            nTrans = (aryBuffer[3] >> 2) & 0x1f;                      //第四个字符剩下的七位右移两位，得到第六个编码
            szSource.AppendChar(m_aryBase32Alphabet[nTrans]);        //查表，将对应的字符加入传出的字符串

            nTrans = 0;
            nTrans = ((aryBuffer[3] << 3) & 0x18) + ((aryBuffer[4] >> 5) & 0x7);//第四个字符剩下的两位和第五个字符的前三位组成第七个编码
            szSource.AppendChar(m_aryBase32Alphabet[nTrans]);        //查表，将对应的字符加入传出的字符串
        }
        else
        {
            szSource.Append(_T("=="));
        }

        if (nCount != nLength / BASE32_ENCODE_GROUP_MEMBER - 1 || nTail < 1) //如果结尾差一个字符满一组就要补一个一个=
        {
            nTrans = 0;
            nTrans = (aryBuffer[4]) & 0x1f;                      //第五个字符剩下的五位得到第六个编码
            szSource.AppendChar(m_aryBase32Alphabet[nTrans]);        //查表，将对应的字符加入传出的字符串
        }
        else
        {
            szSource.AppendChar(_T('='));
        }
    }

    if (arySrc != NULL)
    {
        delete[] arySrc;
        arySrc = NULL;
    }
    return TRUE;
}

/*解码*/
BOOL CBase32Encoder::Decode(CString& szSource)
{
    int nLength = szSource.GetLength();

    /*由于Base64的编码规则，Base64的编码必须为8的倍数*/
    if (nLength % BASE32_DECODE_GROUP_MEMBER)
    {
        return FALSE;
    }

    int nTail = 0;
    CString strTemp = szSource;
    
    strTemp.TrimRight('=');
    nTail = szSource.GetLength() - strTemp.GetLength();

    TCHAR * arySrc = new TCHAR[nLength]();
    int * aryCode = new int[nLength]();
    CopyMemory(arySrc, szSource.GetString(), (nLength * sizeof(TCHAR)));
    szSource.Empty();

    /*查表，将编码翻译成数值*/
    for (int nCount = 0; nCount < nLength; nCount++)
    {
        for (int nCount1 = 0; nCount1 < 32; nCount1++)
        {
            if (arySrc[nCount] == m_aryBase32Alphabet[nCount1])
            {
                aryCode[nCount] = nCount1;
                break;
            }
        }
    }

    if (arySrc != NULL)
    {
        delete[] arySrc;
        arySrc = NULL;
    }

    /*八个编码作为一组进行解码*/
    for (int nCount = 0; nCount < nLength / BASE32_DECODE_GROUP_MEMBER; nCount++)
    {
        int aryBuffer[BASE32_DECODE_GROUP_MEMBER] = { 0 };
        CopyMemory(aryBuffer, &aryCode[nCount * BASE32_DECODE_GROUP_MEMBER], BASE32_DECODE_GROUP_MEMBER*sizeof(int));

        TCHAR chTrans = ((aryBuffer[0] << 3) & 0xf8) + ((aryBuffer[1] >> 2) & 0x7); //第一个编码左移三位，加上第二个编码的前三位，组成第一个字符
        szSource.AppendChar(chTrans);

        if (nCount != nLength / BASE32_DECODE_GROUP_MEMBER - 1 || nTail < 6) //如果不是最后一组，或者结尾少于六个等号就继续
        {
            chTrans = 0;
            chTrans = ((aryBuffer[1] << 6) & 0xc0) + ((aryBuffer[2] << 1) & 0x3e) + ((aryBuffer[3] >> 4) & 0x1); //第二个编码剩下的两位加上第三个编码和第四个编码的前一位组成第二个字符
            szSource.AppendChar(chTrans);
        }

        if (nCount != nLength / BASE32_DECODE_GROUP_MEMBER - 1 || nTail < 4)//如果不是最后一组，或者结尾少于四个等号就继续
        {
            chTrans = 0;
            chTrans = ((aryBuffer[3] << 4) & 0xf0) + ((aryBuffer[4] >> 1) & 0xf); //第四个编码剩下的四位与第五个编码的前四位组成第三个字符
            szSource.AppendChar(chTrans);
        }

        if (nCount != nLength / BASE32_DECODE_GROUP_MEMBER - 1 || nTail < 3)//如果不是最后一组，或者结尾少于三个等号就继续
        {
            chTrans = 0;
            chTrans = ((aryBuffer[4] << 7) & 0x80) + ((aryBuffer[5] << 2) & 0x7c) + ((aryBuffer[6] >> 3) & 0xf); //第五个编码剩下的一位与第六个编码和第七个编码的前两位组成第四个字符
            szSource.AppendChar(chTrans);
        }

        if (nCount != nLength / BASE32_DECODE_GROUP_MEMBER - 1 || nTail == 0)//如果不是最后一组，或者结尾没有等号就继续
        {
            chTrans = 0;
            chTrans = ((aryBuffer[6] << 5) & 0xe0) + (aryBuffer[7]  & 0x1f); //第七个编码剩下的三位与第八个编码组成第五个编码
            szSource.AppendChar(chTrans);
        }

    }

    if (aryCode != NULL)
    {
        delete aryCode;
        aryCode = NULL;
    }
    return TRUE;

}
