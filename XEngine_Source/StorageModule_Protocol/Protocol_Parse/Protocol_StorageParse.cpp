#include "pch.h"
#include "Protocol_StorageParse.h"
/********************************************************************
//	Created:	2019/6/28   15:05
//	Filename: 	E:\NetEngine_Windows\NetEngine_SourceCode\NetEngine_Storage\NetEngine_XStorageProtocol\XStorageProtocol_Core\XStorageProtocol_Core.cpp
//	File Path:	E:\NetEngine_Windows\NetEngine_SourceCode\NetEngine_Storage\NetEngine_XStorageProtocol\XStorageProtocol_Core
//	File Base:	XStorageProtocol_Core
//	File Ext:	cpp
//  Project:    NetEngine(网络通信引擎)
//	Author:		qyt
//	Purpose:	协议处理函数
//	History:
*********************************************************************/
CProtocol_StorageParse::CProtocol_StorageParse()
{

}
CProtocol_StorageParse::~CProtocol_StorageParse()
{

}
//////////////////////////////////////////////////////////////////////////
//                           公有函数
//////////////////////////////////////////////////////////////////////////
/********************************************************************
函数名称：Protocol_StorageParse_QueryFile
函数功能：文件查询请求解析函数
 参数.一：lpszMsgBuffer
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：要解析的缓冲区
 参数.二：ptszTimeStart
  In/Out：Out
  类型：字符指针
  可空：N
  意思：查询开始时间
 参数.三：ptszTimeEnd
  In/Out：Out
  类型：字符指针
  可空：N
  意思：查询结束时间
 参数.四：ptszBuckKey
  In/Out：Out
  类型：字符指针
  可空：Y
  意思：输出文件所属BUCKET
 参数.五：ptszFileName
  In/Out：Out
  类型：字符指针
  可空：Y
  意思：查询的文件名
 参数.六：ptszFileHash
  In/Out：Out
  类型：字符指针
  可空：Y
  意思：查询的文件HASH
 参数.七：pInt_Mode
  In/Out：Out
  类型：整数型指针
  可空：Y
  意思：输出查询返回模式
 参数.八：pxhToken
  In/Out：Out
  类型：整数型指针
  可空：Y
  意思：输出TOKEN
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CProtocol_StorageParse::Protocol_StorageParse_QueryFile(LPCTSTR lpszMsgBuffer, TCHAR* ptszTimeStart, TCHAR* ptszTimeEnd, TCHAR* ptszBuckKey /* = NULL */, TCHAR* ptszFileName /* = NULL */, TCHAR* ptszFileHash /* = NULL */, int* pInt_Mode /* = NULL */, XNETHANDLE* pxhToken /* = NULL */)
{
    Protocol_IsErrorOccur = FALSE;

    if (NULL == lpszMsgBuffer)
    {
        Protocol_IsErrorOccur = true;
        Protocol_dwErrorCode = ERROR_XENGINE_STORAGE_PROTOCOL_PARAMENT;
        return FALSE;
    }
    Json::Value st_JsonRoot;
    Json::CharReaderBuilder st_JsonBuild;
    Json::CharReader* pSt_JsonReader(st_JsonBuild.newCharReader());

    JSONCPP_STRING st_JsonError;
    //解析JSON
    if (!pSt_JsonReader->parse(lpszMsgBuffer, lpszMsgBuffer + _tcslen(lpszMsgBuffer), &st_JsonRoot, &st_JsonError))
    {
        Protocol_IsErrorOccur = true;
        Protocol_dwErrorCode = ERROR_XENGINE_STORAGE_PROTOCOL_PARSE;
        return FALSE;
    }
    delete pSt_JsonReader;
    pSt_JsonReader = NULL;

    if (NULL != ptszTimeStart)
    {
		if (!st_JsonRoot["lpszTimeStart"].isNull())
		{
			_tcscpy(ptszTimeStart, st_JsonRoot["lpszTimeStart"].asCString());
		}
    }
    if (NULL != ptszTimeEnd)
    {
		if (!st_JsonRoot["lpszTimeEnd"].isNull())
		{
			_tcscpy(ptszTimeEnd, st_JsonRoot["lpszTimeEnd"].asCString());
		}
    }
    if (NULL != ptszFileName)
    {
        if (!st_JsonRoot["lpszFileName"].isNull())
        {
            _tcscpy(ptszFileName, st_JsonRoot["lpszFileName"].asCString());
        }
    }
    if (NULL != ptszFileHash)
    {
        if (!st_JsonRoot["lpszFileHash"].isNull())
        {
            _tcscpy(ptszFileHash, st_JsonRoot["lpszFileHash"].asCString());
        }
    }
	if (NULL != ptszBuckKey)
	{
		if (!st_JsonRoot["lpszBuckKey"].isNull())
		{
			_tcscpy(ptszBuckKey, st_JsonRoot["lpszBuckKey"].asCString());
		}
	}
    if (NULL != pInt_Mode)
    {
		if (!st_JsonRoot["nMode"].isNull())
		{
            *pInt_Mode = st_JsonRoot["nMode"].asInt();
		}
    }
    if (NULL != pxhToken)
    {
        if (!st_JsonRoot["xhToken"].isNull())
        {
            *pxhToken = st_JsonRoot["xhToken"].asUInt64();
        }
    }
    return true;
}
/********************************************************************
函数名称：Protocol_StorageParse_ReportFile
函数功能：解析文件报告协议
 参数.一：lpszMsgBuffer
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要解析的内容
 参数.二：nMsgLen
  In/Out：In
  类型：整数型
  可空：N
  意思：输入要解析的内容大小
 参数.三：pppSt_DBFile
  In/Out：Out
  类型：三级指针
  可空：N
  意思：输出解析到的文件列表
 参数.四：pInt_ListCount
  In/Out：In
  类型：整数型指针
  可空：N
  意思：输出列表个数
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CProtocol_StorageParse::Protocol_StorageParse_ReportFile(LPCTSTR lpszMsgBuffer, int nMsgLen, XSTORAGECORE_DBFILE*** pppSt_DBFile, int *pInt_ListCount)
{
    Protocol_IsErrorOccur = FALSE;

	Json::Value st_JsonRoot;
	Json::CharReaderBuilder st_JsonBuild;
	Json::CharReader* pSt_JsonReader(st_JsonBuild.newCharReader());

	JSONCPP_STRING st_JsonError;
	//解析JSON
	if (!pSt_JsonReader->parse(lpszMsgBuffer, lpszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
	{
		Protocol_IsErrorOccur = true;
		Protocol_dwErrorCode = ERROR_XENGINE_STORAGE_PROTOCOL_PARSE;
		return FALSE;
	}
	delete pSt_JsonReader;
	pSt_JsonReader = NULL;

    int nCount = st_JsonRoot["Count"].asInt();
    Json::Value st_JsonArray = st_JsonRoot["List"];

    BaseLib_OperatorMemory_Malloc((XPPPMEM)pppSt_DBFile, nCount, sizeof(XSTORAGECORE_DBFILE));
    for (int i = 0; i < nCount; i++)
    {
        if (st_JsonArray[i]["nFileSize"].isNull())
        {
            (*pppSt_DBFile)[i]->st_ProtocolFile.nFileSize = 0;
        }
        else
        {
            (*pppSt_DBFile)[i]->st_ProtocolFile.nFileSize = st_JsonArray[i]["nFileSize"].asInt64();
        }

		if (!st_JsonArray[i]["tszBuckKey"].isNull())
		{
			_tcscpy((*pppSt_DBFile)[i]->tszBuckKey, st_JsonArray[i]["tszBuckKey"].asCString());
		}
        if (!st_JsonArray[i]["tszFileName"].isNull())
        {
            _tcscpy((*pppSt_DBFile)[i]->st_ProtocolFile.tszFileName, st_JsonArray[i]["tszFileName"].asCString());
        }
        if (!st_JsonArray[i]["tszFilePath"].isNull())
        {
            _tcscpy((*pppSt_DBFile)[i]->st_ProtocolFile.tszFilePath, st_JsonArray[i]["tszFilePath"].asCString());
        }
        if (!st_JsonArray[i]["tszFileHash"].isNull())
        {
            _tcscpy((*pppSt_DBFile)[i]->st_ProtocolFile.tszFileHash, st_JsonArray[i]["tszFileHash"].asCString());
        }
        if (!st_JsonArray[i]["tszFileTime"].isNull())
        {
            _tcscpy((*pppSt_DBFile)[i]->st_ProtocolFile.tszFileTime, st_JsonArray[i]["tszFileTime"].asCString());
        }
        if (!st_JsonArray[i]["tszFileUser"].isNull())
        {
            _tcscpy((*pppSt_DBFile)[i]->st_ProtocolFile.tszFileUser, st_JsonArray[i]["tszFileUser"].asCString());
        }
    }
    *pInt_ListCount = nCount;
    return true;
}
/********************************************************************
函数名称：Protocol_StorageParse_DirOperator
函数功能：文件夹操作协议
 参数.一：lpszMsgBuffer
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：要解析的缓冲区
 参数.二：ptszUserDir
  In/Out：Out
  类型：字符指针
  可空：N
  意思：导出获取到的文件夹
 参数.三：ptszBuckKey
  In/Out：Out
  类型：字符指针
  可空：N
  意思：导出桶的KEY
 参数.四：pInt_Operator
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：导出操作类型
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CProtocol_StorageParse::Protocol_StorageParse_DirOperator(LPCTSTR lpszMsgBuffer, TCHAR* ptszUserDir, TCHAR* ptszBuckKey, int* pInt_Operator)
{
    Protocol_IsErrorOccur = FALSE;

    if ((NULL == lpszMsgBuffer) || (NULL == ptszUserDir))
    {
        Protocol_IsErrorOccur = true;
        Protocol_dwErrorCode = ERROR_XENGINE_STORAGE_PROTOCOL_PARAMENT;
        return FALSE;
    }
    Json::Value st_JsonRoot;
    Json::CharReaderBuilder st_JsonBuild;
    Json::CharReader* pSt_JsonReader(st_JsonBuild.newCharReader());

    JSONCPP_STRING st_JsonError;
    //解析JSON
    if (!pSt_JsonReader->parse(lpszMsgBuffer, lpszMsgBuffer + _tcslen(lpszMsgBuffer), &st_JsonRoot, &st_JsonError))
    {
        Protocol_IsErrorOccur = true;
        Protocol_dwErrorCode = ERROR_XENGINE_STORAGE_PROTOCOL_PARSE;
        return FALSE;
    }
    delete pSt_JsonReader;
    pSt_JsonReader = NULL;

    *pInt_Operator = st_JsonRoot["nOPerator"].asInt();
    _tcscpy(ptszBuckKey, st_JsonRoot["lpszBuckKey"].asCString());
    if (!st_JsonRoot["lpszUserDir"].isNull())
    {
        _tcscpy(ptszUserDir, st_JsonRoot["lpszUserDir"].asCString());
    }
    return true;
}
/********************************************************************
函数名称：Protocol_StorageParse_ProxyNotify
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
 参数.四：ptszBuckKey
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出路径的KEY
 参数.五：ptszFileName
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出文件名称
 参数.六：ptszFileHash
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出文件HASH
 参数.七：pInt_FileSize
  In/Out：Out
  类型：整数型
  可空：N
  意思：输出文件大小
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CProtocol_StorageParse::Protocol_StorageParse_ProxyNotify(LPCTSTR lpszMsgBuffer, int nMsgLen, TCHAR* ptszClientAddr, TCHAR* ptszBuckKey, TCHAR* ptszFileName, TCHAR* ptszFileHash, __int64x* pInt_FileSize)
{
    Protocol_IsErrorOccur = FALSE;

    if ((NULL == lpszMsgBuffer) || (NULL == ptszClientAddr))
    {
        Protocol_IsErrorOccur = true;
        Protocol_dwErrorCode = ERROR_XENGINE_STORAGE_PROTOCOL_PARAMENT;
        return FALSE;
    }
    Json::Value st_JsonRoot;
    Json::CharReaderBuilder st_JsonBuild;
    Json::CharReader* pSt_JsonReader(st_JsonBuild.newCharReader());

    JSONCPP_STRING st_JsonError;
    //解析JSON
    if (!pSt_JsonReader->parse(lpszMsgBuffer, lpszMsgBuffer + _tcslen(lpszMsgBuffer), &st_JsonRoot, &st_JsonError))
    {
        Protocol_IsErrorOccur = true;
        Protocol_dwErrorCode = ERROR_XENGINE_STORAGE_PROTOCOL_PARSE;
        return FALSE;
    }
    delete pSt_JsonReader;
    pSt_JsonReader = NULL;

    _tcscpy(ptszClientAddr, st_JsonRoot["lpszClientAddr"].asCString());
    _tcscpy(ptszFileName, st_JsonRoot["lpszFileName"].asCString());
    _tcscpy(ptszFileHash, st_JsonRoot["lpszFileHash"].asCString());
    _tcscpy(ptszBuckKey, st_JsonRoot["lpszBuckKey"].asCString());
    *pInt_FileSize = st_JsonRoot["nFileSize"].asInt64();

    return true;
}
/********************************************************************
函数名称：Protocol_StorageParse_SpeedLimit
函数功能：速度限制请求解析函数
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
 参数.三：pInt_Code
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出回复值
 参数.四：pInt_Limit
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出回复的限制
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CProtocol_StorageParse::Protocol_StorageParse_SpeedLimit(LPCTSTR lpszMsgBuffer, int nMsgLen, int* pInt_Code, int* pInt_Limit)
{
	Protocol_IsErrorOccur = FALSE;

	if ((NULL == lpszMsgBuffer) || (NULL == pInt_Limit))
	{
		Protocol_IsErrorOccur = true;
		Protocol_dwErrorCode = ERROR_XENGINE_STORAGE_PROTOCOL_PARAMENT;
		return FALSE;
	}
	Json::Value st_JsonRoot;
	Json::CharReaderBuilder st_JsonBuild;
	Json::CharReader* pSt_JsonReader(st_JsonBuild.newCharReader());

	JSONCPP_STRING st_JsonError;
	//解析JSON
	if (!pSt_JsonReader->parse(lpszMsgBuffer, lpszMsgBuffer + _tcslen(lpszMsgBuffer), &st_JsonRoot, &st_JsonError))
	{
		Protocol_IsErrorOccur = true;
		Protocol_dwErrorCode = ERROR_XENGINE_STORAGE_PROTOCOL_PARSE;
		return FALSE;
	}
	delete pSt_JsonReader;
	pSt_JsonReader = NULL;

    *pInt_Code = st_JsonRoot["code"].asInt();
    *pInt_Limit = st_JsonRoot["nLimitSpeed"].asInt();
	return true;
}
/********************************************************************
函数名称：Protocol_StorageParse_P2PToken
函数功能：P2P查找文件TOKEN匹配函数
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
 参数.三：pxhToken
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出解析到的TOKEN
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CProtocol_StorageParse::Protocol_StorageParse_P2PToken(LPCTSTR lpszMsgBuffer, int nMsgLen, XNETHANDLE *pxhToken)
{
	Protocol_IsErrorOccur = FALSE;

	if ((NULL == lpszMsgBuffer))
	{
		Protocol_IsErrorOccur = true;
		Protocol_dwErrorCode = ERROR_XENGINE_STORAGE_PROTOCOL_PARAMENT;
		return FALSE;
	}
	Json::Value st_JsonRoot;
	Json::CharReaderBuilder st_JsonBuild;
	Json::CharReader* pSt_JsonReader(st_JsonBuild.newCharReader());

	JSONCPP_STRING st_JsonError;
	//解析JSON
	if (!pSt_JsonReader->parse(lpszMsgBuffer, lpszMsgBuffer + _tcslen(lpszMsgBuffer), &st_JsonRoot, &st_JsonError))
	{
		Protocol_IsErrorOccur = true;
		Protocol_dwErrorCode = ERROR_XENGINE_STORAGE_PROTOCOL_PARSE;
		return FALSE;
	}
	delete pSt_JsonReader;
	pSt_JsonReader = NULL;

    if (st_JsonRoot["xhToken"].isNull())
    {
		Protocol_IsErrorOccur = true;
		Protocol_dwErrorCode = ERROR_XENGINE_STORAGE_PROTOCOL_TOKEN;
		return FALSE;
    }
    *pxhToken = st_JsonRoot["xhToken"].asUInt64();
	return true;
}