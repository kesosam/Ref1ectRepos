#pragma once

enum emEncryptMode
{
    ECB,  //电子密码本模式
    CBC   //区块链模式
};

//aes加密类
class CAESEncrypt
{
public:                                                                 
    CAESEncrypt();
    virtual ~CAESEncrypt();

    /*输入输出*/                  
    BOOL EncrypteAsHexString(CString& IN OUT strSrc, CString& IN strKey, int nMode, CString strIV);
    BOOL DecrypteHexString(CString& IN OUT str, CString& IN strKey, int nMode, CString strIV);
    BOOL EncrypteFromFile(CString strSrcFilePath, CString strDstFilePath, CString strKey, int nMode, CString strIV);
    BOOL DecrypteFromFile(CString strSrcFilePath, CString strDstFilePath, CString strKey, int nMode, CString strIV);

    /*填充 */
    UINT ZeroPadding(char* IN OUT  strSrc, UINT nLength);
    UINT UnPadding(char* IN OUT  strSrc, UINT nLength);

    /*电子密码本模式加密(ECB)*/
    BOOL ECBEncrypt(char* arySrc, int nSrcLen, CStringA IN strKey, char* aryResult);
    /*电子密码本模式解密*/
    BOOL ECBDecrypt(CStringA& IN OUT strResult, CStringA IN strKey, char* arySrc, int nSrcLen);
    /**/
    BOOL CBCEncrypt(char* arySrc, int nSrcLen, CStringA IN strKey, char* aryResult, CStringA IN OUT strIV);
    BOOL CBCDecrypt(CStringA& strResult, CStringA IN strKey, CStringA strIV, char* arySrc, int nSrcLen);

    /*密钥扩展算法*/
    BOOL KeyExpantion(char* pUserKey, int nKeyNum, int nRoundNum);
    /*轮函数*/
    BOOL Round(char IN OUT aryState[16], int nRoundNum);
    BOOL RevRound(char IN OUT aryState[16], int nRoundNum);

    /*字节代换*/
    BOOL Subchars(char IN OUT btState[], int nSize, BOOL bRevOrder = FALSE);
    /*逆字节代换*/
    BOOL InvSubchars(char IN OUT btState[], int nSize, BOOL bRevOrder = FALSE);

    /*行移位*/
    BOOL ShiftRow(char IN OUT btState[16]);
    /*逆行移位*/
    BOOL InvShiftRow(char IN OUT btState[16]);
    /*列混合*/
    BOOL MixColumn(char IN OUT btState[16]);
    /*逆列混合*/
    BOOL InvMixColumn(char IN OUT btState[16]);
    /*轮密钥相加*/
    BOOL AddRoundKey(char IN OUT btState[16], int nRoundCount, BOOL IsDeCrypt = FALSE);

    /*在有限域GF(256)上的运算*/
    char GFMul0x2(char btInput);      //乘以2
    char GFMul0x3(char btInput);      //乘以3
    char GFMul0x4(char btInput);      //乘以4
    char GFMul0x8(char btInput);      //乘以8
    char GFMul0x9(char btInput);      //乘以9
    char GFMul0xb(char btInput);      //乘以11
    char GFMul0xc(char btInput);      //乘以12
    char GFMul0xd(char btInput);      //乘以13
    char GFMul0xe(char btInput);      //乘以14
    char GFMul(char n, char s);         //在有限域GF(256)上的运算
private:
    DWORD m_aryRoundKeys[56];        //轮密钥
    static char m_arySBox[256];      //S盒
    static char m_ReverseSBox[256];  //逆S盒
    static char m_MCMatrix[4][4];      //列混合中用到的常矩阵
    static char m_RevMCMatrix[4][4];   //逆列混合中用到的常矩阵
};