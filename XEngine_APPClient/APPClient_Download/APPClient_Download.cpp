//P2P 分布式(超线程下载示例代码),目前仅支持局域网,示例代码仅仅是演示如何实现,正式使用需要做调整
//P2P Distributed(Hyper-Threading download sample code),only supports LAN,The example code is just to demonstrate how to implement it, and it needs to be adjusted for business
//你可以使用libcurl实现HTTP下载功能,主要是利用了Range字段实现分块下载
//You can use libcurl to implement the HTTP download, mainly using the Range field to achieve block download
#ifdef _MSC_BUILD
#include <windows.h>
#include <tchar.h>
#pragma comment(lib,"XEngine_BaseLib/XEngine_BaseLib")
#pragma comment(lib,"XEngine_Client/XClient_APIHelp")
#pragma comment(lib,"XEngine_SystemSdk/XEngine_SystemApi")
#pragma comment(lib,"Ws2_32")
#ifdef _WIN64
#pragma comment(lib,"../../XEngine_Source/x64/Debug/jsoncpp")
#else
#pragma comment(lib,"../../XEngine_Source/Debug/jsoncpp")
#endif
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
#include <XEngine_Include/XEngine_Client/APIClient_Define.h>
#include <XEngine_Include/XEngine_Client/APIClient_Error.h>
#include <XEngine_Include/XEngine_SystemSdk/ProcFile_Define.h>
#include <XEngine_Include/XEngine_SystemSdk/SystemApi_Define.h>
#include <XEngine_Include/XEngine_SystemSdk/SystemApi_Error.h>
#include "../../XEngine_Source/XStorage_Protocol.h"
using namespace std;

//需要优先配置XEngine
//WINDOWS使用VS2022 x86 或者 x64 debug 编译
//linux macos::g++ -std=c++17 -Wall -g APPClient_Download.cpp -o APPClient_Download.exe -I ../../XEngine_Source/XEngine_Depend/XEngine_Module/jsoncpp -L ../../XEngine_Release -lXEngine_BaseLib -lXClient_APIHelp -lXEngine_SystemApi -ljsoncpp -Wl,-rpath=../../XEngine_Release

typedef struct 
{
	XENGINE_PROTOCOL_FILE st_ProtocolFile;
	XCHAR tszBuckKey[128];
	XCHAR tszIPAddr[64];                                     
}P2PFILE_INFO;

//解析局域网中所有文件
void P2PParse_List(LPCXSTR lpszMsgBuffer, int nMsgLen, list<P2PFILE_INFO>* pStl_ListFile)
{
	Json::Value st_JsonRoot;
	JSONCPP_STRING st_JsonError;
	Json::CharReaderBuilder st_JsonBuilder;

	std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_JsonBuilder.newCharReader());
	//解析JSON
	if (!pSt_JsonReader->parse(lpszMsgBuffer, lpszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
	{
		return;
	}

	int nCount = st_JsonRoot["Count"].asInt();
	Json::Value st_JsonArray = st_JsonRoot["List"];
	for (int i = 0; i < nCount; i++)
	{
		P2PFILE_INFO st_P2PFile;
		memset(&st_P2PFile, '\0', sizeof(P2PFILE_INFO));

		st_P2PFile.st_ProtocolFile.nFileSize = st_JsonArray[i]["nFileSize"].asInt();
		_tcsxcpy(st_P2PFile.st_ProtocolFile.tszFileHash, st_JsonArray[i]["tszFileHash"].asCString());
		_tcsxcpy(st_P2PFile.st_ProtocolFile.tszFileName, st_JsonArray[i]["tszFileName"].asCString());
		_tcsxcpy(st_P2PFile.st_ProtocolFile.tszFilePath, st_JsonArray[i]["tszFilePath"].asCString());
		_tcsxcpy(st_P2PFile.st_ProtocolFile.tszFileTime, st_JsonArray[i]["tszFileTime"].asCString());
		_tcsxcpy(st_P2PFile.st_ProtocolFile.tszFileUser, st_JsonArray[i]["tszFileUser"].asCString());
		_tcsxcpy(st_P2PFile.tszIPAddr, st_JsonArray[i]["tszTableName"].asCString());
		_tcsxcpy(st_P2PFile.tszBuckKey, st_JsonArray[i]["tszBuckKey"].asCString());

		pStl_ListFile->push_back(st_P2PFile);
	}
}
//创建分布式文件下载器
typedef struct  
{
	XHANDLE xhToken;            //下载句柄
	__int64x nPosStart;
	__int64x nPosEnd;
}P2PFILE_PIECE;
void P2PFile_Create(list<P2PFILE_INFO>* pStl_ListFile, LPCXSTR lpszFile)
{
	P2PFILE_PIECE* pSt_P2PFile = new P2PFILE_PIECE[pStl_ListFile->size()];

	__int64x nPos = 0;
	//得到每个块大小
	__int64x nPiece = pStl_ListFile->front().st_ProtocolFile.nFileSize / pStl_ListFile->size();
	//这是一个简单的分布式块算法示例.怎么做分布式,可以根据自己需求做算法拆解
	list<P2PFILE_INFO>::const_iterator stl_ListIterator = pStl_ListFile->begin();
	for (int i = 0; stl_ListIterator != pStl_ListFile->end(); stl_ListIterator++, i++)
	{
		XCHAR tszDLUrl[1024];
		XCHAR tszRange[128];

		memset(tszDLUrl, '\0', sizeof(tszDLUrl));
		memset(tszRange, '\0', sizeof(tszRange));

		_xstprintf(tszDLUrl, _X("%s/%s/%s"), stl_ListIterator->tszIPAddr, stl_ListIterator->tszBuckKey, stl_ListIterator->st_ProtocolFile.tszFileName);
		//是否是最后一块
		if ((int)pStl_ListFile->size() == (i + 1))
		{
			pSt_P2PFile[i].nPosStart = nPos;
			pSt_P2PFile[i].nPosEnd = 0;
			_xstprintf(tszRange, _X("%lld-"), pSt_P2PFile[i].nPosStart);
		}
		else
		{
			pSt_P2PFile[i].nPosStart = nPos;
			pSt_P2PFile[i].nPosEnd = nPiece;
			nPos += nPiece;
			_xstprintf(tszRange, _X("%lld-%lld"), pSt_P2PFile[i].nPosStart, pSt_P2PFile[i].nPosEnd);
		}
		pSt_P2PFile[i].xhToken = APIClient_File_Create(tszDLUrl, lpszFile, true, tszRange);
		if (NULL == pSt_P2PFile[i].xhToken)
		{
			_xtprintf("create download task is failed:%X\n", APIClient_GetLastError());
		}
		APIClient_File_Start(pSt_P2PFile[i].xhToken);
	}
	//直到所有完成
	while (1)
	{
		bool bComplete = true;
		for (unsigned int i = 0; i < pStl_ListFile->size(); i++)
		{
			XCLIENT_APIFILE st_TaskInfo;
			memset(&st_TaskInfo, '\0', sizeof(XCLIENT_APIFILE));

			APIClient_File_Query(pSt_P2PFile[i].xhToken, &st_TaskInfo);
			if (ENUM_XCLIENT_APIHELP_FILE_STATUS_COMPLETE != st_TaskInfo.en_DownStatus)
			{
				bComplete = FALSE;
			}
			_xtprintf("DLToken:%p DLTotal:%lf DLNow:%lf DLStatus:%d\n", pSt_P2PFile[i].xhToken, st_TaskInfo.dlTotal, st_TaskInfo.dlNow, st_TaskInfo.en_DownStatus);
		}
		if (bComplete)
		{
			break;
		}
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}

	for (unsigned int i = 0; i < pStl_ListFile->size(); i++)
	{
		APIClient_File_Delete(pSt_P2PFile[i].xhToken);
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
	int nBodyLen = 0;
	XCHAR *ptszMsgBody = NULL;
	//请求分布式存储文件所有位置
	LPCXSTR lpszUrl = _X("http://127.0.0.1:5100/Api/Manage/Query");
	LPCXSTR lpszFile = _X("D:\\XEngine_Storage\\XEngine_APPClient\\Debug\\qq.exe");

	Json::Value st_JsonRoot;
	st_JsonRoot["nMode"] = 1;          //使用P2P下载
	st_JsonRoot["lpszBuckKey"] = "storagekey2";
	//st_JsonRoot["lpszFileName"] = "qq.exe";
	st_JsonRoot["lpszFileHash"] = "E50A10E2C9414F0397FF85393A6F68C9";

	if (!APIClient_Http_Request(_X("POST"), lpszUrl, st_JsonRoot.toStyledString().c_str(), &nHTTPCode, &ptszMsgBody, &nBodyLen))
	{
		return -1;
	}
	_xtprintf("%s\n", ptszMsgBody);

	list<P2PFILE_INFO> stl_ListFile;
	P2PParse_List(ptszMsgBody, nBodyLen, &stl_ListFile);

	if (stl_ListFile.empty())
	{
		return -1;
	}
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