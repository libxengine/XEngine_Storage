#pragma once
/********************************************************************
//    Created:     2021/10/26  09:39:57
//    File Name:   D:\XEngine_Storage\XEngine_Source\XEngine_StorageComponents\XStorage_P2XPPeer\P2XPPeer_Manage\P2XPPeer_Manage.h
//    File Path:   D:\XEngine_Storage\XEngine_Source\XEngine_StorageComponents\XStorage_P2XPPeer\P2XPPeer_Manage
//    File Base:   P2XPPeer_Manage
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     P2P节点管理器
//    History:
*********************************************************************/

class CP2XPPeer_Manage
{
public:
    CP2XPPeer_Manage();
    ~CP2XPPeer_Manage();
public:
    BOOL P2XPPeer_Manage_Add(XENGINE_P2XP_PEERINFO st_PeerInfo);                                      //向列表中添加一个节点
	BOOL P2XPPeer_Manage_Get(LPCTSTR lpszAddr, XENGINE_P2XP_PEERINFO* pSt_PeerInfo = NULL);           //查找指定用户名对应的节点
	BOOL P2XPPeer_Manage_GetUser(LPCTSTR lpszUser, XENGINE_P2XP_PEERINFO* pSt_PeerInfo = NULL);       //查找指定用户名对应的节点
    BOOL P2XPPeer_Manage_GetLan(LPCTSTR lpszPubAddr, LPCTSTR lpszPriAddr, XENGINE_P2XPPEER_PROTOCOL*** pppSt_P2XPClient, int* pInt_ListCount);        //获取公网下的局域网IP地址列表
    BOOL P2XPPeer_Manage_GetLList(LPCTSTR lpszPubAddr, TCHAR*** pppszP2XPClient, int* pInt_ListCount);
    BOOL P2XPPeer_Manage_GetWList(TCHAR*** pppszP2XPClient, int* pInt_ListCount);
    BOOL P2XPPeer_Manage_Set(LPCTSTR lpszAddr,XENGINE_P2XP_PEERINFO st_PeerInfo);                     //设置指定客户的节点信息
    BOOL P2XPPeer_Manage_Delete(LPCTSTR lpszUserName);                                                  //从列表中删除一个节点
    BOOL P2XPPeer_Manage_DelAll();                                                                      //清空节点列表
    BOOL P2XPPeer_Manage_GetCount(int *pInt_PeerCount);                                                 //获取有多少个节点
protected:
    BOOL P2XPPeer_Manage_AddLan(XENGINE_P2XPPEER_PROTOCOL st_ClientAddr);                                  //添加到局域网节点列表
    BOOL P2XPPeer_Manage_DelLan(XENGINE_P2XPPEER_PROTOCOL st_ClientAddr);                                  //从列表中删除一个节点
private:
    shared_mutex st_rwLocker;                      //节点读写保护区
private:
    unordered_map<string,LPNETENGINE_P2XP_PEERINFO> stl_MapPeerAddr;                //链接地址管理
    unordered_map<string, unordered_map<string, list<XENGINE_P2XPPEER_PROTOCOL> > > stl_MapClients;
};
