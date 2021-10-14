#include "pch.h"
#include "APIHelp_Distributed.h"
/********************************************************************
//    Created:     2021/07/08  15:26:13
//    File Name:   D:\XEngine_Storage\XEngine_Source\StorageModule_APIHelp\APIHelp_Distributed\APIHelp_Distributed.cpp
//    File Path:   D:\XEngine_Storage\XEngine_Source\StorageModule_APIHelp\APIHelp_Distributed
//    File Base:   APIHelp_Distributed
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     分布式帮助函数
//    History:
*********************************************************************/
CAPIHelp_Distributed::CAPIHelp_Distributed()
{

}
CAPIHelp_Distributed::~CAPIHelp_Distributed()
{

}
//////////////////////////////////////////////////////////////////////////
//                               公有函数
//////////////////////////////////////////////////////////////////////////
/********************************************************************
函数名称：APIHelp_Distributed_IsMode
函数功能：判断负载模式是否为指定模式
 参数.一：pStl_ListMode
  In/Out：In
  类型：STL容器指针
  可空：N
  意思：输入支持的模式列表
 参数.二：nMode
  In/Out：In
  类型：整数型
  可空：N
  意思：输入要判断的模式
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CAPIHelp_Distributed::APIHelp_Distributed_IsMode(list<int>* pStl_ListMode, int nMode)
{
	APIHelp_IsErrorOccur = FALSE;

	BOOL bFound = FALSE;
	list<int>::const_iterator stl_ListIterator = pStl_ListMode->begin();
	for (; stl_ListIterator != pStl_ListMode->end(); stl_ListIterator++)
	{
		if (nMode == *stl_ListIterator)
		{
			bFound = TRUE;
			break;
		}
	}
	return bFound;
}
/********************************************************************
函数名称：APIHelp_Distributed_RandomAddr
函数功能：随机选择一个负载的重定向服务器地址
 参数.一：pStl_ListAddr
  In/Out：In
  类型：STL容器指针
  可空：N
  意思：输入负载服务器列表
 参数.二：ptszAddr
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出获取到的负载地址
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CAPIHelp_Distributed::APIHelp_Distributed_RandomAddr(list<string>* pStl_ListAddr, TCHAR* ptszAddr)
{
	APIHelp_IsErrorOccur = FALSE;

	BOOL bFound = FALSE;
	XNETHANDLE xhToken = 0;

	BaseLib_OperatorHandle_Create(&xhToken, 0, pStl_ListAddr->size(), FALSE);
	if (xhToken == pStl_ListAddr->size())
	{
		xhToken--;
	}
	list<string>::const_iterator stl_ListIterator = pStl_ListAddr->begin();
	for (XNETHANDLE i = 0; stl_ListIterator != pStl_ListAddr->end(); stl_ListIterator++, i++)
	{
		if (xhToken == i)
		{
			bFound = TRUE;
			_tcscpy(ptszAddr, stl_ListIterator->c_str());
			break;
		}
	}
	return bFound;
}
/********************************************************************
函数名称：APIHelp_Distributed_FileList
函数功能：解析所有解析到的内容并且打包成指定结构
 参数.一：pStl_ListParse
  In/Out：In
  类型：STL容器指针
  可空：N
  意思：输入要解析的内容列表
 参数.二：pppSt_ListPacket
  In/Out：Out
  类型：三级指针
  可空：N
  意思：输出文件信息结构列表
 参数.三：pInt_ListCount
  In/Out：Out
  类型：整数型
  可空：N
  意思：输出文件列表个数
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CAPIHelp_Distributed::APIHelp_Distributed_FileList(list<APIHELP_LBFILEINFO>* pStl_ListParse, XSTORAGECORE_DBFILE*** pppSt_ListPacket, int* pInt_ListCount)
{
	APIHelp_IsErrorOccur = FALSE;

	if ((NULL == pStl_ListParse) || (NULL == pppSt_ListPacket))
	{
		APIHelp_IsErrorOccur = TRUE;
		APIHelp_dwErrorCode = ERROR_STORAGE_MODULE_APIHELP_PARAMENT;
		return FALSE;
	}

	BaseLib_OperatorMemory_Malloc((XPPPMEM)pppSt_ListPacket, pStl_ListParse->size(), sizeof(XSTORAGECORE_DBFILE));
	list<APIHELP_LBFILEINFO>::const_iterator stl_ListIterator = pStl_ListParse->begin();
	for (int i = 0; stl_ListIterator != pStl_ListParse->end(); stl_ListIterator++, i++)
	{
		APIHelp_Distributed_FileListParse(stl_ListIterator->tszMsgBuffer, stl_ListIterator->nMsgLen, (*pppSt_ListPacket)[i]);
	}
	*pInt_ListCount = pStl_ListParse->size();
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////
//                               保护函数
//////////////////////////////////////////////////////////////////////////
/********************************************************************
函数名称：APIHelp_Distributed_FileListParse
函数功能：文件列表解析函数
 参数.一：lpszMsgBuffer
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要解析的JSON
 参数.二：nMsgLen
  In/Out：In
  类型：整数型
  可空：N
  意思：输入要解析的大小
 参数.三：pSt_DBFile
  In/Out：Out
  类型：数据结构指针
  可空：N
  意思：输出解析后的文件信息
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CAPIHelp_Distributed::APIHelp_Distributed_FileListParse(LPCTSTR lpszMsgBuffer, int nMsgLen, XSTORAGECORE_DBFILE *pSt_DBFile)
{
	APIHelp_IsErrorOccur = FALSE;

	Json::Value st_JsonRoot;
	JSONCPP_STRING st_JsonError;
	Json::CharReaderBuilder st_JsonBuilder;

	std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_JsonBuilder.newCharReader());
	if (!pSt_JsonReader->parse(lpszMsgBuffer, lpszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
	{
		APIHelp_IsErrorOccur = TRUE;
		APIHelp_dwErrorCode = ERROR_STORAGE_MODULE_APIHELP_PARSE;
		return FALSE;
	}
	//查找文件列表,一般只有一个
	int nCount = st_JsonRoot["Count"].asInt();
	Json::Value st_JsonArray = st_JsonRoot["List"];

	for (int i = 0; i < nCount; i++)
	{
		pSt_DBFile->st_ProtocolFile.nFileSize = st_JsonArray[i]["nFileSize"].asInt64();
		_tcscpy(pSt_DBFile->tszTableName, st_JsonArray[i]["tszTableName"].asCString());
		_tcscpy(pSt_DBFile->st_ProtocolFile.tszFileUser, st_JsonArray[i]["tszFileUser"].asCString());
		_tcscpy(pSt_DBFile->st_ProtocolFile.tszFileHash, st_JsonArray[i]["tszFileHash"].asCString());
		_tcscpy(pSt_DBFile->st_ProtocolFile.tszFileName, st_JsonArray[i]["tszFileName"].asCString());
		_tcscpy(pSt_DBFile->st_ProtocolFile.tszFilePath, st_JsonArray[i]["tszFilePath"].asCString());
		_tcscpy(pSt_DBFile->st_ProtocolFile.tszFileTime, st_JsonArray[i]["tszFileTime"].asCString());
		
		break;
	}
	return TRUE;
}