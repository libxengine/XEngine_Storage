#include "APPClient_UPHdr.h"
#include "APPClient_UPInstall.h"
#include "APPClient_UPTask.h"

int main(int argc, char** argv)
{
#ifdef _MSC_BUILD
	WSADATA st_WSAData;
	WSAStartup(MAKEWORD(2, 2), &st_WSAData);
#endif
	LPCXSTR lpszLocalFile = _X("D:\\XEngine_Storage\\XEngine_APPClient\\Debug\\LocalFile.txt");
	LPCXSTR lpszDownload = _X("http://192.168.1.8:5101/storagekey1/xengine/upfile.txt");
	XCHAR* ptszMsgBuffer = NULL;
	int nMsgLen = 0;

	if (!APIClient_Http_Request(_X("GET"), lpszDownload, NULL, NULL, &ptszMsgBuffer, &nMsgLen))
	{
		_xtprintf("APIHelp_HttpRequest_Custom:%lX\n", APIClient_GetLastError());
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
		_xtprintf("FileParser_ReadVer_GetRemote error\n");
		return 0;
	}
	if (!FileParser_ReadVer_GetLocal(lpszLocalFile, &ppSt_LocalList, &nLocalCount, &nLocalVer))
	{
		_xtprintf("FileParser_ReadVer_GetLocal error\n");
		return 0;
	}

	if (nLocalVer >= nRemoteVer)
	{
		_xtprintf("没有新版本可以使用\n");
		return 0;
	}
	list<FILEPARSER_VERSIONINFO> stl_ListUPDate;
	if (!FileParser_Match_Start(&ppSt_RemoteList, nRemoteCount, &ppSt_LocalList, nLocalCount, &stl_ListUPDate))
	{
		_xtprintf("FileParser_ReadVer_GetLocal error\n");
		return 0;
	}
	BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_LocalList, nLocalCount);
	BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_RemoteList, nRemoteCount);

	if (stl_ListUPDate.empty())
	{
		_xtprintf("有更新,但是更新列表为空,无法继续\n");
		return 0;
	}
	list<FILEPARSER_VERSIONINFO>::const_iterator stl_ListIterator = stl_ListUPDate.begin();
	for (; stl_ListIterator != stl_ListUPDate.end(); stl_ListIterator++)
	{
		if (0 == stl_ListIterator->st_LocalVersion.nModuleVersion)
		{
			_xtprintf(_X("增加一个模块，模块名称：%s，老模块版本:%lld,新模块版本:%lld\n"), stl_ListIterator->tszModuleName, stl_ListIterator->st_LocalVersion.nModuleVersion, stl_ListIterator->nModuleVersion);
		}
		else
		{
			_xtprintf(_X("更新一个模块，模块名称：%s，老模块版本:%lld,新模块版本:%lld\n"), stl_ListIterator->tszModuleName, stl_ListIterator->st_LocalVersion.nModuleVersion, stl_ListIterator->nModuleVersion);
		}
	}

	stl_ListIterator = stl_ListUPDate.begin();
	for (; stl_ListIterator != stl_ListUPDate.end(); stl_ListIterator++)
	{
		XCHAR tszPathFile[1024];
		NETHELP_FILEINFO st_TaskInfo;

		memset(tszPathFile, '\0', sizeof(tszPathFile));
		memset(&st_TaskInfo, '\0', sizeof(NETHELP_FILEINFO));

		_xstprintf(tszPathFile, _X("%s%s"), stl_ListIterator->tszModulePath, stl_ListIterator->tszModuleName);
		_xtremove(tszPathFile);
		XHANDLE xhDown = APIClient_File_Create(stl_ListIterator->tszModuleDownload, tszPathFile);
		APIClient_File_Start(xhDown);
		while (true)
		{
			if (!APIClient_File_Query(xhDown, &st_TaskInfo))
			{
				break;
			}
			if (ENUM_NETHELP_APICLIENT_FILE_STATUS_DOWNLOADDING != st_TaskInfo.en_DownStatus)
			{
				break;
			}
		}
		APIClient_File_Delete(xhDown);
	}

	if (!HelpModule_Api_Copy(&stl_ListUPDate))
	{
		_xtprintf("HelpModule_Api_Copy error\n");
		return 0;
	}

	if (!HelpModule_Api_SetVersion(lpszLocalFile, ptszMsgBuffer, nMsgLen))
	{
		_xtprintf("HelpModule_Api_Copy error\n");
		return 0;
	}

	if (!HelpModule_Api_RunExec(&stl_ListUPDate))
	{
		_xtprintf("HelpModule_Api_Copy error\n");
		return 0;
	}

	if (!HelpModule_Api_Clear(ptszMsgBuffer, nMsgLen))
	{
		_xtprintf("HelpModule_Api_Copy error\n");
		return 0;
	}

#ifdef _MSC_BUILD
	WSACleanup();
#endif
	return 0;
}