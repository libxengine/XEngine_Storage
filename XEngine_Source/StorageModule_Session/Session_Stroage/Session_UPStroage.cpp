#include "pch.h"
#include "Session_UPStroage.h"
/********************************************************************
//    Created:     2021/06/03  11:40:02
//    File Name:   D:\XEngine_Storage\StorageModule_Session\Session_Stroage\Session_UPStroage.cpp
//    File Path:   D:\XEngine_Storage\StorageModule_Session\Session_Stroage
//    File Base:   Session_UPStroage
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     存储上传会话
//    History:
*********************************************************************/
CSession_UPStroage::CSession_UPStroage()
{
}
CSession_UPStroage::~CSession_UPStroage()
{
}
//////////////////////////////////////////////////////////////////////////
//                      公有函数
//////////////////////////////////////////////////////////////////////////
/********************************************************************
函数名称：Session_UPStroage_Init
函数功能：初始化上传会话管理器
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CSession_UPStroage::Session_UPStroage_Init()
{
	Session_IsErrorOccur = FALSE;

	return TRUE;
}
/********************************************************************
函数名称：Session_UPStroage_Destory
函数功能：销毁下载管理器
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CSession_UPStroage::Session_UPStroage_Destory()
{
	Session_IsErrorOccur = FALSE;

	st_Locker.lock();
	stl_MapStroage.clear();
	st_Locker.unlock();

	return TRUE;
}
/********************************************************************
函数名称：Session_UPStroage_Insert
函数功能：插入一个会话到下载器
 参数.一：lpszClientAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要操作的客户端
 参数.二：lpszFileDir
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入文件地址
 参数.三：nFileSize
  In/Out：Out
  类型：整数型
  可空：N
  意思：输入文件大小
 参数.四：nLeftCount
  In/Out：In
  类型：整数型
  可空：N
  意思：输入需要写入的大小
 参数.五：nPosStart
  In/Out：In
  类型：整数型
  可空：Y
  意思：输入起始位置
 参数.六：nPostEnd
  In/Out：In
  类型：整数型
  可空：Y
  意思：输入结束位置
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CSession_UPStroage::Session_UPStroage_Insert(LPCTSTR lpszClientAddr, LPCTSTR lpszFileDir, __int64x nFileSize, __int64x nLeftCount, int nPosStart /* = 0 */, int nPostEnd /* = 0 */)
{
	Session_IsErrorOccur = FALSE;

	if ((NULL == lpszClientAddr) || (NULL == lpszFileDir))
	{
		Session_IsErrorOccur = TRUE;
		Session_dwErrorCode = ERROR_STORAGE_MODULE_SESSION_PARAMENT;
		return FALSE;
	}
	//禁止一个客户端启动多个下载会话
	st_Locker.lock_shared();
	unordered_map<tstring, SESSION_STORAGEUPLOADER>::iterator stl_MapIterator = stl_MapStroage.find(lpszClientAddr);
	if (stl_MapIterator != stl_MapStroage.end())
	{
		Session_IsErrorOccur = TRUE;
		Session_dwErrorCode = ERROR_STORAGE_MODULE_SESSION_EXIST;
		st_Locker.unlock_shared();
		return FALSE;
	}
	st_Locker.unlock_shared();

	SESSION_STORAGEUPLOADER st_Client;
	memset(&st_Client, '\0', sizeof(SESSION_STORAGEUPLOADER));

	st_Client.st_StorageInfo.ullPosStart = nPosStart;
	st_Client.st_StorageInfo.ullPosEnd = nPostEnd;
	st_Client.st_StorageInfo.ullRWCount = nLeftCount;
	st_Client.st_StorageInfo.ullCount = nFileSize;
	_tcscpy(st_Client.st_StorageInfo.tszFileDir, lpszFileDir);
	_tcscpy(st_Client.st_StorageInfo.tszClientAddr, lpszClientAddr);
	//填充下载信息
	st_Client.st_StorageInfo.pSt_File = _tfopen(lpszFileDir, _T("wb"));
	if (NULL == st_Client.st_StorageInfo.pSt_File)
	{
		Session_IsErrorOccur = TRUE;
		Session_dwErrorCode = ERROR_STORAGE_MODULE_SESSION_OPENFILE;
		return FALSE;
	}
	if (nPosStart > 0)
	{
		fseek(st_Client.st_StorageInfo.pSt_File, nPosStart, SEEK_SET);
	}
	st_Locker.lock();
	stl_MapStroage.insert(make_pair(lpszClientAddr, st_Client));
	st_Locker.unlock();
	return TRUE;
}
/********************************************************************
函数名称：Session_UPStroage_GetComplete
函数功能：接受的数据是否完毕
 参数.一：lpszClientAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要操作的客户端
 参数.二：pbComplete
  In/Out：Out
  类型：逻辑型指针
  可空：N
  意思：输出是否完成
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CSession_UPStroage::Session_UPStroage_GetComplete(LPCTSTR lpszClientAddr, BOOL* pbComplete)
{
	Session_IsErrorOccur = FALSE;

	if ((NULL == lpszClientAddr) || (NULL == pbComplete))
	{
		Session_IsErrorOccur = TRUE;
		Session_dwErrorCode = ERROR_STORAGE_MODULE_SESSION_PARAMENT;
		return FALSE;
	}

	st_Locker.lock_shared();
	unordered_map<tstring, SESSION_STORAGEUPLOADER>::iterator stl_MapIterator = stl_MapStroage.find(lpszClientAddr);
	if (stl_MapIterator == stl_MapStroage.end())
	{
		Session_IsErrorOccur = TRUE;
		Session_dwErrorCode = ERROR_STORAGE_MODULE_SESSION_NOTFOUND;
		st_Locker.unlock_shared();
		return FALSE;
	}
	if (stl_MapIterator->second.st_StorageInfo.ullRWLen >= stl_MapIterator->second.st_StorageInfo.ullRWCount)
	{
		*pbComplete = TRUE;
	}
	else
	{
		*pbComplete = FALSE;
	}
	st_Locker.unlock_shared();
	return TRUE;
}
/********************************************************************
函数名称：Session_UPStroage_GetInfo
函数功能：获取上传客户端信息
 参数.一：lpszClientAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要操作的客户端
 参数.二：pSt_StorageInfo
  In/Out：Out
  类型：数据结构指针
  可空：N
  意思：输出获取到的内容
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CSession_UPStroage::Session_UPStroage_GetInfo(LPCTSTR lpszClientAddr, SESSION_STORAGEINFO* pSt_StorageInfo)
{
	Session_IsErrorOccur = FALSE;

	if ((NULL == lpszClientAddr) || (NULL == pSt_StorageInfo))
	{
		Session_IsErrorOccur = TRUE;
		Session_dwErrorCode = ERROR_STORAGE_MODULE_SESSION_PARAMENT;
		return FALSE;
	}

	st_Locker.lock_shared();
	unordered_map<tstring, SESSION_STORAGEUPLOADER>::iterator stl_MapIterator = stl_MapStroage.find(lpszClientAddr);
	if (stl_MapIterator == stl_MapStroage.end())
	{
		Session_IsErrorOccur = TRUE;
		Session_dwErrorCode = ERROR_STORAGE_MODULE_SESSION_NOTFOUND;
		st_Locker.unlock_shared();
		return FALSE;
	}
	*pSt_StorageInfo = stl_MapIterator->second.st_StorageInfo;
	st_Locker.unlock_shared();
	return TRUE;
}
/********************************************************************
函数名称：Session_UPStroage_Write
函数功能：写入数据到文件
 参数.一：lpszClientAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要操作的客户端
 参数.二：lpszMsgBuffer
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要写入的数据
 参数.二：nMsgLen
  In/Out：In
  类型：整数型
  可空：N
  意思：输入写入大小
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CSession_UPStroage::Session_UPStroage_Write(LPCTSTR lpszClientAddr, LPCTSTR lpszMsgBuffer, int nMsgLen)
{
	Session_IsErrorOccur = FALSE;

	if ((NULL == lpszClientAddr) || (NULL == lpszMsgBuffer))
	{
		Session_IsErrorOccur = TRUE;
		Session_dwErrorCode = ERROR_STORAGE_MODULE_SESSION_PARAMENT;
		return FALSE;
	}

	st_Locker.lock_shared();
	unordered_map<tstring, SESSION_STORAGEUPLOADER>::iterator stl_MapIterator = stl_MapStroage.find(lpszClientAddr);
	if (stl_MapIterator == stl_MapStroage.end())
	{
		Session_IsErrorOccur = TRUE;
		Session_dwErrorCode = ERROR_STORAGE_MODULE_SESSION_NOTFOUND;
		st_Locker.unlock_shared();
		return FALSE;
	}
	int nCount = nMsgLen;
	int nWLen = 0;
	while (TRUE)
	{
		int nRet = fwrite(lpszMsgBuffer + nWLen, 1, nCount - nWLen, stl_MapIterator->second.st_StorageInfo.pSt_File);
		nWLen += nRet;
		//直到写完
		if (nWLen >= nCount)
		{
			break;
		}
	}
	stl_MapIterator->second.st_StorageInfo.ullRWLen += nMsgLen;
	st_Locker.unlock_shared();
	return TRUE;
}
/********************************************************************
函数名称：Session_UPStroage_Exist
函数功能：客户端是否存在
 参数.一：lpszClientAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要操作的客户端
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CSession_UPStroage::Session_UPStroage_Exist(LPCTSTR lpszClientAddr)
{
	Session_IsErrorOccur = FALSE;

	BOOL bRet = FALSE;
	st_Locker.lock();
	unordered_map<tstring, SESSION_STORAGEUPLOADER>::iterator stl_MapIterator = stl_MapStroage.find(lpszClientAddr);
	if (stl_MapIterator == stl_MapStroage.end())
	{
		bRet = FALSE;
	}
	else
	{
		bRet = TRUE;
	}
	st_Locker.unlock();
	return bRet;
}
/********************************************************************
函数名称：Session_UPStorage_GetAll
函数功能：获取所有上传信息
 参数.一：pppSt_StorageInfo
  In/Out：Out
  类型：三级指针
  可空：N
  意思：输出获取到的上传信息列表
 参数.二：pInt_ListCount
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出获取到的列表个数
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CSession_UPStroage::Session_UPStorage_GetAll(SESSION_STORAGEINFO*** pppSt_StorageInfo, int* pInt_ListCount)
{
	Session_IsErrorOccur = FALSE;

	if ((NULL == pppSt_StorageInfo) || (NULL == pInt_ListCount))
	{
		Session_IsErrorOccur = TRUE;
		Session_dwErrorCode = ERROR_STORAGE_MODULE_SESSION_PARAMENT;
		return FALSE;
	}

	st_Locker.lock_shared();
	*pInt_ListCount = stl_MapStroage.size();
	BaseLib_OperatorMemory_Malloc((XPPPMEM)pppSt_StorageInfo, *pInt_ListCount, sizeof(SESSION_STORAGEINFO));
	unordered_map<tstring, SESSION_STORAGEUPLOADER>::iterator stl_MapIterator = stl_MapStroage.begin();
	for (int i = 0; stl_MapIterator != stl_MapStroage.end(); stl_MapIterator++, i++)
	{
		*(*pppSt_StorageInfo)[i] = stl_MapIterator->second.st_StorageInfo;
	}
	st_Locker.unlock_shared();

	if (0 == *pInt_ListCount)
	{
		Session_IsErrorOccur = TRUE;
		Session_dwErrorCode = ERROR_STORAGE_MODULE_SESSION_EMPTY;
		return FALSE;
	}
	return TRUE;
}
/********************************************************************
函数名称：Session_UPStroage_Delete
函数功能：删除上传会话
 参数.一：lpszClientAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要操作的客户端
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CSession_UPStroage::Session_UPStroage_Delete(LPCTSTR lpszClientAddr)
{
	Session_IsErrorOccur = FALSE;

	st_Locker.lock();
	unordered_map<tstring, SESSION_STORAGEUPLOADER>::iterator stl_MapIterator = stl_MapStroage.find(lpszClientAddr);
	if (stl_MapIterator != stl_MapStroage.end())
	{
		fclose(stl_MapIterator->second.st_StorageInfo.pSt_File);
		stl_MapStroage.erase(stl_MapIterator);
	}
	st_Locker.unlock();
	return TRUE;
}