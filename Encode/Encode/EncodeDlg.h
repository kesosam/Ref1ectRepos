
// EncodeDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "TabBaseN.h"
#include "TabMD5.h"
#include "TabASE.h"

#define TAB_NUMBER 3

// CEncodeDlg �Ի���
class CEncodeDlg : public CDialogEx
{
// ����
public:
	CEncodeDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_ENCODE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;
    CWnd* m_pTabs[10];
    CWnd* m_pCurrentTab;

    CTabBaseN m_TabBaseN;
    CTabMD5 m_TabMD5;
    CTabASE m_TabAES;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedOk();
//    afx_msg void OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult);
    CTabCtrl m_Tab;
//    afx_msg void OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult);
    BOOL CEncodeDlg::InitTabs();
    afx_msg void OnDropFiles(HDROP hDropInfo);
};
