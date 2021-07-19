#pragma once
/********************************************************************
//    Created:     2021/07/08  10:00:07
//    File Name:   D:\XEngine_Storage\XEngine_APPClient\StorageModule_P2PClient\P2XPClient_Socket\P2XPClient_Socket.h
//    File Path:   D:\XEngine_Storage\XEngine_APPClient\StorageModule_P2PClient\P2XPClient_Socket
//    File Base:   P2XPClient_Socket
//    File Ext:    h
//    Project:     XEngine(����ͨ������)
//    Author:      qyt
//    Purpose:     ����IO
//    History:
*********************************************************************/
#define XENGINE_P2XP_IONETWORK_BUFFER_LEN_MAX 2048
//////////////////////////////////////////////////////////////////////////
typedef struct tag_IONetWork_ThreadDatas
{
	shared_ptr<std::thread> pSTDThread;
	TCHAR tszClientAddr[64];                                              //Զ�̵�ַ
	SOCKET hSocket;                                                       //������
	int nRemotePort;                                                      //Զ�����ӵĶ˿�
	BOOL bIsRun;                                                          //�Ƿ�����
	BOOL bIsTcp;                                                          //�Ƿ���TCP
	LPVOID lParam;                                                        //ָ�����ָ��
}IONETWORK_THREADDATAS, * LPIONETWORK_THREADDATAS;
//////////////////////////////////////////////////////////////////////////
class CP2XPClient_Socket
{
public:
	CP2XPClient_Socket();
	~CP2XPClient_Socket();
public:
	BOOL P2XPClient_Socket_Create(XNETHANDLE* pxhNet, LPCTSTR lpszAddr, int nPort, BOOL bIsTcp = TRUE);
	BOOL P2XPClient_Socket_Close(XNETHANDLE xhNet);
	BOOL P2XPClient_Socket_SendClient(XNETHANDLE xhNet, LPCTSTR lpszMsgBuffer, int nLen);
	BOOL P2XPClient_Socket_SendService(LPCTSTR lpszClientAddr, LPCTSTR lpszMsgBuffer, int nLen, BOOL bIsTcp = TRUE);
	BOOL P2XPClient_Socket_Destory();
public:
	BOOL P2XPClient_Socket_Init(int nTCPPort, int nUDPPort);
	BOOL P2XPClient_Socket_SetCallBack(CALLBACK_XENGINE_P2XP_DISTRIBUTION_IOCLIENT_EVENTS fpCall_IOClient, CALLBACK_XENGINE_P2XP_DISTRIBUTION_IOSERVICE_EVENTS fpCall_IOService, LPVOID lPCient = NULL, LPVOID lPService = NULL);
protected:
	static BOOL CALLBACK P2XPClient_Socket_CBLogin(LPCSTR lpszClientAddr, SOCKET hSocket, LPVOID lParam);    //�пͻ����룬�����ַ����ͻ���ַ�Ͷ˿�
	static void CALLBACK P2XPClient_Socket_CBRecv(LPCSTR lpszClientAddr, SOCKET hSocket, LPCSTR lpszMsgBuffer, int nMsgLen, LPVOID lParam);     //���ܵ����ݣ������ַ����Ŀͻ���ַ�ͽ��ܵ�������
	static void CALLBACK P2XPClient_Socket_CBLeave(LPCSTR lpszClientAddr, SOCKET hSocket, LPVOID lParam);    //�ͻ��뿪�����ؿͻ��뿪�ĵ�ַ�Ͷ˿ں���
	static void CALLBACK P2XPClient_Socket_CBUDP(LPCTSTR lpszRecvAddr, SOCKET hSocket, LPCTSTR lpszMsgBuffer, int nLen, LPVOID lParam);
protected:
	static XHTHREAD P2XPClient_Socket_Thread(LPVOID lParam);
private:
	LPVOID m_lPClient;
	LPVOID m_lPService;
	CALLBACK_XENGINE_P2XP_DISTRIBUTION_IOCLIENT_EVENTS lpCall_IOClient;
	CALLBACK_XENGINE_P2XP_DISTRIBUTION_IOSERVICE_EVENTS lpCall_IOService;
private:
	XNETHANDLE xhTCPSocket;
	XHANDLE xhUDPSocket;
private:
	unordered_map<XNETHANDLE, LPIONETWORK_THREADDATAS> stl_MapClient;    //���ݶ˿�ӳ���
};
