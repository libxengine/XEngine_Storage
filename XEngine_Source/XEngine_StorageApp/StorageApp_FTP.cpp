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
static void XEngine_FTPDownload_Thread(xstring lpszClientAddr, xstring lpszFileName)
{
	int nPort = 0;
	XHANDLE xhToken = Session_FTP_GetSocket(lpszClientAddr.c_str(), &nPort);
	if (NULL == xhToken)
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("FTP客户端:%s,请求下载文件失败,获取网络连接失败,错误码:%lX"), lpszClientAddr.c_str(), Session_GetLastError());
		return;
	}
	int nMSGLen = 0;
	XLONG dwEvent = XENGINE_NETCORE_TCP_SELECT_EVENT_LOGIN;
	XCHAR tszClientAddr[XPATH_MIN] = {};
	XCHAR tszMSGBuffer[XPATH_8MAX] = {};
	if (!NetCore_TCPSelect_ReadIOEventEx(xhToken, tszClientAddr, tszMSGBuffer, &nMSGLen, &dwEvent))
	{
		//关闭连接
		APIHelp_Port_Free(nPort);
		NetCore_TCPSelect_StopEx(xhToken);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("FTP客户端:%s,请求下载文件失败,获取网络事件失败,错误码:%lX"), lpszClientAddr.c_str(), NetCore_GetLastError());
		return;
	}
	FILE* pSt_File = _xtfopen(lpszFileName.c_str(), _X("rb"));
	if (NULL == pSt_File)
	{
		APIHelp_Port_Free(nPort);
		NetCore_TCPSelect_StopEx(xhToken);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("FTP客户端:%s,请求下载文件失败,打开文件:%s失败"), lpszClientAddr.c_str(), lpszFileName.c_str());
		return;
	}
	//设置断点续传
	__int64u nPos = 0;
	Session_FTP_GetRetr(lpszClientAddr.c_str(), &nPos);
	if (nPos > 0)
	{
		fseek(pSt_File, (long)nPos, SEEK_SET);
	}

	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("FTP客户端:%s,准备数据文件:%s 完成,开始下载文件数据,下载位置:%llu"), lpszClientAddr.c_str(), lpszFileName.c_str(), nPos);

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
			if (NetCore_TCPSelect_SendEx(xhToken, tszClientAddr, tszMSGBuffer, nRet))
			{
				break;
			}
			else
			{
				if (ERROR_XENGINE_BASELIB_BASELIB_IO_CLOSE == NetCore_GetLastError())
				{
					XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("FTP客户端:%s,请求下载文件失败,网络连接已断开"), lpszClientAddr.c_str());
					break;
				}
				else
				{
					fseek(pSt_File, -nRet, SEEK_CUR);
				}
			}
			//等待10毫秒
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
	}
	fclose(pSt_File);
	//关闭连接
	APIHelp_Port_Free(nPort);
	NetCore_TCPSelect_StopEx(xhToken);
	//通知
	int nSDLen = 1024;
	XCHAR tszSDBuffer[1024] = {};
	FTPProtocol_Parse_SendPacketEx(xhFTPContral, XENGINE_FTPROTOCOL_RESPONSE_226, tszSDBuffer, &nSDLen);
	XEngine_Net_SendMsg(lpszClientAddr.c_str(), tszSDBuffer, nSDLen, STORAGE_NETTYPE_FTPCONTRAL);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("FTP客户端:%s,连接地址:%s 请求下载文件成功,网络连接已断开"), lpszClientAddr.c_str(), tszClientAddr);
}
static void XEngine_FTPUPload_Thread(xstring lpszClientAddr, xstring lpszFileName)
{
	int nPort = 0;
	XHANDLE xhToken = Session_FTP_GetSocket(lpszClientAddr.c_str(), &nPort);
	if (NULL == xhToken)
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("FTP客户端:%s,请求上传文件失败,获取网络连接失败,错误码:%lX"), lpszClientAddr.c_str(), Session_GetLastError());
		return;
	}
	FILE* pSt_File = NULL;
	//设置断点续传
	__int64u nPos = 0;
	Session_FTP_GetRetr(lpszClientAddr.c_str(), &nPos);
	if (nPos > 0)
	{
		pSt_File = _xtfopen(lpszFileName.c_str(), _X("rb+"));
		if (NULL == pSt_File)
		{
			APIHelp_Port_Free(nPort);
			NetCore_TCPSelect_StopEx(xhToken);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("FTP客户端:%s,请求上传文件失败,续传文件不存在:%s"), lpszClientAddr.c_str(), lpszFileName.c_str());
			return;
		}
		fseek(pSt_File, (long)nPos, SEEK_SET);
	}
	else
	{
		int nFD = open(lpszFileName.c_str(), O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
		if (nFD < 0)
		{
			APIHelp_Port_Free(nPort);
			NetCore_TCPSelect_StopEx(xhToken);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("FTP客户端:%s,请求上传文件失败,创建文件:%s失败"), lpszClientAddr.c_str(), lpszFileName.c_str());
			return;
		}
		pSt_File = fdopen(nFD, "wb");
		if (NULL == pSt_File)
		{
			close(nFD);
			APIHelp_Port_Free(nPort);
			NetCore_TCPSelect_StopEx(xhToken);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("FTP客户端:%s,请求上传文件失败,创建文件:%s失败"), lpszClientAddr.c_str(), lpszFileName.c_str());
			return;
		}
	}
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("FTP客户端:%s,创建文件:%s 成功,开始接受数据,位置:%llu"), lpszClientAddr.c_str(), lpszFileName.c_str(), nPos);
	
	while (true)
	{
		int nMSGLen = 0;
		XLONG dwEvent = XENGINE_NETCORE_TCP_SELECT_EVENT_ALL;
		XCHAR tszClientAddr[XPATH_MIN] = {};
		XCHAR tszMSGBuffer[XENGINE_SOCKET_SR_TCP_BUFFSIZE] = {};

		if (!NetCore_TCPSelect_ReadIOEventEx(xhToken, tszClientAddr, tszMSGBuffer, &nMSGLen, &dwEvent))
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			continue;
		}
		
		if (XENGINE_NETCORE_TCP_SELECT_EVENT_LEAVE == dwEvent)
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("FTP客户端:%s,请求上传文件失败,获取网络事件,用户:%s 离开"), lpszClientAddr.c_str(), tszClientAddr);
			break;
		}
		else if (XENGINE_NETCORE_TCP_SELECT_EVENT_RECV == dwEvent)
		{
			fwrite(tszMSGBuffer, 1, nMSGLen, pSt_File);
		}
	}
	fclose(pSt_File);
	//关闭连接
	APIHelp_Port_Free(nPort);
	NetCore_TCPSelect_StopEx(xhToken);
	//通知
	int nSDLen = 1024;
	XCHAR tszSDBuffer[1024] = {};
	FTPProtocol_Parse_SendPacketEx(xhFTPContral, XENGINE_FTPROTOCOL_RESPONSE_226, tszSDBuffer, &nSDLen);
	XEngine_Net_SendMsg(lpszClientAddr.c_str(), tszSDBuffer, nSDLen, STORAGE_NETTYPE_FTPCONTRAL);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("FTP客户端:%s 请求上传文件完毕,网络连接已断开"), lpszClientAddr.c_str());
}
static void XEngine_FTPFile_Thread(xstring lpszClientAddr, xstring lpszFilePath, xstring lpszAlisPath)
{
	int nPort = 0;
	XHANDLE xhToken = Session_FTP_GetSocket(lpszClientAddr.c_str(), &nPort);
	if (NULL == xhToken)
	{
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
		APIHelp_Port_Free(nPort);
		NetCore_TCPSelect_StopEx(xhToken);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("FTP客户端:%s,获取网络事件失败,错误码:%lX"), lpszClientAddr.c_str(), NetCore_GetLastError());
		return;
	}
	XCHAR tszDIRBuffer[XPATH_MAX] = {};
	XCHAR tszCMDBuffer[XPATH_2MAX] = {};
	_xstprintf(tszDIRBuffer, _X("%s%s"), lpszFilePath.c_str(), lpszAlisPath.c_str());
	BaseLib_String_FixPath(tszDIRBuffer, 1);

	_xstprintf(tszCMDBuffer, _X("dir %s"), tszDIRBuffer);
	SystemApi_Process_ReadCmdReturn(tszCMDBuffer, tszMSGBuffer, 0, 6, &nMSGLen, 3);

	nMSGLen += 2;
	_tcsxcat(tszMSGBuffer, _X("\r\n"));

	if (!NetCore_TCPSelect_SendEx(xhToken, tszClientAddr, tszMSGBuffer, nMSGLen))
	{
		//关闭连接
		APIHelp_Port_Free(nPort);
		NetCore_TCPSelect_StopEx(xhToken);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("FTP客户端:%s,连接地址:%s,发送数据大小:%d 失败,错误码:%lX"), lpszClientAddr.c_str(), tszClientAddr, nMSGLen, NetCore_GetLastError());
	}
	//关闭连接
	APIHelp_Port_Free(nPort);
	NetCore_TCPSelect_StopEx(xhToken);
	//通知
	int nSDLen = 1024;
	XCHAR tszSDBuffer[1024] = {};
	FTPProtocol_Parse_SendPacketEx(xhFTPContral, XENGINE_FTPROTOCOL_RESPONSE_226, tszSDBuffer, &nSDLen);
	XEngine_Net_SendMsg(lpszClientAddr.c_str(), tszSDBuffer, nSDLen, STORAGE_NETTYPE_FTPCONTRAL);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("FTP客户端:%s,连接地址:%s,枚举目录:%s 发送数据完毕,网络连接已断开"), lpszClientAddr.c_str(), tszClientAddr, tszDIRBuffer);
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
		Session_FTP_Set(lpszClientAddr, NULL, st_ServiceCfg.st_XFtp.tszFTPDir);

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
		//下载
		XCHAR tszFilePath[XPATH_MAX] = {};
		XCHAR tszAlisPath[XPATH_MAX] = {};
		if (!Session_FTP_Get(lpszClientAddr, NULL, tszFilePath, tszAlisPath))
		{
			FTPProtocol_Parse_SendPacketEx(xhFTPContral, XENGINE_FTPROTOCOL_RESPONSE_503, tszSDBuffer, &nSDLen);
			XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, nNetType);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("FTP客户端:%s,请求下载文件:%s 失败,执行顺序错误"), lpszClientAddr, pSt_KeyValue->tszStrVlu);
			return false;
		}
		XCHAR tszFileName[XPATH_MAX] = {};
		_xstprintf(tszFileName, _X("%s%s"), tszFilePath, tszAlisPath);
		BaseLib_String_FixPath(tszFileName, 1);

		if (tszFileName[_tcsxlen(tszFileName)] != '\\' && tszFileName[_tcsxlen(tszFileName)] != '/')
		{
			_tcsxcat(tszFileName, _X("\\"));
		}
		_tcsxcat(tszFileName, pSt_KeyValue->tszStrVlu);

		if (0 != _xtaccess(tszFileName, 0))
		{
			FTPProtocol_Parse_SendPacketEx(xhFTPContral, XENGINE_FTPROTOCOL_RESPONSE_550, tszSDBuffer, &nSDLen);
			XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, nNetType);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("FTP客户端:%s,请求文件:%s 失败,文件不存在"), lpszClientAddr, tszFileName);
			return false;
		}
		if (!Session_FTP_Set(lpszClientAddr, pSt_KeyValue->tszStrVlu))
		{
			FTPProtocol_Parse_SendPacketEx(xhFTPContral, XENGINE_FTPROTOCOL_RESPONSE_503, tszSDBuffer, &nSDLen);
			XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, nNetType);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("FTP客户端:%s,请求文件:%s 失败,没有设置服务器端口信息"), lpszClientAddr, pSt_KeyValue->tszStrVlu);
			return false;
		}
		FTPProtocol_Parse_SendPacketEx(xhFTPContral, XENGINE_FTPROTOCOL_RESPONSE_150, tszSDBuffer, &nSDLen);
		XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, nNetType);

		std::thread m_ThreadFTPDatas(XEngine_FTPDownload_Thread, xstring(lpszClientAddr), xstring(tszFileName));
		m_ThreadFTPDatas.detach();
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("FTP客户端:%s,请求文件成功,请求信息:%s"), lpszClientAddr, pSt_KeyValue->tszStrVlu);
	}
	else if (0 == _tcsxnicmp(XENGINE_FTPROTOCOL_QUESTION_STOR, pSt_KeyValue->tszStrKey, _tcsxlen(XENGINE_FTPROTOCOL_QUESTION_STOR)))
	{
		//上传
		XCHAR tszFilePath[XPATH_MAX] = {};
		XCHAR tszAlisPath[XPATH_MAX] = {};
		if (!Session_FTP_Get(lpszClientAddr, NULL, tszFilePath, tszAlisPath))
		{
			FTPProtocol_Parse_SendPacketEx(xhFTPContral, XENGINE_FTPROTOCOL_RESPONSE_503, tszSDBuffer, &nSDLen);
			XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, nNetType);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("FTP客户端:%s,请求上传文件:%s 失败,执行顺序错误"), lpszClientAddr, pSt_KeyValue->tszStrVlu);
			return false;
		}
		XCHAR tszFileName[XPATH_MAX] = {};
		_xstprintf(tszFileName, _X("%s%s"), tszFilePath, tszAlisPath);
		BaseLib_String_FixPath(tszFileName, 1);

		if (tszFileName[_tcsxlen(tszFileName)] != '\\' && tszFileName[_tcsxlen(tszFileName)] != '/')
		{
			_tcsxcat(tszFileName, _X("\\"));
		}
		_tcsxcat(tszFileName, pSt_KeyValue->tszStrVlu);

		if (!Session_FTP_Set(lpszClientAddr, pSt_KeyValue->tszStrVlu))
		{
			FTPProtocol_Parse_SendPacketEx(xhFTPContral, XENGINE_FTPROTOCOL_RESPONSE_503, tszSDBuffer, &nSDLen);
			XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, nNetType);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("FTP客户端:%s,请求上传文件:%s 失败,没有设置服务器端口信息"), lpszClientAddr, pSt_KeyValue->tszStrVlu);
			return false;
		}
		std::thread m_ThreadFTPDatas(XEngine_FTPUPload_Thread, xstring(lpszClientAddr), xstring(tszFileName));
		m_ThreadFTPDatas.detach();

		FTPProtocol_Parse_SendPacketEx(xhFTPContral, XENGINE_FTPROTOCOL_RESPONSE_150, tszSDBuffer, &nSDLen);
		XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, nNetType);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("FTP客户端:%s,请求文件成功,请求信息:%s"), lpszClientAddr, pSt_KeyValue->tszStrVlu);
	}
	else if (0 == _tcsxnicmp(XENGINE_FTPROTOCOL_QUESTION_RETR, pSt_KeyValue->tszStrKey, _tcsxlen(XENGINE_FTPROTOCOL_QUESTION_RETR)))
	{
		//断点续传
		if (!Session_FTP_SetRetr(lpszClientAddr, _ttxoll(pSt_KeyValue->tszStrVlu)))
		{
			FTPProtocol_Parse_SendPacketEx(xhFTPContral, XENGINE_FTPROTOCOL_RESPONSE_503, tszSDBuffer, &nSDLen);
			XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, nNetType);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("FTP客户端:%s,请求上传文件:%s 失败,执行顺序错误"), lpszClientAddr, pSt_KeyValue->tszStrVlu);
			return false;
		}
		nSDLen = _xstprintf(tszSDBuffer, _X("350 Restarting at %s. Send STORE or RETRIEVE to initiate transfer.\r\n"), pSt_KeyValue->tszStrVlu);
		XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, nNetType);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("FTP客户端:%s,请求断点续传位置:%s 成功"), lpszClientAddr, pSt_KeyValue->tszStrVlu);
	}
	else if (0 == _tcsxnicmp(XENGINE_FTPROTOCOL_QUESTION_DELE, pSt_KeyValue->tszStrKey, _tcsxlen(XENGINE_FTPROTOCOL_QUESTION_DELE)))
	{
		//删除文件
		XCHAR tszFilePath[XPATH_MAX] = {};
		XCHAR tszAlisPath[XPATH_MAX] = {};
		if (!Session_FTP_Get(lpszClientAddr, NULL, tszFilePath, tszAlisPath))
		{
			FTPProtocol_Parse_SendPacketEx(xhFTPContral, XENGINE_FTPROTOCOL_RESPONSE_503, tszSDBuffer, &nSDLen);
			XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, nNetType);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("FTP客户端:%s,请求上传文件:%s 失败,执行顺序错误"), lpszClientAddr, pSt_KeyValue->tszStrVlu);
			return false;
		}
		XCHAR tszFileName[XPATH_MAX] = {};
		_xstprintf(tszFileName, _X("%s%s"), tszFilePath, tszAlisPath);
		BaseLib_String_FixPath(tszFileName, 1);

		if (tszFileName[_tcsxlen(tszFileName)] != '\\' && tszFileName[_tcsxlen(tszFileName)] != '/')
		{
			_tcsxcat(tszFileName, _X("\\"));
		}
		_tcsxcat(tszFileName, pSt_KeyValue->tszStrVlu);
		_xtremove(tszFileName);

		FTPProtocol_Parse_SendPacketEx(xhFTPContral, XENGINE_FTPROTOCOL_RESPONSE_250, tszSDBuffer, &nSDLen);
		XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, nNetType);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("FTP客户端:%s,删除文件成功:%s"), lpszClientAddr, tszFileName);
	}
	else if (0 == _tcsxnicmp(XENGINE_FTPROTOCOL_QUESTION_QUIT, pSt_KeyValue->tszStrKey, _tcsxlen(XENGINE_FTPROTOCOL_QUESTION_QUIT)))
	{
		FTPProtocol_Parse_SendPacketEx(xhFTPContral, XENGINE_FTPROTOCOL_RESPONSE_221, tszSDBuffer, &nSDLen);
		XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, nNetType);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("FTP客户端:%s,退出成功"), lpszClientAddr);
	}
	else if (0 == _tcsxnicmp(XENGINE_FTPROTOCOL_QUESTION_PWD, pSt_KeyValue->tszStrKey, _tcsxlen(XENGINE_FTPROTOCOL_QUESTION_PWD)))
	{
		//列举目录
		XCHAR tszFTPDir[XPATH_MAX] = {};
		XCHAR tszAliDir[XPATH_MAX] = {};
		Session_FTP_Get(lpszClientAddr, NULL, tszFTPDir, tszAliDir);

		if (_tcsxlen(tszAliDir) > 0)
		{
			nSDLen = _xstprintf(tszSDBuffer, _X("257 \"%s\" is current directory.\r\n"), tszAliDir);
		}
		else
		{
			nSDLen = _xstprintf(tszSDBuffer, _X("257 \"/\" is current directory.\r\n"));
		}
		
		XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, nNetType);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("FTP客户端:%s,查询当前目录:%s,别名:%s 成功"), lpszClientAddr, tszFTPDir, tszAliDir);
	}
	else if (0 == _tcsxnicmp(XENGINE_FTPROTOCOL_QUESTION_CWD, pSt_KeyValue->tszStrKey, _tcsxlen(XENGINE_FTPROTOCOL_QUESTION_CWD)))
	{
		//切换目录
		XCHAR tszAliDir[XPATH_MAX] = {};
		Session_FTP_Get(lpszClientAddr, NULL, NULL, tszAliDir);

		if (_tcsxlen(tszAliDir) > 0)
		{
			//如果是根目录,//并且切换目标不是根目录
			if (1 == _tcsxlen(tszAliDir) && 1 != _tcsxlen(pSt_KeyValue->tszStrVlu))
			{
				_tcsxcat(tszAliDir, pSt_KeyValue->tszStrVlu);
			}
			else if (0 != _tcsxncmp(pSt_KeyValue->tszStrVlu, tszAliDir, _tcsxlen(tszAliDir)))
			{
				_tcsxcat(tszAliDir, _X("/"));
				_tcsxcat(tszAliDir, pSt_KeyValue->tszStrVlu);
			}
		}
		else
		{
			_tcsxcat(tszAliDir, pSt_KeyValue->tszStrVlu);
		}
		Session_FTP_Set(lpszClientAddr, NULL, NULL, tszAliDir);

		nSDLen = _xstprintf(tszSDBuffer, _X("250 Directory changed to %s\r\n"), tszAliDir);
		XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, nNetType);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("FTP客户端:%s,切换目录:%s 成功"), lpszClientAddr, tszAliDir);
	}
	else if (0 == _tcsxnicmp(XENGINE_FTPROTOCOL_QUESTION_MKD, pSt_KeyValue->tszStrKey, _tcsxlen(XENGINE_FTPROTOCOL_QUESTION_MKD)))
	{
		//创建目录
		XCHAR tszFilePath[XPATH_MAX] = {};
		XCHAR tszAlisPath[XPATH_MAX] = {};
		if (!Session_FTP_Get(lpszClientAddr, NULL, tszFilePath, tszAlisPath))
		{
			FTPProtocol_Parse_SendPacketEx(xhFTPContral, XENGINE_FTPROTOCOL_RESPONSE_503, tszSDBuffer, &nSDLen);
			XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, nNetType);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("FTP客户端:%s,请求上传文件:%s 失败,执行顺序错误"), lpszClientAddr, pSt_KeyValue->tszStrVlu);
			return false;
		}
		XCHAR tszFileName[XPATH_MAX] = {};
		_xstprintf(tszFileName, _X("%s%s"), tszFilePath, tszAlisPath);
		BaseLib_String_FixPath(tszFileName, 1);
		if (tszFileName[_tcsxlen(tszFileName)] != '\\' && tszFileName[_tcsxlen(tszFileName)] != '/')
		{
			_tcsxcat(tszFileName, _X("\\"));
		}
		_tcsxcat(tszFileName, pSt_KeyValue->tszStrVlu);
		SystemApi_File_CreateMutilFolderA(tszFileName);

		nSDLen = _xstprintf(tszSDBuffer, _X("257 \"%s\" directory created\r\n"), pSt_KeyValue->tszStrVlu);
		XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, nNetType);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("FTP客户端:%s,创建目录:%s 成功"), lpszClientAddr, pSt_KeyValue->tszStrVlu);
	}
	else if (0 == _tcsxnicmp(XENGINE_FTPROTOCOL_QUESTION_RMD, pSt_KeyValue->tszStrKey, _tcsxlen(XENGINE_FTPROTOCOL_QUESTION_RMD)))
	{
		//删除目录
		XCHAR tszFilePath[XPATH_MAX] = {};
		XCHAR tszAlisPath[XPATH_MAX] = {};
		if (!Session_FTP_Get(lpszClientAddr, NULL, tszFilePath, tszAlisPath))
		{
			FTPProtocol_Parse_SendPacketEx(xhFTPContral, XENGINE_FTPROTOCOL_RESPONSE_503, tszSDBuffer, &nSDLen);
			XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, nNetType);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("FTP客户端:%s,请求上传文件:%s 失败,执行顺序错误"), lpszClientAddr, pSt_KeyValue->tszStrVlu);
			return false;
		}
		XCHAR tszFileName[XPATH_MAX] = {};
		_xstprintf(tszFileName, _X("%s%s"), tszFilePath, tszAlisPath);
		BaseLib_String_FixPath(tszFileName, 1);
		if (tszFileName[_tcsxlen(tszFileName)] != '\\' && tszFileName[_tcsxlen(tszFileName)] != '/')
		{
			_tcsxcat(tszFileName, _X("\\"));
		}
		_tcsxcat(tszFileName, pSt_KeyValue->tszStrVlu);
		SystemApi_File_DeleteMutilFolderA(tszFileName);

		FTPProtocol_Parse_SendPacketEx(xhFTPContral, XENGINE_FTPROTOCOL_RESPONSE_250, tszSDBuffer, &nSDLen);
		XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, nNetType);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("FTP客户端:%s,删除目录:%s 成功"), lpszClientAddr, pSt_KeyValue->tszStrVlu);
	}
	else if (0 == _tcsxnicmp(XENGINE_FTPROTOCOL_QUESTION_CDUP, pSt_KeyValue->tszStrKey, _tcsxlen(XENGINE_FTPROTOCOL_QUESTION_CDUP)))
	{
		XCHAR tszPDir[XPATH_MAX] = {};
		XCHAR tszAlisDir[XPATH_MAX] = {};
		Session_FTP_Get(lpszClientAddr, NULL, NULL, tszAlisDir);
		//是否设置过目录
		if (_tcsxlen(tszAlisDir) <= 1)
		{
			//不允许切换更上级目录
			FTPProtocol_Parse_SendPacketEx(xhFTPContral, XENGINE_FTPROTOCOL_RESPONSE_550, tszSDBuffer, &nSDLen);
			XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, nNetType);
			return false;
		}
		else
		{
			//如果路径是父目录
			BaseLib_String_GetFileAndPathA(tszAlisDir, tszPDir);
			Session_FTP_Set(lpszClientAddr, NULL, NULL, tszPDir);
		}

		nSDLen = _xstprintf(tszSDBuffer, _X("200 Directory changed to %s\r\n"), tszPDir);
		XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, nNetType);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("FTP客户端:%s,切换上级目录:%s 成功"), lpszClientAddr, tszPDir);
	}
	else if (0 == _tcsxnicmp(XENGINE_FTPROTOCOL_QUESTION_LIST, pSt_KeyValue->tszStrKey, _tcsxlen(XENGINE_FTPROTOCOL_QUESTION_LIST)))
	{
		//列举目录请求
		XCHAR tszFilePath[XPATH_MAX] = {};
		XCHAR tszAlisPath[XPATH_MAX] = {};
		if (!Session_FTP_Get(lpszClientAddr, NULL, tszFilePath, tszAlisPath))
		{
			FTPProtocol_Parse_SendPacketEx(xhFTPContral, XENGINE_FTPROTOCOL_RESPONSE_503, tszSDBuffer, &nSDLen);
			XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, nNetType);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("FTP客户端:%s,请求列举目录:%s 失败,执行顺序错误"), lpszClientAddr, pSt_KeyValue->tszStrVlu);
			return false;
		}
		if (_tcsxlen(tszAlisPath) <= 0)
		{
			FTPProtocol_Parse_SendPacketEx(xhFTPContral, XENGINE_FTPROTOCOL_RESPONSE_503, tszSDBuffer, &nSDLen);
			XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, nNetType);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("FTP客户端:%s,请求列举目录:%s 失败,执行顺序错误"), lpszClientAddr, pSt_KeyValue->tszStrVlu);
			return false;
		}
		FTPProtocol_Parse_SendPacketEx(xhFTPContral, XENGINE_FTPROTOCOL_RESPONSE_150, tszSDBuffer, &nSDLen);
		XEngine_Net_SendMsg(lpszClientAddr, tszSDBuffer, nSDLen, nNetType);

		std::thread m_ThreadFTPFiles(XEngine_FTPFile_Thread, xstring(lpszClientAddr), xstring(tszFilePath), xstring(tszAlisPath));
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