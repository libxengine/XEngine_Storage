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
 参数.三：nMode
  In/Out：Out
  类型：整数型
  可空：N
  意思：负载模式
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CAPIHelp_Distributed::APIHelp_Distributed_RandomAddr(list<string>* pStl_ListAddr, TCHAR* ptszAddr, int nMode)
{
	APIHelp_IsErrorOccur = FALSE;

	if (!pStl_ListAddr->empty())
	{
		APIHelp_IsErrorOccur = TRUE;
		APIHelp_dwErrorCode = ERROR_STORAGE_MODULE_APIHELP_PARAMENT;
		return FALSE;
	}

	BOOL bFound = FALSE;
	if (1 == nMode)
	{
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
	}
	else if (2 == nMode)
	{
		bFound = TRUE;
		_tcscpy(ptszAddr, pStl_ListAddr->front().c_str());
	}
	else if (3 == nMode)
	{

		bFound = TRUE;
		_tcscpy(ptszAddr, pStl_ListAddr->back().c_str());
	}

	if (!bFound)
	{
		APIHelp_IsErrorOccur = TRUE;
		APIHelp_dwErrorCode = ERROR_STORAGE_MODULE_APIHELP_NOTFOUND;
		return FALSE;
	}
	return TRUE;
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
/********************************************************************
函数名称：APIHelp_Distributed_DLStorage
函数功能：通过URLKEY得到一个对应下载地址
 参数.一：lpszMsgBuffer
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要解析的URL
 参数.二：pStl_ListBucket
  In/Out：In
  类型：容器指针
  可空：N
  意思：输入要解析的列表
 参数.三：pSt_StorageBucket
  In/Out：Out
  类型：数据结构指针
  可空：N
  意思：输出获取到的可用存储
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CAPIHelp_Distributed::APIHelp_Distributed_DLStorage(LPCTSTR lpszMsgBuffer, list<XENGINE_STORAGEBUCKET>* pStl_ListBucket, XENGINE_STORAGEBUCKET* pSt_StorageBucket)
{
	APIHelp_IsErrorOccur = FALSE;

	if ((NULL == lpszMsgBuffer) || (NULL == pSt_StorageBucket))
	{
		APIHelp_IsErrorOccur = TRUE;
		APIHelp_dwErrorCode = ERROR_STORAGE_MODULE_APIHELP_PARAMENT;
		return FALSE;
	}
	BOOL bFound = FALSE;
	TCHAR tszKeyStr[128];
	memset(tszKeyStr, '\0', sizeof(tszKeyStr));
	//获得key
	int i = 1;
	int nLen = _tcslen(lpszMsgBuffer);
	for (; i < nLen; i++)
	{
		if ('/' == lpszMsgBuffer[i])
		{
			bFound = TRUE;
			memcpy(tszKeyStr, lpszMsgBuffer + 1, i - 1);
			break;
		}
	}
	if (!bFound)
	{
		APIHelp_IsErrorOccur = TRUE;
		APIHelp_dwErrorCode = ERROR_STORAGE_MODULE_APIHELP_NOTFOUND;
		return FALSE;
	}
	bFound = FALSE;
	//获得对应存储
	for (auto stl_ListIterator = pStl_ListBucket->begin(); stl_ListIterator != pStl_ListBucket->end(); stl_ListIterator++)
	{
		if (0 == _tcsncmp(tszKeyStr, stl_ListIterator->tszBuckKey, _tcslen(tszKeyStr)))
		{
			bFound = TRUE;
			*pSt_StorageBucket = *stl_ListIterator;
			break;
		}
	}
	if (!bFound)
	{
		APIHelp_IsErrorOccur = TRUE;
		APIHelp_dwErrorCode = ERROR_STORAGE_MODULE_APIHELP_NOTFOUND;
		return FALSE;
	}
	_tcscpy(pSt_StorageBucket->tszFileName, lpszMsgBuffer + i);
	return TRUE;
}
/********************************************************************
函数名称：APIHelp_Distributed_UPStorage
函数功能：通过分布式存储列表获得一个存储地址
 参数.一：pStl_ListBucket
  In/Out：In
  类型：容器指针
  可空：N
  意思：输入要解析的列表
 参数.二：pSt_StorageBucket
  In/Out：Out
  类型：数据结构指针
  可空：N
  意思：输出获取到的可用存储
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CAPIHelp_Distributed::APIHelp_Distributed_UPStorage(list<XENGINE_STORAGEBUCKET>* pStl_ListBucket, XENGINE_STORAGEBUCKET* pSt_StorageBucket, int nMode)
{
	APIHelp_IsErrorOccur = FALSE;

	if ((NULL == pStl_ListBucket) || (NULL == pSt_StorageBucket))
	{
		APIHelp_IsErrorOccur = TRUE;
		APIHelp_dwErrorCode = ERROR_STORAGE_MODULE_APIHELP_PARAMENT;
		return FALSE;
	}
	int nLastLevel = 99999;
	list<XENGINE_STORAGEBUCKET> stl_BuckSelect;
	//先得到最小级别
	for (auto stl_ListIterator = pStl_ListBucket->begin(); stl_ListIterator != pStl_ListBucket->end(); stl_ListIterator++)
	{
		//只处理启用的
		if (stl_ListIterator->bEnable)
		{
			if (stl_ListIterator->nLevel < nLastLevel)
			{
				nLastLevel = stl_ListIterator->nLevel; //得到最小级别
			}
		}
	}
	//在来获得这个级别的列表
	for (auto stl_ListIterator = pStl_ListBucket->begin(); stl_ListIterator != pStl_ListBucket->end(); stl_ListIterator++)
	{
		//只处理启用的
		if (stl_ListIterator->bEnable)
		{
			//处理优先级
			if (stl_ListIterator->nLevel == nLastLevel)
			{
				int nListCount = 0;
				__int64u nDirCount = 0;   //当前目录大小
				CHAR** ppListFile;
				SystemApi_File_EnumFile(stl_ListIterator->tszFilePath, &ppListFile, &nListCount, NULL, NULL, TRUE, 1);
				for (int j = 0; j < nListCount; j++)
				{
					struct __stat64 st_FStat;
					_stat64(ppListFile[j], &st_FStat);
					nDirCount += st_FStat.st_size;
				}
				BaseLib_OperatorMemory_Free((XPPPMEM)&ppListFile, nListCount);
				//如果当前目录大小大于设定的大小.那么忽略
				if (nDirCount >= APIHelp_Distributed_GetSize(stl_ListIterator->tszBuckSize))
				{
					continue;
				}
				stl_BuckSelect.push_back(*stl_ListIterator);
			}
		}
	}
	//通过指定模式获得一个key
	if (!stl_BuckSelect.empty())
	{
		APIHelp_IsErrorOccur = TRUE;
		APIHelp_dwErrorCode = ERROR_STORAGE_MODULE_APIHELP_NOTFOUND;
		return FALSE;
	}
	if (1 == nMode)
	{
		XNETHANDLE xhToken = 0;
		BaseLib_OperatorHandle_Create(&xhToken, 0, stl_BuckSelect.size(), FALSE);
		if (xhToken == stl_BuckSelect.size())
		{
			xhToken--;
		}
		list<XENGINE_STORAGEBUCKET>::const_iterator stl_ListIterator = stl_BuckSelect.begin();
		for (XNETHANDLE i = 0; stl_ListIterator != stl_BuckSelect.end(); stl_ListIterator++, i++)
		{
			if (xhToken == i)
			{
				*pSt_StorageBucket = *stl_ListIterator;
				break;
			}
		}
	}
	else if (2 == nMode)
	{
		*pSt_StorageBucket = stl_BuckSelect.front();
	}
	else if (3 == nMode)
	{
		*pSt_StorageBucket = stl_BuckSelect.back();
	}

	return TRUE;
}
/********************************************************************
函数名称：APIHelp_Distributed_GetPathKey
函数功能：通过BUCKET名称查找对应路径
 参数.一：pStl_ListBucket
  In/Out：In
  类型：STL容器指针
  可空：N
  意思：输入要操作的BUCKET容器
 参数.二：lpszBuckKey
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要匹配的BUCKET名称
 参数.三：ptszFilePath
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出找到的路径
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CAPIHelp_Distributed::APIHelp_Distributed_GetPathKey(list<XENGINE_STORAGEBUCKET>* pStl_ListBucket, LPCTSTR lpszBuckKey, TCHAR* ptszFilePath)
{
	APIHelp_IsErrorOccur = FALSE;

	if ((NULL == pStl_ListBucket) || (NULL == lpszBuckKey) || (NULL == ptszFilePath))
	{
		APIHelp_IsErrorOccur = TRUE;
		APIHelp_dwErrorCode = ERROR_STORAGE_MODULE_APIHELP_PARAMENT;
		return FALSE;
	}
	BOOL bFound = FALSE;
	for (auto stl_ListIterator = pStl_ListBucket->begin(); stl_ListIterator != pStl_ListBucket->end(); stl_ListIterator++)
	{
		if (0 == _tcsncmp(lpszBuckKey, stl_ListIterator->tszBuckKey, _tcslen(lpszBuckKey)))
		{
			_tcscpy(ptszFilePath, stl_ListIterator->tszFilePath);
			bFound = TRUE;
			break;
		}
	}
	if (!bFound)
	{
		APIHelp_IsErrorOccur = TRUE;
		APIHelp_dwErrorCode = ERROR_STORAGE_MODULE_APIHELP_NOTFOUND;
		return FALSE;
	}
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
/********************************************************************
函数名称：APIHelp_Distributed_GetSize
函数功能：获取存储设置大小
 参数.一：lpszMsgBuffer
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要获取的缓冲区
返回值
  类型：整数型
  意思：获取到的大小字节
备注：
*********************************************************************/
__int64u CAPIHelp_Distributed::APIHelp_Distributed_GetSize(LPCTSTR lpszMsgBuffer)
{
	APIHelp_IsErrorOccur = FALSE;

	TCHAR tszSizeStr[64];
	TCHAR tszUnitStr[4];

	memset(tszSizeStr, '\0', sizeof(tszSizeStr));
	memset(tszUnitStr, '\0', sizeof(tszUnitStr));
	//分别得到数字和单位
	memcpy(tszSizeStr, lpszMsgBuffer, _tcslen(lpszMsgBuffer) - 2);
	BaseLib_OperatorString_GetLastString(lpszMsgBuffer, 2, tszUnitStr);

	__int64u nllSize = _ttoi64(tszSizeStr);
	//得到单位大小
	if (0 == _tcsncmp(tszUnitStr, _T("KB"), 2))
	{
		nllSize = nllSize * 1024;
	}
	else if (0 == _tcsncmp(tszUnitStr, _T("MB"), 2))
	{
		nllSize = nllSize * 1024 * 1024;
	}
	else if (0 == _tcsncmp(tszUnitStr, _T("GB"), 2))
	{
		nllSize = nllSize * 1024 * 1024 * 1024;
	}

	return nllSize;
}