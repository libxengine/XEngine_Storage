#include "../StorageApp_Hdr.h"

bool XEngine_Task_Pass(LPCXSTR lpszAPIName, LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen, RFCCOMPONENTS_HTTP_REQPARAM* pSt_HTTPParam, XCHAR** pptszListHdr, int nHdrCount)
{
	int nSDLen = 2048;
	XCHAR tszSDBuffer[2048];
	RFCCOMPONENTS_HTTP_HDRPARAM st_HDRParam;

	memset(tszSDBuffer, '\0', sizeof(tszSDBuffer));
	memset(&st_HDRParam, '\0', sizeof(RFCCOMPONENTS_HTTP_HDRPARAM));

	st_HDRParam.bIsClose = true;
	st_HDRParam.nHttpCode = 200;
	//下载
	if (0 == _tcsxnicmp(XENGINE_STORAGE_APP_METHOD_UPFILE, lpszAPIName, _tcsxlen(XENGINE_STORAGE_APP_METHOD_UPFILE)))
	{
		XCHAR tszClientAddr[128];
		XSTORAGECORE_DBFILE st_DBFile;

		memset(tszClientAddr, '\0', sizeof(tszClientAddr));
		memset(&st_DBFile, '\0', sizeof(XSTORAGECORE_DBFILE));

		Protocol_StorageParse_ProxyNotify(lpszMsgBuffer, nMsgLen, tszClientAddr, st_DBFile.tszBuckKey, st_DBFile.st_ProtocolFile.tszFileName, st_DBFile.st_ProtocolFile.tszFileHash, &st_DBFile.st_ProtocolFile.nFileSize);
		XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPCENTER);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("业务客户端:%s,请求的上传文件通知协议成功,Bucket:%s,文件名:%s,大小:%d"), lpszClientAddr, st_DBFile.tszBuckKey, st_DBFile.st_ProtocolFile.tszFileName, st_DBFile.st_ProtocolFile.nFileSize);
	}
	else if (0 == _tcsxnicmp(XENGINE_STORAGE_APP_METHOD_DLFILE, lpszAPIName, _tcsxlen(XENGINE_STORAGE_APP_METHOD_DLFILE)))
	{
		__int64x nFileSize = 0;
		XCHAR tszPathKey[MAX_PATH];
		XCHAR tszFileName[MAX_PATH];
		XCHAR tszFileHash[MAX_PATH];
		XCHAR tszClientAddr[128];

		memset(tszPathKey, '\0', MAX_PATH);
		memset(tszFileName, '\0', MAX_PATH);
		memset(tszFileHash, '\0', MAX_PATH);
		memset(tszClientAddr, '\0', sizeof(tszClientAddr));

		Protocol_StorageParse_ProxyNotify(lpszMsgBuffer, nMsgLen, tszClientAddr, tszPathKey, tszFileName, tszFileHash, &nFileSize);
		HttpProtocol_Server_SendMsgEx(xhCenterHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
		XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPCENTER);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("业务客户端:%s,请求的下载文件通知协议成功,文件:%s,大小:%lld"), lpszClientAddr, tszFileName, nFileSize);
	}
	return true;
}