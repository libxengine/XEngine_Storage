#include "pch.h"
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
函数名称：Protocol_StoragePacket_HTTPPacket
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
 参数.三：nCode
  In/Out：In
  类型：整数型
  可空：Y
  意思：输入HTTP错误码
 参数.四：lpszMSGInfo
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入返回的消息信息
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CProtocol_StoragePacket::Protocol_StoragePacket_HTTPPacket(XCHAR* ptszMsgBuffer, int* pInt_MsgLen, int nCode /* = 0 */, LPCXSTR lpszMSGInfo /* = NULL */)
{
	Protocol_IsErrorOccur = false;

	if ((NULL == ptszMsgBuffer) || (NULL == pInt_MsgLen))
	{
		Protocol_IsErrorOccur = true;
		Protocol_dwErrorCode = ERROR_XENGINE_STORAGE_PROTOCOL_PARAMENT;
		return false;
	}
	Json::Value st_JsonRoot;
	st_JsonRoot["code"] = nCode;
	if (NULL != lpszMSGInfo)
	{
		st_JsonRoot["msg"] = lpszMSGInfo;
	}
	//打包输出信息
	*pInt_MsgLen = st_JsonRoot.toStyledString().length();
	memcpy(ptszMsgBuffer, st_JsonRoot.toStyledString().c_str(), *pInt_MsgLen);
	return true;
}
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
	st_JsonRoot["code"] = 0;
	st_JsonRoot["msg"] = _X("success");
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

	st_JsonRoot["code"] = 0;
	st_JsonRoot["msg"] = _X("success");
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
	st_JsonRoot["code"] = 0;
	st_JsonRoot["msg"] = _X("success");
    //打包输出信息
    *pInt_MsgLen = st_JsonRoot.toStyledString().length();
    memcpy(ptszMsgBuffer, st_JsonRoot.toStyledString().c_str(), *pInt_MsgLen);
    return true;
}
/********************************************************************
函数名称：Protocol_StoragePacket_BasicAuth
函数功能：基本验证协议代理转发
 参数.一：lpszRequestType
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：请求的业务类型
 参数.二：lpszMethod
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：方法名称
 参数.三：lpszPostUrl
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：用户提交的地址
 参数.四：lpszClientAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：用户发送的客户端地址
 参数.五：lpszUser
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：用户
 参数.六：lpszPass
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：密码
 参数.七：ptszMsgBuffer
  In/Out：Out
  类型：字符指针
  可空：N
  意思：转发数据包
 参数.八：pInt_MsgLen
  In/Out：Out
  类型：整数型
  可空：N
  意思：导出数据包大小
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CProtocol_StoragePacket::Protocol_StoragePacket_BasicAuth(int nRequestType, LPCXSTR lpszMethod, LPCXSTR lpszPostUrl, LPCXSTR lpszClientAddr, LPCXSTR lpszUser, LPCXSTR lpszPass, XCHAR* ptszMsgBuffer, int* pInt_MsgLen)
{
    Protocol_IsErrorOccur = false;

    if ((NULL == lpszUser) || (NULL == lpszPass))
    {
        Protocol_IsErrorOccur = true;
        Protocol_dwErrorCode = ERROR_XENGINE_STORAGE_PROTOCOL_PARAMENT;
        return false;
    }
    Json::Value st_JsonRoot;

	st_JsonRoot["Type"] = nRequestType;
    st_JsonRoot["lpszMethod"] = lpszMethod;
    st_JsonRoot["lpszPostUrl"] = lpszPostUrl;
    st_JsonRoot["lpszClientAddr"] = lpszClientAddr;
    st_JsonRoot["lpszUser"] = lpszUser;
    st_JsonRoot["lpszPass"] = lpszPass;
	st_JsonRoot["code"] = 0;
	st_JsonRoot["msg"] = _X("success");
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
	st_JsonRoot["code"] = 0;
	st_JsonRoot["msg"] = _X("success");
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
	st_JsonRoot["code"] = 0;
	st_JsonRoot["msg"] = _X("success");
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
	st_JsonRoot["code"] = 0;
	st_JsonRoot["msg"] = _X("success");
	//打包输出信息
	*pInt_MsgLen = st_JsonRoot.toStyledString().length();
	memcpy(ptszMsgBuffer, st_JsonRoot.toStyledString().c_str(), *pInt_MsgLen);
	return true;
}
/********************************************************************
函数名称：Protocol_StoragePacket_Bucket
函数功能：获取bucket信息
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
 参数.三：pStl_ListBucket
  In/Out：In
  类型：STL容器指针
  可空：N
  意思：输入要打包的信息
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CProtocol_StoragePacket::Protocol_StoragePacket_Bucket(XCHAR* ptszMsgBuffer, int* pInt_MsgLen, list<XENGINE_STORAGEBUCKET>* pStl_ListBucket)
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

	auto stl_ListIterator = pStl_ListBucket->begin();
	for (; stl_ListIterator != pStl_ListBucket->end(); stl_ListIterator++)
	{
		Json::Value st_JsonObject;
		Json::Value st_JsonSub;

		st_JsonObject["bEnable"] = stl_ListIterator->bEnable;
		st_JsonObject["nLevel"] = stl_ListIterator->nLevel;
		st_JsonObject["tszBuckSize"] = stl_ListIterator->tszBuckSize;
		st_JsonObject["tszBuckKey"] = stl_ListIterator->tszBuckKey;
		st_JsonObject["tszFilePath"] = stl_ListIterator->tszFilePath;

		st_JsonSub["bCreateDir"] = stl_ListIterator->st_PermissionFlags.bCreateDir;
		st_JsonSub["bRewrite"] = stl_ListIterator->st_PermissionFlags.bRewrite;
		st_JsonSub["bUPLimit"] = stl_ListIterator->st_PermissionFlags.bUPLimit;
		st_JsonSub["bUPReady"] = stl_ListIterator->st_PermissionFlags.bUPReady;
		st_JsonObject["st_PermissionFlags"] = st_JsonSub;
		st_JsonArray.append(st_JsonObject);
	}
	st_JsonRoot["code"] = 0;
	st_JsonRoot["msg"] = "success";
	st_JsonRoot["size"] = st_JsonArray.size();
	st_JsonRoot["array"] = st_JsonArray;
	//打包输出信息
	*pInt_MsgLen = st_JsonRoot.toStyledString().length();
	memcpy(ptszMsgBuffer, st_JsonRoot.toStyledString().c_str(), *pInt_MsgLen);
	return true;
}
/********************************************************************
函数名称：Protocol_StoragePacket_Notfound
函数功能：没有找到
 参数.一：ptszMsgBuffer
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出打好包的XML数据
 参数.二：pInt_MsgLen
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出大小
 参数.三：lpszFileName
  In/Out：Out
  类型：常量字符指针
  可空：N
  意思：输入要打包的数据
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CProtocol_StoragePacket::Protocol_StoragePacket_Notfound(XCHAR* ptszMsgBuffer, int* pInt_MsgLen, LPCXSTR lpszFileName)
{
	Protocol_IsErrorOccur = false;

	if ((NULL == ptszMsgBuffer) || (NULL == pInt_MsgLen))
	{
		Protocol_IsErrorOccur = true;
		Protocol_dwErrorCode = ERROR_XENGINE_STORAGE_PROTOCOL_PARAMENT;
		return false;
	}
	// 创建一个 XML 文档
	XMLDocument m_XMLDocument;
	// XML 声明
	XMLDeclaration* pSt_XMLDeclaration = m_XMLDocument.NewDeclaration(R"(xml version="1.0" encoding="utf-8")");
	m_XMLDocument.InsertFirstChild(pSt_XMLDeclaration);

	// 根元素 <multistatus>
	XMLElement* pSt_XMLRoot = m_XMLDocument.NewElement("d:error");
	pSt_XMLRoot->SetAttribute("xmlns:d", "DAV:");
	m_XMLDocument.InsertEndChild(pSt_XMLRoot);

	// 子元素 <href>
	XMLElement* pSt_XMLHerf = m_XMLDocument.NewElement("d:href");
	pSt_XMLHerf->SetText(lpszFileName);
	pSt_XMLRoot->InsertEndChild(pSt_XMLHerf);
	// 子元素 <status>
	XMLElement* pSt_XMLStatus = m_XMLDocument.NewElement("d:status");
	pSt_XMLStatus->SetText(_X("HTTP/1.1 404 Not Found"));
	pSt_XMLRoot->InsertEndChild(pSt_XMLStatus);
	// 子元素 <responsedescription>
	XMLElement* pSt_XMLResponseDesc = m_XMLDocument.NewElement("d:responsedescription");
	pSt_XMLResponseDesc->SetText(_X("Not found"));
	pSt_XMLRoot->InsertEndChild(pSt_XMLResponseDesc);
	// 将 XML 数据保存到字符串
	XMLPrinter m_XMLPrinter;
	m_XMLDocument.Print(&m_XMLPrinter);

	*pInt_MsgLen = m_XMLPrinter.CStrSize() - 1;
	memcpy(ptszMsgBuffer, m_XMLPrinter.CStr(), *pInt_MsgLen);
	return true;
}
/********************************************************************
函数名称：Protocol_StoragePacket_WDPropfind
函数功能：propfind协议打包处理函数
 参数.一：ptszMsgBuffer
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出打好包的XML数据
 参数.二：pInt_MsgLen
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出大小
 参数.三：ppptszListFile
  In/Out：In
  类型：三级指针
  可空：N
  意思：输入打包的文件列表
 参数.四：nFileCount
  In/Out：In
  类型：整数型
  可空：N
  意思：输入列表个数
 参数.五：lpszBucketPath
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入BUCKET名称
 参数.六：lpszBucketKey
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入BUCKET的KEY
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CProtocol_StoragePacket::Protocol_StoragePacket_WDPropfind(XCHAR* ptszMsgBuffer, int* pInt_MsgLen, XCHAR*** ppptszListFile, int nFileCount, LPCXSTR lpszBucketPath, LPCXSTR lpszBucketKey)
{
	Protocol_IsErrorOccur = false;

	if ((NULL == ptszMsgBuffer) || (NULL == pInt_MsgLen))
	{
		Protocol_IsErrorOccur = true;
		Protocol_dwErrorCode = ERROR_XENGINE_STORAGE_PROTOCOL_PARAMENT;
		return false;
	}
	// 创建一个 XML 文档
	XMLDocument m_XMLDocument;
	// XML 声明
    XMLDeclaration* pSt_XMLDeclaration = m_XMLDocument.NewDeclaration(R"(xml version="1.0" encoding="utf-8")");
	m_XMLDocument.InsertFirstChild(pSt_XMLDeclaration);

	// 根元素 <multistatus>
	XMLElement* pSt_XMLRoot = m_XMLDocument.NewElement("d:multistatus");
	pSt_XMLRoot->SetAttribute("xmlns:d", "DAV:");
	m_XMLDocument.InsertEndChild(pSt_XMLRoot);
	
    for (int i = 0; i < nFileCount; i++)
    {
		XCHAR tszFileName[XPATH_MAX] = {};
		SYSTEMAPI_FILE_ATTR st_FileAttr = {};
		BaseLib_String_GetFileAndPath((*ppptszListFile)[i], NULL, tszFileName);
		SystemApi_File_GetFileAttr((*ppptszListFile)[i], &st_FileAttr);

		XCHAR tszGMTTime[64] = {};
		BaseLib_Time_GMTTime(tszGMTTime, st_FileAttr.nModifyTime);

		// 子元素 <response>
		XMLElement* pSt_XMLResponse = m_XMLDocument.NewElement("d:response");
		pSt_XMLRoot->InsertEndChild(pSt_XMLResponse);
        //文件
        XCHAR tszSourceStr[128] = {};
        XCHAR tszDestStr[128] = {};
        XCHAR tszFileAlis[XPATH_MAX] = {};
        _tcsxcpy(tszFileAlis, (*ppptszListFile)[i] + 1);

        APIHelp_Api_UrlStr(tszSourceStr, (*ppptszListFile)[i]);
        APIHelp_Api_UrlStr(tszDestStr, (*ppptszListFile)[i]);

        int nRLen = 0;
        BaseLib_String_Replace(tszFileAlis, &nRLen, lpszBucketPath + 2, lpszBucketKey, true);
        XMLElement* pSt_XMLhref = m_XMLDocument.NewElement("d:href");
        pSt_XMLhref->SetText(tszFileAlis);
        pSt_XMLResponse->InsertEndChild(pSt_XMLhref);
        //属性
		XMLElement* pSt_XMLPropstat = m_XMLDocument.NewElement("d:propstat");
        pSt_XMLResponse->InsertEndChild(pSt_XMLPropstat);
        //属性内容
		XMLElement* pSt_XMLProp = m_XMLDocument.NewElement("d:prop");
        pSt_XMLPropstat->InsertEndChild(pSt_XMLProp);
		//属性名称
		XMLElement* pSt_XMLPropName = m_XMLDocument.NewElement("d:displayname");
		if (_tcsxlen(tszFileName) <= 0)
		{
			APIHelp_Api_GetLastName(tszFileName, (*ppptszListFile)[i]);
		}
		pSt_XMLPropName->SetText(tszFileName);
        pSt_XMLProp->InsertEndChild(pSt_XMLPropName);

		XCHAR tszFileSize[128] = {};
		_xstprintf(tszFileSize, _X("%llu"), st_FileAttr.nFileSize);
		//属性大小
		XMLElement* pSt_XMLLength = m_XMLDocument.NewElement("d:getcontentlength");
		pSt_XMLLength->SetText(tszFileSize);
		pSt_XMLProp->InsertEndChild(pSt_XMLLength);
		//属性修改时间
		XMLElement* pSt_XMLModifyTime = m_XMLDocument.NewElement("d:getlastmodified");
		pSt_XMLModifyTime->SetText(tszGMTTime);
		pSt_XMLProp->InsertEndChild(pSt_XMLModifyTime);
        if (st_FileAttr.bFile)
        {
			//增加会话属性
			//获得会话
			XENGINE_WEBDAVLOCK st_WDLocker = {};
			if (Session_Webdav_Get(tszFileAlis, &st_WDLocker))
			{
				// 子元素 <lockdiscovery>
				XMLElement* pSt_XMLLockDiscovery = m_XMLDocument.NewElement("D:lockdiscovery");
				pSt_XMLProp->InsertEndChild(pSt_XMLLockDiscovery);
				// 子元素 <activelock>
				XMLElement* pSt_XMLLockActive = m_XMLDocument.NewElement("D:activelock");
				pSt_XMLLockDiscovery->InsertEndChild(pSt_XMLLockActive);
				// 子元素 <locktype>
				XMLElement* pSt_XMLLockType = m_XMLDocument.NewElement("D:locktype");
				pSt_XMLLockActive->InsertEndChild(pSt_XMLLockType);
				if (1 == st_WDLocker.byLockOP)
				{
					XMLElement* pSt_XMLLockWrite = m_XMLDocument.NewElement("D:write");
					pSt_XMLLockType->InsertEndChild(pSt_XMLLockWrite);
				}
				else if (2 == st_WDLocker.byLockOP)
				{
					XMLElement* pSt_XMLLockRead = m_XMLDocument.NewElement("D:read");
					pSt_XMLLockType->InsertEndChild(pSt_XMLLockRead);
				}
				// 子元素 <lockscope>
				XMLElement* pSt_XMLLockScope = m_XMLDocument.NewElement("D:lockscope");
				pSt_XMLLockActive->InsertEndChild(pSt_XMLLockScope);
				if (1 == st_WDLocker.byLockType)
				{
					XMLElement* pSt_XMLLockExclusive = m_XMLDocument.NewElement("D:exclusive");
					pSt_XMLLockScope->InsertEndChild(pSt_XMLLockExclusive);
				}
				else if (2 == st_WDLocker.byLockType)
				{
					XMLElement* pSt_XMLLockShared = m_XMLDocument.NewElement("D:shared");
					pSt_XMLLockScope->InsertEndChild(pSt_XMLLockShared);
				}
				// 子元素 <depth>
				XMLElement* pSt_XMLDepth = m_XMLDocument.NewElement("D:depth");
				pSt_XMLDepth->SetText(_X("Infinity"));
				pSt_XMLLockActive->InsertEndChild(pSt_XMLDepth);
				// 子元素 <owner>
				XMLElement* pSt_XMLOwner = m_XMLDocument.NewElement("D:owner");
				pSt_XMLLockActive->InsertEndChild(pSt_XMLOwner);
				XMLElement* pSt_XMHRef = m_XMLDocument.NewElement("D:href");
				pSt_XMHRef->SetText(st_WDLocker.tszOwner);
				pSt_XMLOwner->InsertEndChild(pSt_XMHRef);
				// 子元素 <timeout>
				XMLElement* pSt_XMLTimeout = m_XMLDocument.NewElement("D:timeout");
				pSt_XMLTimeout->SetText(st_WDLocker.tszTimeout);
				pSt_XMLLockActive->InsertEndChild(pSt_XMLTimeout);
				// 子元素 <locktoken>
				XMLElement* pSt_XMLLockToken = m_XMLDocument.NewElement("D:locktoken");
				pSt_XMLLockActive->InsertEndChild(pSt_XMLLockToken);
				XMLElement* pSt_XMHTokenRef = m_XMLDocument.NewElement("D:href");
				pSt_XMHTokenRef->SetText(st_WDLocker.tszToken);
				pSt_XMLLockToken->InsertEndChild(pSt_XMHTokenRef);
			}
        }
        else
        {
            //集合<d:resourcetype><d:collection/></d:resourcetype>
			XMLElement* pSt_XMLResource = m_XMLDocument.NewElement("d:resourcetype");
            pSt_XMLProp->InsertEndChild(pSt_XMLResource);

			XMLElement* pSt_XMLCollection = m_XMLDocument.NewElement("d:collection");
            pSt_XMLResource->InsertEndChild(pSt_XMLCollection);
        }
		//属性状态
		XMLElement* pSt_XMLStatus = m_XMLDocument.NewElement("d:status");
        pSt_XMLStatus->SetText("HTTP/1.1 200 OK");
        pSt_XMLPropstat->InsertEndChild(pSt_XMLStatus);
    }
	// 将 XML 数据保存到字符串
	XMLPrinter m_XMLPrinter;
	m_XMLDocument.Print(&m_XMLPrinter);

    *pInt_MsgLen = m_XMLPrinter.CStrSize() - 1;
    memcpy(ptszMsgBuffer, m_XMLPrinter.CStr(), *pInt_MsgLen);
    return true;
}
/********************************************************************
函数名称：Protocol_StoragePacket_WDLock
函数功能：webdav锁协议封装
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
 参数.三：pSt_WDLock
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：输入要打包的信息
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CProtocol_StoragePacket::Protocol_StoragePacket_WDLock(XCHAR* ptszMsgBuffer, int* pInt_MsgLen, XENGINE_WEBDAVLOCK* pSt_WDLock)
{
	Protocol_IsErrorOccur = false;

	if ((NULL == ptszMsgBuffer) || (NULL == pInt_MsgLen))
	{
		Protocol_IsErrorOccur = true;
		Protocol_dwErrorCode = ERROR_XENGINE_STORAGE_PROTOCOL_PARAMENT;
		return false;
	}
	// 创建一个 XML 文档
	XMLDocument m_XMLDocument;
	// XML 声明
	XMLDeclaration* pSt_XMLDeclaration = m_XMLDocument.NewDeclaration(R"(xml version="1.0" encoding="utf-8")");
	m_XMLDocument.InsertFirstChild(pSt_XMLDeclaration);

	// 根元素 <multistatus>
	XMLElement* pSt_XMLRoot = m_XMLDocument.NewElement("d:prop");
	pSt_XMLRoot->SetAttribute("xmlns:d", "DAV:");
	m_XMLDocument.InsertEndChild(pSt_XMLRoot);
	// 子元素 <lockdiscovery>
	XMLElement* pSt_XMLLockDiscovery = m_XMLDocument.NewElement("D:lockdiscovery");
	pSt_XMLRoot->InsertEndChild(pSt_XMLLockDiscovery);
	// 子元素 <activelock>
	XMLElement* pSt_XMLLockActive = m_XMLDocument.NewElement("D:activelock");
	pSt_XMLLockDiscovery->InsertEndChild(pSt_XMLLockActive);
	// 子元素 <locktype>
	XMLElement* pSt_XMLLockType = m_XMLDocument.NewElement("D:locktype");
	pSt_XMLLockActive->InsertEndChild(pSt_XMLLockType);
	if (1 == pSt_WDLock->byLockOP)
	{
		XMLElement* pSt_XMLLockWrite = m_XMLDocument.NewElement("D:write");
		pSt_XMLLockType->InsertEndChild(pSt_XMLLockWrite);
	}
	else if (2 == pSt_WDLock->byLockOP)
	{
		XMLElement* pSt_XMLLockRead = m_XMLDocument.NewElement("D:read");
		pSt_XMLLockType->InsertEndChild(pSt_XMLLockRead);
	}
	// 子元素 <lockscope>
	XMLElement* pSt_XMLLockScope = m_XMLDocument.NewElement("D:lockscope");
	pSt_XMLLockActive->InsertEndChild(pSt_XMLLockScope);
	if (1 == pSt_WDLock->byLockType)
	{
		XMLElement* pSt_XMLLockExclusive = m_XMLDocument.NewElement("D:exclusive");
		pSt_XMLLockScope->InsertEndChild(pSt_XMLLockExclusive);
	}
	else if (2 == pSt_WDLock->byLockType)
	{
		XMLElement* pSt_XMLLockShared = m_XMLDocument.NewElement("D:shared");
		pSt_XMLLockScope->InsertEndChild(pSt_XMLLockShared);
	}
	// 子元素 <depth>
	XMLElement* pSt_XMLDepth = m_XMLDocument.NewElement("D:depth");
	pSt_XMLDepth->SetText(_X("Infinity"));
	pSt_XMLLockActive->InsertEndChild(pSt_XMLDepth);
	// 子元素 <owner>
	XMLElement* pSt_XMLOwner = m_XMLDocument.NewElement("D:owner");
	pSt_XMLLockActive->InsertEndChild(pSt_XMLOwner);
	XMLElement* pSt_XMHRef = m_XMLDocument.NewElement("D:href");
	pSt_XMHRef->SetText(pSt_WDLock->tszOwner);
	pSt_XMLOwner->InsertEndChild(pSt_XMHRef);
	// 子元素 <timeout>
	XMLElement* pSt_XMLTimeout = m_XMLDocument.NewElement("D:timeout");
	pSt_XMLTimeout->SetText(pSt_WDLock->tszTimeout);
	pSt_XMLLockActive->InsertEndChild(pSt_XMLTimeout);
	// 子元素 <locktoken>
	XMLElement* pSt_XMLLockToken = m_XMLDocument.NewElement("D:locktoken");
	pSt_XMLLockActive->InsertEndChild(pSt_XMLLockToken);
	XMLElement* pSt_XMHTokenRef = m_XMLDocument.NewElement("D:href");
	pSt_XMHTokenRef->SetText(pSt_WDLock->tszToken);
	pSt_XMLLockToken->InsertEndChild(pSt_XMHTokenRef);
	// 将 XML 数据保存到字符串
	XMLPrinter m_XMLPrinter;
	m_XMLDocument.Print(&m_XMLPrinter);

	*pInt_MsgLen = m_XMLPrinter.CStrSize() - 1;
	memcpy(ptszMsgBuffer, m_XMLPrinter.CStr(), *pInt_MsgLen);
	return true;
}
/********************************************************************
函数名称：Protocol_StoragePacket_WDPropPatch
函数功能：webdavPROPPATCH协议封装
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
  可空：N
  意思：输入文件名
 参数.四：pStl_ListName
  In/Out：In
  类型：STL容器指针
  可空：N
  意思：输入打包的属性列表
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CProtocol_StoragePacket::Protocol_StoragePacket_WDPropPatch(XCHAR* ptszMsgBuffer, int* pInt_MsgLen, LPCXSTR lpszFileName, std::list<string>* pStl_ListName)
{
	Protocol_IsErrorOccur = false;

	if ((NULL == ptszMsgBuffer) || (NULL == pInt_MsgLen))
	{
		Protocol_IsErrorOccur = true;
		Protocol_dwErrorCode = ERROR_XENGINE_STORAGE_PROTOCOL_PARAMENT;
		return false;
	}
	// 创建一个 XML 文档
	XMLDocument m_XMLDocument;
	// XML 声明
	XMLDeclaration* pSt_XMLDeclaration = m_XMLDocument.NewDeclaration(R"(xml version="1.0" encoding="utf-8")");
	m_XMLDocument.InsertFirstChild(pSt_XMLDeclaration);

	// 根元素 <multistatus>
	XMLElement* pSt_XMLRoot = m_XMLDocument.NewElement("d:multistatus");
	pSt_XMLRoot->SetAttribute("xmlns:d", "DAV:");
	m_XMLDocument.InsertEndChild(pSt_XMLRoot);
	// 子元素 <response>
	XMLElement* pSt_XMLResponse = m_XMLDocument.NewElement("D:response");
	pSt_XMLRoot->InsertEndChild(pSt_XMLResponse);
	// 子元素 <href>
	XMLElement* pSt_XMLHRef = m_XMLDocument.NewElement("D:href");
	pSt_XMLHRef->SetText(lpszFileName);
	pSt_XMLResponse->InsertEndChild(pSt_XMLHRef);
	// 子元素 <propstat>
	XMLElement* pSt_XMLPropStat = m_XMLDocument.NewElement("D:propstat");
	pSt_XMLResponse->InsertEndChild(pSt_XMLPropStat);
	// 子元素 <prop>
	XMLElement* pSt_XMLProp = m_XMLDocument.NewElement("D:prop");
	pSt_XMLPropStat->InsertEndChild(pSt_XMLProp);
	// 子元素
	for (auto stl_ListIterator = pStl_ListName->begin(); stl_ListIterator != pStl_ListName->end(); stl_ListIterator++)
	{
		XMLElement* pSt_XMLListValue = m_XMLDocument.NewElement(stl_ListIterator->c_str());
		pSt_XMLProp->InsertEndChild(pSt_XMLListValue);
	}
	// 子元素 <status>
	XMLElement* pSt_XMLStatus = m_XMLDocument.NewElement("D:status");
	pSt_XMLStatus->SetText("HTTP/1.1 200 OK");
	pSt_XMLPropStat->InsertEndChild(pSt_XMLStatus);
	// 将 XML 数据保存到字符串
	XMLPrinter m_XMLPrinter;
	m_XMLDocument.Print(&m_XMLPrinter);

	*pInt_MsgLen = m_XMLPrinter.CStrSize() - 1;
	memcpy(ptszMsgBuffer, m_XMLPrinter.CStr(), *pInt_MsgLen);
	return true;
}