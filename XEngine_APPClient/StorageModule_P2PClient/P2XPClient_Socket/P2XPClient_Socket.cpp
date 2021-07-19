#include "pch.h"
#include "P2XPClient_Socket.h"
/********************************************************************
//    Created:     2021/07/08  10:00:47
//    File Name:   D:\XEngine_Storage\XEngine_APPClient\StorageModule_P2PClient\P2XPClient_Socket\P2XPClient_Socket.cpp
//    File Path:   D:\XEngine_Storage\XEngine_APPClient\StorageModule_P2PClient\P2XPClient_Socket
//    File Base:   P2XPClient_Socket
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     网络IO
//    History:
*********************************************************************/
CP2XPClient_Socket::CP2XPClient_Socket()
{
}
CP2XPClient_Socket::~CP2XPClient_Socket()
{
}
//////////////////////////////////////////////////////////////////////////
//                        公有函数
//////////////////////////////////////////////////////////////////////////
/********************************************************************
函数名称：P2XPClient_Socket_Create
函数功能：创建一个网络客户端并且链接到指定服务器
 参数.一：pxhNet
  In/Out：Out
  类型：网络句柄
  可空：N
  意思：导出创建好的客户端句柄
 参数.二：lpszAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：要链接的服务器地址
 参数.三：nPort
  In/Out：In
  类型：整数型
  可空：N
  意思：服务器端口号
 参数.四：bIsTcp
  In/Out：In
  类型：逻辑型
  可空：Y
  意思：使用TCP还是UDP
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CP2XPClient_Socket::P2XPClient_Socket_Create(XNETHANDLE* pxhNet, LPCTSTR lpszAddr, int nPort, BOOL bIsTcp)
{
    P2XPClient_IsErrorOccur = FALSE;

    //参数安全性检查
    if (NULL == lpszAddr)
    {
        P2XPClient_IsErrorOccur = TRUE;
        P2XPClient_dwErrorCode = ERROR_XENGINE_STORAGE_P2XPCLIENT_PARAMENT;
        return FALSE;
    }
    //申请空间
	IONETWORK_THREADDATAS* pSt_IONetWork = new IONETWORK_THREADDATAS;
    if (NULL == pSt_IONetWork)
    {
        P2XPClient_IsErrorOccur = TRUE;
        P2XPClient_dwErrorCode = ERROR_XENGINE_STORAGE_P2XPCLIENT_MALLOC;
        return FALSE;
    }
    memset(pSt_IONetWork, '\0', sizeof(IONETWORK_THREADDATAS));

    _tcscpy(pSt_IONetWork->tszClientAddr, lpszAddr);
    pSt_IONetWork->nRemotePort = nPort;
    pSt_IONetWork->bIsRun = TRUE;
    pSt_IONetWork->bIsTcp = bIsTcp;
    pSt_IONetWork->lParam = this;
    //判断创建的是UDP还是TCP
    if (bIsTcp)
    {
        if (!XClient_TCPSelect_Create(&pSt_IONetWork->hSocket, lpszAddr, nPort))
        {
            delete pSt_IONetWork;
            pSt_IONetWork = NULL;
            P2XPClient_IsErrorOccur = TRUE;
            P2XPClient_dwErrorCode = XClient_GetLastError();
            return FALSE;
        }
    }
    else
    {
        if (!XClient_UDPSelect_Create(&pSt_IONetWork->hSocket, lpszAddr, nPort))
        {
            delete pSt_IONetWork;
            pSt_IONetWork = NULL;
            P2XPClient_IsErrorOccur = TRUE;
            P2XPClient_dwErrorCode = XClient_GetLastError();
            return FALSE;
        }
    }
    //创建接受数据线程
    pSt_IONetWork->pSTDThread = make_shared<std::thread>(P2XPClient_Socket_Thread, pSt_IONetWork);
    if (!pSt_IONetWork->pSTDThread->joinable())
    {
        delete pSt_IONetWork;
        pSt_IONetWork = NULL;
        P2XPClient_IsErrorOccur = TRUE;
        P2XPClient_dwErrorCode = ERROR_XENGINE_STORAGE_P2XPCLIENT_THREAD;
        return FALSE;
    }
    *pxhNet = pSt_IONetWork->hSocket;
    stl_MapClient.insert(make_pair(*pxhNet, pSt_IONetWork));
    return TRUE;
}
/********************************************************************
函数名称：P2XPClient_Socket_Close
函数功能：关闭一个指定的客户端
 参数.一：xhNet
  In/Out：In
  类型：网络句柄
  可空：N
  意思：要关闭的客户端的句柄
返回值
  类型：逻辑型
  意思：是否关闭成功
备注：
*********************************************************************/
BOOL CP2XPClient_Socket::P2XPClient_Socket_Close(XNETHANDLE xhNet)
{
    P2XPClient_IsErrorOccur = FALSE;
    //开始查询
    unordered_map<XNETHANDLE, LPIONETWORK_THREADDATAS>::iterator stl_MapIterator = stl_MapClient.find(xhNet);
    if (stl_MapIterator != stl_MapClient.end())
    {
        stl_MapIterator->second->bIsRun = FALSE;

        if (NULL != stl_MapIterator->second->pSTDThread)
        {
            stl_MapIterator->second->pSTDThread->join();
        }
        //关闭网络资源
        if (stl_MapIterator->second->bIsTcp)
        {
            XClient_TCPSelect_Close(stl_MapIterator->second->hSocket);
        }
        else
        {
            XClient_UDPSelect_Close(stl_MapIterator->second->hSocket);
        }
        delete stl_MapIterator->second;
        stl_MapIterator->second = NULL;
        stl_MapClient.erase(stl_MapIterator);
    }
    return TRUE;
}
/********************************************************************
函数名称：P2XPClient_Socket_SendClient
函数功能：发送客户端数据包
 参数.一：xhNet
  In/Out：In
  类型：网络句柄
  可空：N
  意思：要操作哪个客户端
 参数.二：lpszMsgBuffer
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：发送的数据缓冲区
 参数.三：nLen
  In/Out：In
  类型：整数型
  可空：N
  意思：发送数据的长度
返回值
  类型：逻辑型
  意思：是否发送成功
备注：
*********************************************************************/
BOOL CP2XPClient_Socket::P2XPClient_Socket_SendClient(XNETHANDLE xhNet, LPCTSTR lpszMsgBuffer, int nLen)
{
    P2XPClient_IsErrorOccur = FALSE;

    if ((0 == nLen) || (NULL == lpszMsgBuffer))
    {
        P2XPClient_IsErrorOccur = TRUE;
        P2XPClient_dwErrorCode = ERROR_XENGINE_STORAGE_P2XPCLIENT_PARAMENT;
        return FALSE;
    }
    //查找指定客户端
    unordered_map<XNETHANDLE, LPIONETWORK_THREADDATAS>::const_iterator stl_MapIterator = stl_MapClient.find(xhNet);
    if (stl_MapIterator == stl_MapClient.end())
    {
        P2XPClient_IsErrorOccur = TRUE;
        P2XPClient_dwErrorCode = ERROR_XENGINE_STORAGE_P2XPCLIENT_NOTFOUND;
        return FALSE;
    }
    if (stl_MapIterator->second->bIsTcp)
    {
        if (!XClient_TCPSelect_SendMsg(stl_MapIterator->second->hSocket, lpszMsgBuffer, nLen))
        {
            P2XPClient_IsErrorOccur = TRUE;
            P2XPClient_dwErrorCode = XClient_GetLastError();
            return FALSE;
        }
    }
    else
    {
        if (!XClient_UDPSelect_SendMsg(stl_MapIterator->second->hSocket, lpszMsgBuffer, nLen))
        {
            P2XPClient_IsErrorOccur = TRUE;
            P2XPClient_dwErrorCode = XClient_GetLastError();
            return FALSE;
        }
    }
    return TRUE;
}
/********************************************************************
函数名称：P2XPClient_Socket_SendService
函数功能：服务器发送数据函数
 参数.一：lpszClientAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：发送给哪个客户端
 参数.二：lpszMsgBuffer
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：发送的数据
 参数.三：nLen
  In/Out：In
  类型：整数型
  可空：N
  意思：发送的数据长度
 参数.四：bIsTcp
  In/Out：In
  类型：逻辑型
  可空：Y
  意思：发送TCP还是UDP
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CP2XPClient_Socket::P2XPClient_Socket_SendService(LPCTSTR lpszClientAddr, LPCTSTR lpszMsgBuffer, int nLen, BOOL bIsTcp /* = TRUE */)
{
    P2XPClient_IsErrorOccur = FALSE;

    if ((NULL == lpszClientAddr) || (NULL == lpszMsgBuffer))
    {
        P2XPClient_IsErrorOccur = TRUE;
        P2XPClient_dwErrorCode = ERROR_XENGINE_STORAGE_P2XPCLIENT_PARAMENT;
        return FALSE;
    }
    //判断发送方式
    if (bIsTcp)
    {
        if (!NetCore_TCPSelect_SendEx(xhTCPSocket, lpszClientAddr, lpszMsgBuffer, nLen))
        {
            P2XPClient_IsErrorOccur = TRUE;
            P2XPClient_dwErrorCode = NetCore_GetLastError();
            return FALSE;
        }
    }
    else
    {
        int nPort = 0;
        TCHAR tszClientAddr[64];
        _tcscpy(tszClientAddr, lpszClientAddr);
        if (!BaseLib_OperatorString_SqliteNetAddr(tszClientAddr, &nPort))
        {
            P2XPClient_IsErrorOccur = TRUE;
            P2XPClient_dwErrorCode = BaseLib_GetLastError();
            return FALSE;
        }
        if (!NetCore_UDPSelect_SendTo(xhUDPSocket, lpszMsgBuffer, &nLen, tszClientAddr, nPort))
        {
            P2XPClient_IsErrorOccur = TRUE;
            P2XPClient_dwErrorCode = NetCore_GetLastError();
            return FALSE;
        }
    }
    return TRUE;
}
/********************************************************************
函数名称：P2XPClient_Socket_Destory
函数功能：销毁网络服务器
返回值
  类型：逻辑型
  意思：是否成功
备注：会同时销毁TCP和UDP服务
*********************************************************************/
BOOL CP2XPClient_Socket::P2XPClient_Socket_Destory()
{
    P2XPClient_IsErrorOccur = FALSE;

    NetCore_TCPSelect_StopEx(xhTCPSocket);
    NetCore_UDPSelect_Stop(xhUDPSocket);
    return TRUE;
}
/********************************************************************
函数名称：P2XPClient_Socket_Init
函数功能：初始化网络服务器
 参数.一：nTCPPort
  In/Out：In
  类型：整数型
  可空：N
  意思：要绑定的TCP端口号
 参数.二：nUDPPort
  In/Out：In
  类型：整数型
  可空：N
  意思：要绑定的UDP端口号
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CP2XPClient_Socket::P2XPClient_Socket_Init(int nTCPPort, int nUDPPort)
{
    P2XPClient_IsErrorOccur = FALSE;

    if (nTCPPort > 0)
    {
        if (!NetCore_TCPSelect_StartEx(&xhTCPSocket, nTCPPort))
        {
            P2XPClient_IsErrorOccur = TRUE;
            P2XPClient_dwErrorCode = NetCore_GetLastError();
            return FALSE;
        }
        NetCore_TCPSelect_SetCallBackEx(xhTCPSocket, P2XPClient_Socket_CBLogin, P2XPClient_Socket_CBRecv, P2XPClient_Socket_CBLeave, this, this, this);
    }
    if (nUDPPort > 0)
    {
        if (NULL == (xhUDPSocket = NetCore_UDPSelect_Init(nUDPPort, TRUE, P2XPClient_Socket_CBUDP, this)))
        {
            P2XPClient_IsErrorOccur = TRUE;
            P2XPClient_dwErrorCode = NetCore_GetLastError();
            return FALSE;
        }
    }
    return TRUE;
}
/********************************************************************
函数名称：P2XPClient_Socket_SetCallBack
函数功能：设置客户端回调函数
 参数.一：fpCall_IOClient
  In/Out：In/Out
  类型：回调函数
  可空：N
  意思：客户端数据回调函数
 参数.二：fpCall_IOService
  In/Out：In/Out
  类型：回调函数
  可空：N
  意思：服务器数据回调函数
 参数.三：lPCient
  In/Out：In/Out
  类型：无类型指针
  可空：Y
  意思：回调函数参数
 参数.四：lPService
  In/Out：In/Out
  类型：无类型指针
  可空：Y
  意思：回调函数参数
返回值
  类型：逻辑型
  意思：是否设置成功
备注：
*********************************************************************/
BOOL CP2XPClient_Socket::P2XPClient_Socket_SetCallBack(CALLBACK_XENGINE_P2XP_DISTRIBUTION_IOCLIENT_EVENTS fpCall_IOClient, CALLBACK_XENGINE_P2XP_DISTRIBUTION_IOSERVICE_EVENTS fpCall_IOService, LPVOID lPCient /* = NULL */, LPVOID lPService /* = NULL */)
{
    P2XPClient_IsErrorOccur = FALSE;

    m_lPClient = lPCient;
    m_lPService = lPService;
    lpCall_IOClient = fpCall_IOClient;
    lpCall_IOService = fpCall_IOService;
    return FALSE;
}
//////////////////////////////////////////////////////////////////////////
//                      线程函数
//////////////////////////////////////////////////////////////////////////
BOOL CALLBACK CP2XPClient_Socket::P2XPClient_Socket_CBLogin(LPCSTR lpszClientAddr, SOCKET hSocket, LPVOID lParam)
{
    CP2XPClient_Socket* pClass_This = (CP2XPClient_Socket*)lParam;
    pClass_This->lpCall_IOService(lpszClientAddr, NULL, 1, TRUE, pClass_This->m_lPService);
    return TRUE;
}
void CALLBACK CP2XPClient_Socket::P2XPClient_Socket_CBRecv(LPCSTR lpszClientAddr, SOCKET hSocket, LPCSTR lpszMsgBuffer, int nMsgLen, LPVOID lParam)
{
    CP2XPClient_Socket* pClass_This = (CP2XPClient_Socket*)lParam;
    pClass_This->lpCall_IOService(lpszClientAddr, lpszMsgBuffer, nMsgLen, TRUE, pClass_This->m_lPService);
}
void CALLBACK CP2XPClient_Socket::P2XPClient_Socket_CBLeave(LPCSTR lpszClientAddr, SOCKET hSocket, LPVOID lParam)
{
    CP2XPClient_Socket* pClass_This = (CP2XPClient_Socket*)lParam;
    pClass_This->lpCall_IOService(lpszClientAddr, NULL, 0, TRUE, pClass_This->m_lPService);
}
void CALLBACK CP2XPClient_Socket::P2XPClient_Socket_CBUDP(LPCTSTR lpszRecvAddr, SOCKET hSocket, LPCTSTR lpszMsgBuffer, int nLen, LPVOID lParam)
{
    CP2XPClient_Socket* pClass_This = (CP2XPClient_Socket*)lParam;
    pClass_This->lpCall_IOService(lpszRecvAddr, lpszMsgBuffer, nLen, FALSE, pClass_This->m_lPService);
}
//////////////////////////////////////////////////////////////////////////
//                      线程函数
//////////////////////////////////////////////////////////////////////////
XHTHREAD CP2XPClient_Socket::P2XPClient_Socket_Thread(LPVOID lParam)
{
    IONETWORK_THREADDATAS* pSt_IONetWork = (IONETWORK_THREADDATAS*)lParam;
    CP2XPClient_Socket* pClass_IONetWork = (CP2XPClient_Socket*)pSt_IONetWork->lParam;

    //初始化一些变量
    TCHAR tszBufferMsg[XENGINE_P2XP_IONETWORK_BUFFER_LEN_MAX];
    TCHAR tszClientAddr[64];
    int nMsgLen;
    //是否继续运行
    while (pSt_IONetWork->bIsRun)
    {
        //重置变量内容信息
        nMsgLen = XENGINE_P2XP_IONETWORK_BUFFER_LEN_MAX;
        memset(tszBufferMsg, '\0', XENGINE_P2XP_IONETWORK_BUFFER_LEN_MAX);
        memset(tszClientAddr, '\0', 64);
        if (pSt_IONetWork->bIsTcp)
        {
            if (!XClient_TCPSelect_RecvMsg(pSt_IONetWork->hSocket, tszBufferMsg, &nMsgLen))
            {
                continue;
            }
#ifdef _WINDOWS
#ifdef WIN64
            _stprintf_s(tszClientAddr, _T("%lld"), pSt_IONetWork->hSocket);
#else
            _stprintf_s(tszClientAddr, _T("%d"), pSt_IONetWork->hSocket);
#endif
#else
            _stprintf_s(tszClientAddr, _T("%d"), pSt_IONetWork->hSocket);
#endif
            pClass_IONetWork->lpCall_IOClient(tszClientAddr, tszBufferMsg, nMsgLen, TRUE, pClass_IONetWork->m_lPClient);
        }
        else
        {
            if (!XClient_UDPSelect_RecvMsg(pSt_IONetWork->hSocket, tszBufferMsg, &nMsgLen, tszClientAddr))
            {
                continue;
            }
            pClass_IONetWork->lpCall_IOClient(tszClientAddr, tszBufferMsg, nMsgLen, FALSE, pClass_IONetWork->m_lPClient);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    return 0;
}
