#include "../StorageApp_Hdr.h"


BOOL XEngine_Task_Manage(LPCTSTR lpszAPIName, LPCTSTR lpszClientAddr, LPCTSTR lpszMsgBuffer, int nMsgLen, RFCCOMPONENTS_HTTP_REQPARAM* pSt_HTTPParam, TCHAR** pptszListHdr, int nHdrCount)
{
	int nSDLen = 2048;
	TCHAR tszSDBuffer[2048];
	RFCCOMPONENTS_HTTP_HDRPARAM st_HDRParam;

	memset(tszSDBuffer, '\0', sizeof(tszSDBuffer));
	memset(&st_HDRParam, '\0', sizeof(RFCCOMPONENTS_HTTP_HDRPARAM));

	//文件存储成功的事件上传
	if (0 == _tcsnicmp(XENGINE_STORAGE_APP_METHOD_ADD, lpszAPIName, _tcslen(XENGINE_STORAGE_APP_METHOD_ADD)))
	{
		int nListCount = 0;
		XSTORAGECORE_DBFILE** ppSt_DBFile;

		XStorageProtocol_Core_ReportFileParse(lpszMsgBuffer, nMsgLen, &ppSt_DBFile, &nListCount);
		for (int i = 0; i < nListCount; i++)
		{
			if (0 == ppSt_DBFile[i]->st_ProtocolFile.nFileSize)
			{
				TCHAR tszFileDir[MAX_PATH];
				_stprintf(tszFileDir, _T("%s %s"), ppSt_DBFile[i]->st_ProtocolFile.tszFilePath, ppSt_DBFile[i]->st_ProtocolFile.tszFileName);
				XStorageSQL_File_FileInsert(ppSt_DBFile[i]);
			}
		}
		st_HDRParam.nHttpCode = 200;
		RfcComponents_HttpServer_SendMsgEx(xhUPHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
		XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPCENTER);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("业务客户端:%s"), lpszClientAddr);
	}
	return TRUE;
}