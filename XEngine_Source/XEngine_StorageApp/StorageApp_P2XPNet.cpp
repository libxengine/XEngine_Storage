#include "StorageApp_Hdr.h"

XHTHREAD CALLBACK XEngine_P2XP_TCPThread(LPVOID lParam)
{
	int nThreadPos = *(int*)lParam;
	nThreadPos++;

	while (bIsRun)
	{
		//等待指定线程事件触发
		if (HelpComponents_Datas_WaitEventEx(xhP2XPPacket, nThreadPos))
		{
			int nListCount = 0;
			TCHAR tszMsgBuffer[4096];
			XENGINE_PROTOCOLHDR st_ProtocolHdr;
			HELPCOMPONENT_PACKET_CLIENT** ppSst_ListAddr;

			HelpComponents_Datas_GetPoolEx(xhP2XPPacket, nThreadPos, &ppSst_ListAddr, &nListCount);
			for (int i = 0; i < nListCount; i++)
			{
				int nMsgLen = 4096;
				memset(&st_ProtocolHdr, '\0', sizeof(XENGINE_PROTOCOLHDR));
				memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

				for (int j = 0; j < ppSst_ListAddr[i]->nPktCount; j++)
				{
					if (!HelpComponents_Datas_GetEx(xhP2XPPacket, ppSst_ListAddr[i]->tszClientAddr, tszMsgBuffer, &nMsgLen, &st_ProtocolHdr))
					{
						DWORD dwRet = Packets_GetLastError();
						if (ERROR_HELPCOMPONENTS_PACKETS_PROTOCOL_GET_ISNULL == dwRet)
						{
							XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("获取消息队列协议失败，获取数据包失败，错误：%lX"), dwRet);
						}
						continue;
					}
					XEngine_Task_TCPP2xp(&st_ProtocolHdr, ppSst_ListAddr[i]->tszClientAddr, tszMsgBuffer, nMsgLen);
				}
			}
			BaseLib_OperatorMemory_Free((XPPPMEM)&ppSst_ListAddr, nListCount);
		}
	}
	return 0;
}
BOOL XEngine_Task_TCPP2xp(XENGINE_PROTOCOLHDR* pSt_ProtocolHdr, LPCTSTR lpszClientAddr, LPCTSTR lpszMsgBuffer, int nMsgLen)
{
	int nSDLen = 4096;
	TCHAR tszSDBuffer[4096];
	memset(tszSDBuffer, '\0', sizeof(tszSDBuffer));

	if (ENUM_XENGINE_COMMUNICATION_PROTOCOL_TYPE_P2XP == pSt_ProtocolHdr->unOperatorType)
	{
		if (XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_P2XP_REQLOGIN == pSt_ProtocolHdr->unOperatorCode)
		{
			XENGINE_P2XP_PEERINFO st_ClientPeer;
			memset(&st_ClientPeer, '\0', sizeof(XENGINE_P2XP_PEERINFO));

			pSt_ProtocolHdr->unOperatorCode = XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_P2XP_REPLOGIN;
			if (!Protocol_P2XPParse_Login(lpszMsgBuffer, nMsgLen, &st_ClientPeer.st_PeerAddr))
			{
				Protocol_P2XPPacket_Common(pSt_ProtocolHdr, tszSDBuffer, &nSDLen, 400, "协议错误");
				XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_TCPP2XP);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("P2XP客户端:%s,处理登录错误,解析协议失败,错误码:%lX"), lpszClientAddr, Protocol_GetLastError());
				return FALSE;
			}
			//获取外网IP所在位置
#ifdef _WINDOWS
			TCHAR tszLocation[128];
			TCHAR tszISPInfo[128];

			memset(tszLocation, '\0', sizeof(tszLocation));
			memset(tszISPInfo, '\0', sizeof(tszISPInfo));

			NetXApi_Address_IPtoAddr(st_ClientPeer.st_PeerAddr.tszPublicAddr, tszLocation, tszISPInfo);
			int nLen = _tcslen(tszISPInfo);
			BaseLib_OperatorString_UTFToAnsi(tszLocation, st_ClientPeer.st_PeerAddr.tszUserLocation, &nLen);
			BaseLib_OperatorString_UTFToAnsi(tszISPInfo, st_ClientPeer.st_PeerAddr.tszUserISP, &nLen);
#else
			NetXApi_Address_IPtoAddr(st_ClientPeer.st_PeerAddr.tszPublicAddr, st_ClientPeer.st_PeerAddr.tszUserLocation, st_ClientPeer.st_PeerAddr.tszUserISP);
#endif
			st_ClientPeer.st_PeerTimer.dwUserTime = time(NULL);
			st_ClientPeer.st_PeerTimer.dwKeepAlive = time(NULL);
			st_ClientPeer.bIsLogin = TRUE;
			_tcscpy(st_ClientPeer.st_PeerAddr.tszConnectAddr, lpszClientAddr);
			if (!P2XPPeer_Manage_Add(st_ClientPeer))
			{
				Protocol_P2XPPacket_Common(pSt_ProtocolHdr, tszSDBuffer, &nSDLen, 500, "协议错误");
				XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_TCPP2XP);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("P2XP客户端:%s,处理登录错误,设置用户信息失败,错误码:%lX"), lpszClientAddr, P2XPPeer_GetLastError());
				return FALSE;
			}
			Protocol_P2XPPacket_Common(pSt_ProtocolHdr, tszSDBuffer, &nSDLen);
			XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_TCPP2XP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("P2XP客户端:%s,处理用户登录请求成功,用户名:%s"), lpszClientAddr, st_ClientPeer.st_PeerAddr.tszUserName);
		}
		else if (XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_P2XP_REQLANLIST == pSt_ProtocolHdr->unOperatorCode)
		{
			TCHAR tszPubAddr[128];
			TCHAR tszPriAddr[128];

			memset(tszPubAddr, '\0', sizeof(tszPubAddr));
			memset(tszPriAddr, '\0', sizeof(tszPriAddr));

			pSt_ProtocolHdr->unOperatorCode = XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_P2XP_REPLANLIST;
			if (!Protocol_P2XPParse_List(lpszMsgBuffer, nMsgLen, tszPubAddr, tszPriAddr))
			{
				Protocol_P2XPPacket_Common(pSt_ProtocolHdr, tszSDBuffer, &nSDLen, 400, "协议错误");
				XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_TCPP2XP);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("P2XP客户端:%s,列表请求失败,协议解析错误,错误码:%lX"), lpszClientAddr, Protocol_GetLastError());
				return FALSE;
			}
			//请求同步列表
			if (_tcslen(tszPriAddr) > 0)
			{
				int nListCount = 0;
				XENGINE_P2XPPEER_PROTOCOL** ppSt_ListClients;
				if (!P2XPPeer_Manage_GetLan(tszPubAddr, tszPriAddr, &ppSt_ListClients, &nListCount))
				{
					Protocol_P2XPPacket_Common(pSt_ProtocolHdr, tszSDBuffer, &nSDLen, 500, "内部处理错误");
					XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_TCPP2XP);
					XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("P2XP客户端:%s,列表请求失败,请求同步局域网列表失败,公有地址:%s,私有地址:%s,错误码:%lX"), lpszClientAddr, tszPubAddr, tszPriAddr, P2XPPeer_GetLastError());
					return FALSE;
				}
				Protocol_P2XPPacket_Lan(pSt_ProtocolHdr, &ppSt_ListClients, nListCount, tszSDBuffer, &nSDLen);
				XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_TCPP2XP);
				BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_ListClients, nListCount);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("客户端:%s,请求同步局域网列表成功,公有地址:%s,私有地址:%s"), lpszClientAddr, tszPubAddr, tszPriAddr);
			}
			else
			{
				//公网下所有列表
				int nListCount = 0;
				TCHAR** ppszClientList;
				list<XENGINE_P2XPPEER_PROTOCOL> stl_ListClient;
				if (P2XPPeer_Manage_GetLList(tszPubAddr, &ppszClientList, &nListCount))
				{
					for (int i = 0; i < nListCount; i++)
					{
						int nLanCount = 0;
						XENGINE_P2XPPEER_PROTOCOL** ppSt_ListClients;

						if (P2XPPeer_Manage_GetLan(tszPubAddr, ppszClientList[i], &ppSt_ListClients, &nLanCount))
						{
							for (int j = 0; j < nLanCount; j++)
							{
								stl_ListClient.push_back(*ppSt_ListClients[j]);
							}
							BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_ListClients, nLanCount);
						}
					}
					BaseLib_OperatorMemory_Free((XPPPMEM)&ppszClientList, nListCount);
				}
				Protocol_P2XPPacket_WLan(pSt_ProtocolHdr, &stl_ListClient, tszSDBuffer, &nSDLen);
				XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_TCPP2XP);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("客户端:%s,请求同步局域网列表成功,公有地址:%s,私有地址:%s"), lpszClientAddr, tszPubAddr, tszPriAddr);
			}
		}
		else if (XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_P2XP_REQCONNECT == pSt_ProtocolHdr->unOperatorCode)
		{
			TCHAR tszMsgResponse[2048];
			XENGINE_P2XPIO_PROTOCOL st_P2XPIONet;
			XENGINE_P2XP_PEERINFO st_P2XPDestPeer;

			memset(tszMsgResponse, '\0', sizeof(tszMsgResponse));
			memset(&st_P2XPIONet, '\0', sizeof(XENGINE_P2XPIO_PROTOCOL));
			memset(&st_P2XPDestPeer, '\0', sizeof(XENGINE_P2XP_PEERINFO));

			if (!Protocol_P2XPParse_Connect(lpszMsgBuffer, nMsgLen, &st_P2XPIONet))
			{
				Protocol_P2XPPacket_Common(pSt_ProtocolHdr, tszSDBuffer, &nSDLen, 400, "协议错误");
				XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_TCPP2XP);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("P2XP客户端:%s,请求连接失败,协议解析错误,错误码:%lX"), lpszClientAddr, Protocol_GetLastError());
				return FALSE;
			}
			if (!P2XPPeer_Manage_GetUser(st_P2XPIONet.tszDestUser, &st_P2XPDestPeer))
			{
				Protocol_P2XPPacket_Common(pSt_ProtocolHdr, tszSDBuffer, &nSDLen, 404, "用户不存在");
				XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_TCPP2XP);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("P2XP客户端:%s,请求连接失败,用户不存在,错误码:%lX"), lpszClientAddr, P2XPPeer_GetLastError());
				return FALSE;
			}
			//发送请求给目标客户端
			Protocol_P2XPPacket_Connect(pSt_ProtocolHdr, &st_P2XPIONet, tszSDBuffer, &nSDLen);
			XEngine_Net_SendMsg(st_P2XPDestPeer.st_PeerAddr.tszConnectAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_TCPP2XP);
			//响应请求客户端
			memset(tszSDBuffer, '\0', sizeof(tszSDBuffer));
			pSt_ProtocolHdr->unOperatorCode = XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_P2XP_REPCONNECT;
			Protocol_P2XPPacket_Common(pSt_ProtocolHdr, tszSDBuffer, &nSDLen);
			XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_TCPP2XP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("P2XP客户端:%s,用户名:%s,请求查询用户:%s 状态成功"), lpszClientAddr, st_P2XPIONet.tszSourceUser, st_P2XPIONet.tszDestUser);
		}
		else if (XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_P2XP_REQUSERQUERY == pSt_ProtocolHdr->unOperatorCode)
		{
			TCHAR tszUserName[128];
			XENGINE_P2XP_PEERINFO st_PeerInfo;

			memset(tszUserName, '\0', sizeof(tszUserName));
			memset(&st_PeerInfo, '\0', sizeof(XENGINE_P2XP_PEERINFO));

			pSt_ProtocolHdr->unOperatorCode = XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_P2XP_REPUSERQUERY;
			if (!Protocol_P2XPParse_User(lpszMsgBuffer, nMsgLen, tszUserName))
			{
				Protocol_P2XPPacket_Common(pSt_ProtocolHdr, tszSDBuffer, &nSDLen, 400, "协议错误");
				XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_TCPP2XP);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("P2XP客户端:%s,查询用户失败,协议解析错误,错误码:%lX"), lpszClientAddr, Protocol_GetLastError());
				return FALSE;

			}
			if (!P2XPPeer_Manage_GetUser(tszUserName, &st_PeerInfo))
			{
				Protocol_P2XPPacket_Common(pSt_ProtocolHdr, tszSDBuffer, &nSDLen, 500, "获取用户失败");
				XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_TCPP2XP);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("P2XP客户端:%s,查询用户失败,获取用户失败,用户名:%s,错误码:%lX"), lpszClientAddr, tszUserName, P2XPPeer_GetLastError());
				return FALSE;
			}
			Protocol_P2XPPacket_User(pSt_ProtocolHdr, &st_PeerInfo.st_PeerAddr, tszSDBuffer, &nSDLen);
			XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_TCPP2XP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("客户端:%s,请求查询用户:%s 成功"), lpszClientAddr, tszUserName);
		}
		else if (XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_STORAGE_REQQUERY == pSt_ProtocolHdr->unOperatorCode)
		{
			TCHAR tszTimeStart[MAX_PATH];
			TCHAR tszTimeEnd[MAX_PATH];
			TCHAR tszFileName[MAX_PATH];
			TCHAR tszFileHash[MAX_PATH];

			memset(tszTimeStart, '\0', MAX_PATH);
			memset(tszTimeEnd, '\0', MAX_PATH);
			memset(tszFileName, '\0', MAX_PATH);
			memset(tszFileHash, '\0', MAX_PATH);
			//用于验证协议是否正确
			if (!Protocol_StorageParse_QueryFile(lpszMsgBuffer, tszTimeStart, tszTimeEnd, tszFileName, tszFileHash))
			{
				Protocol_P2XPPacket_Common(pSt_ProtocolHdr, tszSDBuffer, &nSDLen, 400, "协议错误");
				XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_TCPP2XP);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("P2XP客户端:%s,查询文件失败,解析协议失败,错误码:%lX"), lpszClientAddr, P2XPPeer_GetLastError());
				return FALSE;
			}
			//确认
			pSt_ProtocolHdr->unOperatorCode = XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_STORAGE_REPQUERY;
			Protocol_P2XPPacket_Common(pSt_ProtocolHdr, tszSDBuffer, &nSDLen);
			XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_TCPP2XP);
			//通知
			int nListCount = 0;
			TCHAR** pppszP2XPClient;
			P2XPPeer_Manage_GetAllList(&pppszP2XPClient, &nListCount);
			pSt_ProtocolHdr->unOperatorCode = XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_STORAGE_REQQUERY;
			for (int i = 0; i < nListCount; i++)
			{
				//过滤自己
				if (0 == _tcsncmp(lpszClientAddr, pppszP2XPClient[i], _tcslen(lpszClientAddr)))
				{
					continue;
				}
				XEngine_Net_SendMsg(pppszP2XPClient[i], (LPCTSTR)pSt_ProtocolHdr, sizeof(XENGINE_PROTOCOLHDR), STORAGE_NETTYPE_TCPP2XP);
				XEngine_Net_SendMsg(pppszP2XPClient[i], lpszMsgBuffer, nMsgLen, STORAGE_NETTYPE_TCPP2XP);
			}
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("客户端:%s,请求查询文件:%s,用户个数:%d"), lpszClientAddr, tszFileName, tszFileHash, nListCount--);
			BaseLib_OperatorMemory_Free((XPPPMEM)&pppszP2XPClient, nListCount);
		}
		else
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _T("客户端:%s,请求了一条未知的子协议：%lX"), lpszClientAddr, pSt_ProtocolHdr->unOperatorCode);
		}
	}
	else
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _T("客户端:%s,请求了一条未知的主协议：%lX"), lpszClientAddr, pSt_ProtocolHdr->unOperatorType);
	}
	return TRUE;
}