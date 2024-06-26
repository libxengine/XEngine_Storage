﻿#include "pch.h"
#include "Protocol_StoragePacket.h"
/********************************************************************
//    Created:     2021/10/25  14:43:05
//    File Name:   D:\XEngine_Storage\XEngine_Source\StorageModule_Protocol\Protocol_Packet\Protocol_StoragePacket.cpp
//    File Path:   D:\XEngine_Storage\XEngine_Source\StorageModule_Protocol\Protocol_Packet
//    File Base:   Protocol_StoragePacket
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     存储服务协议打包类
//    History:
*********************************************************************/
CProtocol_StoragePacket::CProtocol_StoragePacket()
{

}
CProtocol_StoragePacket::~CProtocol_StoragePacket()
{

}
//////////////////////////////////////////////////////////////////////////
//                           公有函数
//////////////////////////////////////////////////////////////////////////
/********************************************************************
函数名称：Protocol_StoragePacket_QueryFile
函数功能：查询回复打包协议
 参数.一：ptszMsgBuffer
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出组好包的请求缓冲区
 参数.二：pInt_MsgLen
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出缓冲区大小
 参数.三：pppSt_DBFile
  In/Out：In
  类型：三级指针
  可空：N
  意思：输入文件列表,参数内存由用户管理
 参数.四：nListCount
  In/Out：In
  类型：整数型
  可空：N
  意思：输入文件列表个数
 参数.五：lpszTimeStart
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入查询请求的开始时间
 参数.六：lpszTimeEnd
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入查询请求的结束时间
 参数.七：xhToken
  In/Out：In
  类型：句柄
  可空：Y
  意思：输入返回的TOKEN,如果需要的话
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CProtocol_StoragePacket::Protocol_StoragePacket_QueryFile(XCHAR* ptszMsgBuffer, int* pInt_MsgLen, XSTORAGECORE_DBFILE*** pppSt_DBFile, int nListCount, LPCXSTR lpszTimeStart /* = NULL */, LPCXSTR lpszTimeEnd /* = NULL */, XNETHANDLE xhToken /* = 0 */)
{
    Protocol_IsErrorOccur = false;

    if ((NULL == ptszMsgBuffer) || (NULL == pInt_MsgLen) || (NULL == pppSt_DBFile))
    {
        Protocol_IsErrorOccur = true;
        Protocol_dwErrorCode = ERROR_XENGINE_STORAGE_PROTOCOL_PARAMENT;
        return false;
    }
    Json::Value st_JsonRoot;
    Json::Value st_JsonArray;
    //JSON赋值
    for (int i = 0; i < nListCount; i++)
    {
        Json::Value st_JsonObject;

        st_JsonObject["tszBuckKey"] = (*pppSt_DBFile)[i]->tszBuckKey;
        st_JsonObject["tszFilePath"] = (*pppSt_DBFile)[i]->st_ProtocolFile.tszFilePath;
        st_JsonObject["tszFileName"] = (*pppSt_DBFile)[i]->st_ProtocolFile.tszFileName;
        st_JsonObject["tszFileUser"] = (*pppSt_DBFile)[i]->st_ProtocolFile.tszFileUser;
        st_JsonObject["tszFileHash"] = (*pppSt_DBFile)[i]->st_ProtocolFile.tszFileHash;
        st_JsonObject["tszFileTime"] = (*pppSt_DBFile)[i]->st_ProtocolFile.tszFileTime;
        st_JsonObject["nFileSize"] = (Json::Value::Int64)(*pppSt_DBFile)[i]->st_ProtocolFile.nFileSize;
        //只有在P2P下取文件列表才有效
        if (_tcsxlen((*pppSt_DBFile)[i]->tszTableName) > 0)
        {
            st_JsonObject["tszTableName"] = (*pppSt_DBFile)[i]->tszTableName;
        }
        st_JsonArray.append(st_JsonObject);
    }
    st_JsonRoot["Count"] = nListCount;
    st_JsonRoot["List"] = st_JsonArray;
    if (NULL != lpszTimeStart)
    {
        st_JsonRoot["lpszTimeStart"] = lpszTimeStart;
    }
    if (NULL != lpszTimeEnd)
    {
        st_JsonRoot["lpszTimeEnd"] = lpszTimeEnd;
    }
    if (0 != xhToken)
    {
        st_JsonRoot["xhToken"] = (Json::Value::UInt64)xhToken;
    }
    st_JsonRoot["Code"] = 0;
    st_JsonRoot["Msg"] = _X("ok");
    //打包输出信息
    *pInt_MsgLen = st_JsonRoot.toStyledString().length();
    memcpy(ptszMsgBuffer, st_JsonRoot.toStyledString().c_str(), *pInt_MsgLen);
    return true;
}
/********************************************************************
函数名称：Protocol_StoragePacket_Info
函数功能：返回信息获取请求打包函数
 参数.一：ptszMsgBuffer
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出打好包的缓冲区内容
 参数.二：pInt_MsgLen
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出缓冲区大小
 参数.三：pppSt_DLInfo
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：下载信息列表
 参数.四：pppSt_UPInfo
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：上传信息列表
 参数.五：nDLCount
  In/Out：In
  类型：整数型
  可空：N
  意思：下载列表个数
 参数.六：nUPCount
  In/Out：In
  类型：整数型
  可空：N
  意思：上传列表个数
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CProtocol_StoragePacket::Protocol_StoragePacket_Info(XCHAR* ptszMsgBuffer, int* pInt_MsgLen, SESSION_STORAGEINFO*** pppSt_DLInfo, SESSION_STORAGEINFO*** pppSt_UPInfo, int nDLCount, int nUPCount)
{
	Protocol_IsErrorOccur = false;

	if ((NULL == pppSt_DLInfo) || (NULL == pppSt_UPInfo))
	{
		Protocol_IsErrorOccur = true;
		Protocol_dwErrorCode = ERROR_XENGINE_STORAGE_PROTOCOL_PARAMENT;
		return false;
	}
	Json::Value st_JsonRoot;
	Json::Value st_JsonDLArray;
    Json::Value st_JsonUPArray;

	for (int i = 0; i < nDLCount; i++)
	{
		Json::Value st_JsonObject;
		st_JsonObject["tszClientAddr"] = (*pppSt_DLInfo)[i]->tszClientAddr;
        st_JsonObject["tszFilePath"] = (*pppSt_DLInfo)[i]->tszFileDir;
        st_JsonObject["ullCount"] = (Json::Value::Int64)(*pppSt_DLInfo)[i]->ullCount;
        st_JsonObject["ullRWCount"] = (Json::Value::Int64)(*pppSt_DLInfo)[i]->ullRWCount;
        st_JsonObject["ullRWLen"] = (Json::Value::Int64)(*pppSt_DLInfo)[i]->ullRWLen;
        st_JsonObject["ullPosStart"] = (Json::Value::Int64)(*pppSt_DLInfo)[i]->ullPosStart;
        st_JsonObject["ullPosEnd"] = (Json::Value::Int64)(*pppSt_DLInfo)[i]->ullPosEnd;

        st_JsonDLArray.append(st_JsonObject);
	}
	for (int i = 0; i < nUPCount; i++)
	{
		Json::Value st_JsonObject;
		st_JsonObject["tszClientAddr"] = (*pppSt_DLInfo)[i]->tszClientAddr;
		st_JsonObject["tszFilePath"] = (*pppSt_DLInfo)[i]->tszFileDir;
		st_JsonObject["ullCount"] = (Json::Value::Int64)(*pppSt_DLInfo)[i]->ullCount;
		st_JsonObject["ullRWCount"] = (Json::Value::Int64)(*pppSt_DLInfo)[i]->ullRWCount;
		st_JsonObject["ullRWLen"] = (Json::Value::Int64)(*pppSt_DLInfo)[i]->ullRWLen;
		st_JsonObject["ullPosStart"] = (Json::Value::Int64)(*pppSt_DLInfo)[i]->ullPosStart;
		st_JsonObject["ullPosEnd"] = (Json::Value::Int64)(*pppSt_DLInfo)[i]->ullPosEnd;

        st_JsonUPArray.append(st_JsonObject);
	}
	st_JsonRoot["DLCount"] = st_JsonDLArray.size();
    st_JsonRoot["UPCount"] = st_JsonUPArray.size();

	st_JsonRoot["DLList"] = st_JsonDLArray;
    st_JsonRoot["UPList"] = st_JsonUPArray;

	st_JsonRoot["Code"] = 0;
	st_JsonRoot["Msg"] = _X("ok");
	//打包输出信息
	*pInt_MsgLen = st_JsonRoot.toStyledString().length();
	memcpy(ptszMsgBuffer, st_JsonRoot.toStyledString().c_str(), *pInt_MsgLen);
	return true;
}
/********************************************************************
函数名称：XStorageProtocol_Core_REPQueryDir
函数功能：查询用户目录
 参数.一：ptszMsgBuffer
  In/Out：Out
  类型：字符指针
  可空：N
  意思：导出组好包的协议缓冲区
 参数.二：pInt_MsgLen
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：导出缓冲区大小
 参数.三：pppszListEnum
  In/Out：In
  类型：三级指针
  可空：N
  意思：输入查找到的文件夹,这个内存由用户管理
 参数.四：nListCount
  In/Out：In
  类型：整数型指针
  可空：N
  意思：输入文件夹个数
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CProtocol_StoragePacket::Protocol_StoragePacket_DirOperator(XCHAR* ptszMsgBuffer, int* pInt_MsgLen, XCHAR*** pppszListEnum, int nListCount)
{
    Protocol_IsErrorOccur = false;

    if ((NULL == ptszMsgBuffer) || (NULL == pInt_MsgLen))
    {
        Protocol_IsErrorOccur = true;
        Protocol_dwErrorCode = ERROR_XENGINE_STORAGE_PROTOCOL_PARAMENT;
        return false;
    }
    Json::Value st_JsonRoot;
    Json::Value st_JsonArray;

    for (int i = 0; i < nListCount; i++)
    {
        Json::Value st_JsonObject;
        st_JsonObject["tszFilePath"] = (*pppszListEnum)[i];
        st_JsonArray.append(st_JsonObject);
    }
    st_JsonRoot["Count"] = st_JsonArray.size();
    st_JsonRoot["List"] = st_JsonArray;
    st_JsonRoot["Code"] = 0;
    st_JsonRoot["Msg"] = _X("ok");
    //打包输出信息
    *pInt_MsgLen = st_JsonRoot.toStyledString().length();
    memcpy(ptszMsgBuffer, st_JsonRoot.toStyledString().c_str(), *pInt_MsgLen);
    return true;
}
/********************************************************************
函数名称：Protocol_StoragePacket_BasicAuth
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
bool CProtocol_StoragePacket::Protocol_StoragePacket_BasicAuth(LPCXSTR lpszMethod, LPCXSTR lpszPostUrl, LPCXSTR lpszClientAddr, LPCXSTR lpszUser, LPCXSTR lpszPass, XCHAR* ptszMsgBuffer, int* pInt_MsgLen)
{
    Protocol_IsErrorOccur = false;

    if ((NULL == lpszUser) || (NULL == lpszPass))
    {
        Protocol_IsErrorOccur = true;
        Protocol_dwErrorCode = ERROR_XENGINE_STORAGE_PROTOCOL_PARAMENT;
        return false;
    }
    Json::Value st_JsonRoot;

    st_JsonRoot["lpszMethod"] = lpszMethod;
    st_JsonRoot["lpszPostUrl"] = lpszPostUrl;
    st_JsonRoot["lpszClientAddr"] = lpszClientAddr;
    st_JsonRoot["lpszUser"] = lpszUser;
    st_JsonRoot["lpszPass"] = lpszPass;

    *pInt_MsgLen = st_JsonRoot.toStyledString().length();
    _tcsxcpy(ptszMsgBuffer, st_JsonRoot.toStyledString().c_str());
    return true;
}
/********************************************************************
函数名称：Protocol_StoragePacket_UPDown
函数功能：上传下载完成代理通知协议
 参数.一：ptszMsgBuffer
  In/Out：Out
  类型：字符指针
  可空：N
  意思：数据包
 参数.二：pInt_MsgLen
  In/Out：Out
  类型：整数型
  可空：N
  意思：导出数据包大小
 参数.三：lpszBuckKey
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入文件所属BUCKET
 参数.四：lpszFileName
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：文件的地址
 参数.五：lpszClientAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：用户客户端地址
 参数.六：nFileSize
  In/Out：In
  类型：整数型
  可空：N
  意思：文件大小
 参数.七：bDown
  In/Out：In
  类型：逻辑型
  可空：N
  意思：是上传还是下载
 参数.八：lpszFileHash
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：文件的HASH
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CProtocol_StoragePacket::Protocol_StoragePacket_UPDown(XCHAR* ptszMsgBuffer, int* pInt_MsgLen, LPCXSTR lpszBuckKey, LPCXSTR lpszFileName, LPCXSTR lpszClientAddr, __int64x nFileSize, bool bDown, LPCXSTR lpszFileHash /* = NULL */)
{
    Protocol_IsErrorOccur = false;

    if ((NULL == lpszFileName) || (NULL == lpszClientAddr))
    {
        Protocol_IsErrorOccur = true;
        Protocol_dwErrorCode = ERROR_XENGINE_STORAGE_PROTOCOL_PARAMENT;
        return false;
    }
    Json::Value st_JsonRoot;

    st_JsonRoot["bDown"] = bDown;
    st_JsonRoot["lpszBuckKey"] = lpszBuckKey;
    st_JsonRoot["lpszFileName"] = lpszFileName;
    st_JsonRoot["lpszClientAddr"] = lpszClientAddr;
    st_JsonRoot["nFileSize"] = (Json::Value::Int64)nFileSize;
    if (NULL == lpszFileHash)
    {
        st_JsonRoot["lpszFileHash"];
    }
    else
    {
        st_JsonRoot["lpszFileHash"] = lpszFileHash;
    }

    *pInt_MsgLen = st_JsonRoot.toStyledString().length();
    _tcsxcpy(ptszMsgBuffer, st_JsonRoot.toStyledString().c_str());
    return true;
}
/********************************************************************
函数名称：Protocol_StoragePacket_REQFile
函数功能：查询文件请求函数
 参数.一：ptszMsgBuffer
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出组好包的请求缓冲区
 参数.二：pInt_MsgLen
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出缓冲区大小
 参数.三：lpszFileName
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入要查询的文件名
 参数.四：lpszFileHash
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入要查询的文件MD5
 参数.五：xhToken
  In/Out：In
  类型：句柄
  可空：Y
  意思：消息的TOKEN
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CProtocol_StoragePacket::Protocol_StoragePacket_REQFile(XCHAR* ptszMsgBuffer, int* pInt_MsgLen, LPCXSTR lpszFileName /* = NULL */, LPCXSTR lpszFileHash /* = NULL */, XNETHANDLE xhToken /* = 0 */)
{
	Protocol_IsErrorOccur = false;

	if ((NULL == ptszMsgBuffer) || (NULL == pInt_MsgLen))
	{
		Protocol_IsErrorOccur = true;
		Protocol_dwErrorCode = ERROR_XENGINE_STORAGE_PROTOCOL_PARAMENT;
		return false;
	}
	Json::Value st_JsonRoot;
	if (NULL != lpszFileName)
	{
		st_JsonRoot["lpszFileName"] = lpszFileName;
	}
	if (NULL != lpszFileHash)
	{
		st_JsonRoot["lpszFileHash"] = lpszFileHash;
	}
	if (0 != xhToken)
	{
		st_JsonRoot["xhToken"] = (Json::Value::UInt64)xhToken;
	}
	st_JsonRoot["unOperatorType"] = ENUM_XENGINE_COMMUNICATION_PROTOCOL_TYPE_STORAGE;
	st_JsonRoot["unOperatorCode"] = XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_STORAGE_REQQUERY;
	//打包输出信息
	*pInt_MsgLen = st_JsonRoot.toStyledString().length();
	memcpy(ptszMsgBuffer, st_JsonRoot.toStyledString().c_str(), *pInt_MsgLen);
	return true;
}
/********************************************************************
函数名称：Protocol_StoragePacket_Action
函数功能：动作脚本封装器
 参数.一：ptszMsgBuffer
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出组好包的请求缓冲区
 参数.二：pInt_MsgLen
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出缓冲区大小
 参数.三：xhToken
  In/Out：In
  类型：句柄
  可空：N
  意思：输入管理器句柄
 参数.四：pSt_ActionInfo
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：输入要打包的信息
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CProtocol_StoragePacket::Protocol_StoragePacket_Action(XCHAR* ptszMsgBuffer, int* pInt_MsgLen, XNETHANDLE xhToken, XENGINE_ACTIONINFO* pSt_ActionInfo)
{
	Protocol_IsErrorOccur = false;

	if ((NULL == ptszMsgBuffer) || (NULL == pInt_MsgLen))
	{
		Protocol_IsErrorOccur = true;
		Protocol_dwErrorCode = ERROR_XENGINE_STORAGE_PROTOCOL_PARAMENT;
		return false;
	}
	Json::Value st_JsonRoot;

    st_JsonRoot["xhToken"] = (Json::Value::UInt64)xhToken;
	st_JsonRoot["tszFileName"] = pSt_ActionInfo->tszFileName;
	st_JsonRoot["tszFileUrl"] = pSt_ActionInfo->tszFileUrl;
    st_JsonRoot["byType"] = pSt_ActionInfo->byType;
    st_JsonRoot["tszBucketStr"] = pSt_ActionInfo->tszBucketStr;
	//打包输出信息
	*pInt_MsgLen = st_JsonRoot.toStyledString().length();
	memcpy(ptszMsgBuffer, st_JsonRoot.toStyledString().c_str(), *pInt_MsgLen);
	return true;
}