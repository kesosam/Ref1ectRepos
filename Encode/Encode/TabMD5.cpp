// TabMD5.cpp : 实现文件
//

#include "stdafx.h"
#include "Encode.h"
#include "TabMD5.h"
#include "afxdialogex.h"


// CTabMD5 对话框

IMPLEMENT_DYNAMIC(CTabMD5, CDialogEx)

CTabMD5::CTabMD5(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTabMD5::IDD, pParent)
    , m_strInput(_T(""))
    , m_strOutput(_T(""))
    , m_bShort(FALSE)
    , m_bCap(FALSE)
    , m_nLengthGroup(0)
    , m_nCapGroup(0)
{

}

CTabMD5::~CTabMD5()
{
}

void CTabMD5::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_TAB_MD5_INPUT, m_strInput);
    DDX_Text(pDX, IDC_TAB_MD5_OUTPUT, m_strOutput);
    DDX_Radio(pDX, IDC_RADIO_32, m_nLengthGroup);
    DDX_Radio(pDX, IDC_RADIO_LOWER, m_nCapGroup);
}


BEGIN_MESSAGE_MAP(CTabMD5, CDialogEx)
    ON_BN_CLICKED(IDC_TAB_MD5_ENCODE, &CTabMD5::OnBnClickedTabMd5Encode)
    ON_BN_CLICKED(IDC_RADIO_32, &CTabMD5::OnClickedRadio32)
    ON_BN_CLICKED(IDC_RADIO_16, &CTabMD5::OnBnClickedRadio16)
    ON_BN_CLICKED(IDC_RADIO_LOWER, &CTabMD5::OnBnClickedRadioLower)
    ON_BN_CLICKED(IDC_RADIO_CAP, &CTabMD5::OnBnClickedRadioCap)
END_MESSAGE_MAP()


// CTabMD5 消息处理程序


void CTabMD5::OnBnClickedTabMd5Encode()
{
    // TODO:  在此添加控件通知处理程序代码
    UpdateData(TRUE);
    m_strOutput = m_strInput;

    m_MD5Encoder.Encode(m_strOutput);

    if (m_bShort)
    {
        m_strOutput = m_strOutput.Mid(8,16);
    }

    if (m_bCap)
    {
        m_strOutput.MakeUpper();
    }

    UpdateData(FALSE);
}


//void CTabMD5::OnBnClickedRadio1()
//{
//    // TODO:  在此添加控件通知处理程序代码
//    m_bShort = FALSE;
//}


//void CTabMD5::OnBnClickedRadio2()
//{
//    // TODO:  在此添加控件通知处理程序代码
//}


void CTabMD5::OnClickedRadio32()
{
    // TODO:  在此添加控件通知处理程序代码
    m_nLengthGroup = 0;
    m_bShort = FALSE;
}


void CTabMD5::OnBnClickedRadio16()
{
    // TODO:  在此添加控件通知处理程序代码
    m_nLengthGroup = 1;
    m_bShort = TRUE;
}


void CTabMD5::OnBnClickedRadioLower()
{
    // TODO:  在此添加控件通知处理程序代码
    m_nCapGroup = 0;
    m_bCap = FALSE;
}


void CTabMD5::OnBnClickedRadioCap()
{
    // TODO:  在此添加控件通知处理程序代码
    m_nCapGroup = 1;
    m_bCap = TRUE;
}
