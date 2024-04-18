#ifdef _MSC_BUILD
#include <Windows.h>
#include <tchar.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <thread>
#include <XEngine_Include/XEngine_CommHdr.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Define.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Error.h>
#include "../../XEngine_Source/StorageModule_BTorrent/BTorrent_Define.h"
#include "../../XEngine_Source/StorageModule_BTorrent/BTorrent_Error.h"

#ifdef _MSC_BUILD
#pragma comment(lib,"XEngine_BaseLib/XEngine_BaseLib.lib")
#pragma comment(lib,"../../XEngine_Source/Debug/StorageModule_BTorrent.lib")
#endif

//g++ -std=c++17 -Wall -g NetHelp_APPBTorrent.cpp -o AVCodec_APPPlayer.exe -L ../../XEngine_Source/StorageModule_BTorrent -lXEngine_BaseLib -lStorageModule_BTorrent


int BTorrent_Parse(LPCXSTR lpszFile)
{
	XNETHANDLE xhToken;

	if (!BTorrent_Parse_Init(&xhToken, lpszFile))
	{
		printf("BTorrent_Parse_Init:%lX\n", BTorrent_GetLastError());
		return -1;
	}
	int nListCount = 0;
	BTORRENT_PARSEMAP** ppSt_ParseNode;
	if (!BTorrent_Parse_GetNode(xhToken, &ppSt_ParseNode, &nListCount))
	{
		printf("BTorrent_Parse_GetNode:%lX\n", BTorrent_GetLastError());
		return -1;
	}
	for (int i = 0; i < nListCount; i++)
	{
		printf("BTorrent_Parse_GetNode:%d = %s\n", ppSt_ParseNode[i]->nValue, ppSt_ParseNode[i]->tszValue);
	}
	BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_ParseNode, nListCount);
	nListCount = 0;

	if (!BTorrent_Parse_GetTracker(xhToken, &ppSt_ParseNode, &nListCount))
	{
		printf("BTorrent_Parse_GetTracker:%lX\n", BTorrent_GetLastError());
		return -1;
	}
	for (int i = 0; i < nListCount; i++)
	{
		printf("BTorrent_Parse_GetTracker:%d = %s\n", ppSt_ParseNode[i]->nValue, ppSt_ParseNode[i]->tszValue);
	}
	BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_ParseNode, nListCount);
	nListCount = 0;

	if (!BTorrent_Parse_GetSeeds(xhToken, &ppSt_ParseNode, &nListCount))
	{
		printf("BTorrent_Parse_GetSeeds:%lX\n", BTorrent_GetLastError());
		return -1;
	}
	for (int i = 0; i < nListCount; i++)
	{
		printf("BTorrent_Parse_GetSeeds:%d = %s\n", ppSt_ParseNode[i]->nValue, ppSt_ParseNode[i]->tszValue);
	}
	BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_ParseNode, nListCount);
	nListCount = 0;

	int nPieceLen = 0;
	int nPieceCount = 0;
	if (!BTorrent_Parse_GetPiece(xhToken, &nPieceLen, &nPieceCount))
	{
		printf("BTorrent_Parse_GetPiece:%lX\n", BTorrent_GetLastError());
		return -1;
	}
	printf("BTorrent_Parse_GetPiece:%d %d\n", nPieceLen, nPieceCount);

	XCHAR tszHash[4196];
	XCHAR tszCreator[4196];
	XCHAR tszComment[4196];

	memset(tszHash, '\0', sizeof(tszHash));
	memset(tszCreator, '\0', sizeof(tszCreator));
	memset(tszComment, '\0', sizeof(tszComment));

	if (!BTorrent_Parse_GetInfo(xhToken, tszHash, tszCreator, tszComment))
	{
		printf("BTorrent_Parse_GetPiece:%lX\n", BTorrent_GetLastError());
		return -1;
	}
	printf("BTorrent_Parse_GetInfo:%s\n%s\n%s\n", tszHash, tszCreator, tszComment);

	XCHAR tszFile[MAX_PATH];
	memset(tszFile, '\0', MAX_PATH);
	BTORRENT_FILEINFO** ppSt_FileList;
	if (!BTorrent_Parse_GetFile(xhToken, tszFile, &ppSt_FileList, &nListCount))
	{
		printf("BTorrent_Parse_GetFile:%lX\n", BTorrent_GetLastError());
		return -1;
	}
	for (int i = 0; i < nListCount; i++)
	{
		printf("BTorrent_Parse_GetFile:%s %s %s %s %lld %lld %lld %d %d\n", ppSt_FileList[i]->tszFileHash, ppSt_FileList[i]->tszFilePath, ppSt_FileList[i]->tszFileName, ppSt_FileList[i]->tszFileLink, ppSt_FileList[i]->nFileOffset, ppSt_FileList[i]->nFileSize, ppSt_FileList[i]->nFileTime, ppSt_FileList[i]->nFileStart, ppSt_FileList[i]->nFileEnd);
	}
	BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_FileList, nListCount);

	BTorrent_Parse_Destory(xhToken);
	return 0;
}
int BTorrent_Creator(LPCXSTR lpszFile)
{
	XNETHANDLE xhToken;
#ifdef _MSC_BUILD
	LPCXSTR lpszFileDir = _X("D:\\321");
#else
	LPCXSTR lpszFileDir = _X("./321");
#endif

	if (!BTorrent_Creator_Init(&xhToken, lpszFileDir))
	{
		printf("BTorrent_Creator_Init:%lX\n", BTorrent_GetLastError());
		return -1;
	}

	BTorrent_Creator_AddTracker(xhToken, "http://wwww.xyry.org/anno");
	BTorrent_Creator_SetInfo(xhToken, "xengine", "xengine btorrent V7.11");

	if (!BTorrent_Creator_GetTorrent(xhToken, lpszFile))
	{
		printf("BTorrent_Creator_GetTorrent:%lX\n", BTorrent_GetLastError());
		return -1;
	}
	BTorrent_Creator_Destory(xhToken);
	return 0;
}
int BTorrent_Download()
{
	XNETHANDLE xhToken;
#ifdef _MSC_BUILD
	LPCXSTR lpszBTFile = _X("D:\\123.torrent");
	LPCXSTR lpszFileDir = _X("D:\\321");
#else
	LPCXSTR lpszBTFile = _X("./123.torrent");
	LPCXSTR lpszFileDir = _X("./321");
#endif

	if (!BTorrent_DLoader_Create(&xhToken, lpszBTFile, lpszFileDir))
	{
		printf("BTorrent_Creator_Init:%lX\n", BTorrent_GetLastError());
		return -1;
	}
	bool bRun = TRUE;

	while (1)
	{
		int nListCount = 0;
		ENUM_BTORRENT_EVENT_TYPE** ppenEventList;
		BTorrent_DLoader_Query(xhToken, &ppenEventList, &nListCount);
		for (int i = 0; i < nListCount; i++)
		{
			if (ENUM_BTORRENT_EVENT_TYPE_STATUS == *(ppenEventList[i]))
			{
				BTORRENT_DLOADER st_DLStatus;
				BTorrent_DLoader_GetStatus(xhToken, &st_DLStatus);

				printf("%s-%d:%lld,%lld\n", st_DLStatus.tszFileName, st_DLStatus.nDLStatus, st_DLStatus.nDLoadTotal, st_DLStatus.nDLCount);
			}
			else if (ENUM_BTORRENT_EVENT_TYPE_COMPLETE == *(ppenEventList[i]))
			{
				bRun = FALSE;
				break;
			}
			printf("%d\n", *(ppenEventList[i]));
		}
		if (!bRun)
		{
			break;
		}
		std::this_thread::sleep_for(std::chrono::seconds(1));
		BaseLib_OperatorMemory_Free((XPPPMEM)&ppenEventList, nListCount);
	}

	BTorrent_DLoader_Close(xhToken);
	return 0;
}
int main()
{
#ifdef _MSC_BUILD
	LPCXSTR lpszFile = _X("D:\\321.torrent");
#else
	LPCXSTR lpszFile = _X("./321.torrent");
#endif

	BTorrent_Download();
	BTorrent_Creator(lpszFile);
	BTorrent_Parse(lpszFile);

	return 0;
}