#include "../StorageApp_Hdr.h"

BOOL XEngine_Task_Pass(LPCTSTR lpszAPIName, LPCTSTR lpszClientAddr, LPCTSTR lpszMsgBuffer, int nMsgLen, RFCCOMPONENTS_HTTP_REQPARAM* pSt_HTTPParam, TCHAR** pptszListHdr, int nHdrCount)
{
	int nSDLen = 2048;
	TCHAR tszSDBuffer[2048];
	RFCCOMPONENTS_HTTP_HDRPARAM st_HDRParam;

	memset(tszSDBuffer, '\0', sizeof(tszSDBuffer));
	memset(&st_HDRParam, '\0', sizeof(RFCCOMPONENTS_HTTP_HDRPARAM));

	st_HDRParam.bIsClose = TRUE;
	st_HDRParam.nHttpCode = 200;
	//下载
	if (0 == _tcsnicmp(XENGINE_STORAGE_APP_METHOD_UPFILE, lpszAPIName, _tcslen(XENGINE_STORAGE_APP_METHOD_UPFILE)))
	{
		TCHAR tszClientAddr[128];
		XSTORAGECORE_DBFILE st_DBFile;

		memset(tszClientAddr, '\0', sizeof(tszClientAddr));
		memset(&st_DBFile, '\0', sizeof(XSTORAGECORE_DBFILE));

		Protocol_StorageParse_ProxyNotify(lpszMsgBuffer, nMsgLen, tszClientAddr, st_DBFile.tszBuckKey, st_DBFile.st_ProtocolFile.tszFileName, st_DBFile.st_ProtocolFile.tszFileHash, &st_DBFile.st_ProtocolFile.nFileSize);
		XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPCENTER);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("业务客户端:%s,请求的上传文件通知协议成功,Bucket:%s,文件名:%s,大小:%d"), lpszClientAddr, st_DBFile.tszBuckKey, st_DBFile.st_ProtocolFile.tszFileName, st_DBFile.st_ProtocolFile.nFileSize);
	}
	else if (0 == _tcsnicmp(XENGINE_STORAGE_APP_METHOD_DLFILE, lpszAPIName, _tcslen(XENGINE_STORAGE_APP_METHOD_DLFILE)))
	{
		__int64x nFileSize = 0;
		TCHAR tszPathKey[MAX_PATH];
		TCHAR tszFileName[MAX_PATH];
		TCHAR tszFileHash[MAX_PATH];
		TCHAR tszClientAddr[128];

		memset(tszPathKey, '\0', MAX_PATH);
		memset(tszFileName, '\0', MAX_PATH);
		memset(tszFileHash, '\0', MAX_PATH);
		memset(tszClientAddr, '\0', sizeof(tszClientAddr));

		Protocol_StorageParse_ProxyNotify(lpszMsgBuffer, nMsgLen, tszClientAddr, tszPathKey, tszFileName, tszFileHash, &nFileSize);
		RfcComponents_HttpServer_SendMsgEx(xhCenterHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
		XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPCENTER);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("业务客户端:%s,请求的下载文件通知协议成功,文件:%s,大小:%lld"), lpszClientAddr, tszFileName, nFileSize);
	}
	return TRUE;
}