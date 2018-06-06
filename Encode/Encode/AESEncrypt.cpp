#include "stdafx.h"
#include "AESEncrypt.h"

/*S盒*/
char CAESEncrypt::m_arySBox[256] = { 
 //  0    1     2      3    4     5      6    7     8     9     A      B     C     D     E     F 
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,    // 0
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,    // 1
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,    // 2
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,    // 3
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,    // 4
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,    // 5
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,    // 6
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,    // 7
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,    // 8
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,    // 9
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,    // A
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,    // B
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,    // C
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,    // D
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,    // E
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16 };  // F

//逆S盒
char CAESEncrypt::m_ReverseSBox[256] = {
  //  0    1     2      3    4     5      6    7     8     9     A      B     C     D     E     F 
    0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb, // 0
    0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb, // 1
    0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e, // 2           
    0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25, // 3
    0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92, // 4
    0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84, // 5
    0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06, // 6
    0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b, // 7
    0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73, // 8
    0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e, // 9
    0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b, // A
    0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4, // B
    0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f, // C
    0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef, // D
    0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61, // E
    0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d  // F
};

char CAESEncrypt::m_MCMatrix[4][4] = { 2, 3, 1, 1,
                                       1, 2, 3, 1,
                                       1, 1, 2, 3,
                                       3, 1, 1, 2 };


char CAESEncrypt::m_RevMCMatrix[4][4] = { 0xe, 0xb, 0xd, 0x9,
                                          0x9, 0xe, 0xb, 0xd,
                                          0xd, 0x9, 0xe, 0xb,
                                          0xb, 0xd, 0x9, 0xe };


CAESEncrypt::CAESEncrypt()
{
}


CAESEncrypt::~CAESEncrypt()
{
}

char CAESEncrypt::GFMul0x2(char chInput)
{
    int nResult = chInput << 1;
    int a7 = nResult & 0x00000100;

    if (a7 != 0) {                           
        nResult = nResult & 0x000000ff;
        nResult = nResult ^ 0x1b;
    }

    return (char)nResult;
}

char CAESEncrypt::GFMul0x3(char btInput)
{
    return (char)((int)GFMul0x2(btInput) ^ (int)btInput);
}


char CAESEncrypt::GFMul0x4(char chInput)
{
    return GFMul0x2(GFMul0x2(chInput));
}                                         


char CAESEncrypt::GFMul0x8(char chInput)
{
    return GFMul0x2(GFMul0x4(chInput));
}

             
char CAESEncrypt::GFMul0x9(char chInput)
{
    return (char)((int)GFMul0x8(chInput) ^ (int)chInput);
}

char CAESEncrypt::GFMul0xb(char chInput)
{
    return (char)((int)GFMul0x9(chInput) ^ (int)GFMul0x2(chInput));
}


char CAESEncrypt::GFMul0xc(char chInput)
{
    return (char)((int)GFMul0x8(chInput) ^ (int)GFMul0x4(chInput));
}

char CAESEncrypt::GFMul0xd(char chInput)
{
    return (char)((int)GFMul0xc(chInput) ^ (int)chInput);
}

char CAESEncrypt::GFMul0xe(char chInput)
{
    return (char)((int)GFMul0xc(chInput) ^ (int)GFMul0x2(chInput));
}

char CAESEncrypt::GFMul(char chInputN, char chInputS)
{
    char result;

    if (chInputN == 1)
        result = chInputS;
    else if ((int)chInputN == 2)
        result = GFMul0x2(chInputS);
    else if ((int)chInputN == 3)
        result = GFMul0x3(chInputS);
    else if ((int)chInputN == 0x9)
        result = GFMul0x9(chInputS);
    else if ((int)chInputN == 0xb)//11
        result = GFMul0xb(chInputS);
    else if ((int)chInputN == 0xd)//13
        result = GFMul0xd(chInputS);
    else if ((int)chInputN == 0xe)//14
        result = GFMul0xe(chInputS);

    return result;

}

//密钥扩展算法,把用户密钥扩展为加4乘以密轮数个字节的轮密钥
//  nUserKey        用户密码   
//  nKeyNum         密钥位数
BOOL CAESEncrypt::KeyExpantion(char* pUserKey, int nKeyNum, int nRoundNum)
{
    for (int i = 0; i < nKeyNum; i++)
    { 
        char aryTemp[] = { pUserKey[i * 4 + 3], pUserKey[i * 4 + 2], pUserKey[i * 4 + 1], pUserKey[i * 4] };
        CopyMemory(&m_aryRoundKeys[i], aryTemp, 4);
    }

    //轮常量
    DWORD dwRcon[] = { 0x01000000, 0x02000000,0x04000000, 0x08000000,
                    0x10000000, 0x20000000,0x40000000, 0x80000000,
                    0x1b000000, 0x36000000 };
    int nRound = 0; //轮常量执行轮数

    for (int i = nKeyNum; i < 4*(nRoundNum+1); i++) //将密钥扩展到加密轮数加1乘4个字
    {
        if (0 != i % nKeyNum)   //如果i不是密钥长度的倍数，m_nRoundKeys[i]=m_nRoundKeys[i-nKeyNum]^m_nRoundKeys[i-1] 
        {
            m_aryRoundKeys[i] = m_aryRoundKeys[i - nKeyNum] ^ m_aryRoundKeys[i - 1];
        }
        else
        {
            //如果i是密钥长度的倍数,要先对m_nRoundKeys[i-1]执行移位,字节代换和与轮常量异或，然后才与m_nRoundKeys[i-4]异或
            //第一步要对m_nRoundKeys[i-1]执行左移一字节操作
            char aryResult[4] = { 0 };
     
            aryResult[0] = (m_aryRoundKeys[i - 1] >> 16) & 0xff;
            aryResult[1] = (m_aryRoundKeys[i - 1] >> 8) & 0xff;
            aryResult[2] = m_aryRoundKeys[i - 1] & 0xff;
            aryResult[3] = (m_aryRoundKeys[i - 1] >> 24) & 0xff;
            
            DWORD dwResult = 0;

            //第二步对第一步的结果进行字节代换
            Subchars(aryResult, 4, TRUE);
            CopyMemory(&dwResult, aryResult, 4);

            //第三步与轮常量异或
            if (10 == nRound)
            {
                nRound = 0; //轮常量轮数在0-9之间循环
            }
            dwResult = dwResult ^ dwRcon[nRound];
            nRound++;

            //最后将前三步得到的结果与m_nRoundKeys[i-nKeyNum]异或得到m_nRoundKeys[i]
            m_aryRoundKeys[i] = m_aryRoundKeys[i - nKeyNum] ^ dwResult;
        }   
    }
    return TRUE;
}

//加密轮函数
// btState       状态矩阵
// nRoundNum    加密轮数 
BOOL CAESEncrypt::Round(char IN OUT aryState[16], int nRoundNum)
{
    //  第一轮加密，只用进行轮密钥相加
    AddRoundKey(aryState, 0);     //  添加第i轮的轮密钥

    //  中间 nRoundNum - 2 轮加密，要执行四个步骤
    for (int i = 1; i < nRoundNum; i++)
    {
        Subchars(aryState, 16);       //  字节代换
        
        ShiftRow(aryState);           //  行移位

        MixColumn(aryState);          //  列混合

        AddRoundKey(aryState, i);     //  添加第i轮的轮密钥
    }

    //  最后一轮加密，要执行除列混合之外的其他三个步骤
    Subchars(aryState, 16);           //  字节代换
    ShiftRow(aryState);               //  行移位
    AddRoundKey(aryState, nRoundNum); //  添加最后一个轮密钥

    char aryTemp[16] = { 0 };
    CopyMemory(aryTemp, aryState, 16);

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            aryState[(i * 4) + j] = aryTemp[(j * 4) + i];
        }
    }

    return TRUE;
}

BOOL CAESEncrypt::ECBEncrypt(char* arySrc, int nSrcLen, CStringA IN strKey, char* aryResult)
{
    int nKeyLen = strKey.GetLength();
    int nRoundNum = (nKeyLen / 8) + 8;

    KeyExpantion((char *)strKey.GetString(), nKeyLen / 4, nRoundNum);
    //把输入的字符串分割成16字节为一个单位
    for (int nCount = 0; nCount < nSrcLen / 16; nCount++)
    {
        //把明文数据放入状态矩阵
        char aryState[16];
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                aryState[(i * 4) + j] = arySrc[(nCount * 16) + j * 4 + i];
            }
        }

        //输入轮函数进行加密
        Round(aryState, nRoundNum);
        char szBuffer[20] = { 0 };
        CopyMemory(&aryResult[nCount * 16], aryState, 16);
    }
    aryResult[nSrcLen] = 0;
    memset(m_aryRoundKeys, 0, sizeof(m_aryRoundKeys));
    return TRUE;
}

//字节代换
BOOL CAESEncrypt::Subchars(char IN OUT aryState[], int nSize ,BOOL bRevOrder)
{
    char* aryTemp = new char[nSize];
    CopyMemory(aryTemp, aryState, nSize);
    for (int i = 0; i < nSize; i++)
    {
        int nRow = aryTemp[i] & 0xf;
        int nCol = (aryTemp[i] >> 4) & 0xf;
        if (bRevOrder)
        {
            aryState[nSize - i - 1] = m_arySBox[nCol * 0x10 + nRow];
        }
        else
        {
            aryState[i] = m_arySBox[nCol * 0x10 + nRow];
        }
    }

    if (aryTemp != NULL)
    {
        delete[] aryTemp;
    }
    return TRUE;
}

//行移位
BOOL CAESEncrypt::ShiftRow(char IN OUT btState[16])
{
     //第0行不用移位
     //第1行向左移动1字节
    char btTemp = btState[4];
    btState[4] = btState[5];
    btState[5] = btState[6];
    btState[6] = btState[7];
    btState[7] = btTemp;

    //第2行向左移动2字节
    btTemp = btState[10];
    btState[10] = btState[8];
    btState[8] = btTemp;
    btTemp = btState[11];
    btState[11] = btState[9];
    btState[9] = btTemp;

    //第3行向左移动3字节
    btTemp = btState[15];
    btState[15] = btState[14];
    btState[14] = btState[13];
    btState[13] = btState[12];
    btState[12] = btTemp;

    return TRUE;
}

BOOL CAESEncrypt::MixColumn(char IN OUT aryState[16])
{
    char aryTemp[4][4] = { 0 };
    char aryResult[4][4] = { 0 };
    CopyMemory(aryTemp, aryState, 16);

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++){
            aryResult[i][j] = GFMul(m_MCMatrix[i][0], aryTemp[0][j]) ^ GFMul(m_MCMatrix[i][1], aryTemp[1][j]) ^ GFMul(m_MCMatrix[i][2], aryTemp[2][j]) ^ GFMul(m_MCMatrix[i][3], aryTemp[3][j]);
        }
    }

    CopyMemory(aryState, aryResult, 16);

    return TRUE;
}

//轮密钥相加
BOOL CAESEncrypt::AddRoundKey(char IN OUT btState[16], int nRoundCount, BOOL IsDeCrypt)
{
    char aryRoundKey[16] = { 0 };
    char aryTempKey[16] = { 0 };
    CopyMemory(aryRoundKey, &m_aryRoundKeys[4 * nRoundCount ], 16);

    if (IsDeCrypt)
    {
        for (int i = 0; i < 4; i++)
        {
            char aryTemp[] = { aryRoundKey[3 - i],
                aryRoundKey[7 - i],
                aryRoundKey[11 - i],        
                aryRoundKey[15 - i] };
            CopyMemory(&aryTempKey[i * 4], aryTemp, 4);
        }

        CopyMemory(aryRoundKey, aryTempKey, 16);

        InvMixColumn(aryRoundKey);

        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                btState[(4 * j) + i] = btState[(4 * j) + i] ^ aryRoundKey[(4 * j) + i];
            }
        }
    }
    else
    {
        for (int i = 0; i < 4; i++)
        {
            char aryTemp[] = { aryRoundKey[(4 * i) + 3],
                aryRoundKey[(4 * i) + 2],
                aryRoundKey[(4 * i) + 1],
                aryRoundKey[(4 * i)] };
            CopyMemory(&aryRoundKey[i * 4], aryTemp, 4);
        }

        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                btState[(4 * j) + i] = btState[(4 * j) + i] ^ aryRoundKey[(i * 4) + j];
            }
        }
    }

    return TRUE;
}

BOOL CAESEncrypt::InvSubchars(char IN OUT aryState[], int nSize, BOOL bRevOrder)
{
    char* aryTemp = new char[nSize];
    CopyMemory(aryTemp, aryState, nSize);
    for (int i = 0; i < nSize; i++)
    {
        int nRow = aryTemp[i] & 0xf;
        int nCol = (aryTemp[i] >> 4) & 0xf;
        if (bRevOrder)
        {
            aryState[nSize - i - 1] = m_ReverseSBox[nCol * 0x10 + nRow];
        }
        else
        {
            aryState[i] = m_ReverseSBox[nCol * 0x10 + nRow];
        }
    }

    if (aryTemp != NULL)
    {
        delete[] aryTemp;
    }
    return TRUE;
}

BOOL CAESEncrypt::InvShiftRow(char IN OUT aryState[16])
{
    //第0行不用移位
    //第1行向右移动1字节
    char chTemp = aryState[7];
    aryState[7] = aryState[6];
    aryState[6] = aryState[5];
    aryState[5] = aryState[4];
    aryState[4] = chTemp;

    //第2行向右移动2字节
    chTemp = aryState[8];
    aryState[8] = aryState[10];
    aryState[10] = chTemp;
    chTemp = aryState[9];
    aryState[9] = aryState[11];
    aryState[11] = chTemp;

    //第3行向右移动3字节
    chTemp = aryState[12];
    aryState[12] = aryState[13];
    aryState[13] = aryState[14];
    aryState[14] = aryState[15];
    aryState[15] = chTemp;

    return TRUE;
}

BOOL CAESEncrypt::InvMixColumn(char IN OUT aryState[16])
{
    char aryTemp[4][4] = { 0 };
    char aryResult[4][4] = { 0 };
    CopyMemory(aryTemp, aryState, 16);

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++){
            aryResult[i][j] = GFMul(m_RevMCMatrix[i][0], aryTemp[0][j]) ^ GFMul(m_RevMCMatrix[i][1], aryTemp[1][j]) ^ GFMul(m_RevMCMatrix[i][2], aryTemp[2][j]) ^ GFMul(m_RevMCMatrix[i][3], aryTemp[3][j]);
        }
    }

    CopyMemory(aryState, aryResult, 16);
    return TRUE;
}

BOOL CAESEncrypt::ECBDecrypt(CStringA& IN OUT strResult, CStringA IN strKey, char* arySrc, int nSrcLen)
{
    int nKeyLen = strKey.GetLength();

    int nRoundNum = (nKeyLen / 8) + 8;

    KeyExpantion((char *)strKey.GetString(), nKeyLen / 4, nRoundNum);

    //把输入的字符串分割成16字节为一个单位
    for (int nCount = 0; nCount < nSrcLen / 16; nCount++)
    {
        //把明文数据放入状态矩阵
        char aryState[16];
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                aryState[i * 4 + j] = arySrc[(16 * nCount) + j * 4 + i];
            }
        }
        //输入解密轮函数进行解密
        RevRound(aryState, nRoundNum);
        char szBuffer[20] = { 0 };
        CopyMemory(szBuffer, aryState, 16);
        CopyMemory(&arySrc[nCount * 16], aryState, 16);
        szBuffer[16] = 0;
        strResult += szBuffer;
    }
    memset(m_aryRoundKeys, 0, sizeof(m_aryRoundKeys));
    return TRUE;
}

BOOL CAESEncrypt::RevRound(char IN OUT aryState[16], int nRoundNum)
{

    //  最后一轮，只用进行轮密钥相加
    AddRoundKey(aryState, nRoundNum);     //  添加第i轮的轮密钥

    //  中间 nRoundNum - 2 轮，要执行四个步骤
    for (int i = nRoundNum - 1; i >= 1; i--)
    {
        InvSubchars(aryState, 16);       //  逆字节代换

        InvShiftRow(aryState);           //  逆行移位

        InvMixColumn(aryState);          //  逆列混合

        AddRoundKey(aryState, i, TRUE);     //  添加第i轮的轮密钥
    }

    //  第0轮，要执行除逆列混合之外的其他三个步骤
    InvSubchars(aryState, 16);           //  逆字节代换
    InvShiftRow(aryState);               //  逆行移位
    AddRoundKey(aryState, 0); //  添加最后一个轮密钥

    char aryTemp[16] = { 0 };
    CopyMemory(aryTemp, aryState, 16);

    for (int i = 0; i < 4; i++)
     {
     for (int j = 0; j < 4; j++)
     {
     aryState[(i * 4) + j] = aryTemp[(j * 4) + i];
     }
     }

    return TRUE;
}

BOOL CAESEncrypt::EncrypteAsHexString(CString& IN OUT strSrc, CString& IN OUT strKey, int nMode, CString strIV)
{
    int nKeyLen = strKey.GetLength();

    //检查加密字符串长度和密钥长度
    if (0 == strSrc.GetLength() ||
        (16 != nKeyLen && 
        24 != nKeyLen &&
        32 != nKeyLen))
    {
        return FALSE;
    }

    CStringA strSrcA = CW2A(strSrc);
    CStringA strKeyA = CW2A(strKey);
    CStringA strIVA = CW2A(strIV);
    char* szBuffer = new char[strSrcA.GetLength() + 20];
    memset(szBuffer, 0, strSrcA.GetLength() + 20);
    CopyMemory(szBuffer, strSrcA.GetString(), strSrcA.GetLength());
    
    int nSrcLen = ZeroPadding(szBuffer, strSrcA.GetLength()); 

    char *aryResult = new char[nSrcLen + 1];

    switch (nMode)
    {
    case ECB:
    {
        ECBEncrypt(szBuffer, nSrcLen, strKeyA, aryResult);
        break;
    }
    case CBC:
    {
        CBCEncrypt(szBuffer, nSrcLen, strKeyA, aryResult, strIVA);
        break;
    }
    default:
        break;
    }
    

    CString strTemp;
    
    strSrc.Empty();
    for (int i = 0; i < nSrcLen; i++)
    {
        strTemp.Format(_T("%0.2x"), aryResult[i]);
        strSrc += strTemp.Right(2);
    }
    if (aryResult != NULL)
    {
        delete[] aryResult;
    }
    return TRUE;
}

UINT CAESEncrypt::ZeroPadding(char* IN OUT strSrc, UINT nLength)
{
    int nCount = 16 - nLength % 16;
    int nPading = 0;
    if (nCount > 10)
    {
        nPading = nCount - 2;
    }
    else
    {
        nPading = nCount - 1;
    }

    for (int i = 0; i < nPading; i++)
    {
        strSrc[nLength + i] = '0';
    }

    CStringA strTemp;
    strTemp.Format("%d", nCount);

    if (nCount > 10)
    {
        strSrc[nLength + nPading] = strTemp.GetAt(0);
        strSrc[nLength + nPading + 1] = strTemp.GetAt(1);
    }
    else
    {
        strSrc[nLength + nPading] = strTemp.GetAt(0);
    }
    return nLength + nCount;
}

UINT CAESEncrypt::UnPadding(char* IN OUT strSrc, UINT nLength)
{
    char* strTemp;
    strTemp = strSrc + nLength - 2;

    int nCount = atoi(strTemp);

    strSrc[nLength - nCount] = 0;

    return nLength - nCount;
}

BOOL CAESEncrypt::DecrypteHexString(CString& IN OUT strSrc, CString& IN strKey, int nMode, CString strIV)
{
    CStringA strSrcA;
    CStringA strKeyA = CW2A(strKey);
    CStringA strIVA = CW2A(strIV);
    CString strTemp = strSrc;
    CStringA strResult;
    int nSrcLen = strSrc.GetLength();

    char chData[24] = { 0 };
    int nTemp = nSrcLen;
    for (int i = 0; i < (nSrcLen / 2); i++)
    {
        nTemp -= 2;
        int nResult = swscanf_s(strTemp, _T("%2x"), chData);
        strSrcA.AppendChar(chData[0]);
        strTemp = strTemp.Right(nTemp);
    }

    nSrcLen = strSrcA.GetLength();
    char* szBuffer = new char[nSrcLen];
    CopyMemory(szBuffer, strSrcA.GetString(), nSrcLen);

    switch (nMode)
    {
    case ECB:
    {
        ECBDecrypt(strResult, strKeyA, szBuffer, nSrcLen);
        break;
    }
    case CBC:
    {
        CBCDecrypt(strResult, strKeyA, strIVA, szBuffer, nSrcLen);
    }
    default:
        break;
    }

    UnPadding((char *)strResult.GetString(), strSrcA.GetLength());
    strSrc = strResult;

    return TRUE;
}

BOOL CAESEncrypt::EncrypteFromFile(CString strSrcFilePath, CString strDstFilePath, CString strKey, int nMode, CString strIV)
{
    int nKeyLen = strKey.GetLength();
    if (strSrcFilePath.IsEmpty() ||
        (16 != nKeyLen &&
        24 != nKeyLen &&
        32 != nKeyLen) || strSrcFilePath.IsEmpty()||
        strDstFilePath.IsEmpty())
    {
        return FALSE;
    }

    CStringA strKeyA = CW2A(strKey);
    CStringA strIVA = CW2A(strIV);
    CFile fileSrc(strSrcFilePath, CFile::modeRead | CFile::typeBinary);
    CFile fileDst(strDstFilePath, CFile::modeReadWrite | CFile::modeCreate | CFile::typeBinary);
    char szBuffer[4116] = { 0 };
    ULONGLONG nTotalReaded = 0;
    UINT nAfterPadding = 0;

    while (fileSrc.GetLength() != nTotalReaded)
    {
        UINT nReadedSize = 0;
        memset(szBuffer, 0, 4100);
        nReadedSize = fileSrc.Read(szBuffer, 4096);
        nTotalReaded += nReadedSize;
       

        if (fileSrc.GetLength() == nTotalReaded)
        {
            nReadedSize = ZeroPadding(szBuffer, nReadedSize);
        }

        CStringA strData = szBuffer;
        
        char *aryResult = new char[nReadedSize + 1];
        memset(aryResult, 0, nReadedSize);

        switch (nMode)
        {
        case ECB:
        {
            ECBEncrypt(szBuffer, nReadedSize, strKeyA, aryResult);
            break;
        }
        case CBC:
        {
            CBCEncrypt(szBuffer, nReadedSize, strKeyA, aryResult, strIVA);
            break;
        }
        default:
            break;
        }

        fileDst.Write(aryResult, nReadedSize);
        if (aryResult != NULL)
        {
            delete[] aryResult;
        }
    } 
    fileDst.Flush();
    fileDst.Close();
    fileSrc.Close();
    return TRUE;
}

BOOL CAESEncrypt::DecrypteFromFile(CString strSrcFilePath, CString strDstFilePath, CString strKey, int nMode, CString strIV)
{
    int nKeyLen = strKey.GetLength();
    if (strSrcFilePath.IsEmpty() ||
        (16 != nKeyLen &&
        24 != nKeyLen &&
        32 != nKeyLen))
    {
        return FALSE;
    }

    CStringA strKeyA = CW2A(strKey);
    CStringA strIVA = CW2A(strIV);
    CFile fileSrc(strSrcFilePath, CFile::modeRead | CFile::typeBinary);
    CFile fileDst(strDstFilePath, CFile::modeReadWrite | CFile::modeCreate | CFile::typeBinary);
    char szBuffer[4116] = { 0 };
    ULONGLONG nTotalReaded = 0;
    UINT nPadding = 0;

    while (fileSrc.GetLength() != nTotalReaded)
    {
        UINT nReadSize = 0;
        memset(szBuffer, 0, 4116);
        nReadSize = fileSrc.Read(szBuffer, 4096);
        nTotalReaded += nReadSize;
        CStringA strData;

        switch (nMode)
        {
        case ECB:
        {
            ECBDecrypt(strData, strKeyA, szBuffer,nReadSize);
            break;
        }
        case CBC:
        {
            CBCDecrypt(strData, strKeyA, strIVA, szBuffer, nReadSize);
            break;
        }
        default:
            break;
        }

        if (fileSrc.GetLength() == nTotalReaded)
        {
            nReadSize = UnPadding(szBuffer, nReadSize);
        }

        fileDst.Write(szBuffer, nReadSize);
    }

    fileDst.Flush();
    fileDst.Close();
    fileSrc.Close();

    return TRUE;
}

BOOL CAESEncrypt::CBCEncrypt(char* arySrc, int nSrcLen, CStringA strKey, char* aryResult, CStringA strIV)
{
    int nKeyLen = strKey.GetLength();
    int nRoundNum = (nKeyLen / 8) + 8;

    char aryIV[16] = { 0 };
    CopyMemory(aryIV, strIV.GetString(), 16);

    KeyExpantion((char *)strKey.GetString(), nKeyLen / 4, nRoundNum);
    //把输入的字符串分割成16字节为一个单位
    for (int nCount = 0; nCount < nSrcLen / 16; nCount++)
    {
        //把明文数据放入状态矩阵
        char aryState[16];
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                aryState[(i * 4) + j] = arySrc[(nCount * 16) + j * 4 + i];
            }
        }

        //将状态矩阵与初始化向量或上一个密文块进行异或
        for (int n = 0; n < 16; n++)
        {
            aryState[n] ^= aryIV[n];
        }

        //输入轮函数进行加密
        Round(aryState, nRoundNum);
        char szBuffer[20] = { 0 };
        CopyMemory(&aryResult[nCount * 16], aryState, 16);
        CopyMemory(aryIV, aryState, 16);
    }
    aryResult[nSrcLen] = 0;
    memset(m_aryRoundKeys, 0, sizeof(m_aryRoundKeys));
    return TRUE;
}

BOOL CAESEncrypt::CBCDecrypt(CStringA& strResult, CStringA IN strKey, CStringA strIV, char* arySrc, int nSrcLen)
{
    int nKeyLen = strKey.GetLength();

    int nRoundNum = (nKeyLen / 8) + 8;

    KeyExpantion((char *)strKey.GetString(), nKeyLen / 4, nRoundNum);

    char aryIV[16] = { 0 };
    CopyMemory(aryIV, strIV.GetString(), 16);

    //把输入的字符串分割成16字节为一个单位
    for (int nCount = 0; nCount < nSrcLen / 16; nCount++)
    {
        //把明文数据放入状态矩阵
        char aryState[16] = { 0 };
        char aryBackup[16] = { 0 };
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                aryState[(i * 4) + j] = arySrc[(nCount * 16) + j * 4 + i];
            }
        }

        CopyMemory(aryBackup, aryState, 16);

        //输入解密轮函数进行解密
        RevRound(aryState, nRoundNum);

        if (nCount == 0)
        {
            char aryTemp[16] = { 0 };
            CopyMemory(aryTemp, aryIV, 16);

            for (int i = 0; i < 4; i++)
            {
                for (int j = 0; j < 4; j++)
                {
                    aryIV[(i * 4) + j] = aryTemp[(j * 4) + i];
                }
            }
        }

        //将状态矩阵与初始化向量或上一个密文块进行异或
        for (int n = 0; n < 16; n++)
        {
            aryState[n] ^= aryIV[n];
        }

        char szBuffer[20] = { 0 };
        CopyMemory(szBuffer, aryState, 16);
        CopyMemory(&arySrc[nCount * 16], aryState, 16);
        CopyMemory(aryIV, aryBackup, 16);
        szBuffer[16] = 0;
        strResult += szBuffer;
    }
    memset(m_aryRoundKeys, 0, sizeof(m_aryRoundKeys));
    return TRUE;


}