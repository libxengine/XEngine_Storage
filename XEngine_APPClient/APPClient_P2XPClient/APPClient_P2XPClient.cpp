#ifdef _MSC_BUILD
#include <windows.h>
#include <tchar.h>
#pragma comment(lib,"x86/XEngine_BaseLib/XEngine_BaseLib")
#pragma comment(lib,"x86/XEngine_Client/XClient_Socket")
#pragma comment(lib,"x86/XEngine_Core/XEngine_NetXApi")
#pragma comment(lib,"x86/XEngine_NetHelp/NetHelp_APIHelp")
#pragma comment(lib,"Ws2_32")
#pragma comment(lib,"../../XEngine_Source/Debug/jsoncpp")
#else
#include <sys/types.h>
#include <sys/socket.h>
#endif
#include <json/json.h>
#include <XEngine_Include/XEngine_CommHdr.h>
#include <XEngine_Include/XEngine_ProtocolHdr.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Define.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Error.h>
#include <XEngine_Include/XEngine_Client/XClient_Define.h>
#include <XEngine_Include/XEngine_Client/XClient_Error.h>
#include <XEngine_Include/XEngine_Core/NetXApi_Define.h>
#include <XEngine_Include/XEngine_Core/NetXApi_Error.h>
#include <XEngine_Include/XEngine_NetHelp/APIHelp_Define.h>
#include <XEngine_Include/XEngine_NetHelp/APIHelp_Error.h>
#include "../../XEngine_Source/XStorage_Protocol.h"

//需要优先配置XEngine
//WINDOWS使用VS2022 x86 debug 编译
//linux使用下面的命令编译
//g++ -std=c++17 -Wall -g APPClient_P2XPClient.cpp -o APPClient_P2XPClient.exe -I ../../XEngine_Source/XEngine_ThirdPart/jsoncpp -L /usr/local/lib/XEngine_Release/XEngine_BaseLib -L /usr/local/lib/XEngine_Release/XEngine_Core -L /usr/local/lib/XEngine_Release/XEngine_Client -L /usr/local/lib/XEngine_Release/XEngine_NetHelp -L ../../XEngine_Source/XEngine_ThirdPart/jsoncpp -lXEngine_BaseLib -lXEngine_OPenSsl -lXEngine_NetXApi -lXClient_Socket -lNetHelp_APIHelp -ljsoncpp

LPCTSTR lpszUserName = _T("123123aa");
LPCTSTR lpszAddr = _T("192.168.1.8");

TCHAR tszPublicAddr[128];
TCHAR tszPrivateAddr[128];

int nPort = 5103;
SOCKET m_hSocket;

int APPClient_P2XPLogin()
{
	Json::Value st_JsonRoot;
	TCHAR tszMsgBuffer[2048];
	XENGINE_PROTOCOLHDR st_ProtocolHdr;
	ENUM_XENGINE_NETXAPI_SOCKET_CONNECTTYPE dwNetType;

	memset(tszPublicAddr, '\0', sizeof(tszPublicAddr));
	memset(tszPrivateAddr, '\0', sizeof(tszPrivateAddr));
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
	memset(&st_ProtocolHdr, '\0', sizeof(XENGINE_PROTOCOLHDR));

	st_ProtocolHdr.wHeader = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_HEADER;
	st_ProtocolHdr.unOperatorType = ENUM_XENGINE_COMMUNICATION_PROTOCOL_TYPE_P2XP;
	st_ProtocolHdr.unOperatorCode = XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_P2XP_REQLOGIN;
	st_ProtocolHdr.byVersion = 2;
	st_ProtocolHdr.wTail = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_TAIL;

	//获取网络连接类型
	if (!NetXApi_Socket_GetNetConnectType(&dwNetType))
	{
		return -1;
	}
	int nListCount = 0;
	NETXAPI_CARDINFO** ppSt_APICard;
	NetXApi_Socket_GetCardInfo(&ppSt_APICard, &nListCount, AF_INET);
	APIHelp_NetWork_GetIPNet(tszPublicAddr);
	if (nListCount <= 0)
	{
		return -1;
	}
	//得到IP地址
	strcpy(tszPrivateAddr, ppSt_APICard[0]->tszIPAddr);
	BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_APICard, nListCount);

	st_JsonRoot["tszUserName"] = lpszUserName;
	st_JsonRoot["tszPrivateAddr"] = tszPrivateAddr;
	st_JsonRoot["tszPublicAddr"] = tszPublicAddr;
	st_JsonRoot["dwConnectType"] = (Json::Value::UInt)dwNetType;
	st_JsonRoot["dwPeerType"] = 0;
	st_ProtocolHdr.unPacketSize = st_JsonRoot.toStyledString().length();

	int nMsgLen = sizeof(XENGINE_PROTOCOLHDR) + st_ProtocolHdr.unPacketSize;
	memcpy(tszMsgBuffer, &st_ProtocolHdr, sizeof(XENGINE_PROTOCOLHDR));
	memcpy(tszMsgBuffer + sizeof(XENGINE_PROTOCOLHDR), st_JsonRoot.toStyledString().c_str(), st_ProtocolHdr.unPacketSize);

	if (!XClient_TCPSelect_SendMsg(m_hSocket, tszMsgBuffer, nMsgLen))
	{
		return -1;
	}

	nMsgLen = sizeof(tszMsgBuffer);
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
	XClient_TCPSelect_RecvMsg(m_hSocket, tszMsgBuffer, &nMsgLen, FALSE);

	printf("APPClient_P2XPLogin:%s\n", tszMsgBuffer + sizeof(XENGINE_PROTOCOLHDR));
	return 0;
}

int APPClient_P2XPList()
{
	Json::Value st_JsonRoot;
	TCHAR tszMsgBuffer[2048];
	XENGINE_PROTOCOLHDR st_ProtocolHdr;

	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
	memset(&st_ProtocolHdr, '\0', sizeof(XENGINE_PROTOCOLHDR));

	st_ProtocolHdr.wHeader = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_HEADER;
	st_ProtocolHdr.unOperatorType = ENUM_XENGINE_COMMUNICATION_PROTOCOL_TYPE_P2XP;
	st_ProtocolHdr.unOperatorCode = XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_P2XP_REQLANLIST;
	st_ProtocolHdr.byVersion = 2;
	st_ProtocolHdr.wTail = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_TAIL;

	st_JsonRoot["tszPublicAddr"] = tszPublicAddr;
	st_JsonRoot["tszPrivateAddr"] = tszPrivateAddr;

	st_ProtocolHdr.unPacketSize = st_JsonRoot.toStyledString().length();

	int nMsgLen = sizeof(XENGINE_PROTOCOLHDR) + st_ProtocolHdr.unPacketSize;
	memcpy(tszMsgBuffer, &st_ProtocolHdr, sizeof(XENGINE_PROTOCOLHDR));
	memcpy(tszMsgBuffer + sizeof(XENGINE_PROTOCOLHDR), st_JsonRoot.toStyledString().c_str(), st_ProtocolHdr.unPacketSize);

	if (!XClient_TCPSelect_SendMsg(m_hSocket, tszMsgBuffer, nMsgLen))
	{
		return -1;
	}

	nMsgLen = sizeof(tszMsgBuffer);
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
	XClient_TCPSelect_RecvMsg(m_hSocket, tszMsgBuffer, &nMsgLen, FALSE);

	printf("APPClient_P2XPList:%s\n", tszMsgBuffer + sizeof(XENGINE_PROTOCOLHDR));
	return 0;
}

int APPClient_P2XPGetUser()
{
	Json::Value st_JsonRoot;
	TCHAR tszMsgBuffer[2048];
	XENGINE_PROTOCOLHDR st_ProtocolHdr;

	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
	memset(&st_ProtocolHdr, '\0', sizeof(XENGINE_PROTOCOLHDR));

	st_ProtocolHdr.wHeader = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_HEADER;
	st_ProtocolHdr.unOperatorType = ENUM_XENGINE_COMMUNICATION_PROTOCOL_TYPE_P2XP;
	st_ProtocolHdr.unOperatorCode = XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_P2XP_REQUSERQUERY;
	st_ProtocolHdr.byVersion = 2;
	st_ProtocolHdr.wTail = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_TAIL;

	st_JsonRoot["tszUserName"] = lpszUserName;

	st_ProtocolHdr.unPacketSize = st_JsonRoot.toStyledString().length();

	int nMsgLen = sizeof(XENGINE_PROTOCOLHDR) + st_ProtocolHdr.unPacketSize;
	memcpy(tszMsgBuffer, &st_ProtocolHdr, sizeof(XENGINE_PROTOCOLHDR));
	memcpy(tszMsgBuffer + sizeof(XENGINE_PROTOCOLHDR), st_JsonRoot.toStyledString().c_str(), st_ProtocolHdr.unPacketSize);

	if (!XClient_TCPSelect_SendMsg(m_hSocket, tszMsgBuffer, nMsgLen))
	{
		return -1;
	}

	nMsgLen = sizeof(tszMsgBuffer);
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
	XClient_TCPSelect_RecvMsg(m_hSocket, tszMsgBuffer, &nMsgLen, FALSE);

	printf("APPClient_P2XPGetUser:%s\n", tszMsgBuffer + sizeof(XENGINE_PROTOCOLHDR));
	return 0;
}

int APPClient_P2XPConnect()
{
	Json::Value st_JsonRoot;
	TCHAR tszMsgBuffer[2048];
	XENGINE_PROTOCOLHDR st_ProtocolHdr;

	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
	memset(&st_ProtocolHdr, '\0', sizeof(XENGINE_PROTOCOLHDR));

	st_ProtocolHdr.wHeader = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_HEADER;
	st_ProtocolHdr.unOperatorType = ENUM_XENGINE_COMMUNICATION_PROTOCOL_TYPE_P2XP;
	st_ProtocolHdr.unOperatorCode = XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_P2XP_REQCONNECT;
	st_ProtocolHdr.byVersion = 2;
	st_ProtocolHdr.wTail = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_TAIL;

	st_JsonRoot["tszPublicAddr"] = tszPublicAddr;
	st_JsonRoot["tszPrivateAddr"] = tszPrivateAddr;

	st_ProtocolHdr.unPacketSize = st_JsonRoot.toStyledString().length();

	int nMsgLen = sizeof(XENGINE_PROTOCOLHDR) + st_ProtocolHdr.unPacketSize;
	memcpy(tszMsgBuffer, &st_ProtocolHdr, sizeof(XENGINE_PROTOCOLHDR));
	memcpy(tszMsgBuffer + sizeof(XENGINE_PROTOCOLHDR), st_JsonRoot.toStyledString().c_str(), st_ProtocolHdr.unPacketSize);

	if (!XClient_TCPSelect_SendMsg(m_hSocket, tszMsgBuffer, nMsgLen))
	{
		return -1;
	}

	nMsgLen = sizeof(tszMsgBuffer);
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
	XClient_TCPSelect_RecvMsg(m_hSocket, tszMsgBuffer, &nMsgLen, FALSE);

	printf("%s\n", tszMsgBuffer + sizeof(XENGINE_PROTOCOLHDR));
	return 0;
}

int main()
{
#ifdef _MSC_BUILD
	WSADATA st_WSAData;
	WSAStartup(MAKEWORD(2, 2), &st_WSAData);
#endif

	if (!XClient_TCPSelect_Create(&m_hSocket, lpszAddr, nPort))
	{
		return -1;
	}
	APPClient_P2XPLogin();
	APPClient_P2XPList();
	APPClient_P2XPGetUser();
	APPClient_P2XPConnect();

	XClient_TCPSelect_Close(m_hSocket);
	
#ifdef _MSC_BUILD
	WSACleanup();
#endif
	return 0;
}