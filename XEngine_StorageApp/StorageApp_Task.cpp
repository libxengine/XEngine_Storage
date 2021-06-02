#include "StorageApp_Hdr.h"

BOOL XEngine_Task_Handle(LPCTSTR lpszClientAddr, LPCTSTR lpszMsgBuffer, int nMsgLen)
{
	int nSDLen = 0;
	ULONGLONG ullSize = 0;
	TCHAR tszRVBuffer[2048];
	TCHAR tszSDBuffer[2048];
	TCHAR tszFileDir[1024];
	TCHAR tszHttpUrl[MAX_PATH];
	TCHAR tszMethond[64];
	TCHAR tszHttpVer[64];

	memset(tszRVBuffer, '\0', sizeof(tszRVBuffer));
	memset(tszSDBuffer, '\0', sizeof(tszSDBuffer));
	memset(tszFileDir, '\0', sizeof(tszFileDir));
	memset(tszHttpUrl, '\0', sizeof(tszHttpUrl));
	memset(tszMethond, '\0', sizeof(tszMethond));
	memset(tszHttpVer, '\0', sizeof(tszHttpVer));

	if (!RfcComponents_HttpServer_PostMsg(lpszClientAddr, lpszMsgBuffer, nMsgLen, tszRVBuffer, &nSDLen))
	{
		RfcComponents_HttpServer_GetHdr(tszSDBuffer, &ullSize, 400);
		XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, ullSize);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("客户端:%s,投递用户数据失败,错误：%lX"), lpszClientAddr, HttpServer_GetLastError());
		return FALSE;
	}
	if (nSDLen <= 0)
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("客户端:%s,发送的数据不完整,需要继续组包"), lpszClientAddr);
		return TRUE;
	}
	
	TCHAR *ptszTok = _tcstok(tszRVBuffer, _T("\r\n"));
	int nRet = _stscanf(ptszTok, _T("%s %s %s"), tszMethond, tszHttpUrl, tszHttpVer);
	if (3 != nRet)
	{
		RfcComponents_HttpServer_GetHdr(tszSDBuffer, &ullSize, 400);
		XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, ullSize);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("客户端:%s,解析HTTP协议字段失败"), lpszClientAddr);
		return FALSE;
	}
	_stprintf(tszFileDir, _T("%s%s"), st_ServiceCfg.st_XStorage.tszFileDir, tszHttpUrl);
	
	if (!Session_DLStroage_Insert(lpszClientAddr, tszFileDir, &ullSize))
	{
		ullSize = 0;
		RfcComponents_HttpServer_GetHdr(tszSDBuffer, &ullSize, 404);
		XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, ullSize);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("客户端:%s,投递用户数据失败,错误：%lX"), lpszClientAddr, HttpServer_GetLastError());
		return FALSE;
	}
	ULONGLONG ullPrint = ullSize;
	RfcComponents_HttpServer_GetHdr(tszSDBuffer, &ullSize);
	XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, (int)ullSize);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("客户端:%s,请求下载文件成功,文件名:%s,大小:%llu"), lpszClientAddr, tszFileDir, ullPrint);
	return TRUE;
}
