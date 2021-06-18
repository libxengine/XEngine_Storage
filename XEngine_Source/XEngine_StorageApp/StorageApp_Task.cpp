#include "StorageApp_Hdr.h"

BOOL XEngine_Task_Auth(LPCTSTR lpszClientAddr, TCHAR** pptszListHdr, int nHdrCount, int nSDType)
{
	int nSDLen = 1024;
	int nAuthType = 0;
	int nAuthLen = MAX_PATH;
	LPCTSTR lpszClientType;
	TCHAR tszAuthStr[MAX_PATH];
	TCHAR tszSDBuffer[1024];
	RFCCOMPONENTS_HTTP_HDRPARAM st_HDRParam;

	memset(tszAuthStr, '\0', MAX_PATH);
	memset(tszSDBuffer, '\0', sizeof(tszSDBuffer));
	memset(&st_HDRParam, '\0', sizeof(RFCCOMPONENTS_HTTP_HDRPARAM));


	if (STORAGE_NETTYPE_HTTPDOWNLOAD == nSDLen)
	{
		lpszClientType = _T("下载客户端");
	}
	else if (STORAGE_NETTYPE_HTTPUPLOADER == nSDLen)
	{
		lpszClientType = _T("上传客户端");
	}
	else
	{
		lpszClientType = _T("业务客户端");
	}

	if (!RfcComponents_HttpHelp_GetAuthInfo(&pptszListHdr, nHdrCount, tszAuthStr, &nAuthLen, &nAuthType))
	{
		st_HDRParam.bIsClose = TRUE;
		st_HDRParam.bAuth = TRUE;
		st_HDRParam.nHttpCode = 402;

		RfcComponents_HttpServer_SendMsgEx(xhCenterHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
		XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPCENTER);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("%s:%s,没有验证信息,无法继续"), lpszClientType, lpszClientAddr);
		return FALSE;
	}

	if (1 == nAuthType)
	{
		TCHAR tszUserName[64];
		TCHAR tszUserPass[64];

		memset(tszUserName, '\0', sizeof(tszUserName));
		memset(tszUserPass, '\0', sizeof(tszUserPass));

		OPenSsl_Help_BasicDecoder(tszAuthStr, tszUserName, tszUserPass);
	}
	else
	{
		st_HDRParam.bIsClose = TRUE;
		st_HDRParam.bAuth = TRUE;
		st_HDRParam.nHttpCode = 402;

		RfcComponents_HttpServer_SendMsgEx(xhCenterHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
		XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPCENTER);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("%s:%s,没有验证信息,无法继续"), lpszClientType, lpszClientAddr);
		return FALSE;
	}
	return TRUE;
}