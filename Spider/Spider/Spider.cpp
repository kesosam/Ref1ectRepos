#include "stdafx.h"
#include "Spider.h"
#include "SkyChaserHttp.h"

CSpider::CSpider()
{
}


CSpider::~CSpider()
{
}

BOOL CSpider::ReadConfigFile()
{
    char szBuffer[1024] = { 0 };

    memset(szBuffer, 0, 1024);
    GetPrivateProfileStringA("CrawlSetting", "Url", NULL, szBuffer, 1024, m_strIniFileName);
    m_strDestUrl = szBuffer;
    if (m_strDestUrl.IsEmpty())
    {
        printf("[URL未设置]");
        return FALSE;
    }

    m_nWaitTime = GetPrivateProfileIntA("CrawlSetting", "WaitTime(Minutes)", 30, m_strIniFileName);

    memset(szBuffer, 0, 1024);
    char szDefaultFile[] = "Result.txt";
    GetPrivateProfileStringA("CrawlSetting", "WriteFile", szDefaultFile, szBuffer, 1024, m_strIniFileName);
    m_strSaveFileName = szBuffer;

    return TRUE;
}

BOOL CSpider::Crawl()
{
    BOOL bFlussh = TRUE;
    CSkyChaserHttp http;
    string strResult;
    http.sc_appendHeader("Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8");
    http.sc_appendHeader("Connection: keep-alive");
    http.sc_appendHeader("Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/67.0.3396.62 Safari/537.36");

    CURLcode ResultCode = http.sc_get(m_strDestUrl.GetString(), strResult);
    if (ResultCode != 0)
    {
        printf("[错误码:%d]", ResultCode);
        return FALSE;
    }

    CFile file;
    if (!file.Open(CA2W(m_strSaveFileName), CFile::modeReadWrite))
    {
        file.Open(CA2W(m_strSaveFileName), CFile::modeCreate | CFile::modeReadWrite);
    }

    file.SeekToEnd();
    file.Write(strResult.c_str(), strResult.length());
    file.Flush();
    file.Close();
    m_LastCrawlTime = CTime::GetCurrentTime();
    printf("[%d/%d/%d %02d:%02d:%02d]成功抓取数据\r\n", 
            m_LastCrawlTime.GetYear(), 
            m_LastCrawlTime.GetMonth(),
            m_LastCrawlTime.GetDay(),
            m_LastCrawlTime.GetHour(),
            m_LastCrawlTime.GetMinute(),
            m_LastCrawlTime.GetSecond());
    return TRUE;
}

BOOL CSpider::Init()
{
    char szBuffer[MAX_PATH] = { 0 };
    GetModuleFileNameA(NULL, szBuffer, 1024);
    m_strIniFileName = szBuffer;
    int n = m_strIniFileName.ReverseFind('\\');
    m_strIniFileName = m_strIniFileName.Left(m_strIniFileName.ReverseFind('\\')+1);
    m_strIniFileName += "Config.ini";

    m_hEventTimeUp = CreateEvent(NULL, FALSE, FALSE, NULL);
    m_hEventEndThread = CreateEvent(NULL, FALSE, FALSE, NULL);
    CFile file;
    if (!file.Open(_T("Config.ini"), CFile::modeRead))
    {
        file.Open(_T("Config.ini"), CFile::modeCreate);
        file.Close();

        WritePrivateProfileStringA("CrawlSetting", "WriteFile", "Result.txt", m_strIniFileName);
        WritePrivateProfileStringA("CrawlSetting", "Url", "http://176871203135005101.standard.hutoudaili.com/?num=500", m_strIniFileName);
        WritePrivateProfileStringA("CrawlSetting", "WaitTime(Minutes)", "30", m_strIniFileName);
    }
    else
    {
        file.Close();
    }
    return TRUE;
}

DWORD CSpider::TimerThreadProc(_In_ LPVOID lpParameter)
{
    CSpider* pThis = reinterpret_cast<CSpider*>(lpParameter);
    CTime CurrentTime;
    int nResult = 0;
    while (TRUE)
    {
        nResult = WaitForSingleObject(pThis->m_hEventEndThread, 0);
        if (nResult == WAIT_OBJECT_0)
        {
            break;
        }

        CurrentTime = CTime::GetCurrentTime();
        CTimeSpan TimeSpin;
        TimeSpin = CurrentTime - pThis->m_LastCrawlTime;
        if (TimeSpin.GetMinutes() >= pThis->m_nWaitTime)
        {
            SetEvent(pThis->m_hEventTimeUp);
        }
        Sleep(10000);
    }
    return 0;
}

BOOL CSpider::Start()
{
    Init();

    AfxBeginThread((AFX_THREADPROC)TimerThreadProc, this);

    while (TRUE)
    {
        WaitForSingleObject(m_hEventTimeUp, INFINITE);
        if (!ReadConfigFile())
        {
            printf("读取配置文件失败\r\n");
            SetEvent(m_hEventEndThread);
            return FALSE;
        }
        
        if (!Crawl())
        {
            printf("抓取失败\r\n");
            continue;
        }
    }
    return TRUE;
}

