#pragma once

enum emEncryptMode
{
    ECB,  //�������뱾ģʽ
    CBC   //������ģʽ
};

//aes������
class CAESEncrypt
{
public:                                                                 
    CAESEncrypt();
    virtual ~CAESEncrypt();

    /*�������*/                  
    BOOL EncrypteAsHexString(CString& IN OUT strSrc, CString& IN strKey, int nMode, CString strIV);
    BOOL DecrypteHexString(CString& IN OUT str, CString& IN strKey, int nMode, CString strIV);
    BOOL EncrypteFromFile(CString strSrcFilePath, CString strDstFilePath, CString strKey, int nMode, CString strIV);
    BOOL DecrypteFromFile(CString strSrcFilePath, CString strDstFilePath, CString strKey, int nMode, CString strIV);

    /*��� */
    UINT ZeroPadding(char* IN OUT  strSrc, UINT nLength);
    UINT UnPadding(char* IN OUT  strSrc, UINT nLength);

    /*�������뱾ģʽ����(ECB)*/
    BOOL ECBEncrypt(char* arySrc, int nSrcLen, CStringA IN strKey, char* aryResult);
    /*�������뱾ģʽ����*/
    BOOL ECBDecrypt(CStringA& IN OUT strResult, CStringA IN strKey, char* arySrc, int nSrcLen);
    /**/
    BOOL CBCEncrypt(char* arySrc, int nSrcLen, CStringA IN strKey, char* aryResult, CStringA IN OUT strIV);
    BOOL CBCDecrypt(CStringA& strResult, CStringA IN strKey, CStringA strIV, char* arySrc, int nSrcLen);

    /*��Կ��չ�㷨*/
    BOOL KeyExpantion(char* pUserKey, int nKeyNum, int nRoundNum);
    /*�ֺ���*/
    BOOL Round(char IN OUT aryState[16], int nRoundNum);
    BOOL RevRound(char IN OUT aryState[16], int nRoundNum);

    /*�ֽڴ���*/
    BOOL Subchars(char IN OUT btState[], int nSize, BOOL bRevOrder = FALSE);
    /*���ֽڴ���*/
    BOOL InvSubchars(char IN OUT btState[], int nSize, BOOL bRevOrder = FALSE);

    /*����λ*/
    BOOL ShiftRow(char IN OUT btState[16]);
    /*������λ*/
    BOOL InvShiftRow(char IN OUT btState[16]);
    /*�л��*/
    BOOL MixColumn(char IN OUT btState[16]);
    /*���л��*/
    BOOL InvMixColumn(char IN OUT btState[16]);
    /*����Կ���*/
    BOOL AddRoundKey(char IN OUT btState[16], int nRoundCount, BOOL IsDeCrypt = FALSE);

    /*��������GF(256)�ϵ�����*/
    char GFMul0x2(char btInput);      //����2
    char GFMul0x3(char btInput);      //����3
    char GFMul0x4(char btInput);      //����4
    char GFMul0x8(char btInput);      //����8
    char GFMul0x9(char btInput);      //����9
    char GFMul0xb(char btInput);      //����11
    char GFMul0xc(char btInput);      //����12
    char GFMul0xd(char btInput);      //����13
    char GFMul0xe(char btInput);      //����14
    char GFMul(char n, char s);         //��������GF(256)�ϵ�����
private:
    DWORD m_aryRoundKeys[56];        //����Կ
    static char m_arySBox[256];      //S��
    static char m_ReverseSBox[256];  //��S��
    static char m_MCMatrix[4][4];      //�л�����õ��ĳ�����
    static char m_RevMCMatrix[4][4];   //���л�����õ��ĳ�����
};