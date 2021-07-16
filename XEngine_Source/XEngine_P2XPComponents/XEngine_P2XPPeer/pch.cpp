#include "pch.h"
#include "P2XPPeer_Manage/P2XPPeer_Manage.h"
/********************************************************************
//    Created:     2021/07/08  13:16:52
//    File Name:   D:\XEngine_Storage\XEngine_Source\XEngine_P2XPComponents\XEngine_P2XPPeer\pch.cpp
//    File Path:   D:\XEngine_Storage\XEngine_Source\XEngine_P2XPComponents\XEngine_P2XPPeer
//    File Base:   pch
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     P2P节点管理器导出函数实现
//    History:
*********************************************************************/
BOOL PeerManage_IsErrorOccur = FALSE;
DWORD PeerManage_dwErrorCode = 0;
//////////////////////////////////////////////////////////////////////////
CNetEngine_P2XPPeerManage m_P2PPeerManage;
//////////////////////////////////////////////////////////////////////////
extern "C" DWORD P2XPPeer_GetLastError(int* pInt_ErrorCode)
{
	if (NULL != pInt_ErrorCode)
	{
		*pInt_ErrorCode = errno;
	}
	return PeerManage_dwErrorCode;
}
//////////////////////////////////////////////////////////////////////////
extern "C" BOOL P2XPPeer_Manage_Add(XENGINE_P2XP_PEERINFO st_PeerInfo)
{
	return m_P2PPeerManage.P2XPPeer_Manage_Add(st_PeerInfo);
}
extern "C" BOOL P2XPPeer_Manage_Get(LPCTSTR lpszAddr, XENGINE_P2XP_PEERINFO * pSt_PeerInfo /* = NULL */)
{
	return m_P2PPeerManage.P2XPPeer_Manage_Get(lpszAddr, pSt_PeerInfo);
}
extern "C" BOOL P2XPPeer_Manage_GetUser(LPCTSTR lpszUser, XENGINE_P2XP_PEERINFO * pSt_PeerInfo)
{
	return m_P2PPeerManage.P2XPPeer_Manage_GetUser(lpszUser, pSt_PeerInfo);
}
extern "C" BOOL P2XPPeer_Manage_GetLan(LPCTSTR lpszPubAddr, LPCTSTR lpszPriAddr, XENGINE_P2XPPEER_PROTOCOL * **pppSt_P2XPClient, int* pInt_ListCount)
{
	return m_P2PPeerManage.P2XPPeer_Manage_GetLan(lpszPubAddr, lpszPriAddr, pppSt_P2XPClient, pInt_ListCount);
}
extern "C" BOOL P2XPPeer_Manage_GetLList(LPCTSTR lpszPubAddr, TCHAR * **pppszP2XPClient, int* pInt_ListCount)
{
	return m_P2PPeerManage.P2XPPeer_Manage_GetLList(lpszPubAddr, pppszP2XPClient, pInt_ListCount);
}
extern "C" BOOL P2XPPeer_Manage_GetWList(TCHAR * **pppszP2XPClient, int* pInt_ListCount)
{
	return m_P2PPeerManage.P2XPPeer_Manage_GetWList(pppszP2XPClient, pInt_ListCount);
}
extern "C" BOOL P2XPPeer_Manage_Set(LPCTSTR lpszAddr, XENGINE_P2XP_PEERINFO st_PeerInfo)
{
	return m_P2PPeerManage.P2XPPeer_Manage_Set(lpszAddr, st_PeerInfo);
}
extern "C" BOOL P2XPPeer_Manage_Delete(LPCTSTR lpszUserName)
{
	return m_P2PPeerManage.P2XPPeer_Manage_Delete(lpszUserName);
}
extern "C" BOOL P2XPPeer_Manage_DelAll()
{
	return m_P2PPeerManage.P2XPPeer_Manage_DelAll();
}
extern "C" BOOL P2XPPeer_Manage_GetCount(int* pInt_PeerCount)
{
	return m_P2PPeerManage.P2XPPeer_Manage_GetCount(pInt_PeerCount);
}