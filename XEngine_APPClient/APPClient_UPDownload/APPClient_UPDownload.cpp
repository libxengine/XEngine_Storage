#ifdef _WINDOWS
#include <windows.h>
#include <tchar.h>
#pragma comment(lib,"XEngine_BaseLib/XEngine_BaseLib")
#pragma comment(lib,"XEngine_Core/XEngine_OPenSsl")
#pragma comment(lib,"XEngine_NetHelp/NetHelp_APIHelp")
#pragma comment(lib,"Ws2_32")
#else
#endif
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <list>
#include <XEngine_Include/XEngine_CommHdr.h>
#include <XEngine_Include/XEngine_Types.h>
#include <XEngine_Include/XEngine_ProtocolHdr.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Define.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Error.h>
#include <XEngine_Include/XEngine_Core/OPenSsl_Define.h>
#include <XEngine_Include/XEngine_Core/OPenSsl_Error.h>
#include <XEngine_Include/XEngine_NetHelp/APIHelp_Define.h>
#include <XEngine_Include/XEngine_NetHelp/APIHelp_Error.h>
using namespace std;

//需要优先配置XEngine
//WINDOWS使用VS2022 x86 debug 编译
//linux使用下面的命令编译
//g++ -std=c++17 -Wall -g APPClient_UPDownload.cpp -o APPClient_UPDownload.exe -L /usr/local/lib/XEngine_Release/XEngine_BaseLib -L /usr/local/lib/XEngine_Release/XEngine_Core -L /usr/local/lib/XEngine_Release/XEngine_NetHelp -lXEngine_BaseLib -lXEngine_OPenSsl -lNetHelp_APIHelp

//上传文件
void File_UPLoad()
{
	LPCTSTR lpszUrl = _T("http://192.168.1.8:5102/2.txt");
	int nLen = 0;
	int nCode = 0;
	TCHAR* ptszMsgBuffer = NULL;
	TCHAR tszBaseBuffer[128];
	TCHAR tszHdrBuffer[MAX_PATH];
	LPCTSTR lpszMsgBuffer = _T("01234");
	LPCTSTR lpszMsgBuffer2 = _T("56789");

	memset(tszBaseBuffer, '\0', sizeof(tszBaseBuffer));
	memset(tszHdrBuffer, '\0', MAX_PATH);
	OPenSsl_Help_BasicEncoder("123123aa", "123123", tszBaseBuffer);

	_stprintf(tszHdrBuffer, _T("Range: bytes=0-5/10\r\nAuthorization: %s\r\n"), tszBaseBuffer);
	if (!APIHelp_HttpRequest_Post(lpszUrl, lpszMsgBuffer, &nCode, &ptszMsgBuffer, &nLen, tszHdrBuffer))
	{
		printf("upload failed:%lX\n", APIHelp_GetLastError());
		return;
	}
	printf("upload:%d\n", nCode);
	BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ptszMsgBuffer);

	memset(tszHdrBuffer, '\0', MAX_PATH);
	_stprintf(tszHdrBuffer, _T("Range: bytes=5-9/10\r\nAuthorization: %s\r\n"), tszBaseBuffer);
	if (!APIHelp_HttpRequest_Post(lpszUrl, lpszMsgBuffer2, &nCode, &ptszMsgBuffer, &nLen, tszHdrBuffer))
	{
		printf("upload failed:%lX\n", APIHelp_GetLastError());
		return;
	}
	printf("upload:%d\n%s\n", nCode, ptszMsgBuffer);
	BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
}
//下载文件
void File_Download()
{
	LPCTSTR lpszUrl = _T("http://192.168.1.8:5101/storagekey2/2.txt");

	int nLen = 0;
	int nCode = 0;
	TCHAR* ptszMsgBuffer = NULL;
	TCHAR tszBaseBuffer[128];
	TCHAR tszHdrBuffer[MAX_PATH];

	memset(tszBaseBuffer, '\0', sizeof(tszBaseBuffer));
	memset(tszHdrBuffer, '\0', MAX_PATH);
	OPenSsl_Help_BasicEncoder("123123aa", "123123", tszBaseBuffer);

	_stprintf(tszHdrBuffer, _T("Range: bytes=0-5\r\nAuthorization: %s\r\n"), tszBaseBuffer);
	if (!APIHelp_HttpRequest_Get(lpszUrl, &ptszMsgBuffer, &nLen, &nCode, tszHdrBuffer))
	{
		printf("download failed:%lX\n", APIHelp_GetLastError());
		return;
	}
	printf("download:%d,%s\n", nLen, ptszMsgBuffer);
	BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ptszMsgBuffer);

	memset(tszHdrBuffer, '\0', MAX_PATH);
	_stprintf(tszHdrBuffer, _T("Range: bytes=5-10\r\nAuthorization: %s\r\n"), tszBaseBuffer);
	if (!APIHelp_HttpRequest_Get(lpszUrl, &ptszMsgBuffer, &nLen, &nCode, tszHdrBuffer))
	{
		printf("download failed:%lX\n", APIHelp_GetLastError());
		return;
	}
	printf("download:%d,%s\n", nLen, ptszMsgBuffer);
	BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
}

int main()
{
#ifdef _WINDOWS
	WSADATA st_WSAData;
	WSAStartup(MAKEWORD(2, 2), &st_WSAData);
#endif

	File_UPLoad();
	File_Download();
	
#ifdef _WINDOWS
	WSACleanup();
#endif
	return 0;
}