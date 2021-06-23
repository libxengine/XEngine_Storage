#include "pch.h"
#include "Session_DLStroage.h"
/********************************************************************
//    Created:     2021/06/02  14:11:33
//    File Name:   D:\XEngine_Storage\StorageModule_Session\Session_Stroage\Session_DLStroage.cpp
//    File Path:   D:\XEngine_Storage\StorageModule_Session\Session_Stroage
//    File Base:   Session_DLStroage
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     存储下载会话
//    History:
*********************************************************************/
CSession_DLStroage::CSession_DLStroage()
{
}
CSession_DLStroage::~CSession_DLStroage()
{
}
//////////////////////////////////////////////////////////////////////////
//                      公有函数
//////////////////////////////////////////////////////////////////////////
/********************************************************************
函数名称：Session_DLStroage_Init
函数功能：初始化下载会话管理器
 参数.一：nPoolCount
  In/Out：In
  类型：整数型
  可空：N
  意思：输入最大运行多少个下载同时进行
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CSession_DLStroage::Session_DLStroage_Init(int nPoolCount /* = 1 */)
{
	Session_IsErrorOccur = FALSE;

	for (int i = 0; i < nPoolCount; i++)
	{
		SESSION_STORAGELIST st_StorageList;

		st_StorageList.pStl_ListStorage = new list<SESSION_STORAGEINFO>;
		st_StorageList.st_Locker = make_shared<shared_mutex>();

		st_Locker.lock();
		stl_MapStroage.insert(make_pair(i, st_StorageList));
		st_Locker.unlock();
	}
	return TRUE;
}
/********************************************************************
函数名称：Session_DLStroage_Destory
函数功能：销毁下载管理器
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CSession_DLStroage::Session_DLStroage_Destory()
{
	Session_IsErrorOccur = FALSE;

	st_Locker.lock();
	unordered_map<int, SESSION_STORAGELIST>::iterator stl_MapIterator = stl_MapStroage.begin();
	for (; stl_MapIterator != stl_MapStroage.end(); stl_MapIterator++)
	{
		stl_MapIterator->second.st_Locker->lock();
		list<SESSION_STORAGEINFO>::iterator stl_ListIterator = stl_MapIterator->second.pStl_ListStorage->begin();
		for (; stl_ListIterator != stl_MapIterator->second.pStl_ListStorage->end(); stl_ListIterator++)
		{
			fclose(stl_ListIterator->pSt_File);
		}
		stl_MapIterator->second.pStl_ListStorage->clear();
		delete stl_MapIterator->second.pStl_ListStorage;
		stl_MapIterator->second.pStl_ListStorage = NULL;
		stl_MapIterator->second.st_Locker->unlock();
	}
	stl_MapStroage.clear();
	st_Locker.unlock();

	return TRUE;
}
/********************************************************************
函数名称：Session_DLStroage_Insert
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
 参数.三：pInt_Count
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出文件大小
 参数.四：pInt_LeftCount
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出需要读取大小
 参数.五：nPosStart
  In/Out：In
  类型：整数型
  可空：Y
  意思：输入开始位置
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
BOOL CSession_DLStroage::Session_DLStroage_Insert(LPCTSTR lpszClientAddr, LPCTSTR lpszFileDir, __int64x* pInt_Count, __int64x* pInt_LeftCount, int nPosStart /* = 0 */, int nPostEnd /* = 0 */)
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
	unordered_map<int, SESSION_STORAGELIST>::iterator stl_MapIterator = stl_MapStroage.begin();
	for (; stl_MapIterator != stl_MapStroage.end(); stl_MapIterator++)
	{
		stl_MapIterator->second.st_Locker->lock_shared();
		list<SESSION_STORAGEINFO>::iterator stl_ListIterator= stl_MapIterator->second.pStl_ListStorage->begin();
		for (; stl_ListIterator != stl_MapIterator->second.pStl_ListStorage->end(); stl_ListIterator++)
		{
			if (0 == _tcsncmp(lpszClientAddr, stl_ListIterator->tszClientAddr, _tcslen(lpszClientAddr)))
			{
				Session_IsErrorOccur = TRUE;
				Session_dwErrorCode = ERROR_STORAGE_MODULE_SESSION_EXIST;
				stl_MapIterator->second.st_Locker->unlock_shared();
				st_Locker.unlock_shared();
				return FALSE;
			}
		}
		stl_MapIterator->second.st_Locker->unlock_shared();
	}
	st_Locker.unlock_shared();

	SESSION_STORAGEINFO st_Client;
	struct _stat st_FStat;

	memset(&st_Client, '\0', sizeof(SESSION_STORAGEINFO));
	_stat(lpszFileDir, &st_FStat);

	st_Client.ullPosStart = nPosStart;
	st_Client.ullPosEnd = nPostEnd;
	st_Client.ullCount = st_FStat.st_size;
	_tcscpy(st_Client.tszFileDir, lpszFileDir);
	_tcscpy(st_Client.tszClientAddr, lpszClientAddr);
	//填充下载信息
	st_Client.pSt_File = _tfopen(lpszFileDir, _T("rb"));
	if (NULL == st_Client.pSt_File)
	{
		Session_IsErrorOccur = TRUE;
		Session_dwErrorCode = ERROR_STORAGE_MODULE_SESSION_OPENFILE;
		return FALSE;
	}
	//是否有范围
	if ((nPosStart > 0) || (nPostEnd > 0))
	{
		fseek(st_Client.pSt_File, nPosStart, SEEK_SET);
		//计算需要读取的大小
		if (nPostEnd > 0)
		{
			st_Client.ullRWCount = nPostEnd - nPosStart;
		}
		else
		{
			st_Client.ullRWCount = st_Client.ullCount - nPosStart;
		}
	}
	else
	{
		st_Client.ullRWCount = st_Client.ullCount;
	}
	if (NULL != pInt_Count)
	{
		*pInt_Count = st_Client.ullCount;
	}
	if (NULL != pInt_LeftCount)
	{
		*pInt_LeftCount = st_Client.ullRWCount;
	}
	//查找一个最小队列
	int nListPos = 0;
	size_t nListCount = 99999;
	st_Locker.lock();
	stl_MapIterator = stl_MapStroage.begin();
	for (int i = 0; stl_MapIterator != stl_MapStroage.end(); stl_MapIterator++, i++)
	{
		stl_MapIterator->second.st_Locker->lock_shared();
		if (nListCount > stl_MapIterator->second.pStl_ListStorage->size())
		{
			nListPos = i;
			nListCount = stl_MapIterator->second.pStl_ListStorage->size();
		}
		stl_MapIterator->second.st_Locker->unlock_shared();
	}
	stl_MapIterator = stl_MapStroage.find(nListPos);
	stl_MapIterator->second.st_Locker->lock();
	stl_MapIterator->second.pStl_ListStorage->push_back(st_Client);
	stl_MapIterator->second.st_Locker->unlock();
	st_Locker.unlock();
	return TRUE;
}
/********************************************************************
函数名称：Session_DLStroage_GetList
函数功能：获得下载器中的列表索引信息
 参数.一：nPool
  In/Out：In
  类型：整数型
  可空：N
  意思：输入要操作的队列
 参数.二：nIndex
  In/Out：In
  类型：整数型
  可空：N
  意思：输入要操作的队列索引
 参数.三：ptszClientAddr
  In/Out：In
  类型：整数型
  可空：N
  意思：输出客户端地址
 参数.四：ptszMsgBuffer
  In/Out：In
  类型：整数型
  可空：N
 参数.五：pInt_MsgLen
  In/Out：In
  类型：整数型
  可空：N
  意思：输出获取的缓冲区大小
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CSession_DLStroage::Session_DLStroage_GetList(int nPool, int nIndex, TCHAR* ptszClientAddr, TCHAR* ptszMsgBuffer, int* pInt_MsgLen)
{
	Session_IsErrorOccur = FALSE;

	if ((NULL == ptszClientAddr) || (NULL == ptszMsgBuffer))
	{
		Session_IsErrorOccur = TRUE;
		Session_dwErrorCode = ERROR_STORAGE_MODULE_SESSION_PARAMENT;
		return FALSE;
	}

	st_Locker.lock_shared();
	unordered_map<int, SESSION_STORAGELIST>::iterator stl_MapIterator = stl_MapStroage.find(nPool);
	if (stl_MapIterator == stl_MapStroage.end())
	{
		Session_IsErrorOccur = TRUE;
		Session_dwErrorCode = ERROR_STORAGE_MODULE_SESSION_NOTFOUND;
		st_Locker.unlock_shared();
		return FALSE;
	}
	stl_MapIterator->second.st_Locker->lock_shared();
	list<SESSION_STORAGEINFO>::iterator stl_ListIterator = stl_MapIterator->second.pStl_ListStorage->begin();
	for (int i = 0; stl_ListIterator != stl_MapIterator->second.pStl_ListStorage->end(); stl_ListIterator++, i++)
	{
		if (nIndex == i)
		{
			_tcscpy(ptszClientAddr, stl_ListIterator->tszClientAddr);

			if (stl_ListIterator->ullRWLen >= stl_ListIterator->ullRWCount)
			{
				*pInt_MsgLen = 0;
			}
			else
			{
				if (*pInt_MsgLen > (stl_ListIterator->ullRWCount - stl_ListIterator->ullRWLen))
				{
					*pInt_MsgLen = int(stl_ListIterator->ullRWCount - stl_ListIterator->ullRWLen);
				}
				*pInt_MsgLen = fread(ptszMsgBuffer, 1, *pInt_MsgLen, stl_ListIterator->pSt_File);
				stl_ListIterator->ullRWLen += *pInt_MsgLen;
			}
			break;
		}
	}
	stl_MapIterator->second.st_Locker->unlock_shared();
	st_Locker.unlock_shared();
	return TRUE;
}
/********************************************************************
函数名称：Session_DLStroage_GetCount
函数功能：获取队列拥有的个数
 参数.一：nPool
  In/Out：In
  类型：整数型
  可空：N
  意思：输入要操作的队列
 参数.二：pInt_ListCount
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出队列个数
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CSession_DLStroage::Session_DLStroage_GetCount(int nPool, int* pInt_ListCount)
{
	Session_IsErrorOccur = FALSE;

	if (NULL == pInt_ListCount)
	{
		Session_IsErrorOccur = TRUE;
		Session_dwErrorCode = ERROR_STORAGE_MODULE_SESSION_PARAMENT;
		return FALSE;
	}

	st_Locker.lock_shared();
	unordered_map<int, SESSION_STORAGELIST>::iterator stl_MapIterator = stl_MapStroage.find(nPool);
	if (stl_MapIterator == stl_MapStroage.end())
	{
		Session_IsErrorOccur = TRUE;
		Session_dwErrorCode = ERROR_STORAGE_MODULE_SESSION_NOTFOUND;
		st_Locker.unlock_shared();
		return FALSE;
	}
	stl_MapIterator->second.st_Locker->lock_shared();
	*pInt_ListCount = stl_MapIterator->second.pStl_ListStorage->size();
	stl_MapIterator->second.st_Locker->unlock_shared();
	st_Locker.unlock_shared();
	return TRUE;
}
/********************************************************************
函数名称：Session_DLStroage_Delete
函数功能：删除一个队列
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
BOOL CSession_DLStroage::Session_DLStroage_Delete(LPCTSTR lpszClientAddr)
{
	Session_IsErrorOccur = FALSE;

	BOOL bFound = FALSE;
	st_Locker.lock_shared();
	unordered_map<int, SESSION_STORAGELIST>::iterator stl_MapIterator = stl_MapStroage.begin();
	for (; stl_MapIterator != stl_MapStroage.end(); stl_MapIterator++)
	{
		stl_MapIterator->second.st_Locker->lock();
		list<SESSION_STORAGEINFO>::iterator stl_ListIterator = stl_MapIterator->second.pStl_ListStorage->begin();
		for (; stl_ListIterator != stl_MapIterator->second.pStl_ListStorage->end(); stl_ListIterator++)
		{
			if (0 == _tcsncmp(lpszClientAddr, stl_ListIterator->tszClientAddr, _tcslen(lpszClientAddr)))
			{
				bFound = TRUE;
				fclose(stl_ListIterator->pSt_File);
				stl_MapIterator->second.pStl_ListStorage->erase(stl_ListIterator);
				break;
			}
		}
		stl_MapIterator->second.st_Locker->unlock();

		if (bFound)
		{
			break;
		}
	}
	st_Locker.unlock_shared();
	return TRUE;
}