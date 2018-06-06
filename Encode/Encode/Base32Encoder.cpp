#include "stdafx.h"
#include "Base32Encoder.h"

//Base64�����
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

/*����*/
BOOL CBase32Encoder::Encode(CString& szSource)
{
    if (szSource.IsEmpty())
    {
        return FALSE;
    }

    int nLength = szSource.GetLength();
    int nTail = 0;

    if (nLength < BASE32_ENCODE_GROUP_MEMBER) //���������ַ��������Ƿ�Ϊ5�ı����������Ľ�β��=
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

    /*5���ַ�Ϊһ����б���*/
    for (int nCount = 0; nCount < nLength / BASE32_ENCODE_GROUP_MEMBER; nCount++)
    {
        TCHAR aryBuffer[BASE32_ENCODE_GROUP_MEMBER] = { 0 };
        CopyMemory(aryBuffer, &arySrc[nCount * BASE32_ENCODE_GROUP_MEMBER], BASE32_ENCODE_GROUP_MEMBER*sizeof(TCHAR));

        int nTrans = (aryBuffer[0] >> 3) & 0x1f; //��һ���ַ�������λ��ֻȡ����λ������λ����
        szSource.AppendChar(m_aryBase32Alphabet[nTrans]); //�������Ӧ���ַ����봫�����ַ���


        nTrans = 0;
        nTrans = ((aryBuffer[0] << 2) & 0x1c) + ((aryBuffer[1] >> 6) & 0x3);//��һ���ַ�ʣ�µ���λ��ڶ����ַ���ǰ��λ��ӣ��õ��ڶ�������
        szSource.AppendChar(m_aryBase32Alphabet[nTrans]);

        if (nCount != nLength / BASE32_ENCODE_GROUP_MEMBER - 1 || nTail < 4) //�����β���ĸ��ַ���һ���Ҫ������=
        {
            nTrans = 0;
            nTrans = (aryBuffer[1] >> 1) & 0x1f;                      //�ڶ����ַ�ʣ�µ���λ����һλ���õ�����������
            szSource.AppendChar(m_aryBase32Alphabet[nTrans]);        //�������Ӧ���ַ����봫�����ַ���

            nTrans = 0;
            nTrans = ((aryBuffer[1] << 4) & 0x10) + ((aryBuffer[2] >> 4) & 0xf); //�ڶ����ַ�ʣ�µ�һλ��������ַ���ǰ��λ��ɵ��ĸ�����
            szSource.AppendChar(m_aryBase32Alphabet[nTrans]);
        }
        else
        {
            szSource.Append(_T("=="));
        }

        if (nCount != nLength / BASE32_ENCODE_GROUP_MEMBER - 1 || nTail < 3) //�����β�������ַ���һ���Ҫ��һ���ĸ�=
        {
            nTrans = 0;
            nTrans = ((aryBuffer[2] << 1) & 0x1e) + ((aryBuffer[3] >> 7) & 0x1);//�������ַ�ʣ�µ���λ�͵��ĸ��ַ���ǰһλ��ɵ��������
            szSource.AppendChar(m_aryBase32Alphabet[nTrans]);        //�������Ӧ���ַ����봫�����ַ���

            
        }
        else
        {
            szSource.AppendChar(_T('='));
        }

        if (nCount != nLength / BASE32_ENCODE_GROUP_MEMBER - 1 || nTail < 2) //�����β�������ַ���һ���Ҫ��һ������=
        {
            nTrans = 0;
            nTrans = (aryBuffer[3] >> 2) & 0x1f;                      //���ĸ��ַ�ʣ�µ���λ������λ���õ�����������
            szSource.AppendChar(m_aryBase32Alphabet[nTrans]);        //�������Ӧ���ַ����봫�����ַ���

            nTrans = 0;
            nTrans = ((aryBuffer[3] << 3) & 0x18) + ((aryBuffer[4] >> 5) & 0x7);//���ĸ��ַ�ʣ�µ���λ�͵�����ַ���ǰ��λ��ɵ��߸�����
            szSource.AppendChar(m_aryBase32Alphabet[nTrans]);        //�������Ӧ���ַ����봫�����ַ���
        }
        else
        {
            szSource.Append(_T("=="));
        }

        if (nCount != nLength / BASE32_ENCODE_GROUP_MEMBER - 1 || nTail < 1) //�����β��һ���ַ���һ���Ҫ��һ��һ��=
        {
            nTrans = 0;
            nTrans = (aryBuffer[4]) & 0x1f;                      //������ַ�ʣ�µ���λ�õ�����������
            szSource.AppendChar(m_aryBase32Alphabet[nTrans]);        //�������Ӧ���ַ����봫�����ַ���
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

/*����*/
BOOL CBase32Encoder::Decode(CString& szSource)
{
    int nLength = szSource.GetLength();

    /*����Base64�ı������Base64�ı������Ϊ8�ı���*/
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

    /*��������뷭�����ֵ*/
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

    /*�˸�������Ϊһ����н���*/
    for (int nCount = 0; nCount < nLength / BASE32_DECODE_GROUP_MEMBER; nCount++)
    {
        int aryBuffer[BASE32_DECODE_GROUP_MEMBER] = { 0 };
        CopyMemory(aryBuffer, &aryCode[nCount * BASE32_DECODE_GROUP_MEMBER], BASE32_DECODE_GROUP_MEMBER*sizeof(int));

        TCHAR chTrans = ((aryBuffer[0] << 3) & 0xf8) + ((aryBuffer[1] >> 2) & 0x7); //��һ������������λ�����ϵڶ��������ǰ��λ����ɵ�һ���ַ�
        szSource.AppendChar(chTrans);

        if (nCount != nLength / BASE32_DECODE_GROUP_MEMBER - 1 || nTail < 6) //����������һ�飬���߽�β���������Ⱥžͼ���
        {
            chTrans = 0;
            chTrans = ((aryBuffer[1] << 6) & 0xc0) + ((aryBuffer[2] << 1) & 0x3e) + ((aryBuffer[3] >> 4) & 0x1); //�ڶ�������ʣ�µ���λ���ϵ���������͵��ĸ������ǰһλ��ɵڶ����ַ�
            szSource.AppendChar(chTrans);
        }

        if (nCount != nLength / BASE32_DECODE_GROUP_MEMBER - 1 || nTail < 4)//����������һ�飬���߽�β�����ĸ��Ⱥžͼ���
        {
            chTrans = 0;
            chTrans = ((aryBuffer[3] << 4) & 0xf0) + ((aryBuffer[4] >> 1) & 0xf); //���ĸ�����ʣ�µ���λ�����������ǰ��λ��ɵ������ַ�
            szSource.AppendChar(chTrans);
        }

        if (nCount != nLength / BASE32_DECODE_GROUP_MEMBER - 1 || nTail < 3)//����������һ�飬���߽�β���������Ⱥžͼ���
        {
            chTrans = 0;
            chTrans = ((aryBuffer[4] << 7) & 0x80) + ((aryBuffer[5] << 2) & 0x7c) + ((aryBuffer[6] >> 3) & 0xf); //���������ʣ�µ�һλ�����������͵��߸������ǰ��λ��ɵ��ĸ��ַ�
            szSource.AppendChar(chTrans);
        }

        if (nCount != nLength / BASE32_DECODE_GROUP_MEMBER - 1 || nTail == 0)//����������һ�飬���߽�βû�еȺžͼ���
        {
            chTrans = 0;
            chTrans = ((aryBuffer[6] << 5) & 0xe0) + (aryBuffer[7]  & 0x1f); //���߸�����ʣ�µ���λ��ڰ˸�������ɵ��������
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
