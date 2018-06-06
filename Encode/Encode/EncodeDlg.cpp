
// EncodeDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Encode.h"
#include "EncodeDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CEncodeDlg �Ի���



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


// CEncodeDlg ��Ϣ�������

BOOL CEncodeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
    InitTabs();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CEncodeDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
		CDialogEx::OnSysCommand(nID, lParam);
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CEncodeDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CEncodeDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CEncodeDlg::OnBnClickedOk()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    //CDialogEx::OnOK();
}



void CEncodeDlg::OnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    m_pCurrentTab->ShowWindow(SW_HIDE);

    int nIndex = m_Tab.GetCurSel();
    m_pCurrentTab = m_pTabs[nIndex];
    m_pCurrentTab->ShowWindow(SW_SHOW);
    m_pCurrentTab->BringWindowToTop();
    *pResult = 0;
}

BOOL CEncodeDlg::InitTabs()
{
    m_Tab.InsertItem(0, _T("BaseN����"));
    m_Tab.InsertItem(1, _T("MD5����"));
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
    // TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ

    CDialogEx::OnDropFiles(hDropInfo);
}
