#pragma once

#include "MD5Encoder.h"

// CTabMD5 �Ի���

class CTabMD5 : public CDialogEx
{
	DECLARE_DYNAMIC(CTabMD5)

public:
	CTabMD5(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTabMD5();

// �Ի�������
	enum { IDD = IDD_MD5_TAB };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    CString m_strInput;
    CString m_strOutput;
    CMD5Encoder m_MD5Encoder;
    BOOL m_bCap;
    BOOL m_bShort;
    afx_msg void OnBnClickedTabMd5Encode();
    int m_nLengthGroup;
    int m_nCapGroup;

//    afx_msg void OnBnClickedRadio1();
//    afx_msg void OnBnClickedRadio2();
    afx_msg void OnClickedRadio32();
    afx_msg void OnBnClickedRadio16();
    afx_msg void OnBnClickedRadioLower();
    afx_msg void OnBnClickedRadioCap();
};
