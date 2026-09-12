#include "StorageApp_Hdr.h"

XHTHREAD XCALLBACK XEngine_FTPContral_Thread(XPVOID lParam)
{
	int nThreadPos = *(int*)lParam;
	nThreadPos++;

	while (bIsRun)
	{
		//等待指定线程事件触发
		if (FTPProtocol_Parse_EventWaitEx(xhFTPContral, nThreadPos))
		{
			int nListCount = 0;
			XENGINE_MANAGEPOOL_TASKEVENT** ppSt_PKTClient;
			//获取当前队列池中所有触发上传客户端
			FTPProtocol_Parse_GetPoolEx(xhFTPContral, nThreadPos, &ppSt_PKTClient, &nListCount);
			for (int i = 0; i < nListCount; i++)
			{
				for (int j = 0; j < ppSt_PKTClient[i]->nPktCount; j++)
				{
					XENGINE_KEYVALUE st_KeyValue = {};
					//获得指定上传客户端触发信息
					if (FTPProtocol_Parse_GetClientEx(xhFTPContral, ppSt_PKTClient[i]->tszClientAddr, &st_KeyValue))
					{
						XEngine_Task_FTP(ppSt_PKTClient[i]->tszClientAddr, &st_KeyValue, STORAGE_NETTYPE_FTPCONTRAL);
					}
				}
			}
			BaseLib_Memory_Free((XPPPMEM)&ppSt_PKTClient, nListCount);
		}
	}
	return 0;
}
static void XEngine_FTPDatas_Thread(LPCXSTR lpszClientAddr, LPCXSTR lpszFileName)
{
	XHANDLE xhToken = Session_FTP_GetSocket(lpszClientAddr);
	if (NULL == xhToken)
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("FTP客户端:%s,获取网络连接失败,错误码:%lX"), lpszClientAddr, Session_GetLastError());
		return;
	}
	FILE* pSt_File = _xtfopen(lpszFileName, _X("rb"));
	if (NULL == pSt_File)
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("FTP客户端:%s,请求被动模式失败,打开文件:%s失败"), lpszClientAddr, lpszFileName);
		return;
	}
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("FTP客户端:%s,准备数据文件:%s 完成,开始发送数据"), lpszClientAddr, lpszFileName);

	while (true)
	{
		XCHAR tszMSGBuffer[XPATH_2MAX] = {};
		int nRet = fread(tszMSGBuffer, 1, sizeof(tszMSGBuffer), pSt_File);
		if (nRet <= 0)
		{
			break;
		}
		while (true)
		{
			if (NetCore_TCPSelect_SendEx(xhToken, lpszClientAddr, tszMSGBuffer, nRet))
			{
				break;
			}
			else
			{
				if (ERROR_XENGINE_BASELIB_BASELIB_IO_CLOSE == NetCore_GetLastError())
				{
					XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("FTP客户端:%s,请求被动模式失败,网络连接已断开"), lpszClientAddr);
					break;
				}
			}
			//等待10毫秒
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
	}
	fclose(pSt_File);
	//关闭连接
	NetCore_TCPSelect_StopEx(xhToken);
	//通知
	int nSDLen = 1024;
	XCHAR tszSDBuffer[1024] = {};
	FTPProtocol_Parse_SendPacketEx(xhFTPContral, XENGINE_FTPROTOCOL_RESPONSE_226, tszSDBuffer, &nSDLen);
	XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, STORAGE_NETTYPE_FTPCONTRAL);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("FTP客户端:%s,文件发送数据完毕,网络连接已断开"), lpszClientAddr);
}
static void XEngine_FTPFile_Thread(xstring lpszClientAddr, xstring lpszFilePath)
{
	int nPort = 0;
	XHANDLE xhToken = Session_FTP_GetSocket(lpszClientAddr.c_str(), &nPort);
	if (NULL == xhToken)
	{
		//关闭连接
		APIHelp_Port_Free(nPort);   //还原端口
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("FTP客户端:%s,获取网络连接失败,错误码:%lX"), lpszClientAddr.c_str(), Session_GetLastError());
		return;
	}
	int nMSGLen = 0;
	XLONG dwEvent = XENGINE_NETCORE_TCP_SELECT_EVENT_LOGIN;
	XCHAR tszClientAddr[XPATH_MIN] = {};
	XCHAR tszMSGBuffer[XPATH_8MAX] = {};
	if (!NetCore_TCPSelect_ReadIOEventEx(xhToken, tszClientAddr, tszMSGBuffer, &nMSGLen, &dwEvent))
	{
		//关闭连接
		NetCore_TCPSelect_StopEx(xhToken);
		APIHelp_Port_Free(nPort);   //还原端口
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("FTP客户端:%s,获取网络事件失败,错误码:%lX"), lpszClientAddr.c_str(), NetCore_GetLastError());
		return;
	}
	
	XCHAR tszCMDBuffer[XPATH_2MAX] = {}; 
	_xstprintf(tszCMDBuffer, _X("dir %s"), lpszFilePath.c_str());
	SystemApi_Process_ReadCmdReturn(tszCMDBuffer, tszMSGBuffer, 0, 6, &nMSGLen, 3);
	/*
	int nListCount = 0;
	XCHAR** pptszFileList;
	SystemApi_File_EnumFileA(lpszFilePath, &pptszFileList, &nListCount, false);
	for (int i = 0; i < nListCount; i++)
	{
		XCHAR tszFileName[XPATH_MAX] = {};
		XCHAR tszFileAttr[XPATH_2MAX] = {};
		SYSTEMAPI_FILE_ATTR st_FileAttr = {};
		SystemApi_File_GetFileAttr(pptszFileList[i], &st_FileAttr);

		BaseLib_String_GetFileAndPath(pptszFileList[i], NULL, tszFileName);
		if (st_FileAttr.bFile)
		{
			
		}
		_xsntprintf(tszFileAttr, XPATH_2MAX, _X("%s 1 ftp ftp %lld %s %s "), pptszFileList[i], st_FileAttr.nFileSize, st_FileAttr.tszFileTime, st_FileAttr.tszFileDate);
	}
	*/
	if (!NetCore_TCPSelect_SendEx(xhToken, tszClientAddr, tszMSGBuffer, nMSGLen))
	{
		//关闭连接
		NetCore_TCPSelect_StopEx(xhToken);
		APIHelp_Port_Free(nPort);   //还原端口
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("FTP客户端:%s,连接地址:%s,发送数据大小:%d 失败,错误码:%lX"), lpszClientAddr.c_str(), tszClientAddr, nMSGLen, NetCore_GetLastError());
	}
	//关闭连接
	NetCore_TCPSelect_StopEx(xhToken);
	APIHelp_Port_Free(nPort);   //还原端口
	//通知
	int nSDLen = 1024;
	XCHAR tszSDBuffer[1024] = {};
	FTPProtocol_Parse_SendPacketEx(xhFTPContral, XENGINE_FTPROTOCOL_RESPONSE_226, tszSDBuffer, &nSDLen);
	XEngine_Net_SendMsg(lpszClientAddr.c_str(), tszSDBuffer, nSDLen, STORAGE_NETTYPE_FTPCONTRAL);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("FTP客户端:%s,连接地址:%s,枚举发送数据完毕,网络连接已断开"), lpszClientAddr.c_str(), tszClientAddr);
}

bool XEngine_Task_FTP(LPCXSTR lpszClientAddr, XENGINE_KEYVALUE *pSt_KeyValue, int nNetType)
{
	int nSDLen = 1024;
	int nRVLen = 1024;
	XCHAR tszSDBuffer[1024] = {};
	XCHAR tszRVBuffer[1024] = {};

	if (0 == _tcsxnicmp(XENGINE_FTPROTOCOL_QUESTION_USER, pSt_KeyValue->tszStrKey, _tcsxlen(XENGINE_FTPROTOCOL_QUESTION_USER)))
	{
		//用户请求
		FTPProtocol_Parse_SendPacketEx(xhFTPContral, XENGINE_FTPROTOCOL_RESPONSE_331, tszSDBuffer, &nSDLen);
		XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, nNetType);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("FTP客户端:%s,请求用户名成功,请求信息:%s"), lpszClientAddr, pSt_KeyValue->tszStrVlu);
	}
	else if (0 == _tcsxnicmp(XENGINE_FTPROTOCOL_QUESTION_PASS, pSt_KeyValue->tszStrKey, _tcsxlen(XENGINE_FTPROTOCOL_QUESTION_PASS)))
	{
		//密码请求
		Session_FTP_Insert(lpszClientAddr, false);

		FTPProtocol_Parse_SendPacketEx(xhFTPContral, XENGINE_FTPROTOCOL_RESPONSE_230, tszSDBuffer, &nSDLen);
		XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, nNetType);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("FTP客户端:%s,请求密码成功,请求信息:%s"), lpszClientAddr, pSt_KeyValue->tszStrVlu);
	}
	else if (0 == _tcsxnicmp(XENGINE_FTPROTOCOL_QUESTION_TYPE, pSt_KeyValue->tszStrKey, _tcsxlen(XENGINE_FTPROTOCOL_QUESTION_TYPE)))
	{
		//类型请求
		FTPProtocol_Parse_SendPacketEx(xhFTPContral, XENGINE_FTPROTOCOL_RESPONSE_200, tszSDBuffer, &nSDLen);
		XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, nNetType);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("FTP客户端:%s,请求类型成功,请求信息:%s"), lpszClientAddr, pSt_KeyValue->tszStrVlu);
	}
	else if (0 == _tcsxnicmp(XENGINE_FTPROTOCOL_QUESTION_PASV, pSt_KeyValue->tszStrKey, _tcsxlen(XENGINE_FTPROTOCOL_QUESTION_PASV)))
	{
		//被动模式请求
		int nPort = 0;
		APIADDR_IPADDR st_IPAddr = {};

		APIAddr_IPAddr_IsIPV4Addr(st_ServiceCfg.tszIPAddr, &st_IPAddr);
		FTPProtocol_Parse_SendPacketEx(xhFTPContral, XENGINE_FTPROTOCOL_RESPONSE_227, tszRVBuffer, &nRVLen, false);
		if (!APIHelp_Port_Get(&nPort))
		{
			FTPProtocol_Parse_SendPacketEx(xhFTPContral, XENGINE_FTPROTOCOL_RESPONSE_425, tszSDBuffer, &nSDLen);
			XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, nNetType);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("FTP客户端:%s,请求被动模式失败,资源耗尽,请求信息:%s"), lpszClientAddr, pSt_KeyValue->tszStrVlu);
			return false;
		}
		
		int p1 = nPort / 256;
		int p2 = nPort % 256;
		nSDLen = _xstprintf(tszSDBuffer, _X("%d %s (%d,%d,%d,%d,%d,%d)"), XENGINE_FTPROTOCOL_RESPONSE_227, tszRVBuffer, st_IPAddr.nIPAddr1, st_IPAddr.nIPAddr2, st_IPAddr.nIPAddr3, st_IPAddr.nIPAddr4, p1, p2);
		
		XHANDLE xhToken = NetCore_TCPSelect_StartEx(nPort, 100, true);
		if (NULL == xhToken)
		{
			FTPProtocol_Parse_SendPacketEx(xhFTPContral, XENGINE_FTPROTOCOL_RESPONSE_425, tszSDBuffer, &nSDLen);
			XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, nNetType);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("FTP客户端:%s,请求被动模式失败,资源耗尽,请求信息:%s"), lpszClientAddr, pSt_KeyValue->tszStrVlu);
			return false;
		}
		Session_FTP_SetSocket(lpszClientAddr, nPort, xhToken);
		XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, nNetType);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("FTP客户端:%s,请求被动模式成功,请求地址:%s:%d"), lpszClientAddr, st_ServiceCfg.tszIPAddr, nPort);
	}
	else if (0 == _tcsxnicmp(XENGINE_FTPROTOCOL_QUESTION_RETR, pSt_KeyValue->tszStrKey, _tcsxlen(XENGINE_FTPROTOCOL_QUESTION_RETR)))
	{
		if (0 != _xtaccess(pSt_KeyValue->tszStrVlu, 0))
		{
			FTPProtocol_Parse_SendPacketEx(xhFTPContral, XENGINE_FTPROTOCOL_RESPONSE_550, tszSDBuffer, &nSDLen);
			XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, nNetType);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("FTP客户端:%s,请求文件:%s 失败,文件不存在"), lpszClientAddr, pSt_KeyValue->tszStrVlu);
			return false;
		}
		if (!Session_FTP_Set(lpszClientAddr, pSt_KeyValue->tszStrVlu))
		{
			FTPProtocol_Parse_SendPacketEx(xhFTPContral, XENGINE_FTPROTOCOL_RESPONSE_503, tszSDBuffer, &nSDLen);
			XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, nNetType);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("FTP客户端:%s,请求文件:%s 失败,没有设置服务器端口信息"), lpszClientAddr, pSt_KeyValue->tszStrVlu);
			return false;
		}

		std::thread m_ThreadFTPDatas(XEngine_FTPDatas_Thread, lpszClientAddr, pSt_KeyValue->tszStrVlu);
		m_ThreadFTPDatas.detach();

		FTPProtocol_Parse_SendPacketEx(xhFTPContral, XENGINE_FTPROTOCOL_RESPONSE_150, tszSDBuffer, &nSDLen);
		XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, nNetType);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("FTP客户端:%s,请求文件成功,请求信息:%s"), lpszClientAddr, pSt_KeyValue->tszStrVlu);
	}
	else if (0 == _tcsxnicmp(XENGINE_FTPROTOCOL_QUESTION_QUIT, pSt_KeyValue->tszStrKey, _tcsxlen(XENGINE_FTPROTOCOL_QUESTION_QUIT)))
	{
		FTPProtocol_Parse_SendPacketEx(xhFTPContral, XENGINE_FTPROTOCOL_RESPONSE_221, tszSDBuffer, &nSDLen);
		XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, nNetType);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("FTP客户端:%s,退出成功"), lpszClientAddr);
	}
	else if (0 == _tcsxnicmp(XENGINE_FTPROTOCOL_QUESTION_PWD, pSt_KeyValue->tszStrKey, _tcsxlen(XENGINE_FTPROTOCOL_QUESTION_PWD)))
	{
#ifdef _MSC_BUILD
		_getcwd(tszRVBuffer, nRVLen);
#else
		getcwd(tszRVBuffer, nRVLen);
#endif
		nSDLen = _xstprintf(tszSDBuffer, _X("257 \"%s\" is current directory.\r\n"), tszRVBuffer);
		XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, nNetType);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("FTP客户端:%s,查询当前目录:%s 成功"), lpszClientAddr, tszRVBuffer);
	}
	else if (0 == _tcsxnicmp(XENGINE_FTPROTOCOL_QUESTION_CWD, pSt_KeyValue->tszStrKey, _tcsxlen(XENGINE_FTPROTOCOL_QUESTION_CWD)))
	{
		nSDLen = _xstprintf(tszSDBuffer, _X("250 Directory changed to %s\r\n"), pSt_KeyValue->tszStrVlu);
		XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, nNetType);
		Session_FTP_Set(lpszClientAddr, NULL, pSt_KeyValue->tszStrVlu);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("FTP客户端:%s,切换目录:%s 成功"), lpszClientAddr, pSt_KeyValue->tszStrVlu);
	}
	else if (0 == _tcsxnicmp(XENGINE_FTPROTOCOL_QUESTION_CDUP, pSt_KeyValue->tszStrKey, _tcsxlen(XENGINE_FTPROTOCOL_QUESTION_CDUP)))
	{
		nSDLen = _xstprintf(tszSDBuffer, _X("200 Directory changed to %s\r\n"), pSt_KeyValue->tszStrVlu);
		XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, nNetType);
		Session_FTP_Set(lpszClientAddr, NULL, pSt_KeyValue->tszStrVlu);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("FTP客户端:%s,切换目录:%s 成功"), lpszClientAddr, pSt_KeyValue->tszStrVlu);
	}
	else if (0 == _tcsxnicmp(XENGINE_FTPROTOCOL_QUESTION_LIST, pSt_KeyValue->tszStrKey, _tcsxlen(XENGINE_FTPROTOCOL_QUESTION_LIST)))
	{
		//列举目录请求
		XCHAR tszFilePath[XPATH_MAX] = {};
		if (!Session_FTP_Get(lpszClientAddr, NULL, tszFilePath))
		{
			FTPProtocol_Parse_SendPacketEx(xhFTPContral, XENGINE_FTPROTOCOL_RESPONSE_503, tszSDBuffer, &nSDLen);
			XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, nNetType);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("FTP客户端:%s,请求列举目录:%s 失败,执行顺序错误"), lpszClientAddr, pSt_KeyValue->tszStrVlu);
			return false;
		}
		if (_tcsxlen(tszFilePath) <= 0)
		{
			FTPProtocol_Parse_SendPacketEx(xhFTPContral, XENGINE_FTPROTOCOL_RESPONSE_503, tszSDBuffer, &nSDLen);
			XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, nNetType);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("FTP客户端:%s,请求列举目录:%s 失败,执行顺序错误"), lpszClientAddr, pSt_KeyValue->tszStrVlu);
			return false;
		}
		FTPProtocol_Parse_SendPacketEx(xhFTPContral, XENGINE_FTPROTOCOL_RESPONSE_150, tszSDBuffer, &nSDLen);
		XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, nNetType);

		std::thread m_ThreadFTPFiles(XEngine_FTPFile_Thread, xstring(lpszClientAddr), xstring(tszFilePath));
		m_ThreadFTPFiles.detach();
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("FTP客户端:%s,列举目录:%s 成功"), lpszClientAddr, tszFilePath);
	}
	else
	{
		FTPProtocol_Parse_SendPacketEx(xhFTPContral, XENGINE_FTPROTOCOL_RESPONSE_502, tszSDBuffer, &nSDLen);
		XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, nNetType);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("FTP客户端:%s,发送的方法 %s:%s 不支持"), lpszClientAddr, pSt_KeyValue->tszStrKey, pSt_KeyValue->tszStrVlu);
		return false;
	}
	
	return true;
}