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
bool CProtocol_StorageParse::Protocol_StorageParse_QueryFile(LPCXSTR lpszMsgBuffer, XCHAR* ptszTimeStart, XCHAR* ptszTimeEnd, XCHAR* ptszBuckKey /* = NULL */, XCHAR* ptszFileName /* = NULL */, XCHAR* ptszFileHash /* = NULL */, int* pInt_Mode /* = NULL */, XNETHANDLE* pxhToken /* = NULL */)
{
    Protocol_IsErrorOccur = false;

    if (NULL == lpszMsgBuffer)
    {
        Protocol_IsErrorOccur = true;
        Protocol_dwErrorCode = ERROR_XENGINE_STORAGE_PROTOCOL_PARAMENT;
        return false;
    }
    Json::Value st_JsonRoot;
    Json::CharReaderBuilder st_JsonBuild;
    Json::CharReader* pSt_JsonReader(st_JsonBuild.newCharReader());

    JSONCPP_STRING st_JsonError;
    //解析JSON
    if (!pSt_JsonReader->parse(lpszMsgBuffer, lpszMsgBuffer + _tcsxlen(lpszMsgBuffer), &st_JsonRoot, &st_JsonError))
    {
        Protocol_IsErrorOccur = true;
        Protocol_dwErrorCode = ERROR_XENGINE_STORAGE_PROTOCOL_PARSE;
        return false;
    }
    delete pSt_JsonReader;
    pSt_JsonReader = NULL;

    if (NULL != ptszTimeStart)
    {
		if (!st_JsonRoot["lpszTimeStart"].isNull())
		{
			_tcsxcpy(ptszTimeStart, st_JsonRoot["lpszTimeStart"].asCString());
		}
    }
    if (NULL != ptszTimeEnd)
    {
		if (!st_JsonRoot["lpszTimeEnd"].isNull())
		{
			_tcsxcpy(ptszTimeEnd, st_JsonRoot["lpszTimeEnd"].asCString());
		}
    }
    if (NULL != ptszFileName)
    {
        if (!st_JsonRoot["lpszFileName"].isNull())
        {
            _tcsxcpy(ptszFileName, st_JsonRoot["lpszFileName"].asCString());
        }
    }
    if (NULL != ptszFileHash)
    {
        if (!st_JsonRoot["lpszFileHash"].isNull())
        {
            _tcsxcpy(ptszFileHash, st_JsonRoot["lpszFileHash"].asCString());
        }
    }
	if (NULL != ptszBuckKey)
	{
		if (!st_JsonRoot["lpszBuckKey"].isNull())
		{
			_tcsxcpy(ptszBuckKey, st_JsonRoot["lpszBuckKey"].asCString());
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
bool CProtocol_StorageParse::Protocol_StorageParse_ReportFile(LPCXSTR lpszMsgBuffer, int nMsgLen, XSTORAGECORE_DBFILE*** pppSt_DBFile, int *pInt_ListCount)
{
    Protocol_IsErrorOccur = false;

	Json::Value st_JsonRoot;
	Json::CharReaderBuilder st_JsonBuild;
	Json::CharReader* pSt_JsonReader(st_JsonBuild.newCharReader());

	JSONCPP_STRING st_JsonError;
	//解析JSON
	if (!pSt_JsonReader->parse(lpszMsgBuffer, lpszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
	{
		Protocol_IsErrorOccur = true;
		Protocol_dwErrorCode = ERROR_XENGINE_STORAGE_PROTOCOL_PARSE;
		return false;
	}
	delete pSt_JsonReader;
	pSt_JsonReader = NULL;

    int nCount = st_JsonRoot["Count"].asInt();
    Json::Value st_JsonArray = st_JsonRoot["List"];

    BaseLib_Memory_Malloc((XPPPMEM)pppSt_DBFile, nCount, sizeof(XSTORAGECORE_DBFILE));
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
			_tcsxcpy((*pppSt_DBFile)[i]->tszBuckKey, st_JsonArray[i]["tszBuckKey"].asCString());
		}
        if (!st_JsonArray[i]["tszFileName"].isNull())
        {
            _tcsxcpy((*pppSt_DBFile)[i]->st_ProtocolFile.tszFileName, st_JsonArray[i]["tszFileName"].asCString());
        }
        if (!st_JsonArray[i]["tszFilePath"].isNull())
        {
            _tcsxcpy((*pppSt_DBFile)[i]->st_ProtocolFile.tszFilePath, st_JsonArray[i]["tszFilePath"].asCString());
        }
        if (!st_JsonArray[i]["tszFileHash"].isNull())
        {
            _tcsxcpy((*pppSt_DBFile)[i]->st_ProtocolFile.tszFileHash, st_JsonArray[i]["tszFileHash"].asCString());
        }
        if (!st_JsonArray[i]["tszFileTime"].isNull())
        {
            _tcsxcpy((*pppSt_DBFile)[i]->st_ProtocolFile.tszFileTime, st_JsonArray[i]["tszFileTime"].asCString());
        }
        if (!st_JsonArray[i]["tszFileUser"].isNull())
        {
            _tcsxcpy((*pppSt_DBFile)[i]->st_ProtocolFile.tszFileUser, st_JsonArray[i]["tszFileUser"].asCString());
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
bool CProtocol_StorageParse::Protocol_StorageParse_DirOperator(LPCXSTR lpszMsgBuffer, XCHAR* ptszUserDir, XCHAR* ptszBuckKey, int* pInt_Operator)
{
    Protocol_IsErrorOccur = false;

    if ((NULL == lpszMsgBuffer))
    {
        Protocol_IsErrorOccur = true;
        Protocol_dwErrorCode = ERROR_XENGINE_STORAGE_PROTOCOL_PARAMENT;
        return false;
    }
    Json::Value st_JsonRoot;
    Json::CharReaderBuilder st_JsonBuild;
    Json::CharReader* pSt_JsonReader(st_JsonBuild.newCharReader());

    JSONCPP_STRING st_JsonError;
    //解析JSON
    if (!pSt_JsonReader->parse(lpszMsgBuffer, lpszMsgBuffer + _tcsxlen(lpszMsgBuffer), &st_JsonRoot, &st_JsonError))
    {
        Protocol_IsErrorOccur = true;
        Protocol_dwErrorCode = ERROR_XENGINE_STORAGE_PROTOCOL_PARSE;
        return false;
    }
    delete pSt_JsonReader;
    pSt_JsonReader = NULL;

	if (!st_JsonRoot["nOPerator"].isNull() && NULL != pInt_Operator)
	{
		*pInt_Operator = st_JsonRoot["nOPerator"].asInt();
	}
    if (!st_JsonRoot["lpszBuckKey"].isNull() && NULL != ptszBuckKey)
    {
		_tcsxcpy(ptszBuckKey, st_JsonRoot["lpszBuckKey"].asCString());
    }
    if (!st_JsonRoot["lpszUserDir"].isNull() && NULL != ptszUserDir)
    {
        _tcsxcpy(ptszUserDir, st_JsonRoot["lpszUserDir"].asCString());
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
bool CProtocol_StorageParse::Protocol_StorageParse_ProxyNotify(LPCXSTR lpszMsgBuffer, int nMsgLen, XCHAR* ptszClientAddr, XCHAR* ptszBuckKey, XCHAR* ptszFileName, XCHAR* ptszFileHash, __int64x* pInt_FileSize)
{
    Protocol_IsErrorOccur = false;

    if ((NULL == lpszMsgBuffer) || (NULL == ptszClientAddr))
    {
        Protocol_IsErrorOccur = true;
        Protocol_dwErrorCode = ERROR_XENGINE_STORAGE_PROTOCOL_PARAMENT;
        return false;
    }
    Json::Value st_JsonRoot;
    Json::CharReaderBuilder st_JsonBuild;
    Json::CharReader* pSt_JsonReader(st_JsonBuild.newCharReader());

    JSONCPP_STRING st_JsonError;
    //解析JSON
    if (!pSt_JsonReader->parse(lpszMsgBuffer, lpszMsgBuffer + _tcsxlen(lpszMsgBuffer), &st_JsonRoot, &st_JsonError))
    {
        Protocol_IsErrorOccur = true;
        Protocol_dwErrorCode = ERROR_XENGINE_STORAGE_PROTOCOL_PARSE;
        return false;
    }
    delete pSt_JsonReader;
    pSt_JsonReader = NULL;

    _tcsxcpy(ptszClientAddr, st_JsonRoot["lpszClientAddr"].asCString());
    _tcsxcpy(ptszFileName, st_JsonRoot["lpszFileName"].asCString());
    _tcsxcpy(ptszFileHash, st_JsonRoot["lpszFileHash"].asCString());
    _tcsxcpy(ptszBuckKey, st_JsonRoot["lpszBuckKey"].asCString());
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
bool CProtocol_StorageParse::Protocol_StorageParse_SpeedLimit(LPCXSTR lpszMsgBuffer, int nMsgLen, int* pInt_Code, int* pInt_Limit)
{
	Protocol_IsErrorOccur = false;

	if ((NULL == lpszMsgBuffer) || (NULL == pInt_Limit))
	{
		Protocol_IsErrorOccur = true;
		Protocol_dwErrorCode = ERROR_XENGINE_STORAGE_PROTOCOL_PARAMENT;
		return false;
	}
	Json::Value st_JsonRoot;
	Json::CharReaderBuilder st_JsonBuild;
	Json::CharReader* pSt_JsonReader(st_JsonBuild.newCharReader());

	JSONCPP_STRING st_JsonError;
	//解析JSON
	if (!pSt_JsonReader->parse(lpszMsgBuffer, lpszMsgBuffer + _tcsxlen(lpszMsgBuffer), &st_JsonRoot, &st_JsonError))
	{
		Protocol_IsErrorOccur = true;
		Protocol_dwErrorCode = ERROR_XENGINE_STORAGE_PROTOCOL_PARSE;
		return false;
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
bool CProtocol_StorageParse::Protocol_StorageParse_P2PToken(LPCXSTR lpszMsgBuffer, int nMsgLen, XNETHANDLE *pxhToken)
{
	Protocol_IsErrorOccur = false;

	if ((NULL == lpszMsgBuffer))
	{
		Protocol_IsErrorOccur = true;
		Protocol_dwErrorCode = ERROR_XENGINE_STORAGE_PROTOCOL_PARAMENT;
		return false;
	}
	Json::Value st_JsonRoot;
	Json::CharReaderBuilder st_JsonBuild;
	Json::CharReader* pSt_JsonReader(st_JsonBuild.newCharReader());

	JSONCPP_STRING st_JsonError;
	//解析JSON
	if (!pSt_JsonReader->parse(lpszMsgBuffer, lpszMsgBuffer + _tcsxlen(lpszMsgBuffer), &st_JsonRoot, &st_JsonError))
	{
		Protocol_IsErrorOccur = true;
		Protocol_dwErrorCode = ERROR_XENGINE_STORAGE_PROTOCOL_PARSE;
		return false;
	}
	delete pSt_JsonReader;
	pSt_JsonReader = NULL;

    if (st_JsonRoot["xhToken"].isNull())
    {
		Protocol_IsErrorOccur = true;
		Protocol_dwErrorCode = ERROR_XENGINE_STORAGE_PROTOCOL_TOKEN;
		return false;
    }
    *pxhToken = st_JsonRoot["xhToken"].asUInt64();
	return true;
}
/********************************************************************
函数名称：Protocol_StorageParse_Action
函数功能：动作解析器
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
 参数.三：pSt_ActionInfo
  In/Out：Out
  类型：数据结构指针
  可空：N
  意思：输出解析的数据
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CProtocol_StorageParse::Protocol_StorageParse_Action(LPCXSTR lpszMsgBuffer, int nMsgLen, XENGINE_ACTIONINFO* pSt_ActionInfo)
{
	Protocol_IsErrorOccur = false;

	if ((NULL == lpszMsgBuffer))
	{
		Protocol_IsErrorOccur = true;
		Protocol_dwErrorCode = ERROR_XENGINE_STORAGE_PROTOCOL_PARAMENT;
		return false;
	}
	Json::Value st_JsonRoot;
    JSONCPP_STRING st_JsonError;
    Json::CharReaderBuilder st_JsonBuilder;
	//解析JSON
    std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_JsonBuilder.newCharReader());
	if (!pSt_JsonReader->parse(lpszMsgBuffer, lpszMsgBuffer + _tcsxlen(lpszMsgBuffer), &st_JsonRoot, &st_JsonError))
	{
		Protocol_IsErrorOccur = true;
		Protocol_dwErrorCode = ERROR_XENGINE_STORAGE_PROTOCOL_PARSE;
		return false;
	}

	if (st_JsonRoot["tszFileUrl"].isNull() || st_JsonRoot["byType"].isNull())
	{
		Protocol_IsErrorOccur = true;
		Protocol_dwErrorCode = ERROR_XENGINE_STORAGE_PROTOCOL_TOKEN;
		return false;
	}
	
    pSt_ActionInfo->byType = st_JsonRoot["byType"].asInt();
    _tcsxcpy(pSt_ActionInfo->tszBucketStr, st_JsonRoot["tszBucketStr"].asCString());
    _tcsxcpy(pSt_ActionInfo->tszFileName, st_JsonRoot["tszFileName"].asCString());
    _tcsxcpy(pSt_ActionInfo->tszFileUrl, st_JsonRoot["tszFileUrl"].asCString());
	return true;
}
/********************************************************************
函数名称：Protocol_StorageParse_WDLock
函数功能：WEBDAV加锁协议解析函数
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
 参数.三：pSt_WDLock
  In/Out：Out
  类型：数据结构指针
  可空：N
  意思：输出解析的数据
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CProtocol_StorageParse::Protocol_StorageParse_WDLock(LPCXSTR lpszMsgBuffer, int nMsgLen, XENGINE_WEBDAVLOCK* pSt_WDLock)
{
    Protocol_IsErrorOccur = false;

	XMLDocument m_XMLDocument;
	XMLError m_XMLResult = m_XMLDocument.Parse(lpszMsgBuffer);
	if (XML_SUCCESS != m_XMLResult)
    {
		Protocol_IsErrorOccur = true;
		Protocol_dwErrorCode = ERROR_XENGINE_STORAGE_PROTOCOL_PARSE;
		return false;
	}
	//获取根元素
	XMLElement* pSt_XMLRoot = m_XMLDocument.RootElement();
	if (NULL == pSt_XMLRoot)
    {
		Protocol_IsErrorOccur = true;
		Protocol_dwErrorCode = ERROR_XENGINE_STORAGE_PROTOCOL_ROOT;
		return false;
	}
	//解析lockscope
	XMLElement* pSt_XMLLockScope = pSt_XMLRoot->FirstChildElement("D:lockscope");
	if (NULL != pSt_XMLLockScope)
    {
		XMLElement* pSt_XMLExclusive = pSt_XMLLockScope->FirstChildElement("D:exclusive");
		if (NULL != pSt_XMLExclusive)
        {
            pSt_WDLock->byLockType = 1;
		}
		XMLElement* pSt_XMLShared = pSt_XMLLockScope->FirstChildElement("D:shared");
		if (NULL != pSt_XMLShared)
		{
            pSt_WDLock->byLockType = 2;
		}
	}
	//解析locktype
	XMLElement* pSt_XMLLockType = pSt_XMLRoot->FirstChildElement("D:locktype");
	if (NULL != pSt_XMLLockType)
    {
		XMLElement* pSt_XMLTypeWrite = pSt_XMLLockType->FirstChildElement("D:write");
		if (NULL != pSt_XMLTypeWrite)
        {
            pSt_WDLock->byLockOP = 1;
		}
		XMLElement* pSt_XMLTypeRead = pSt_XMLLockType->FirstChildElement("D:read");
		if (NULL != pSt_XMLTypeRead)
		{
            pSt_WDLock->byLockOP = 2;
		}
	}
	// 解析owner
	XMLElement* pSt_XMLLockOwner = pSt_XMLRoot->FirstChildElement("D:owner");
	if (NULL != pSt_XMLLockOwner)
    {
		XMLElement* pSt_XMLHRef = pSt_XMLLockOwner->FirstChildElement("D:href");
		if (NULL != pSt_XMLHRef)
        {
            _tcsxcpy(pSt_WDLock->tszOwner, pSt_XMLHRef->GetText());
		}
	}
    return true;
}
/********************************************************************
函数名称：Protocol_StorageParse_WDPropPatch
函数功能：解析proppatch协议
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
 参数.三：pStl_ListName
  In/Out：Out
  类型：LIST容器指针
  可空：N
  意思：输出解析的数据
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CProtocol_StorageParse::Protocol_StorageParse_WDPropPatch(LPCXSTR lpszMsgBuffer, int nMsgLen, std::list<string>* pStl_ListName)
{
	Protocol_IsErrorOccur = false;

	XMLDocument m_XMLDocument;
	XMLError m_XMLResult = m_XMLDocument.Parse(lpszMsgBuffer);
	if (XML_SUCCESS != m_XMLResult)
	{
		Protocol_IsErrorOccur = true;
		Protocol_dwErrorCode = ERROR_XENGINE_STORAGE_PROTOCOL_PARSE;
		return false;
	}
	//获取根元素
	XMLElement* pSt_XMLRoot = m_XMLDocument.RootElement();
	if (NULL == pSt_XMLRoot)
	{
		Protocol_IsErrorOccur = true;
		Protocol_dwErrorCode = ERROR_XENGINE_STORAGE_PROTOCOL_ROOT;
		return false;
	}
	XMLElement* pSt_XMLPropertySet = pSt_XMLRoot->FirstChildElement("D:set");
	if (NULL != pSt_XMLPropertySet)
	{
		XMLElement* pSt_XMLPropertyProp = pSt_XMLPropertySet->FirstChildElement("D:prop");
		if (NULL != pSt_XMLPropertyProp)
		{
			//获得所有名称
			for (XMLElement* pSt_XMLChild = pSt_XMLPropertyProp->FirstChildElement(); NULL != pSt_XMLChild; pSt_XMLChild = pSt_XMLChild->NextSiblingElement())
			{
				pStl_ListName->push_back(pSt_XMLChild->Name());
			}
		}
	}
	return true;
}