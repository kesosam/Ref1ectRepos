// TumblrLogin.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "TumblrLogin.h"
#include "SkyChaserHttp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define ERRO_NEED_CAPTCHE -1

// 唯一的应用程序对象

CWinApp theApp;

using namespace std;

HRESULT LogIn(string& strUserName, string& strPassword, CSkyChaserHttp& http)
{
	string strRet;
    string strCheck;
	http.sc_get("https://www.tumblr.com/login", strRet);

    CSkyChaserHttp::sc_getMidString(strRet, strCheck,
        "<li class=\"",
        "\"",
        0);
    if (strCheck == "post_container" || strCheck == "notification single_notification alt takeover-container")
    {
        return S_OK;
    }

	string strFormKey;
	CSkyChaserHttp::sc_getMidString(strRet, strFormKey,
		"id=\"tumblr_form_key\" content=\"",
		"\"",
		0);

	CSkyChaserHttp::sc_urlEncodeAnsi(strUserName, false);
	CSkyChaserHttp::sc_urlEncodeAnsi(strFormKey, false);


	CStringA strPostData;
	strPostData.Format("determine_email=%s&user%%5Bemail%%5D=%s&user%%5Bpassword%%5D=%s&tumblelog%%5Bname%%5D=&user%%5Bage%%5D=&context=no_referer&version=STANDARD&follow=&form_key=%s&seen_suggestion=0&used_suggestion=0&used_auto_suggestion=0&about_tumblr_slide=&random_username_suggestions=%%5B%%22SaltyHologramStudent%%22%%2C%%22ImpossibleCloudCollection%%22%2C%%22SecretArcadeStudent%%22%%2C%%22ValiantlyBeardedBouquet%%22%2C%%22SecretBlizzardStudent%%22%%5D&action=signup_determine",
		strUserName.c_str(),
		strUserName.c_str(),
		strPassword.c_str(),
		strFormKey.c_str());

	strRet.clear();
	http.sc_post("https://www.tumblr.com/login", strPostData.GetString(), strRet);


    strCheck.clear();
	CSkyChaserHttp::sc_getMidString(strRet, strCheck,
		"<head>\n        <title>",
		"</title>",
		0);
	if (strCheck == "Request denied.")
	{
		throw ERROR_ACCESS_DENIED;
	}
	
	strCheck.clear();
	CSkyChaserHttp::sc_getMidString(strRet, strCheck,
		"Tumblr.RegistrationForm.errors = [\"",
		"\"]",
		0);
	//用户名或密码错误
	if (strCheck == "Your email or password were incorrect.")
	{
		throw ERROR_LOGON_FAILURE;
	}

	//需要输入验证码
	if (strCheck == "Don't forget to fill out the Captcha!")
	{
		throw ERRO_NEED_CAPTCHE;
	}

	strCheck.clear();
	CSkyChaserHttp::sc_getMidString(strRet, strCheck,
        "<li class=\"",
		"\"",
		0);
    if (strCheck == "post_container" || strCheck == "notification single_notification alt takeover-container")
	{
		return S_OK;
	}
	
	
	throw E_FAIL;
}


HRESULT ChangePassword(string& strPassword, CSkyChaserHttp& http, string& strNewPassword)
{
    string strRet;
    string strCheck;
    string strID;
    http.sc_get("https://www.tumblr.com/settings/account", strRet);
    
    string strFormKey;
    CSkyChaserHttp::sc_getMidString(strRet, strFormKey,
        "id=\"tumblr_form_key\" content=\"",
        "\"",
        0);

    CSkyChaserHttp::sc_getMidString(strRet, strID,
        "href=\"/settings/blog/",
        "\"",
        0);

    CStringA strXTumblrFormKey;
    strXTumblrFormKey.Format("X-tumblr-form-key:%s", strFormKey.c_str());
    http.sc_appendHeader(strXTumblrFormKey.GetString());

    strRet.clear();
    http.sc_post("https://www.tumblr.com/svc/secure_form_key", "", strRet,true);

    string strSecureFormKey;
    CSkyChaserHttp::sc_getMidString(strRet, strSecureFormKey,
        "X-Tumblr-Secure-Form-Key:",
        "\r\n",
        0);

    CStringA strXTumblrPuppies;
    strXTumblrPuppies.Format("X-tumblr-puppies:%s", strSecureFormKey.c_str());

    /*strRet.clear();
    http.sc_post("https://www.tumblr.com/settings/save", NULL, strRet);

    strRet.clear();
    http.sc_post("https://www.tumblr.com/settings/save", NULL, strRet);*/

    http.sc_deleteHeader("Content-Type: application/x-www-form-urlencoded; charset=UTF-8");
    http.sc_appendHeader("Content-Type: application/json");
    http.sc_appendHeader(strXTumblrPuppies.GetString());
    http.sc_appendHeader("Referer: https://www.tumblr.com/settings/account");

    CStringA strPostData;
    strPostData.Format("{\"form_key\":\"%s\",\"page\":\"account\",\"tumblelog_name_or_id\":\"%s\",\"device\":\"other\",\"is_safemode_enabled\":\"1\",\"is_mobile\":\"\",\"password[current]\":\"%s\",\"password[new]\":\"%s\",\"password[confirm]\":\"%s\"}",
        strFormKey.c_str(),
        strID.c_str(),
        strPassword.c_str(),
        strNewPassword.c_str(),
        strNewPassword.c_str());

    
    strRet.clear();
    http.sc_post("https://www.tumblr.com/settings/save", strPostData.GetString(), strRet);


    CSkyChaserHttp::sc_getMidString(strRet, strCheck,
        "\"response\":\"",
        "\"",
        0);
    if (strCheck == "Saved")
    {
        return S_OK;
    }
    throw E_FAIL;
}

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	HMODULE hModule = ::GetModuleHandle(NULL);

	if (hModule != NULL)
	{
		// 初始化 MFC 并在失败时显示错误
		if (!AfxWinInit(hModule, NULL, ::GetCommandLine(), 0))
		{
			// TODO:  更改错误代码以符合您的需要
			_tprintf(_T("错误:  MFC 初始化失败\n"));
			nRetCode = 1;
		}
		else
		{
			// TODO:  在此处为应用程序的行为编写代码。

			string strUserName = "holynonly@naver.com";
			string strPassWord = "kwc6594112";
            string strNewPassWord = "test6594112";

            //string strPassWord = "test6594112";
            //string strNewPassWord =  "kwc6594112";

			CSkyChaserHttp http;
			http.sc_setCookieFile("Cookie.txt");
			http.sc_setProxy("127.0.0.1:8080");
			http.sc_appendHeader("Accept: */*");
			http.sc_appendHeader("Connection: close");
			http.sc_appendHeader("Content-Type: application/x-www-form-urlencoded; charset=UTF-8");
			http.sc_appendHeader("X-Requested-With: XMLHttpRequest");

			try
			{
				LogIn(strUserName, strPassWord, http);
				printf("登陆成功\r\n");
                ChangePassword(strPassWord, http, strNewPassWord);
                printf("密码修改成功\r\n");
			}
			catch (HRESULT hResult)
			{
				LPVOID lpMsgBuf;
				FormatMessage(
					FORMAT_MESSAGE_ALLOCATE_BUFFER |
					FORMAT_MESSAGE_FROM_SYSTEM |
					FORMAT_MESSAGE_IGNORE_INSERTS,
					NULL,
					hResult,
					0, // Default language
					(LPTSTR)&lpMsgBuf,
					0,
					NULL
					);
				// Process any inserts in lpMsgBuf.
				// ...
				// Display the string.
				printf("失败:%s", CW2A((LPTSTR)lpMsgBuf));
				// Free the buffer.
				LocalFree(lpMsgBuf);
				
			}
			catch (int e)
			{
				if (e == -1)
				{
					printf("失败:需要输入验证码\r\n");
				}
			}
		}
	}
	else
	{
		// TODO:  更改错误代码以符合您的需要
		_tprintf(_T("错误:  GetModuleHandle 失败\n"));
		nRetCode = 1;
	}
	system("pause");
	return nRetCode;
}
