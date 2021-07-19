#include "pch.h"
#include "P2XPClient_Socket.h"
/********************************************************************
//    Created:     2021/07/08  10:00:47
//    File Name:   D:\XEngine_Storage\XEngine_APPClient\StorageModule_P2PClient\P2XPClient_Socket\P2XPClient_Socket.cpp
//    File Path:   D:\XEngine_Storage\XEngine_APPClient\StorageModule_P2PClient\P2XPClient_Socket
//    File Base:   P2XPClient_Socket
//    File Ext:    cpp
//    Project:     XEngine(����ͨ������)
//    Author:      qyt
//    Purpose:     ����IO
//    History:
*********************************************************************/
CP2XPClient_Socket::CP2XPClient_Socket()
{
}
CP2XPClient_Socket::~CP2XPClient_Socket()
{
}
//////////////////////////////////////////////////////////////////////////
//                        ���к���
//////////////////////////////////////////////////////////////////////////
/********************************************************************
�������ƣ�P2XPClient_Socket_Create
�������ܣ�����һ������ͻ��˲������ӵ�ָ��������
 ����.һ��pxhNet
  In/Out��Out
  ���ͣ�������
  �ɿգ�N
  ��˼�����������õĿͻ��˾��
 ����.����lpszAddr
  In/Out��In
  ���ͣ������ַ�ָ��
  �ɿգ�N
  ��˼��Ҫ���ӵķ�������ַ
 ����.����nPort
  In/Out��In
  ���ͣ�������
  �ɿգ�N
  ��˼���������˿ں�
 ����.�ģ�bIsTcp
  In/Out��In
  ���ͣ��߼���
  �ɿգ�Y
  ��˼��ʹ��TCP����UDP
����ֵ
  ���ͣ��߼���
  ��˼���Ƿ�ɹ�
��ע��
*********************************************************************/
BOOL CP2XPClient_Socket::P2XPClient_Socket_Create(XNETHANDLE* pxhNet, LPCTSTR lpszAddr, int nPort, BOOL bIsTcp)
{
    P2XPClient_IsErrorOccur = FALSE;

    //������ȫ�Լ��
    if (NULL == lpszAddr)
    {
        P2XPClient_IsErrorOccur = TRUE;
        P2XPClient_dwErrorCode = ERROR_XENGINE_STORAGE_P2XPCLIENT_PARAMENT;
        return FALSE;
    }
    //����ռ�
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
    //�жϴ�������UDP����TCP
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
    //�������������߳�
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
�������ƣ�P2XPClient_Socket_Close
�������ܣ��ر�һ��ָ���Ŀͻ���
 ����.һ��xhNet
  In/Out��In
  ���ͣ�������
  �ɿգ�N
  ��˼��Ҫ�رյĿͻ��˵ľ��
����ֵ
  ���ͣ��߼���
  ��˼���Ƿ�رճɹ�
��ע��
*********************************************************************/
BOOL CP2XPClient_Socket::P2XPClient_Socket_Close(XNETHANDLE xhNet)
{
    P2XPClient_IsErrorOccur = FALSE;
    //��ʼ��ѯ
    unordered_map<XNETHANDLE, LPIONETWORK_THREADDATAS>::iterator stl_MapIterator = stl_MapClient.find(xhNet);
    if (stl_MapIterator != stl_MapClient.end())
    {
        stl_MapIterator->second->bIsRun = FALSE;

        if (NULL != stl_MapIterator->second->pSTDThread)
        {
            stl_MapIterator->second->pSTDThread->join();
        }
        //�ر�������Դ
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
�������ƣ�P2XPClient_Socket_SendClient
�������ܣ����Ϳͻ������ݰ�
 ����.һ��xhNet
  In/Out��In
  ���ͣ�������
  �ɿգ�N
  ��˼��Ҫ�����ĸ��ͻ���
 ����.����lpszMsgBuffer
  In/Out��In
  ���ͣ������ַ�ָ��
  �ɿգ�N
  ��˼�����͵����ݻ�����
 ����.����nLen
  In/Out��In
  ���ͣ�������
  �ɿգ�N
  ��˼���������ݵĳ���
����ֵ
  ���ͣ��߼���
  ��˼���Ƿ��ͳɹ�
��ע��
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
    //����ָ���ͻ���
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
�������ƣ�P2XPClient_Socket_SendService
�������ܣ��������������ݺ���
 ����.һ��lpszClientAddr
  In/Out��In
  ���ͣ������ַ�ָ��
  �ɿգ�N
  ��˼�����͸��ĸ��ͻ���
 ����.����lpszMsgBuffer
  In/Out��In
  ���ͣ������ַ�ָ��
  �ɿգ�N
  ��˼�����͵�����
 ����.����nLen
  In/Out��In
  ���ͣ�������
  �ɿգ�N
  ��˼�����͵����ݳ���
 ����.�ģ�bIsTcp
  In/Out��In
  ���ͣ��߼���
  �ɿգ�Y
  ��˼������TCP����UDP
����ֵ
  ���ͣ��߼���
  ��˼���Ƿ�ɹ�
��ע��
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
    //�жϷ��ͷ�ʽ
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
�������ƣ�P2XPClient_Socket_Destory
�������ܣ��������������
����ֵ
  ���ͣ��߼���
  ��˼���Ƿ�ɹ�
��ע����ͬʱ����TCP��UDP����
*********************************************************************/
BOOL CP2XPClient_Socket::P2XPClient_Socket_Destory()
{
    P2XPClient_IsErrorOccur = FALSE;

    NetCore_TCPSelect_StopEx(xhTCPSocket);
    NetCore_UDPSelect_Stop(xhUDPSocket);
    return TRUE;
}
/********************************************************************
�������ƣ�P2XPClient_Socket_Init
�������ܣ���ʼ�����������
 ����.һ��nTCPPort
  In/Out��In
  ���ͣ�������
  �ɿգ�N
  ��˼��Ҫ�󶨵�TCP�˿ں�
 ����.����nUDPPort
  In/Out��In
  ���ͣ�������
  �ɿգ�N
  ��˼��Ҫ�󶨵�UDP�˿ں�
����ֵ
  ���ͣ��߼���
  ��˼���Ƿ�ɹ�
��ע��
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
�������ƣ�P2XPClient_Socket_SetCallBack
�������ܣ����ÿͻ��˻ص�����
 ����.һ��fpCall_IOClient
  In/Out��In/Out
  ���ͣ��ص�����
  �ɿգ�N
  ��˼���ͻ������ݻص�����
 ����.����fpCall_IOService
  In/Out��In/Out
  ���ͣ��ص�����
  �ɿգ�N
  ��˼�����������ݻص�����
 ����.����lPCient
  In/Out��In/Out
  ���ͣ�������ָ��
  �ɿգ�Y
  ��˼���ص���������
 ����.�ģ�lPService
  In/Out��In/Out
  ���ͣ�������ָ��
  �ɿգ�Y
  ��˼���ص���������
����ֵ
  ���ͣ��߼���
  ��˼���Ƿ����óɹ�
��ע��
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
//                      �̺߳���
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
//                      �̺߳���
//////////////////////////////////////////////////////////////////////////
XHTHREAD CP2XPClient_Socket::P2XPClient_Socket_Thread(LPVOID lParam)
{
    IONETWORK_THREADDATAS* pSt_IONetWork = (IONETWORK_THREADDATAS*)lParam;
    CP2XPClient_Socket* pClass_IONetWork = (CP2XPClient_Socket*)pSt_IONetWork->lParam;

    //��ʼ��һЩ����
    TCHAR tszBufferMsg[XENGINE_P2XP_IONETWORK_BUFFER_LEN_MAX];
    TCHAR tszClientAddr[64];
    int nMsgLen;
    //�Ƿ��������
    while (pSt_IONetWork->bIsRun)
    {
        //���ñ���������Ϣ
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
