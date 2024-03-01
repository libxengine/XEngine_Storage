#ifdef _MSC_BUILD
#include <windows.h>
#include <tchar.h>
#pragma comment(lib,"XEngine_BaseLib/XEngine_BaseLib")
#pragma comment(lib,"XEngine_Core/XEngine_OPenSsl")
#pragma comment(lib,"XEngine_Client/XClient_APIHelp")
#pragma comment(lib,"Ws2_32")
#pragma comment(lib,"../../XEngine_Source/Debug/jsoncpp")
#ifdef _WIN64
#pragma comment(lib,"../../XEngine_Source/x64/Debug/jsoncpp")
#else
#pragma comment(lib,"../../XEngine_Source/Debug/jsoncpp")
#endif
#endif
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <list>
#include <json/json.h>
#include <XEngine_Include/XEngine_CommHdr.h>
#include <XEngine_Include/XEngine_Types.h>
#include <XEngine_Include/XEngine_ProtocolHdr.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Define.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Error.h>
#include <XEngine_Include/XEngine_Core/OPenSsl_Define.h>
#include <XEngine_Include/XEngine_Core/OPenSsl_Error.h>
#include <XEngine_Include/XEngine_Client/APIClient_Define.h>
#include <XEngine_Include/XEngine_Client/APIClient_Error.h>
using namespace std;

//需要优先配置XEngine
//WINDOWS使用VS2022 x86 或者 x64 debug 编译
//linux macos::g++ -std=c++17 -Wall -g APPClient_UPDownload.cpp -o APPClient_UPDownload.exe -I ../../XEngine_Source/XEngine_ThirdPart/jsoncpp -L ../../XEngine_Source/XEngine_ThirdPart/jsoncpp -lXEngine_BaseLib -lXEngine_OPenSsl -lXClient_APIHelp -ljsoncpp


//上传文件
void File_UPLoad()
{
	LPCXSTR lpszUrl = _X("http://127.0.0.1:5102/api?filename=newfile4.txt&storeagekey=storagekey2");
	int nLen = 0;
	int nCode = 0;
	XCHAR* ptszMsgBuffer = NULL;
	XCHAR tszBaseBuffer[128];
	XCHAR tszHdrBuffer[MAX_PATH];
	XCHAR tszKeyBuffer[MAX_PATH];
	LPCXSTR lpszMsgBuffer = _X("01234");
	LPCXSTR lpszMsgBuffer2 = _X("56789");

	memset(tszBaseBuffer, '\0', sizeof(tszBaseBuffer));
	memset(tszHdrBuffer, '\0', MAX_PATH);
	memset(tszKeyBuffer, '\0', MAX_PATH);
	OPenSsl_Help_BasicEncoder("123123aa", "123123", tszBaseBuffer);

	_xstprintf(tszHdrBuffer, _X("Range: bytes=0-5/10\r\nAuthorization: %s\r\n"), tszBaseBuffer);
	if (!APIClient_Http_Request(_X("POST"), lpszUrl, lpszMsgBuffer, &nCode, &ptszMsgBuffer, &nLen, tszHdrBuffer))
	{
		_xtprintf("upload failed:%lX\n", APIClient_GetLastError());
		return;
	}
	JSONCPP_STRING st_JsonError;
	Json::Value st_JsonRoot;
	Json::CharReaderBuilder st_JsonBuilder;
	std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_JsonBuilder.newCharReader());
	if (!pSt_JsonReader->parse(ptszMsgBuffer, ptszMsgBuffer + nLen, &st_JsonRoot, &st_JsonError))
	{
		return;
	}
	_tcsxcpy(tszKeyBuffer, st_JsonRoot["lpszBuckKey"].asCString());

	_xtprintf("upload:%d\n", nCode);
	BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
	//断点续传必须指定storagekey
	nLen = 0;
	memset(tszHdrBuffer, '\0', MAX_PATH);
	_xstprintf(tszHdrBuffer, _X("Range: bytes=5-10/10\r\nAuthorization: %s\r\nStorageKey: %s\r\n"), tszBaseBuffer, tszKeyBuffer);
	if (!APIClient_Http_Request(_X("POST"), lpszUrl, lpszMsgBuffer2, &nCode, &ptszMsgBuffer, &nLen, tszHdrBuffer))
	{
		_xtprintf("upload failed:%lX\n", APIClient_GetLastError());
		return;
	}
	_xtprintf("upload:%d\n%s\n", nCode, ptszMsgBuffer);
	BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
}
//下载文件
void File_Download()
{
	LPCXSTR lpszUrl = _X("http://127.0.0.1:5101/storagekey2/newfile4.txt");

	int nLen = 0;
	XCHAR* ptszMsgBuffer = NULL;
	XCHAR tszBaseBuffer[128];
	XCHAR tszHdrBuffer[MAX_PATH];

	memset(tszBaseBuffer, '\0', sizeof(tszBaseBuffer));
	memset(tszHdrBuffer, '\0', MAX_PATH);
	OPenSsl_Help_BasicEncoder("123123aa", "123123", tszBaseBuffer);

	_xstprintf(tszHdrBuffer, _X("Range: bytes=0-5\r\nAuthorization: %s\r\n"), tszBaseBuffer);
	if (!APIClient_Http_Request(_X("GET"), lpszUrl, NULL, NULL, &ptszMsgBuffer, &nLen, tszHdrBuffer))
	{
		_xtprintf("download failed:%lX\n", APIClient_GetLastError());
		return;
	}
	_xtprintf("download:%d,%s\n", nLen, ptszMsgBuffer);
	BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ptszMsgBuffer);

	memset(tszHdrBuffer, '\0', MAX_PATH);
	_xstprintf(tszHdrBuffer, _X("Range: bytes=5-10\r\nAuthorization: %s\r\n"), tszBaseBuffer);
	if (!APIClient_Http_Request(_X("GET"), lpszUrl, NULL, NULL, &ptszMsgBuffer, &nLen, tszHdrBuffer))
	{
		_xtprintf("download failed:%lX\n", APIClient_GetLastError());
		return;
	}
	_xtprintf("download:%d,%s\n", nLen, ptszMsgBuffer);
	BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
}

int main()
{
#ifdef _MSC_BUILD
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