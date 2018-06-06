// TabASE.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Encode.h"
#include "TabASE.h"
#include "afxdialogex.h"


// CTabASE �Ի���

IMPLEMENT_DYNAMIC(CTabASE, CDialogEx)

CTabASE::CTabASE(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTabASE::IDD, pParent)
    , m_strInput(_T(""))
    , m_strOutput(_T(""))
    , m_strKey(_T(""))
    , m_bImportFromFile(FALSE)
    , m_strImportPath(_T(""))
    , m_strExportFile(_T(""))
    , m_strIV(_T(""))
    , m_nMode(ECB)
{
}

CTabASE::~CTabASE()
{
}

void CTabASE::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_INPUT, m_strInput);
    DDX_Text(pDX, IDC_OUTPUT, m_strOutput);
    DDX_Text(pDX, IDC_KEY, m_strKey);
    DDX_Check(pDX, IDC_CHECK_IMPORT, m_bImportFromFile);
    DDX_Text(pDX, IDC_EDIT_IMPORT_PATH, m_strImportPath);
    DDX_Text(pDX, IDC_EDIT_EXPORT_PATH, m_strExportFile);
    DDX_Text(pDX, IDC_EDIT_IV, m_strIV);
    DDV_MaxChars(pDX, m_strIV, 16);
    DDX_Radio(pDX, IDC_RADIO_ECB, m_nMode);
    DDX_Control(pDX, IDC_ENCRYTP, m_ctrlButtonEncrypt);
    DDX_Control(pDX, IDC_DECRYTP, m_ctrlButtonDecrypt);
}


BEGIN_MESSAGE_MAP(CTabASE, CDialogEx)
    ON_BN_CLICKED(IDOK, &CTabASE::OnBnClickedOk)
    ON_BN_CLICKED(IDC_ENCRYTP, &CTabASE::OnBnClickedEncrytp)
    ON_BN_CLICKED(IDC_DECRYTP, &CTabASE::OnBnClickedDecrytp)
    ON_WM_DROPFILES()
    ON_BN_CLICKED(IDC_BUTTON_OPEN_IMPORT_PATH, &CTabASE::OnBnClickedButtonOpenImportPath)
    ON_BN_CLICKED(IDC_BUTTON_OPEN_EXPORT_PATH, &CTabASE::OnBnClickedButtonOpenExportPath)
    ON_BN_CLICKED(IDC_RADIO_CBC, &CTabASE::OnBnClickedRadioCbc)
    ON_BN_CLICKED(IDC_RADIO_ECB, &CTabASE::OnBnClickedRadioEcb)
    ON_BN_CLICKED(IDC_CHECK_IMPORT, &CTabASE::OnBnClickedCheckImport)
    ON_MESSAGE(WM_FINISH_ENCRYPT, &CTabASE::OnFinishEncrypt)
    ON_MESSAGE(WM_FINISH_DECRYPT, &CTabASE::OnFinishDecrypt)
END_MESSAGE_MAP()


// CTabASE ��Ϣ�������


void CTabASE::OnBnClickedOk()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    //CDialogEx::OnOK();
}


void CTabASE::OnBnClickedEncrytp()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    UpdateData(TRUE);
    BOOL bResult = FALSE;
    
    AfxBeginThread((AFX_THREADPROC)EncryptionThreadFunction, this);

    m_ctrlButtonEncrypt.SetWindowTextW(_T("������..."));
    m_ctrlButtonEncrypt.EnableWindow(FALSE);
}

void CTabASE::OnBnClickedDecrytp()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    UpdateData(TRUE);

    AfxBeginThread((AFX_THREADPROC)DecryptionThreadFunction, this);
    m_ctrlButtonDecrypt.SetWindowTextW(_T("������..."));
    m_ctrlButtonDecrypt.EnableWindow(FALSE);
}


void CTabASE::OnDropFiles(HDROP hDropInfo)
{
    // TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
    UpdateData(TRUE);
    TCHAR szPath[MAX_PATH] = { 0 };
    int nResult = DragQueryFile(hDropInfo, 0, szPath, MAX_PATH);
    if (nResult != 0)
    {
        m_strImportPath = szPath;
    }
    UpdateData(FALSE);
    DragFinish(hDropInfo);
    CDialogEx::OnDropFiles(hDropInfo);
}


void CTabASE::OnBnClickedButtonOpenImportPath()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    UpdateData(TRUE);
    CFileDialog dlgOpen(TRUE);
    dlgOpen.DoModal();
    m_strImportPath = dlgOpen.GetPathName();
    UpdateData(FALSE);
}


void CTabASE::OnBnClickedButtonOpenExportPath()          
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    UpdateData(TRUE);
    CFileDialog dlgSave(FALSE,_T("cpt"));
    dlgSave.DoModal();
    m_strExportFile = dlgSave.GetPathName();
    UpdateData(FALSE);
}


void CTabASE::OnBnClickedRadioCbc()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    m_nMode = CBC;
}


void CTabASE::OnBnClickedRadioEcb()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    m_nMode = ECB;
}


void CTabASE::OnBnClickedCheckImport()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������

}

UINT __cdecl CTabASE::EncryptionThreadFunction(LPVOID pParam)
{
    CTabASE* pTab = reinterpret_cast<CTabASE*>(pParam);

    BOOL bResult = FALSE;
    if (pTab->m_bImportFromFile)
    {
        bResult = pTab->m_AESEncrypt.EncrypteFromFile(pTab->m_strImportPath, pTab->m_strExportFile, pTab->m_strKey, pTab->m_nMode, pTab->m_strIV);
    }
    else
    {
        CString strTemp = pTab->m_strInput;
        bResult = pTab->m_AESEncrypt.EncrypteAsHexString(strTemp, pTab->m_strKey, pTab->m_nMode, pTab->m_strIV);

        if (bResult)
        {
            pTab->m_strOutput = strTemp;
        }
    }

    pTab->SendMessage(WM_FINISH_ENCRYPT, bResult);

    return 0;
}

UINT __cdecl CTabASE::DecryptionThreadFunction(LPVOID pParam)
{
    CTabASE* pTab = reinterpret_cast<CTabASE*>(pParam);
    BOOL bResult = FALSE;
    if (pTab->m_bImportFromFile)
    {
        bResult = pTab->m_AESEncrypt.DecrypteFromFile(pTab->m_strImportPath, pTab->m_strExportFile, pTab->m_strKey, pTab->m_nMode, pTab->m_strIV);
    }
    else
    {
        CString strTemp = pTab->m_strInput;

        if (bResult)
        {
            pTab->m_strOutput = strTemp;
        }
    }

    pTab->SendMessage(WM_FINISH_DECRYPT, bResult);
    
    return 0;
}


afx_msg LRESULT CTabASE::OnFinishEncrypt(WPARAM wParam, LPARAM lParam)
{
    BOOL bResult = wParam;
    if (!bResult)
    {
        AfxMessageBox(_T("��Կ����Ϊ16��14��32"));
    }
    else
    {
        UpdateData(FALSE);
        AfxMessageBox(_T("�������"));
    }

    m_ctrlButtonEncrypt.SetWindowTextW(_T("����"));
    m_ctrlButtonEncrypt.EnableWindow(TRUE);
    
    return 0;
}


afx_msg LRESULT CTabASE::OnFinishDecrypt(WPARAM wParam, LPARAM lParam)
{
    BOOL bResult = wParam;
    if (!bResult)
    {
        AfxMessageBox(_T("��Կ����Ϊ16��14��32"));
    }
    else
    {
        UpdateData(FALSE);
        AfxMessageBox(_T("�������"));
    }

    m_ctrlButtonDecrypt.SetWindowTextW(_T("����"));
    m_ctrlButtonDecrypt.EnableWindow(TRUE);

    return 0;
}
