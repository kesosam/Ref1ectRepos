#include "stdafx.h"
#include "Base64Encoder.h"

//Base64�����
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

/*����*/
BOOL CBase64Encoder::Encode(CString& szSource)
{
    if (szSource.IsEmpty())
    {
        return FALSE;
    } 

    int nLength = szSource.GetLength();
    int nTail = 0;

    if (nLength < BASE64_ENCODE_GROUP_MEMBER) //���������ַ��������Ƿ�Ϊ3�ı����������Ľ�β��=
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

    /*3���ַ�Ϊһ����б���*/
    for (int nCount = 0; nCount < nLength / BASE64_ENCODE_GROUP_MEMBER; nCount++)
    {
        TCHAR aryBuffer[BASE64_ENCODE_GROUP_MEMBER] = { 0 };
        CopyMemory(aryBuffer, &arySrc[nCount * BASE64_ENCODE_GROUP_MEMBER], BASE64_ENCODE_GROUP_MEMBER*sizeof(TCHAR));

        int nTrans = (aryBuffer[0] >> 2) & 0x3f; //��һ���ַ�������λ��ֻȡ����λ������λ����
        szSource.AppendChar(m_aryBase64Alphabet[nTrans]); //�������Ӧ���ַ����봫�����ַ���


        nTrans = 0;
        nTrans = ((aryBuffer[0] << 4) & 0x30) + ((aryBuffer[1] >> 4) & 0xf);//��һ���ַ�ʣ�µ���λ��ڶ����ַ���ǰ4λ��ӣ��õ��ڶ�������
        szSource.AppendChar(m_aryBase64Alphabet[nTrans]);

        if (nCount != nLength / BASE64_ENCODE_GROUP_MEMBER - 1 || nTail != 2) //�����β�������ַ���һ���Ҫ������=
        {
            nTrans = 0;
            nTrans = ((aryBuffer[1] << 2) & 0x3c) + ((aryBuffer[2] >> 6) & 0x3); //�ڶ����ַ�ʣ�µ��ĸ��ַ���������ַ���ǰ���ַ���ɵ���������
            szSource.AppendChar(m_aryBase64Alphabet[nTrans]);
        }
        else
        {
            szSource.AppendChar(_T('='));
        }

        if (nCount != nLength / BASE64_ENCODE_GROUP_MEMBER - 1 || nTail == 0) //�����β��һ���ַ���һ���Ҫ��һ��=
        {
            nTrans = 0;
            nTrans = aryBuffer[2] & 0x3f;
            szSource.AppendChar(m_aryBase64Alphabet[nTrans]); //�������ַ�ʣ�µ���λ��Ϊ���һ������
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

/*����*/
BOOL CBase64Encoder::Decode(CString& szSource)
{
    int nLength = szSource.GetLength();

    /*����Base64�ı������Base64�ı������Ϊ4�ı���*/
    if (nLength % 4)
    {
        return FALSE;
    }

    int nTail = 0;
    int nCheck = szSource.ReverseFind('=');

    /*�жϽ�β�м���=*/
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

    /*��������뷭�����ֵ*/
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

    /*�ĸ�������Ϊһ����н���*/
    for (int nCount = 0; nCount < nLength / BASE64_DECODE_GROUP_MEMBER; nCount++)
    {
        int aryBuffer[BASE64_DECODE_GROUP_MEMBER] = { 0 };
        CopyMemory(aryBuffer, &aryCode[nCount * BASE64_DECODE_GROUP_MEMBER], BASE64_DECODE_GROUP_MEMBER*sizeof(int));

        TCHAR chTrans = ((aryBuffer[0] << 2) & 0xfc) + ((aryBuffer[1] >> 4) & 0x3); //��һ������������λ�����ϵڶ��������ǰ��λ����ɵ�һ���ַ�
        szSource.AppendChar(chTrans);

        if (nCount != nLength / BASE64_DECODE_GROUP_MEMBER - 1 || nTail != 2) //����������һ�飬���߽�βû�������Ⱥžͼ���
        {
            chTrans = 0;
            chTrans = ((aryBuffer[1] << 4) & 0xf0) + ((aryBuffer[2] >> 2) & 0xf); //�ڶ�������ʣ�µ���λ���ϵ����������ǰ��λ��ɵڶ����ַ�
            szSource.AppendChar(chTrans);

            if (nCount != nLength / BASE64_DECODE_GROUP_MEMBER - 1 || nTail != 1)//����������һ�飬���߽�βû��һ���Ⱥžͼ���
            {
                chTrans = 0;
                chTrans = ((aryBuffer[2] << 6) & 0xc0) + (aryBuffer[3] & 0x3f); //����������ʣ�µ���λ�����һ������������һ���ַ�
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