#include "StorageApp_Hdr.h"

BOOL XEngine_Task_ProxyAuth(LPCTSTR lpszClientAddr, LPCTSTR lpszPostUrl, TCHAR** pptszListHdr, int nHdrCount, int nSDType)
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

	if (!st_ServiceCfg.st_XProxy.st_XProxyAuth.bAuth)
	{
		return TRUE;
	}

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
		XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, nSDType);
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
		if (_tcslen(st_ServiceCfg.st_XProxy.st_XProxyAuth.tszAuthProxy) > 0)
		{
			tstring m_StrBody;
			int nResponseCode = 0;
			
			XStorageProtocol_Proxy_PacketBasicAuth(lpszPostUrl, lpszClientAddr, tszUserName, tszUserPass, tszSDBuffer, &nSDLen);
			APIHelp_HttpRequest_Post(st_ServiceCfg.st_XProxy.st_XProxyAuth.tszAuthProxy, tszSDBuffer, &m_StrBody, NULL, NULL, &nResponseCode);

			if (200 != nResponseCode)
			{
				st_HDRParam.bIsClose = TRUE;
				st_HDRParam.bAuth = TRUE;
				st_HDRParam.nHttpCode = nResponseCode;

				RfcComponents_HttpServer_SendMsgEx(xhCenterHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
				XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, nSDType);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("%s:%s,用户验证失败,用户名:%s,密码:%s,错误码:%d,错误内容:%s"), lpszClientType, lpszClientAddr, tszUserPass, tszUserPass, nResponseCode, m_StrBody.c_str());
				return FALSE;
			}
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("%s:%s,代理服务:%s 验证通过,用户名:%s,密码:%s"), lpszClientType, lpszClientAddr, st_ServiceCfg.st_XProxy.st_XProxyAuth.tszAuthProxy, tszUserName, tszUserPass);
		}
		else
		{
			if (!Session_User_Exist(tszUserName, tszUserPass))
			{
				st_HDRParam.bIsClose = TRUE;
				st_HDRParam.bAuth = TRUE;
				st_HDRParam.nHttpCode = 402;

				RfcComponents_HttpServer_SendMsgEx(xhCenterHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
				XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, nSDType);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("%s:%s,验证用户失败,无法继续"), lpszClientType, lpszClientAddr);
				return FALSE;
			}
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("%s:%s,本地验证用户验证通过,用户名:%s,密码:%s"), lpszClientType, lpszClientAddr, tszUserName, tszUserPass);
		}
	}
	else
	{
		st_HDRParam.bIsClose = TRUE;
		st_HDRParam.bAuth = TRUE;
		st_HDRParam.nHttpCode = 402;

		RfcComponents_HttpServer_SendMsgEx(xhCenterHttp, tszSDBuffer, &nSDLen, &st_HDRParam);
		XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, nSDType);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("%s:%s,没有验证信息,无法继续"), lpszClientType, lpszClientAddr);
		return FALSE;
	}
	return TRUE;
}