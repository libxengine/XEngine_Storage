#include "pch.h"
#include "P2XPClient_Help.h"
/********************************************************************
//	Created:	2014/6/27  17:51
//	File Name: 	F:\NetEngine_Windows\NetEngine_SourceCode\NetEngine_P2xp\NetEngine_P2XPClient\P2XPClient_Help\P2XPClient_Help.cpp
//	File Path:	F:\NetEngine_Windows\NetEngine_SourceCode\NetEngine_P2xp\NetEngine_P2XPClient\P2XPClient_Help
//	File Base:	P2XPClient_Help
//	File Ext:	cpp
//  Project:    XyRySoft_NetEngine(网络通信引擎)
//	Author:		qyt 486179@qq.com
//	Purpose:	帮助类实现
//	History:
*********************************************************************/
CP2XPClient_Help::CP2XPClient_Help()
{
}
CP2XPClient_Help::~CP2XPClient_Help()
{
}
//////////////////////////////////////////////////////////////////////////
//                          公有函数
//////////////////////////////////////////////////////////////////////////
/********************************************************************
函数名称：P2XPClinet_Help_GetInfo
函数功能：初始化客户端节点信息
 参数.一：pSt_ClientInfo
  In/Out：Out
  类型：数据结构指针
  可空：N
  意思：导出获取到的数据信息结构
返回值
  类型：逻辑型
  意思：是否获取成功
备注：
*********************************************************************/
BOOL CP2XPClient_Help::P2XPClinet_Help_GetInfo(XENGINE_P2XPPEER_PROTOCOL *pSt_ClientInfo)
{
    P2XPClient_IsErrorOccur = FALSE;

    if (NULL == pSt_ClientInfo)
    {
        P2XPClient_IsErrorOccur = TRUE;
        P2XPClient_dwErrorCode = ERROR_XENGINE_STORAGE_P2XPCLIENT_PARAMENT;
        return FALSE;
    }
    //获取网络连接类型
	if (!NetXApi_Socket_GetNetConnectType((DWORD*)&pSt_ClientInfo->dwConnectType))
	{
		P2XPClient_IsErrorOccur = TRUE;
		P2XPClient_dwErrorCode = NetXApi_GetLastError();
		return FALSE;
	}
    //获取外网IP地址
    int nListCount = 0;
    APIHELP_NETCARD** ppSt_APICard;
    if (!APIHelp_NetWork_GetIPAddr(&ppSt_APICard, &nListCount, TRUE, pSt_ClientInfo->tszPublicAddr))
    {
        P2XPClient_IsErrorOccur = TRUE;
        P2XPClient_dwErrorCode = APIHelp_GetLastError();
        return FALSE;
    }
    if (nListCount <= 0)
    {
		P2XPClient_IsErrorOccur = TRUE;
		P2XPClient_dwErrorCode = ERROR_XENGINE_STORAGE_P2XPCLIENT_NOTIPADDR;
		return FALSE;
    }
    P2XPClinet_Help_GetIPAddr(ppSt_APICard[0]->tszIPAddr, pSt_ClientInfo->tszPrivateAddr);
    BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_APICard, nListCount);
    //判断连接类型来确定节点类型
    P2XPClinet_Help_GetType(&pSt_ClientInfo->dwPeerType);
    return TRUE;
}
/********************************************************************
函数名称：P2XPClinet_Help_GetType
函数功能：获取此电脑节点类型
 参数.一：pdwPeerType
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：导出此节点类型
返回值
  类型：逻辑型
  意思：是否获取成功
备注：
*********************************************************************/
BOOL CP2XPClient_Help::P2XPClinet_Help_GetType(WORD *pdwPeerType)
{
    P2XPClient_IsErrorOccur = FALSE;

    if ((NULL == pdwPeerType))
    {
        P2XPClient_IsErrorOccur = TRUE;
        P2XPClient_dwErrorCode = ERROR_XENGINE_STORAGE_P2XPCLIENT_PARAMENT;
        return FALSE;
    }
    SYSTEMAPI_CPU_INFOMATION st_CPUInfo;
    SYSTEMAPI_MEMORY_INFOMATION st_SysMemory;
    memset(&st_CPUInfo, '\0', sizeof(st_CPUInfo));
    memset(&st_SysMemory, '\0', sizeof(st_SysMemory));

    if (!SystemApi_HardWare_GetCpuInfomation(&st_CPUInfo))
    {
        P2XPClient_IsErrorOccur = TRUE;
        P2XPClient_dwErrorCode = SystemApi_GetLastError();
        return FALSE;
    }
    if (!SystemApi_System_GetMemoryUsage(&st_SysMemory, XENGINE_SYSTEMSDK_API_SYSTEM_SIZE_GB))
    {
        P2XPClient_IsErrorOccur = TRUE;
        P2XPClient_dwErrorCode = SystemApi_GetLastError();
        return FALSE;
    }
    if ((st_CPUInfo.nCpuSpeed > 3) && (st_CPUInfo.nCpuNumber > 8) && (st_SysMemory.dwMemory_Total > 8))
    {
        *pdwPeerType = XENGINE_P2XP_PEER_TYPE_SUPER;
    }
    else
    {
        *pdwPeerType = XENGINE_P2XP_PEER_TYPE_NORMAL;
    }
    return TRUE;
}
/********************************************************************
函数名称：P2XPClient_Help_UDPHold
函数功能：UDP打洞函数
 参数.一：lpszAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入对方IP地址
 参数.二：nPort
  In/Out：In
  类型：整数型
  可空：N
  意思：输入对方端口
返回值
  类型：逻辑型
  意思：是否成功
备注：使用此函数后,对方可以使用这个IP和端口为本机器发送数据
*********************************************************************/
BOOL CP2XPClient_Help::P2XPClient_Help_UDPHold(LPCTSTR lpszAddr, int nPort)
{
    P2XPClient_IsErrorOccur = FALSE;

    SOCKET m_Socket;
    if (!XClient_UDPSelect_Create(&m_Socket))
    {
        P2XPClient_IsErrorOccur = TRUE;
        P2XPClient_dwErrorCode = XClient_GetLastError();
        return FALSE;
    }
    if (!XClient_UDPSelect_Bind(m_Socket,nPort))
    {
        P2XPClient_IsErrorOccur = TRUE;
        P2XPClient_dwErrorCode = XClient_GetLastError();
        XClient_UDPSelect_Close(m_Socket);
        return FALSE;
    }
    LPCTSTR lpszMsgBuffer = _T("Hello");
    if (!XClient_UDPSelect_SendMsg(m_Socket, lpszMsgBuffer, _tcslen(lpszMsgBuffer), lpszAddr, nPort))
    {
        P2XPClient_IsErrorOccur = TRUE;
        P2XPClient_dwErrorCode = XClient_GetLastError();
        XClient_UDPSelect_Close(m_Socket);
        return FALSE;
    }
    XClient_UDPSelect_Close(m_Socket);

    return TRUE;
}
//////////////////////////////////////////////////////////////////////////
//                    保护函数
//////////////////////////////////////////////////////////////////////////
BOOL CP2XPClient_Help::P2XPClinet_Help_GetIPAddr(LPCTSTR lpszNetAddr, TCHAR *ptszNetAddr)
{
    P2XPClient_IsErrorOccur = FALSE;

    TCHAR tszNetAddr[128];
    memset(tszNetAddr,'\0',sizeof(tszNetAddr));
    _tcscpy(tszNetAddr,lpszNetAddr);

    TCHAR *ptszString = _tcstok(tszNetAddr,_T(";"));
    while (NULL != ptszString)
    {
        if (ptszString[_tcslen(ptszString) - 1] != '1')
        {
            _tcscpy(ptszNetAddr,ptszString);
            break;
        }
        ptszString = _tcstok(NULL, _T(";"));
    }
    return TRUE;
}
