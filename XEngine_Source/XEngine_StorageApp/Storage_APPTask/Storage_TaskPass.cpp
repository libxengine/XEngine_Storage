#include "../StorageApp_Hdr.h"

bool XEngine_Task_Pass(LPCXSTR lpszAPIName, LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen, RFCCOMPONENTS_HTTP_REQPARAM* pSt_HTTPParam)
{
	int nSDLen = 2048;
	XCHAR tszSDBuffer[2048];
	LPCXSTR lpszAPIUPFile = _X("UPFile");
	LPCXSTR lpszAPIDLFile = _X("DLFile");
	RFCCOMPONENTS_HTTP_HDRPARAM st_HDRParam;

	memset(tszSDBuffer, '\0', sizeof(tszSDBuffer));
	memset(&st_HDRParam, '\0', sizeof(RFCCOMPONENTS_HTTP_HDRPARAM));

	st_HDRParam.bIsClose = true;
	st_HDRParam.nHttpCode = 200;
	//下载
	if (0 == _tcsxnicmp(lpszAPIUPFile, lpszAPIName, _tcsxlen(lpszAPIUPFile)))
	{
		XCHAR tszClientAddr[128];
		XSTORAGECORE_DBFILE st_DBFile;

		memset(tszClientAddr, '\0', sizeof(tszClientAddr));
		memset(&st_DBFile, '\0', sizeof(XSTORAGECORE_DBFILE));

		Protocol_StorageParse_ProxyNotify(lpszMsgBuffer, nMsgLen, tszClientAddr, st_DBFile.tszBuckKey, st_DBFile.st_ProtocolFile.tszFileName, st_DBFile.st_ProtocolFile.tszFileHash, &st_DBFile.st_ProtocolFile.nFileSize);
		XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPCENTER);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("业务客户端:%s,请求的上传文件通知协议成功,Bucket:%s,文件名:%s,大小:%d"), lpszClientAddr, st_DBFile.tszBuckKey, st_DBFile.st_ProtocolFile.tszFileName, st_DBFile.st_ProtocolFile.nFileSize);
	}
	else if (0 == _tcsxnicmp(lpszAPIDLFile, lpszAPIName, _tcsxlen(lpszAPIDLFile)))
	{
		__int64x nFileSize = 0;
		XCHAR tszPathKey[XPATH_MAX];
		XCHAR tszFileName[XPATH_MAX];
		XCHAR tszFileHash[XPATH_MAX];
		XCHAR tszClientAddr[128];

		memset(tszPathKey, '\0', XPATH_MAX);
		memset(tszFileName, '\0', XPATH_MAX);
		memset(tszFileHash, '\0', XPATH_MAX);
		memset(tszClientAddr, '\0', sizeof(tszClientAddr));

		Protocol_StorageParse_ProxyNotify(lpszMsgBuffer, nMsgLen, tszClientAddr, tszPathKey, tszFileName, tszFileHash, &nFileSize);
		HttpProtocol_Server_SendMsgEx(xhCenterHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
		XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPCENTER);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("业务客户端:%s,请求的下载文件通知协议成功,文件:%s,大小:%lld"), lpszClientAddr, tszFileName, nFileSize);
	}
	return true;
}