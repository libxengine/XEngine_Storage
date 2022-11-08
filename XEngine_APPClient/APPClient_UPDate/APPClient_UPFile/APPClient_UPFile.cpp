#include "APPClient_UPHdr.h"
#include "APPClient_UPInstall.h"
#include "APPClient_UPTask.h"

int main(int argc, char** argv)
{
#ifdef _MSC_BUILD
	WSADATA st_WSAData;
	WSAStartup(MAKEWORD(2, 2), &st_WSAData);
#endif
	LPCTSTR lpszLocalFile = _T("D:\\XEngine_Storage\\XEngine_APPClient\\Debug\\LocalFile.txt");
	LPCTSTR lpszDownload = _T("http://192.168.1.8:5101/storagekey1/xengine/upfile.txt");
	TCHAR* ptszMsgBuffer = NULL;
	int nMsgLen = 0;

	if (!APIHelp_HttpRequest_Custom(_T("GET"), lpszDownload, NULL, NULL, &ptszMsgBuffer, &nMsgLen))
	{
		printf("APIHelp_HttpRequest_Custom:%lX\n", APIHelp_GetLastError());
		return 0;
	}
	string m_StrDes;
	int nLocalCount = 0;
	int nRemoteCount = 0;
	__int64x nLocalVer = 0;
	__int64x nRemoteVer = 0;
	FILEPARSER_VERSIONINFO** ppSt_LocalList;
	FILEPARSER_VERSIONINFO** ppSt_RemoteList;
	if (!FileParser_ReadVer_GetRemote(ptszMsgBuffer, nMsgLen, &ppSt_RemoteList, &nRemoteCount, &nRemoteVer, &m_StrDes))
	{
		printf("FileParser_ReadVer_GetRemote error\n");
		return 0;
	}
	if (!FileParser_ReadVer_GetLocal(lpszLocalFile, &ppSt_LocalList, &nLocalCount, &nLocalVer))
	{
		printf("FileParser_ReadVer_GetLocal error\n");
		return 0;
	}

	if (nLocalVer >= nRemoteVer)
	{
		printf("没有新版本可以使用\n");
		return 0;
	}
	list<FILEPARSER_VERSIONINFO> stl_ListUPDate;
	if (!FileParser_Match_Start(&ppSt_RemoteList, nRemoteCount, &ppSt_LocalList, nLocalCount, &stl_ListUPDate))
	{
		printf("FileParser_ReadVer_GetLocal error\n");
		return 0;
	}
	BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_LocalList, nLocalCount);
	BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_RemoteList, nRemoteCount);

	if (stl_ListUPDate.empty())
	{
		printf("有更新,但是更新列表为空,无法继续\n");
		return 0;
	}
	list<FILEPARSER_VERSIONINFO>::const_iterator stl_ListIterator = stl_ListUPDate.begin();
	for (; stl_ListIterator != stl_ListUPDate.end(); stl_ListIterator++)
	{
		if (0 == stl_ListIterator->st_LocalVersion.nModuleVersion)
		{
			printf(_T("增加一个模块，模块名称：%s，老模块版本:%lld,新模块版本:%lld\n"), stl_ListIterator->tszModuleName, stl_ListIterator->st_LocalVersion.nModuleVersion, stl_ListIterator->nModuleVersion);
		}
		else
		{
			printf(_T("更新一个模块，模块名称：%s，老模块版本:%lld,新模块版本:%lld\n"), stl_ListIterator->tszModuleName, stl_ListIterator->st_LocalVersion.nModuleVersion, stl_ListIterator->nModuleVersion);
		}
	}

	stl_ListIterator = stl_ListUPDate.begin();
	for (; stl_ListIterator != stl_ListUPDate.end(); stl_ListIterator++)
	{
		TCHAR tszPathFile[1024];
		NETDOWNLOAD_TASKINFO st_TaskInfo;

		memset(tszPathFile, '\0', sizeof(tszPathFile));
		memset(&st_TaskInfo, '\0', sizeof(NETDOWNLOAD_TASKINFO));

		_stprintf_s(tszPathFile, _T("%s%s"), stl_ListIterator->tszModulePath, stl_ListIterator->tszModuleName);
		_tremove(tszPathFile);
		XHANDLE xhDown = DownLoad_Http_Create(stl_ListIterator->tszModuleDownload, tszPathFile);
		while (TRUE)
		{
			if (!DownLoad_Http_Query(xhDown, &st_TaskInfo))
			{
				break;
			}
			if (ENUM_XENGINE_DOWNLOAD_STATUS_DOWNLOADDING != st_TaskInfo.en_DownStatus)
			{
				break;
			}
		}
		DownLoad_Http_Delete(xhDown);
	}

	if (!HelpModule_Api_Copy(&stl_ListUPDate))
	{
		printf("HelpModule_Api_Copy error\n");
		return 0;
	}

	if (!HelpModule_Api_SetVersion(lpszLocalFile, ptszMsgBuffer, nMsgLen))
	{
		printf("HelpModule_Api_Copy error\n");
		return 0;
	}

	if (!HelpModule_Api_RunExec(&stl_ListUPDate))
	{
		printf("HelpModule_Api_Copy error\n");
		return 0;
	}

	if (!HelpModule_Api_Clear(ptszMsgBuffer, nMsgLen))
	{
		printf("HelpModule_Api_Copy error\n");
		return 0;
	}

#ifdef _MSC_BUILD
	WSACleanup();
#endif
	return 0;
}