#include "StorageApp_Hdr.h"

bool StorageApp_HTTPHelp_Verification(int nClient, LPCXSTR lpszClientAddr, RFCCOMPONENTS_HTTP_REQPARAM* pSt_HTTPParam, XCHAR** pptszListHdr, int nHdrCount, LPCXSTR lpszRestApi)
{
	int nSDLen = 2048;
	int nRVLen = 2048;
	XCHAR tszSDBuffer[2048] = {};
	XCHAR tszRVBuffer[2048] = {};
	RFCCOMPONENTS_HTTP_HDRPARAM st_HDRParam = {};

	st_HDRParam.nHttpCode = 401;
	st_HDRParam.bIsClose = true;
	st_HDRParam.bAuth = true;
	//打包验证信息
	int nHDRLen = 0;
	XCHAR tszHDRBuffer[XPATH_MAX] = {};
	if (1 == st_ServiceCfg.st_XProxy.nVType)
	{
		Verification_HTTP_BasicServerPacket(tszHDRBuffer, &nHDRLen);
	}
	else
	{
		XCHAR tszNonceStr[64] = {};
		XCHAR tszOpaqueStr[64] = {};
		Verification_HTTP_DigestServerPacket(tszHDRBuffer, &nHDRLen, tszNonceStr, tszOpaqueStr);
	}
	//后去验证方法
	int nALen = 0;
	int nVType = 0;
	XCHAR tszAuthStr[XPATH_MAX] = {};
	HttpProtocol_ServerHelp_GetAuthInfo(&pptszListHdr, nHdrCount, tszAuthStr, &nALen, &nVType);
	//验证方式是否一致
	if (st_ServiceCfg.st_XProxy.nVType != nVType)
	{
		HttpProtocol_Server_SendMsgEx(xhCenterHttp, tszSDBuffer, &nSDLen, &st_HDRParam, NULL, 0, tszHDRBuffer);
		XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, nClient);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端:%s,用户验证失败,验证方式错误,请求:%d,需求:%d"), lpszClientAddr, nVType, st_ServiceCfg.st_XProxy.nVType);
		return false;
	}
	int nHTTPCode = 0;
	XCLIENT_APIHTTP st_APIHttp = {};

	XCHAR* ptszMSGBuffer = NULL;
	Protocol_StoragePacket_Auth(STORAGE_NETTYPE_FTPCONTRAL, pSt_HTTPParam->tszHttpMethod, pSt_HTTPParam->tszHttpUri, lpszClientAddr, tszAuthStr, tszRVBuffer, &nRVLen);
	if (!APIClient_Http_Request(_X("POST"), lpszRestApi, tszRVBuffer, &nHTTPCode, &ptszMSGBuffer, &nRVLen, NULL, NULL, &st_APIHttp))
	{
		st_HDRParam.nHttpCode = 500;
		Protocol_StoragePacket_HTTPPacket(tszRVBuffer, &nRVLen, ERROR_STORAGE_PROTOCOL_HTTP_MANAGE_SERVERDOWN, "api server is down,cant verification");
		HttpProtocol_Server_SendMsgEx(xhCenterHttp, tszSDBuffer, &nSDLen, &st_HDRParam, tszRVBuffer, nRVLen, tszHDRBuffer);
		XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, nClient);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端:%s,用户验证失败,GET请求验证服务:%s 失败,错误码:%lX"), lpszClientAddr, lpszRestApi, APIClient_GetLastError());
		return false;
	}
	if (200 != nHTTPCode)
	{
		st_HDRParam.nHttpCode = 500;
		Protocol_StoragePacket_HTTPPacket(tszRVBuffer, &nRVLen, ERROR_STORAGE_PROTOCOL_HTTP_MANAGE_SERVERDOWN, "api server is down,cant verification");
		HttpProtocol_Server_SendMsgEx(xhCenterHttp, tszSDBuffer, &nSDLen, &st_HDRParam, tszRVBuffer, nRVLen, tszHDRBuffer);
		XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, nClient);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端:%s,用户验证失败,GET请求验证服务:%s 失败,错误:%d"), lpszClientAddr, lpszRestApi, nHTTPCode);
		return false;
	}
	BaseLib_Memory_FreeCStyle((XPPMEM)&ptszMSGBuffer);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("HTTP客户端:%s,HTTP验证类型:%d 通过"), lpszClientAddr, nVType);
	return true;
}