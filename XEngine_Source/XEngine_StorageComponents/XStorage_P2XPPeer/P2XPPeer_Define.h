﻿#pragma once
/********************************************************************
//    Created:     2021/07/08  13:15:34
//    File Name:   D:\XEngine_Storage\XEngine_Source\XEngine_P2XPComponents\XEngine_P2XPPeer\P2XPPeer_Define.h
//    File Path:   D:\XEngine_Storage\XEngine_Source\XEngine_P2XPComponents\XEngine_P2XPPeer
//    File Base:   P2XPPeer_Define
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     节点管理器导出定义
//    History:
*********************************************************************/
//////////////////////////////////////////////////////////////////////////
//                       导出数据结构
//////////////////////////////////////////////////////////////////////////
//计时器
typedef struct tag_P2XP_Timer_Information
{
    __int64x dwUserTime;                                                  //用户存在时间
    __int64x dwPacketTime;                                                //包时间,拿到包后的存活时间
    __int64x dwKeepAlive;                                                 //保活计时器时间,经过网络时间
    unsigned short int usTTL : 1;                                         //路由数
}P2XP_TIMER_INFOMATION, *LPP2XP_TIMER_INFOMATION;
//一个节点信息
typedef struct tag_NetEngine_P2XPPeer_Info
{
    XENGINE_P2XPPEER_PROTOCOL st_PeerAddr;                                //P2P通信的时候使用的地址（客户方使用）
    P2XP_TIMER_INFOMATION st_PeerTimer;                                   //节点时间信息
    XENGINE_IPADDRINFO st_IPAddrInfo;                                     //IP地址信息
    BOOL bIsLogin;
}XENGINE_P2XP_PEERINFO,*LPNETENGINE_P2XP_PEERINFO;
//////////////////////////////////////////////////////////////////////////
//                       导出函数
//////////////////////////////////////////////////////////////////////////
extern "C" DWORD P2XPPeer_GetLastError(int *pInt_ErrorCode = NULL);
/************************************************************************/
/*                       节点管理器导出函数                             */
/************************************************************************/
/********************************************************************
函数名称：P2XPPeer_Manage_Add
函数功能：添加一个客户到一个节点信息
 参数.一：pSt_PeerInfo
  In/Out：In
  类型：结构体指针
  可空：N
  意思：要添加的节点信息
返回值
  类型：逻辑型
  意思：是否添加成功
备注：
*********************************************************************/
extern "C" BOOL P2XPPeer_Manage_Add(XENGINE_P2XP_PEERINFO st_PeerInfo);
/********************************************************************
函数名称：P2XPPeer_Manage_Get
函数功能：获取P2P节点对应信息
 参数.一：lpszAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：要查询的地址
 参数.二：pSt_PeerInfo
  In/Out：Out
  类型：数据结构指针
  可空：Y
  意思：如果为空将不导出对应的数据
返回值
  类型：逻辑型
  意思：是否查找成功
备注：
*********************************************************************/
extern "C" BOOL P2XPPeer_Manage_Get(LPCSTR lpszAddr,XENGINE_P2XP_PEERINFO *pSt_PeerInfo = NULL);
/********************************************************************
函数名称：P2XPPeer_Manage_GetUser
函数功能：通过用户名获取节点信息
 参数.一：lpszUser
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：要搜索的用户的 标识符
 参数.二：pSt_PeerInfo
  In/Out：Out
  类型：数据结构指针
  可空：Y
  意思：导出获取到的信息
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL P2XPPeer_Manage_GetUser(LPCSTR lpszUser, XENGINE_P2XP_PEERINFO *pSt_PeerInfo = NULL);
/********************************************************************
函数名称：P2XPPeer_Manage_GetLan
函数功能：获取局域网IP地址列表
 参数.一：lpszPubAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：要查找的局域网相同公网的IP地址
 参数.二：lpszPriAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要获取同一公网下不同私有网段的IP列表
 参数.三：pppSt_P2XPClient
  In/Out：Out
  类型：三级指针
  可空：N
  意思：导出局域网IP地址列表
 参数.四：pInt_ListCount
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出地址列表个数
返回值
  类型：逻辑型
  意思：是否成功
备注：参数三需要调用基础库的BaseLib_OperatorMemory_Free函数进行内存释放
*********************************************************************/
extern "C" BOOL P2XPPeer_Manage_GetLan(LPCSTR lpszPubAddr, LPCSTR lpszPriAddr, XENGINE_P2XPPEER_PROTOCOL * **pppSt_P2XPClient, int* pInt_ListCount);
/********************************************************************
函数名称：P2XPPeer_Manage_GetLList
函数功能：获取同一公网下所有局域网主IP列表
 参数.一：lpszPubAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要获取的IP地址
 参数.二：pppszP2XPClient
  In/Out：Out
  类型：三级指针
  可空：N
  意思：输出内网主IP地址列表
 参数.三：pInt_ListCount
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出列表个数
返回值
  类型：逻辑型
  意思：是否成功
备注：参数二需要调用基础库的BaseLib_OperatorMemory_Free函数进行内存释放
*********************************************************************/
extern "C" BOOL P2XPPeer_Manage_GetLList(LPCSTR lpszPubAddr, CHAR*** pppszP2XPClient, int* pInt_ListCount);
/********************************************************************
函数名称：P2XPPeer_Manage_GetWList
函数功能：获取所有连接到的公网地址列表
 参数.一：pppszP2XPClient
  In/Out：Out
  类型：三级指针
  可空：N
  意思：输出地址列表
 参数.二：pInt_ListCount
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出列表个数
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL P2XPPeer_Manage_GetWList(CHAR*** pppszP2XPClient, int* pInt_ListCount);
/********************************************************************
函数名称：P2XPPeer_Manage_GetAllList
函数功能：获取所有客户端
 参数.一：pppszP2XPClient
  In/Out：Out
  类型：三级指针
  可空：N
  意思：输出客户端列表
 参数.二：pInt_ListCount
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出客户端个数
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL P2XPPeer_Manage_GetAllList(TCHAR*** pppszP2XPClient, int* pInt_ListCount);
/********************************************************************
函数名称：P2XPPeer_Manage_Set
函数功能：设置指定客户的节点信息
 参数.一：lpszAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：要设置的客户
 参数.二：st_PeerInfo
  In/Out：In
  类型：结构体
  可空：N
  意思：要设置的节点信息
返回值
  类型：逻辑型
  意思：是否设置成功
备注：
*********************************************************************/
extern "C" BOOL P2XPPeer_Manage_Set(LPCSTR lpszAddr,XENGINE_P2XP_PEERINFO st_PeerInfo);
/********************************************************************
函数名称：P2XPPeer_Manage_Delete
函数功能：删除一个指定的节点
 参数.一：pszUserName
  In/Out：In
  类型：字符串指针
  可空：N
  意思：要删除的节点名称
返回值
  类型：逻辑型
  意思：是否成功删除
备注：
*********************************************************************/
extern "C" BOOL P2XPPeer_Manage_Delete(LPCSTR lpszUserName);
/********************************************************************
函数名称：P2XPPeer_Manage_DelAll
函数功能：清空节点列表数据
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL P2XPPeer_Manage_DelAll();
/********************************************************************
函数名称：P2XPPeer_Manage_GetCount
函数功能：获取节点个数
 参数.一：pInt_PeerCount
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：导出获取到的个数
返回值
  类型：逻辑型
  意思：是否成功获取到节点个数
备注：
*********************************************************************/
extern "C" BOOL P2XPPeer_Manage_GetCount(int *pInt_PeerCount);