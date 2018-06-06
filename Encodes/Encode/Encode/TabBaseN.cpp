// TabBaseN.cpp : 实现文件
//

#include "stdafx.h"
#include "Encode.h"
#include "TabBaseN.h"
#include "afxdialogex.h"


// CTabBaseN 对话框

IMPLEMENT_DYNAMIC(CTabBaseN, CDialogEx)

CTabBaseN::CTabBaseN(CWnd* pParent /*=NULL*/)
            : CDialogEx(CTabBaseN::IDD, pParent)
            , m_strInput(_T(""))
            , m_strOutput(_T(""))
            , m_pCurentEecode(&m_base64)
            , m_nRadioGroup(0)
{

}

CTabBaseN::~CTabBaseN()
{
}

void CTabBaseN::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_BASEN_INPUT, m_strInput);
    DDX_Text(pDX, IDC_BASEN_OUTPUT, m_strOutput);
    DDX_Radio(pDX, IDC_SELECT_BASE64, m_nRadioGroup);
}


BEGIN_MESSAGE_MAP(CTabBaseN, CDialogEx)
    ON_COMMAND(IDC_SELECT_BASE32, &CTabBaseN::OnSelectBase32)
    ON_COMMAND(IDC_SELECT_BASE64, &CTabBaseN::OnSelectBase64)
    ON_BN_CLICKED(IDC_BASEN_ENCODE, &CTabBaseN::OnBnClickedBasenEncode)
    ON_BN_CLICKED(IDC_BASEN_DECODE, &CTabBaseN::OnBnClickedBasenDecode)
END_MESSAGE_MAP()


// CTabBaseN 消息处理程序


void CTabBaseN::OnSelectBase32()
{
    // TODO:  在此添加命令处理程序代码
    m_pCurentEecode = &m_base32;
}


void CTabBaseN::OnSelectBase64()
{
    // TODO:  在此添加命令处理程序代码
    m_pCurentEecode = &m_base64;
}


void CTabBaseN::OnBnClickedBasenEncode()
{
    UpdateData(TRUE);
    m_strOutput = m_strInput;

    BOOL bResult = m_pCurentEecode->Encode(m_strOutput);
    if (bResult == FALSE)
    {
        m_strOutput = _T("输入无效!");
    }
    UpdateData(FALSE);
}


void CTabBaseN::OnBnClickedBasenDecode()
{
    // TODO:  在此添加控件通知处理程序代码
    UpdateData(TRUE);
    m_strOutput = m_strInput;

    BOOL bResult = m_pCurentEecode->Decode(m_strOutput);
    if (bResult == FALSE)
    {
        m_strOutput = _T("输入无效!");
    }

    UpdateData(FALSE);
}


BOOL CTabBaseN::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  在此添加额外的初始化

    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常:  OCX 属性页应返回 FALSE
}
