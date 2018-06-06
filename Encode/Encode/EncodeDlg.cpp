
// EncodeDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Encode.h"
#include "EncodeDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CEncodeDlg 对话框



CEncodeDlg::CEncodeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEncodeDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    
}

void CEncodeDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_TAB, m_Tab);
}

BEGIN_MESSAGE_MAP(CEncodeDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDOK, &CEncodeDlg::OnBnClickedOk)
//    ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, &CEncodeDlg::OnTcnSelchangeTab)
//ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, &CEncodeDlg::OnTcnSelchangeTab)
ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, &CEncodeDlg::OnSelchangeTab)
ON_WM_DROPFILES()
END_MESSAGE_MAP()


// CEncodeDlg 消息处理程序

BOOL CEncodeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	//ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	//ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			//pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
    InitTabs();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CEncodeDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
		CDialogEx::OnSysCommand(nID, lParam);
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CEncodeDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CEncodeDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CEncodeDlg::OnBnClickedOk()
{
    // TODO:  在此添加控件通知处理程序代码
    //CDialogEx::OnOK();
}



void CEncodeDlg::OnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
{
    // TODO:  在此添加控件通知处理程序代码
    m_pCurrentTab->ShowWindow(SW_HIDE);

    int nIndex = m_Tab.GetCurSel();
    m_pCurrentTab = m_pTabs[nIndex];
    m_pCurrentTab->ShowWindow(SW_SHOW);
    m_pCurrentTab->BringWindowToTop();
    *pResult = 0;
}

BOOL CEncodeDlg::InitTabs()
{
    m_Tab.InsertItem(0, _T("BaseN编码"));
    m_Tab.InsertItem(1, _T("MD5编码"));
    m_Tab.InsertItem(2, _T("AES"));

    m_TabBaseN.Create(IDD_BASEN_TAB, this);
    m_TabMD5.Create(IDD_MD5_TAB, this);
    m_TabAES.Create(IDD_AES_TAB, this);

    m_pTabs[0] = &m_TabBaseN;
    m_pTabs[1] = &m_TabMD5;
    m_pTabs[2] = &m_TabAES;

    CRect rect;
    m_Tab.GetWindowRect(&rect);
    m_Tab.SetCurSel(0);
    rect.top -= 20;
    rect.left -= 15;


    for (int nCount = 0; nCount < TAB_NUMBER; nCount++)
    {
        m_pTabs[nCount]->MoveWindow(rect, TRUE);
    }

    m_pCurrentTab = m_pTabs[0];
    m_pCurrentTab->ShowWindow(SW_SHOW);

    return TRUE;
}

void CEncodeDlg::OnDropFiles(HDROP hDropInfo)
{
    // TODO:  在此添加消息处理程序代码和/或调用默认值

    CDialogEx::OnDropFiles(hDropInfo);
}
