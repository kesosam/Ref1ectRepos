#pragma once

#include "resource.h"
#include "stdafx.h"

class CSpider
{
public:
    CSpider();
    ~CSpider();

    BOOL Init();
    BOOL ReadConfigFile();
    BOOL Crawl();

    BOOL Start();

    static DWORD TimerThreadProc(_In_ LPVOID lpParameter);

    int m_nWaitTime;
    HANDLE m_hEventTimeUp;
    HANDLE m_hEventEndThread;
    CTime m_LastCrawlTime;
private:
    CStringA m_strDestUrl;
    CStringA m_strSaveFileName;
    CStringA m_strIniFileName;
    
};
