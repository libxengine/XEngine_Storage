#include "../StorageApp_Hdr.h"

BOOL XEngine_Task_P2P(LPCTSTR lpszAPIName, LPCTSTR lpszClientAddr, LPCTSTR lpszMsgBuffer, int nMsgLen, RFCCOMPONENTS_HTTP_REQPARAM* pSt_HTTPParam, TCHAR** pptszListHdr, int nHdrCount)
{
	int nSDLen = 4096;
	int nPkLen = 4096;

	TCHAR tszSDBuffer[4096];
	TCHAR tszPKBuffer[4096];
	RFCCOMPONENTS_HTTP_HDRPARAM st_HDRParam;

	memset(tszSDBuffer, '\0', sizeof(tszSDBuffer));
	memset(tszPKBuffer, '\0', sizeof(tszPKBuffer));
	memset(&st_HDRParam, '\0', sizeof(RFCCOMPONENTS_HTTP_HDRPARAM));
	//获取客户端对应的关联数据内容
	if (0 == _tcsnicmp(XENGINE_STORAGE_APP_METHOD_LOGIN, lpszAPIName, _tcslen(XENGINE_STORAGE_APP_METHOD_LOGIN)))
	{
		XENGINE_P2XP_PEERINFO st_ClientPeer;
		memset(&st_ClientPeer, '\0', sizeof(XENGINE_P2XP_PEERINFO));

		st_HDRParam.bIsClose = TRUE;
		st_HDRParam.nHttpCode = 500;
		if (!P2XPProtocol_Parse_Login(lpszMsgBuffer, nMsgLen, &st_ClientPeer.st_PeerAddr))
		{
			P2XPProtocol_Packet_Common(tszPKBuffer, &nPkLen, 400, "协议错误");
			RfcComponents_HttpServer_SendMsgEx(xhCenterHttp, tszSDBuffer, &nSDLen, &st_HDRParam, tszPKBuffer, nPkLen);
			XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPCENTER);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("业务客户端:%s,处理登录错误,解析协议失败,错误码:%lX"), lpszClientAddr, P2XPProtocol_GetLastError());
			return FALSE;
		}
		//获取外网IP所在位置
		NetXApi_Address_IPtoAddr(st_ClientPeer.st_PeerAddr.tszPublicAddr, st_ClientPeer.st_PeerAddr.tszUserLocation, st_ClientPeer.st_PeerAddr.tszUserArea);
		st_ClientPeer.st_PeerTimer.dwUserTime = time(NULL);
		st_ClientPeer.st_PeerTimer.dwKeepAlive = time(NULL);
		st_ClientPeer.bIsLogin = TRUE;
		_tcscpy(st_ClientPeer.st_PeerAddr.tszConnectAddr, lpszClientAddr);
		if (!P2XPPeer_Manage_Add(st_ClientPeer))
		{
			P2XPProtocol_Packet_Common(tszPKBuffer, &nPkLen, 500, "设置用户信息失败");
			RfcComponents_HttpServer_SendMsgEx(xhCenterHttp, tszSDBuffer, &nSDLen, &st_HDRParam, tszPKBuffer, nPkLen);
			XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPCENTER);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("业务客户端:%s,处理登录错误,设置用户信息失败,错误码:%lX"), lpszClientAddr, P2XPPeer_GetLastError());
			return FALSE;
		}
		P2XPProtocol_Packet_Common(tszPKBuffer, &nPkLen);
		RfcComponents_HttpServer_SendMsgEx(xhCenterHttp, tszSDBuffer, &nSDLen, &st_HDRParam, tszPKBuffer, nPkLen);
		XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPCENTER);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("业务客户端:%s,处理用户登录请求成功,用户名:%s"), lpszClientAddr, st_ClientPeer.st_AuthUser.tszUserName);
	}
	else if (0 == _tcsnicmp(XENGINE_STORAGE_APP_METHOD_LIST, lpszAPIName, _tcslen(XENGINE_STORAGE_APP_METHOD_LIST)))
	{
		TCHAR tszPubAddr[128];
		TCHAR tszPriAddr[128];

		memset(tszPriAddr, '\0', sizeof(tszPriAddr));
		memset(tszPriAddr, '\0', sizeof(tszPriAddr));

		st_HDRParam.bIsClose = TRUE;
		st_HDRParam.nHttpCode = 200;
		if (!P2XPProtocol_Parse_List(lpszMsgBuffer, nMsgLen, tszPubAddr, tszPriAddr))
		{
			P2XPProtocol_Packet_Common(tszPKBuffer, &nPkLen, 400, "协议错误");
			RfcComponents_HttpServer_SendMsgEx(xhCenterHttp, tszSDBuffer, &nSDLen, &st_HDRParam, tszPKBuffer, nPkLen);
			XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPCENTER);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("业务客户端:%s,列表请求失败,协议解析错误,错误码:%lX"), lpszClientAddr, P2XPProtocol_GetLastError());
			return FALSE;
		}
		//请求同步列表
		if (_tcslen(tszPriAddr) > 0)
		{
			int nListCount = 0;
			XENGINE_P2XPPEER_PROTOCOL** ppSt_ListClients;
			if (!P2XPPeer_Manage_GetLan(tszPubAddr, tszPriAddr, &ppSt_ListClients, &nListCount))
			{
				P2XPProtocol_Packet_Common(tszPKBuffer, &nPkLen, 500, "内部处理错误");
				RfcComponents_HttpServer_SendMsgEx(xhCenterHttp, tszSDBuffer, &nSDLen, &st_HDRParam, tszPKBuffer, nPkLen);
				XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPCENTER);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("业务客户端:%s,列表请求失败,请求同步局域网列表失败,公有地址:%s,私有地址:%s,错误码:%lX"), lpszClientAddr, tszPubAddr, tszPriAddr, P2XPPeer_GetLastError());
				return FALSE;
			}
			P2XPProtocol_Packet_Lan(&ppSt_ListClients, nListCount, tszPKBuffer, &nPkLen);
			RfcComponents_HttpServer_SendMsgEx(xhCenterHttp, tszSDBuffer, &nSDLen, &st_HDRParam, tszPKBuffer, nPkLen);
			XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPCENTER);
			BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_ListClients, nListCount);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("客户端:%s,请求同步局域网列表成功,公有地址:%s,私有地址:%s"), lpszClientAddr, tszPubAddr, tszPriAddr);
		}
		else
		{

		}
	}
	else if (0 == _tcsnicmp(XENGINE_STORAGE_APP_METHOD_USER, lpszAPIName, _tcslen(XENGINE_STORAGE_APP_METHOD_USER)))
	{
		TCHAR tszUserName[128];
		XENGINE_P2XP_PEERINFO st_PeerInfo;

		memset(tszUserName, '\0', sizeof(tszUserName));
		memset(&st_PeerInfo, '\0', sizeof(XENGINE_P2XP_PEERINFO));

		st_HDRParam.bIsClose = TRUE;
		st_HDRParam.nHttpCode = 200;
		if (!P2XPProtocol_Parse_User(lpszMsgBuffer, nMsgLen, tszUserName))
		{
			P2XPProtocol_Packet_Common(tszPKBuffer, &nPkLen, 400, "协议错误");
			RfcComponents_HttpServer_SendMsgEx(xhCenterHttp, tszSDBuffer, &nSDLen, &st_HDRParam, tszPKBuffer, nPkLen);
			XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPCENTER);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("业务客户端:%s,查询用户失败,协议解析错误,错误码:%lX"), lpszClientAddr, P2XPProtocol_GetLastError());
			return FALSE;

		}
		if (!P2XPPeer_Manage_GetUser(tszUserName, &st_PeerInfo))
		{
			P2XPProtocol_Packet_Common(tszPKBuffer, &nPkLen, 500, "获取用户失败");
			RfcComponents_HttpServer_SendMsgEx(xhCenterHttp, tszSDBuffer, &nSDLen, &st_HDRParam, tszPKBuffer, nPkLen);
			XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPCENTER);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("业务客户端:%s,查询用户失败,获取用户失败,用户名:%s,错误码:%lX"), lpszClientAddr, tszUserName,P2XPPeer_GetLastError());
			return FALSE;
		}
		P2XPProtocol_Packet_User(&st_PeerInfo, tszPKBuffer, &nPkLen);
		RfcComponents_HttpServer_SendMsgEx(xhCenterHttp, tszSDBuffer, &nSDLen, &st_HDRParam, tszPKBuffer, nPkLen);
		XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPCENTER);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("客户端:%s,请求查询用户:%s 成功"), lpszClientAddr, tszUserName);
	}
	else if (0 == _tcsnicmp(XENGINE_STORAGE_APP_METHOD_CONNECT, lpszAPIName, _tcslen(XENGINE_STORAGE_APP_METHOD_CONNECT)))
	{
		int nResponseLen = 2048;
		TCHAR tszMsgResponse[2048];
		XENGINE_P2XPIO_PROTOCOL st_P2XPIONet;
		XENGINE_P2XP_PEERINFO st_P2XPDestPeer;

		memset(tszMsgResponse, '\0', sizeof(tszMsgResponse));
		memset(&st_P2XPIONet, '\0', sizeof(XENGINE_P2XPIO_PROTOCOL));
		memset(&st_P2XPDestPeer, '\0', sizeof(XENGINE_P2XP_PEERINFO));

		st_HDRParam.bIsClose = TRUE;
		st_HDRParam.nHttpCode = 200;
		if (!P2XPProtocol_Parse_Connect(lpszMsgBuffer, nMsgLen, &st_P2XPIONet))
		{
			P2XPProtocol_Packet_Common(tszPKBuffer, &nPkLen, 400, "协议错误");
			RfcComponents_HttpServer_SendMsgEx(xhCenterHttp, tszSDBuffer, &nSDLen, &st_HDRParam, tszPKBuffer, nPkLen);
			XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPCENTER);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("业务客户端:%s,请求连接失败,协议解析错误,错误码:%lX"), lpszClientAddr, P2XPProtocol_GetLastError());
			return FALSE;
		}
		if (!P2XPPeer_Manage_GetUser(st_P2XPIONet.tszDestUser, &st_P2XPDestPeer))
		{
			P2XPProtocol_Packet_Common(tszPKBuffer, &nPkLen, 404, "用户不存在");
			RfcComponents_HttpServer_SendMsgEx(xhCenterHttp, tszSDBuffer, &nSDLen, &st_HDRParam, tszPKBuffer, nPkLen);
			XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_HTTPCENTER);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("业务客户端:%s,请求连接失败,用户不存在,错误码:%lX"), lpszClientAddr, P2XPPeer_GetLastError());
			return FALSE;
		}
	}
	else
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _T("客户端:%s,请求了一条未知的子协议：%s"),lpszClientAddr, lpszAPIName);
	}
	return TRUE;
}