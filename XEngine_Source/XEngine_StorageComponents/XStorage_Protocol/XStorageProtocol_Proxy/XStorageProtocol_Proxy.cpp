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
 参数.一：lpszMethod
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：方法名称
 参数.二：lpszPostUrl
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：用户提交的地址
 参数.三：lpszClientAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：用户发送的客户端地址
 参数.四：lpszUser
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：用户
 参数.五：lpszPass
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：密码
 参数.六：ptszMsgBuffer
  In/Out：Out
  类型：字符指针
  可空：N
  意思：转发数据包
 参数.七：pInt_MsgLen
  In/Out：Out
  类型：整数型
  可空：N
  意思：导出数据包大小
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CXStorageProtocol_Proxy::XStorageProtocol_Proxy_PacketBasicAuth(LPCTSTR lpszMethod, LPCTSTR lpszPostUrl, LPCTSTR lpszClientAddr, LPCTSTR lpszUser, LPCTSTR lpszPass, TCHAR* ptszMsgBuffer, int* pInt_MsgLen)
{
    XStorage_IsErrorOccur = FALSE;

    if ((NULL == lpszUser) || (NULL == lpszPass))
    {
        XStorage_IsErrorOccur = TRUE;
        XStorage_dwErrorCode = ERROR_XENGINE_XSTROGE_PROTOCOL_PARAMENT;
        return FALSE;
    }
    Json::Value st_JsonRoot;

	st_JsonRoot["lpszMethod"] = lpszMethod;
    st_JsonRoot["lpszPostUrl"] = lpszPostUrl;
    st_JsonRoot["lpszClientAddr"] = lpszClientAddr;
    st_JsonRoot["lpszUser"] = lpszUser;
    st_JsonRoot["lpszPass"] = lpszPass;

    *pInt_MsgLen = st_JsonRoot.toStyledString().length();
    _tcscpy(ptszMsgBuffer, st_JsonRoot.toStyledString().c_str());
    return TRUE;
}
/********************************************************************
函数名称：XStorageProtocol_Proxy_PacketUPDown
函数功能：上传下载完成代理通知协议
 参数.一：lpszFileName
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：文件的地址
 参数.二：lpszClientAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：用户客户端地址
 参数.三：nFileSize
  In/Out：In
  类型：整数型
  可空：N
  意思：文件大小
 参数.四：ptszMsgBuffer
  In/Out：Out
  类型：字符指针
  可空：N
  意思：数据包
 参数.五：pInt_MsgLen
  In/Out：Out
  类型：整数型
  可空：N
  意思：导出数据包大小
 参数.六：lpszFileHash
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：文件的HASH
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CXStorageProtocol_Proxy::XStorageProtocol_Proxy_PacketUPDown(LPCTSTR lpszFileName, LPCTSTR lpszClientAddr, __int64x nFileSize, TCHAR* ptszMsgBuffer, int* pInt_MsgLen, LPCTSTR lpszFileHash)
{
	XStorage_IsErrorOccur = FALSE;

	if ((NULL == lpszFileName) || (NULL == lpszClientAddr))
	{
		XStorage_IsErrorOccur = TRUE;
		XStorage_dwErrorCode = ERROR_XENGINE_XSTROGE_PROTOCOL_PARAMENT;
		return FALSE;
	}
	Json::Value st_JsonRoot;

	st_JsonRoot["lpszFileName"] = lpszFileName;
	st_JsonRoot["lpszClientAddr"] = lpszClientAddr;
	st_JsonRoot["nFileSize"] = nFileSize;
	if (NULL == lpszFileHash)
	{
		st_JsonRoot["lpszFileHash"];
	}
	else
	{
		st_JsonRoot["lpszFileHash"] = lpszFileHash;
	}

	*pInt_MsgLen = st_JsonRoot.toStyledString().length();
	_tcscpy(ptszMsgBuffer, st_JsonRoot.toStyledString().c_str());
	return TRUE;
}
/********************************************************************
函数名称：XStorageProtocol_Proxy_ParseUPDown
函数功能：解析上传下载通知协议
 参数.一：lpszMsgBuffer
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要解析的内容
 参数.二：nMsgLen
  In/Out：In
  类型：整数型
  可空：N
  意思：输入要解析的大小
 参数.三：ptszClientAddr
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出文件客户端地址
 参数.四：ptszFileName
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出文件名称
 参数.五：ptszFileHash
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出文件HASH
 参数.六：pInt_FileSize
  In/Out：Out
  类型：整数型
  可空：N
  意思：输出文件大小
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CXStorageProtocol_Proxy::XStorageProtocol_Proxy_ParseNotify(LPCTSTR lpszMsgBuffer, int nMsgLen, TCHAR* ptszClientAddr, TCHAR* ptszFileName, TCHAR* ptszFileHash, __int64x* pInt_FileSize)
{
	XStorage_IsErrorOccur = FALSE;

	if ((NULL == lpszMsgBuffer) || (NULL == ptszClientAddr))
	{
		XStorage_IsErrorOccur = TRUE;
		XStorage_dwErrorCode = ERROR_XENGINE_XSTROGE_PROTOCOL_PARAMENT;
		return FALSE;
	}
	Json::Value st_JsonRoot;
	Json::CharReaderBuilder st_JsonBuild;
	Json::CharReader* pSt_JsonReader(st_JsonBuild.newCharReader());

	JSONCPP_STRING st_JsonError;
	//解析JSON
	if (!pSt_JsonReader->parse(lpszMsgBuffer, lpszMsgBuffer + _tcslen(lpszMsgBuffer), &st_JsonRoot, &st_JsonError))
	{
		XStorage_IsErrorOccur = TRUE;
		XStorage_dwErrorCode = ERROR_XENGINE_XSTROGE_PROTOCOL_PARSE;
		return FALSE;
	}
	delete pSt_JsonReader;
	pSt_JsonReader = NULL;

	_tcscpy(ptszClientAddr, st_JsonRoot["lpszClientAddr"].asCString());
	_tcscpy(ptszFileName, st_JsonRoot["lpszFileName"].asCString());
	_tcscpy(ptszFileHash, st_JsonRoot["lpszFileHash"].asCString());
	*pInt_FileSize = st_JsonRoot["nFileSize"].asInt64();

	return TRUE;
}