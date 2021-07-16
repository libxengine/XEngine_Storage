#pragma once
/********************************************************************
//    Created:     2021/07/08  13:15:42
//    File Name:   D:\XEngine_Storage\XEngine_Source\XEngine_P2XPComponents\XEngine_P2XPPeer\P2XPPeer_Error.h
//    File Path:   D:\XEngine_Storage\XEngine_Source\XEngine_P2XPComponents\XEngine_P2XPPeer
//    File Base:   P2XPPeer_Error
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     P2P节点管理器导出错误
//    History:
*********************************************************************/
/************************************************************************/
/*                    节点管理器错误                                    */
/************************************************************************/
#define ERROR_XENGINE_P2XP_PEER_MANAGE_ADD_ISEXIST 0xB0E0001           //用户已经存在
#define ERROR_XENGINE_P2XP_PEER_MANAGE_ADD_MALLOC 0xB0E0002            //申请内存失败
#define ERROR_XENGINE_P2XP_PEER_MANAGE_GET_PARAMENT 0xB0E0010          //获取用户信息失败,参数错误
#define ERROR_XENGINE_P2XP_PEER_MANAGE_GET_NOTFOUND 0xB0E0011          //没有找到用户
#define ERROR_XENGINE_P2XP_PEER_MANAGE_SET_PARAMENT 0xB0E0020          //参数错误
#define ERROR_XENGINE_P2XP_PEER_MANAGE_SET_NOTFOUND 0xB0E0021          //没有找到用户
#define ERROR_XENGINE_P2XP_PEER_MANAGE_GETWLIST_PARAMENT 0xB0E0030     //参数错误
#define ERROR_XENGINE_P2XP_PEER_MANAGE_GETWLIST_NOTFOUND 0xB0E0031     //没有找到
#define ERROR_XENGINE_P2XP_PEER_MANAGE_DELETE_PARAMENT 0xB0E0040       //参数错误
#define ERROR_XENGINE_P2XP_PEER_MANAGE_DELETE_NOTFOUND 0xB0E0041       //没有找到用户
#define ERROR_XENGINE_P2XP_PEER_MANAGE_GETLAN_PARAMENT 0xB0E0050       //参数错误
#define ERROR_XENGINE_P2XP_PEER_MANAGE_GETLAN_NOTPUBADDRR 0xB0E0051    //公网IP地址没有找到
#define ERROR_XENGINE_P2XP_PEER_MANAGE_GETLAN_NOTPRIADDR 0xB0E0052     //私有网络IP地址没有找到
#define ERROR_XENGINE_P2XP_PEER_MANAGE_GETLAN_NOLIST 0xB0E0053         //没有数据存在
#define ERROR_XENGINE_P2XP_PEER_MANAGE_GETLAN_NOTFOUND 0xB0E0054       //没有找到链接地址
#define ERROR_XENGINE_P2XP_PEER_MANAGE_GETUSER_PARAMENT 0xB0E0060      //参数错误
#define ERROR_XENGINE_P2XP_PEER_MANAGE_GETUSER_NOTFOUND 0xB0E0061      //没有找到
#define ERROR_XENGINE_P2XP_PEER_MANAGE_GETLLIST_PARAMENT 0xB0E0070     //获取列表失败,参数错误
#define ERROR_XENGINE_P2XP_PEER_MANAGE_GETLLIST_NOTFOUND 0xB0E0071     //没有找到指定列表
