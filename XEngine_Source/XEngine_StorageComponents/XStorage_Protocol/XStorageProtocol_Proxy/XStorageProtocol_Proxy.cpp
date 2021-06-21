#include "pch.h"
#include "XStorageProtocol_Proxy.h"
/********************************************************************
//    Created:     2021/06/21  10:22:51
//    File Name:   D:\XEngine_Storage\XEngine_Source\XEngine_StorageComponents\XStorage_Protocol\XStorageProtocol_Proxy\XStorageProtocol_Proxy.cpp
//    File Path:   D:\XEngine_Storage\XEngine_Source\XEngine_StorageComponents\XStorage_Protocol\XStorageProtocol_Proxy
//    File Base:   XStorageProtocol_Proxy
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     代理转发类
//    History:
*********************************************************************/
CXStorageProtocol_Proxy::CXStorageProtocol_Proxy()
{

}
CXStorageProtocol_Proxy::~CXStorageProtocol_Proxy()
{

}
//////////////////////////////////////////////////////////////////////////
//                           公有函数
//////////////////////////////////////////////////////////////////////////
/********************************************************************
函数名称：XStorageProtocol_Proxy_PacketBasicAuth
函数功能：基本验证协议代理转发
 参数.一：lpszPostUrl
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：用户提交的地址
 参数.二：lpszClientAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：用户发送的客户端地址
 参数.三：lpszUser
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：用户
 参数.四：lpszPass
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：密码
 参数.五：ptszMsgBuffer
  In/Out：Out
  类型：字符指针
  可空：N
  意思：转发数据包
 参数.六：pInt_MsgLen
  In/Out：Out
  类型：整数型
  可空：N
  意思：导出数据包大小
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CXStorageProtocol_Proxy::XStorageProtocol_Proxy_PacketBasicAuth(LPCTSTR lpszPostUrl, LPCTSTR lpszClientAddr, LPCTSTR lpszUser, LPCTSTR lpszPass, TCHAR* ptszMsgBuffer, int* pInt_MsgLen)
{
    XStorage_IsErrorOccur = FALSE;

    if ((NULL == lpszUser) || (NULL == lpszPass))
    {
        XStorage_IsErrorOccur = TRUE;
        XStorage_dwErrorCode = ERROR_XENGINE_XSTROGE_PROTOCOL_COMM_PARAMENT;
        return FALSE;
    }
    Json::Value st_JsonRoot;

    st_JsonRoot["lpszPostUrl"] = lpszPostUrl;
    st_JsonRoot["lpszClientAddr"] = lpszClientAddr;
    st_JsonRoot["lpszUser"] = lpszUser;
    st_JsonRoot["lpszPass"] = lpszPass;

    *pInt_MsgLen = st_JsonRoot.toStyledString().length();
    _tcscpy(ptszMsgBuffer, st_JsonRoot.toStyledString().c_str());
    return TRUE;
}