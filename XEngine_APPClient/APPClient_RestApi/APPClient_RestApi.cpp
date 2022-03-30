#ifdef _MSC_BUILD
#include <windows.h>
#include <tchar.h>
#pragma comment(lib,"../../XEngine_Source/Debug/jsoncpp")
#pragma comment(lib,"XEngine_BaseLib/XEngine_BaseLib")
#pragma comment(lib,"XEngine_NetHelp/NetHelp_APIHelp")
#pragma comment(lib,"Ws2_32")
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
#include <XEngine_Include/XEngine_NetHelp/APIHelp_Define.h>
#include <XEngine_Include/XEngine_NetHelp/APIHelp_Error.h>
using namespace std;

//g++ -std=c++17 -Wall -g APPClient_RestApi.cpp -o APPClient_RestApi.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_NetHelp -lXEngine_BaseLib -lNetHelp_APIHelp -ljsoncpp -Wl,-rpath=../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_NetHelp,--disable-new-dtags
//需要优先配置XEngine
//WINDOWS使用VS2022 x86 debug 编译
//linux使用下面的命令编译
//g++ -std=c++17 -Wall -g APPClient_RestApi.cpp -o APPClient_RestApi.exe -I ../../XEngine_Source/XEngine_ThirdPart/jsoncpp -L /usr/local/lib/XEngine_Release/XEngine_BaseLib -L /usr/local/lib/XEngine_Release/XEngine_NetHelp -L ../../XEngine_Source/XEngine_ThirdPart/jsoncpp -lXEngine_BaseLib -lNetHelp_APIHelp -ljsoncpp

//查询
void API_Manage_Query()
{
	LPCTSTR lpszUrl = _T("http://192.168.1.8:5100/Api/Manage/QueryFile");
	int nLen = 0;
	int nCode = 0;
	TCHAR* ptszMsgBuffer = NULL;
	Json::Value st_JsonRoot;
	st_JsonRoot["lpszTimeStart"];
	st_JsonRoot["lpszTimeEnd"];
	st_JsonRoot["lpszBuckKey"] = "storagekey2";

	if (!APIHelp_HttpRequest_Post(lpszUrl, st_JsonRoot.toStyledString().c_str(), &nCode, &ptszMsgBuffer, &nLen))
	{
		printf("API_Manage_Query:%lX\n", APIHelp_GetLastError());
		return;
	}
	printf("API_Manage_Query:%d,%s\n", nCode, ptszMsgBuffer);
	BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
}
//插入
void API_Manage_Insert()
{
	LPCTSTR lpszUrl = _T("http://192.168.1.8:5100/Api/Manage/Insert");
	int nLen = 0;
	int nCode = 0;
	TCHAR* ptszMsgBuffer = NULL;
	Json::Value st_JsonRoot;
	Json::Value st_JsonArray;
	Json::Value st_JsonObject;

	st_JsonObject["nFileSize"] = 10;
	st_JsonObject["tszFileHash"] = "85E62C9D28FCE775A68DD126E0519F25";
	st_JsonObject["tszFileName"] = "2.txt";
	st_JsonObject["tszBuckKey"] = "storagekey2";
	st_JsonArray.append(st_JsonObject);

	st_JsonRoot["List"] = st_JsonArray;
	st_JsonRoot["Count"] = 1;

	if (!APIHelp_HttpRequest_Post(lpszUrl, st_JsonRoot.toStyledString().c_str(), &nCode, &ptszMsgBuffer, &nLen))
	{
		printf("API_Manage_Insert:%lX\n", APIHelp_GetLastError());
		return;
	}
	printf("API_Manage_Insert:%d,%s\n", nCode, ptszMsgBuffer);
	BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
}
//删除
void API_Manage_Delete()
{
	LPCTSTR lpszUrl = _T("http://192.168.1.8:5100/Api/Manage/Delete");
	int nLen = 0;
	int nCode = 0;
	TCHAR* ptszMsgBuffer = NULL;
	Json::Value st_JsonRoot;
	Json::Value st_JsonArray;
	Json::Value st_JsonObject;

	st_JsonObject["tszFileHash"] = "85E62C9D28FCE775A68DD126E0519F25";
	st_JsonArray.append(st_JsonObject);

	st_JsonRoot["List"] = st_JsonArray;
	st_JsonRoot["Count"] = 1;

	if (!APIHelp_HttpRequest_Post(lpszUrl, st_JsonRoot.toStyledString().c_str(), &nCode, &ptszMsgBuffer, &nLen))
	{
		printf("API_Manage_Delete:%lX\n", APIHelp_GetLastError());
		return;
	}
	printf("API_Manage_Delete:%d,%s\n", nCode, ptszMsgBuffer);
	BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
}
//文件夹
void API_Manage_Dir()
{
	LPCTSTR lpszUrl = _T("http://192.168.1.8:5100/Api/Manage/Dir");
	int nLen = 0;
	int nCode = 0;
	TCHAR* ptszMsgBuffer = NULL;
	Json::Value st_JsonRoot;
	//创建
	st_JsonRoot["lpszBuckKey"] = "storagekey1";
	st_JsonRoot["lpszUserDir"] = "user";
	st_JsonRoot["nOPerator"] = 1;
	if (!APIHelp_HttpRequest_Post(lpszUrl, st_JsonRoot.toStyledString().c_str(), &nCode, &ptszMsgBuffer, &nLen))
	{
		printf("API_Manage_Dir:%lX\n", APIHelp_GetLastError());
		return;
	}
	printf("API_Manage_Dir:%d,%s\n", nCode, ptszMsgBuffer);
	BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
	//查询
	st_JsonRoot.clear();
	st_JsonRoot["lpszBuckKey"] = "storagekey1";
	st_JsonRoot["lpszUserDir"];
	st_JsonRoot["nOPerator"] = 0;
	if (!APIHelp_HttpRequest_Post(lpszUrl, st_JsonRoot.toStyledString().c_str(), &nCode, &ptszMsgBuffer, &nLen))
	{
		printf("API_Manage_Dir:%lX\n", APIHelp_GetLastError());
		return;
	}
	printf("API_Manage_Dir:%d,%s\n", nCode, ptszMsgBuffer);
	BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
	//删除
	st_JsonRoot.clear();
	st_JsonRoot["lpszBuckKey"] = "storagekey1";
	st_JsonRoot["lpszUserDir"] = "user";
	st_JsonRoot["nOPerator"] = 2;
	if (!APIHelp_HttpRequest_Post(lpszUrl, st_JsonRoot.toStyledString().c_str(), &nCode, &ptszMsgBuffer, &nLen))
	{
		printf("API_Manage_Dir:%lX\n", APIHelp_GetLastError());
		return;
	}
	printf("API_Manage_Dir:%d,%s\n", nCode, ptszMsgBuffer);
	BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
}
//任务管理
void API_Manage_Task()
{
	LPCTSTR lpszUrl = _T("http://192.168.1.8:5100/Api/Manage/Task");
	int nLen = 0;
	int nCode = 0;
	TCHAR* ptszMsgBuffer = NULL;
	if (!APIHelp_HttpRequest_Post(lpszUrl, NULL, &nCode, &ptszMsgBuffer, &nLen))
	{
		printf("API_Manage_Task:%lX\n", APIHelp_GetLastError());
		return;
	}
	printf("API_Manage_Task:%d,%s\n", nCode, ptszMsgBuffer);
	BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
}

int main()
{
#ifdef _WINDOWS
	WSADATA st_WSAData;
	WSAStartup(MAKEWORD(2, 2), &st_WSAData);
#endif

	API_Manage_Query();
	API_Manage_Insert();
	API_Manage_Delete();
	API_Manage_Dir();
	API_Manage_Task();
#ifdef _WINDOWS
	WSACleanup();
#endif
	return 0;
}