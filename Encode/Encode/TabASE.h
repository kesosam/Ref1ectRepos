#pragma once
#include "resource.h"
#include "AESEncrypt.h"
#include "Base64Encoder.h"
#include "afxwin.h"

#define WM_FINISH_ENCRYPT  WM_USER + 1
#define WM_FINISH_DECRYPT  WM_USER + 2

// CTabASE 对话框

class CTabASE : public CDialogEx
{
	DECLARE_DYNAMIC(CTabASE)

public:
	CTabASE(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTabASE();

// 对话框数据
	enum { IDD = IDD_AES_TAB };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedOk();
    CString m_strInput;
    CString m_strOutput;
    CString m_strKey;
    CAESEncrypt m_AESEncrypt;
    afx_msg void OnBnClickedEncrytp();
    afx_msg void OnBnClickedDecrytp();
    BOOL m_bImportFromFile;
    afx_msg void OnDropFiles(HDROP hDropInfo);
    CString m_strImportPath;
    CString m_strExportFile;
    CString m_strIV;
    afx_msg void OnBnClickedButtonOpenImportPath();
    afx_msg void OnBnClickedButtonOpenExportPath();
    BOOL m_nMode;

    afx_msg void OnBnClickedRadioCbc();
    afx_msg void OnBnClickedRadioEcb();
    afx_msg void OnBnClickedCheckImport();

    static UINT __cdecl EncryptionThreadFunction(LPVOID pParam);
    static UINT __cdecl DecryptionThreadFunction(LPVOID pParam);

protected:
    afx_msg LRESULT OnFinishEncrypt(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnFinishDecrypt(WPARAM wParam, LPARAM lParam);
public:
    CButton m_ctrlButtonEncrypt;
    CButton m_ctrlButtonDecrypt;
};
