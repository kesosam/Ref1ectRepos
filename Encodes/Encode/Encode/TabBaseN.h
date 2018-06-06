#pragma once

#include "Base32Encoder.h"
#include "Base64Encoder.h"
#include "afxwin.h"

// CTabBaseN �Ի���

class CTabBaseN : public CDialogEx
{
	DECLARE_DYNAMIC(CTabBaseN)

public:
	CTabBaseN(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTabBaseN();

// �Ի�������
	enum { IDD = IDD_BASEN_TAB };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnSelectBase32();
    afx_msg void OnSelectBase64();
    CString m_strInput;
    CString m_strOutput;
    afx_msg void OnBnClickedBasenEncode();
    afx_msg void OnBnClickedBasenDecode();
    CBase32Encoder m_base32;
    CBase64Encoder m_base64;
    IEncoder* m_pCurentEecode;
    virtual BOOL OnInitDialog();
    int m_nRadioGroup;
};
