//P2P 分布式(超线程下载示例代码),目前仅支持局域网,示例代码仅仅是演示如何实现,正式使用需要做调整
//P2P Distributed(Hyper-Threading download sample code),only supports LAN,The example code is just to demonstrate how to implement it, and it needs to be adjusted for business
//你可以使用libcurl实现HTTP下载功能,主要是利用了Range字段实现分块下载
//You can use libcurl to implement the HTTP download, mainly using the Range field to achieve block download
#ifdef _MSC_BUILD
#include <windows.h>
#include <tchar.h>
#pragma comment(lib,"x86/XEngine_BaseLib/XEngine_BaseLib")
#pragma comment(lib,"x86/XEngine_NetHelp/NetHelp_APIHelp")
#pragma comment(lib,"x86/XEngine_DownLoad/XEngine_DownLoad")
#pragma comment(lib,"x86/XEngine_SystemSdk/XEngine_SystemApi")
#pragma comment(lib,"Ws2_32")
#pragma comment(lib,"../../XEngine_Source/Debug/jsoncpp")
#else
#endif
#include <list>
#include <thread>
#include <json/json.h>
#include <XEngine_Include/XEngine_CommHdr.h>
#include <XEngine_Include/XEngine_Types.h>
#include <XEngine_Include/XEngine_ProtocolHdr.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Define.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Error.h>
#include <XEngine_Include/XEngine_NetHelp/APIHelp_Define.h>
#include <XEngine_Include/XEngine_NetHelp/APIHelp_Error.h>
#include <XEngine_Include/XEngine_DownLoad/DownLoad_Define.h>
#include <XEngine_Include/XEngine_DownLoad/DownLoad_Error.h>
#include <XEngine_Include/XEngine_SystemSdk/ProcFile_Define.h>
#include <XEngine_Include/XEngine_SystemSdk/SystemApi_Define.h>
#include <XEngine_Include/XEngine_SystemSdk/SystemApi_Error.h>
#include "../../XEngine_Source/XStorage_Protocol.h"
using namespace std;

//需要优先配置XEngine
//WINDOWS使用VS2022 x86 debug 编译
//linux::g++ -std=c++17 -Wall -g APPClient_Download.cpp -o APPClient_Download.exe -I ../../XEngine_Source/XEngine_ThirdPart/jsoncpp -L /usr/local/lib/XEngine_Release/XEngine_BaseLib -L /usr/local/lib/XEngine_Release/XEngine_NetHelp -L /usr/local/lib/XEngine_Release/XEngine_DownLoad -L /usr/local/lib/XEngine_Release/XEngine_SystemSdk -L ../../XEngine_Source/XEngine_ThirdPart/jsoncpp -lXEngine_BaseLib -lNetHelp_APIHelp -lXEngine_Download -lXEngine_SystemApi -ljsoncpp
//macos::g++ -std=c++17 -Wall -g APPClient_Download.cpp -o APPClient_Download.exe -I ../../XEngine_Source/XEngine_ThirdPart/jsoncpp -L ../../XEngine_Source/XEngine_ThirdPart/jsoncpp -lXEngine_BaseLib -lNetHelp_APIHelp -lXEngine_Download -lXEngine_SystemApi -ljsoncpp

typedef struct 
{
	XENGINE_PROTOCOL_FILE st_ProtocolFile;
	CHAR tszIPAddr[64];                                     
}P2PFILE_INFO;

//解析局域网中所有文件
void P2PParse_List(LPCTSTR lpszMsgBuffer, int nMsgLen, list<P2PFILE_INFO>* pStl_ListFile)
{
	Json::Value st_JsonRoot;
	Json::CharReaderBuilder st_JsonBuild;
	Json::CharReader* pSt_JsonReader(st_JsonBuild.newCharReader());

	JSONCPP_STRING st_JsonError;
	//解析JSON
	if (!pSt_JsonReader->parse(lpszMsgBuffer, lpszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
	{
		return;
	}
	delete pSt_JsonReader;
	pSt_JsonReader = NULL;

	int nCount = st_JsonRoot["Count"].asInt();
	Json::Value st_JsonArray = st_JsonRoot["List"];
	for (int i = 0; i < nCount; i++)
	{
		P2PFILE_INFO st_P2PFile;
		memset(&st_P2PFile, '\0', sizeof(P2PFILE_INFO));

		st_P2PFile.st_ProtocolFile.nFileSize = st_JsonArray[i]["nFileSize"].asInt();
		_tcscpy(st_P2PFile.st_ProtocolFile.tszFileHash, st_JsonArray[i]["tszFileHash"].asCString());
		_tcscpy(st_P2PFile.st_ProtocolFile.tszFileName, st_JsonArray[i]["tszFileName"].asCString());
		_tcscpy(st_P2PFile.st_ProtocolFile.tszFilePath, st_JsonArray[i]["tszFilePath"].asCString());
		_tcscpy(st_P2PFile.st_ProtocolFile.tszFileTime, st_JsonArray[i]["tszFileTime"].asCString());
		_tcscpy(st_P2PFile.st_ProtocolFile.tszFileUser, st_JsonArray[i]["tszFileUser"].asCString());
		_tcscpy(st_P2PFile.tszIPAddr, st_JsonArray[i]["tszTableName"].asCString());

		pStl_ListFile->push_back(st_P2PFile);
	}
}
//创建分布式文件下载器
typedef struct  
{
	XNETHANDLE xhToken;            //下载句柄
	__int64x nPosStart;
	__int64x nPosEnd;
}P2PFILE_PIECE;
void P2PFile_Create(list<P2PFILE_INFO>* pStl_ListFile, LPCTSTR lpszFile)
{
	P2PFILE_PIECE* pSt_P2PFile = new P2PFILE_PIECE[pStl_ListFile->size()];

	int nPos = 0;
	//得到每个块大小
	int nPiece = pStl_ListFile->front().st_ProtocolFile.nFileSize / pStl_ListFile->size();
	//这是一个简单的分布式块算法示例.怎么做分布式,可以根据自己需求做算法拆解
	list<P2PFILE_INFO>::const_iterator stl_ListIterator = pStl_ListFile->begin();
	for (int i = 0; stl_ListIterator != pStl_ListFile->end(); stl_ListIterator++, i++)
	{
		TCHAR tszDLUrl[1024];
		TCHAR tszRange[128];

		memset(tszDLUrl, '\0', sizeof(tszDLUrl));
		memset(tszRange, '\0', sizeof(tszRange));

		_stprintf(tszDLUrl, _T("%s/%s/%s"), stl_ListIterator->tszIPAddr, stl_ListIterator->st_ProtocolFile.tszFilePath + 2, stl_ListIterator->st_ProtocolFile.tszFileName);
		//是否是最后一块
		if ((int)pStl_ListFile->size() == (i + 1))
		{
			pSt_P2PFile[i].nPosStart = nPos;
			pSt_P2PFile[i].nPosEnd = 0;
			_stprintf(tszRange, _T("%lld-"), pSt_P2PFile[i].nPosStart);
		}
		else
		{
			pSt_P2PFile[i].nPosStart = nPos;
			pSt_P2PFile[i].nPosEnd = nPiece;
			nPos += nPiece;
			_stprintf(tszRange, _T("%lld-%lld"), pSt_P2PFile[i].nPosStart, pSt_P2PFile[i].nPosEnd);
		}
		
		if (!DownLoad_Http_Create(&pSt_P2PFile[i].xhToken, tszDLUrl, lpszFile, tszRange, NULL, NULL, NULL))
		{
			printf("create download task is failed:%X\n", Download_GetLastError());
		}
	}
	//直到所有完成
	while (1)
	{
		BOOL bComplete = TRUE;
		for (unsigned int i = 0; i < pStl_ListFile->size(); i++)
		{
			NETDOWNLOAD_TASKINFO st_TaskInfo;
			memset(&st_TaskInfo, '\0', sizeof(NETDOWNLOAD_TASKINFO));

			DownLoad_Http_Query(pSt_P2PFile[i].xhToken, &st_TaskInfo);
			if (ENUM_XENGINE_DOWNLOAD_STATUS_COMPLETE != st_TaskInfo.en_DownStatus)
			{
				bComplete = FALSE;
			}
			printf("DLToken:%lld DLTotal:%lf DLNow:%lf DLStatus:%d\n", pSt_P2PFile[i].xhToken, st_TaskInfo.dlTotal, st_TaskInfo.dlNow, st_TaskInfo.en_DownStatus);
		}
		if (bComplete)
		{
			break;
		}
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}

	for (unsigned int i = 0; i < pStl_ListFile->size(); i++)
	{
		DownLoad_Http_Delete(pSt_P2PFile[i].xhToken);
	}
	delete[] pSt_P2PFile;
	pSt_P2PFile = NULL;
}

int main()
{
#ifdef _MSC_BUILD
	WSADATA st_WSAData;
	WSAStartup(MAKEWORD(2, 2), &st_WSAData);
#endif

	int nHTTPCode = 0;
	int nBodyLen = 2048;
	TCHAR *ptszMsgBody = NULL;
	//请求分布式存储文件所有位置
	LPCTSTR lpszUrl = _T("http://127.0.0.1:5100/Api/Manage/Query");
	LPCTSTR lpszFile = _T("D:\\XEngine_Storage\\XEngine_APPClient\\Debug\\qq.exe");

	Json::Value st_JsonRoot;
	st_JsonRoot["nMode"] = 1;          //使用P2P下载
	st_JsonRoot["lpszBuckKey"] = "storagekey2";
	st_JsonRoot["lpszFileName"] = "qq.exe";

	if (!APIHelp_HttpRequest_Post(lpszUrl, st_JsonRoot.toStyledString().c_str(), &nHTTPCode, &ptszMsgBody, &nBodyLen))
	{
		return -1;
	}
	printf("%s\n", ptszMsgBody);

	list<P2PFILE_INFO> stl_ListFile;
	P2PParse_List(ptszMsgBody, nBodyLen, &stl_ListFile);

	//创建稀疏文件(一个空白的文件)
	if (!SystemApi_File_CreateSparseFile(lpszFile, stl_ListFile.front().st_ProtocolFile.nFileSize))
	{
		return -1;
	}
	P2PFile_Create(&stl_ListFile, lpszFile);

	BaseLib_OperatorMemory_FreeCStyle((VOID**)&ptszMsgBody);

#ifdef _MSC_BUILD
	WSACleanup();
#endif
	return 0;
}