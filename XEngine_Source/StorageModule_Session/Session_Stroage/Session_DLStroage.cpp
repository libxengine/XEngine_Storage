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
 参数.一：nMaxConnect
  In/Out：
  类型：
  可空：
  意思：
 参数.二：
  In/Out：
  类型：
  可空：
  意思：
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CSession_DLStroage::Session_DLStroage_Init(int nMaxConnect)
{
	Session_IsErrorOccur = false;

	m_nMaxConnect = nMaxConnect;
	return true;
}
/********************************************************************
函数名称：Session_DLStroage_Destory
函数功能：销毁下载管理器
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CSession_DLStroage::Session_DLStroage_Destory()
{
	Session_IsErrorOccur = false;

	st_Locker.lock();
	stl_MapStroage.clear();
	st_Locker.unlock();

	return true;
}
/********************************************************************
函数名称：Session_DLStroage_Insert
函数功能：插入一个会话到下载器
 参数.一：lpszClientAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要操作的客户端
 参数.二：lpszBuckKey
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入BUCKET名称
 参数.三：lpszFileDir
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入文件地址
 参数.四：pInt_Count
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出文件大小
 参数.五：pInt_LeftCount
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出需要读取大小
 参数.六：nPosStart
  In/Out：In
  类型：整数型
  可空：Y
  意思：输入开始位置
 参数.七：nPostEnd
  In/Out：In
  类型：整数型
  可空：Y
  意思：输入结束位置
 参数.八：lpszFileHash
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：文件的HASH值
 参数.九：xhToken
  In/Out：In
  类型：句柄
  可空：Y
  意思：输入限制器句柄
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CSession_DLStroage::Session_DLStroage_Insert(LPCXSTR lpszClientAddr, LPCXSTR lpszBuckKey, LPCXSTR lpszFileDir, __int64x* pInt_Count, __int64x* pInt_LeftCount, int nPosStart /* = 0 */, int nPostEnd /* = 0 */, LPCXSTR lpszFileHash /* = NULL */, int nLimit /* = 0 */, XHANDLE xhToken /* = NULL */)
{
	Session_IsErrorOccur = false;

	if ((NULL == lpszClientAddr) || (NULL == lpszFileDir))
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_STORAGE_MODULE_SESSION_PARAMENT;
		return false;
	}
	//禁止一个客户端启动多个下载会话
	st_Locker.lock_shared();
	unordered_map<string, SESSION_STORAGEINFO>::iterator stl_MapIterator = stl_MapStroage.find(lpszClientAddr);
	if (stl_MapIterator != stl_MapStroage.end())
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_STORAGE_MODULE_SESSION_EXIST;
		st_Locker.unlock_shared();
		return false;
	}
	st_Locker.unlock_shared();

	SESSION_STORAGEINFO st_Client;
	struct _xtstat st_FStat;

	memset(&st_Client, '\0', sizeof(SESSION_STORAGEINFO));
	int nRet = _xtstat(lpszFileDir, &st_FStat);
	if (-1 == nRet)
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_STORAGE_MODULE_SESSION_OPENFILE;
		return false;
	}
	st_Client.ullPosStart = nPosStart;
	st_Client.ullPosEnd = nPostEnd;
	st_Client.ullCount = st_FStat.st_size;
	_tcsxcpy(st_Client.tszFileDir, lpszFileDir);
	_tcsxcpy(st_Client.tszClientAddr, lpszClientAddr);
	_tcsxcpy(st_Client.tszBuckKey, lpszBuckKey);
	if (NULL != lpszFileHash)
	{
		_tcsxcpy(st_Client.tszFileHash, lpszFileHash);
	}
	//设置限制
	if (nLimit > 0)
	{
		st_Client.nLimit = nLimit;
		st_Client.xhToken = xhToken;
	}
	//填充下载信息
	st_Client.pSt_File = _xtfopen(lpszFileDir, _X("rb"));
	if (NULL == st_Client.pSt_File)
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_STORAGE_MODULE_SESSION_OPENFILE;
		return false;
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
	st_Locker.lock();
	stl_MapStroage.insert(make_pair(lpszClientAddr, st_Client));
	st_Locker.unlock();
	return true;
}
/********************************************************************
函数名称：Session_DLStroage_GetBuffer
函数功能：获得下载器中指定缓冲区
 参数.一：lpszClientAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入客户端地址
 参数.二：ptszMsgBuffer
  In/Out：In
  类型：字符指针
  可空：N
 参数.三：pInt_MsgLen
  In/Out：In
  类型：整数型指针
  可空：N
  意思：输出获取的缓冲区大小
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CSession_DLStroage::Session_DLStroage_GetBuffer(LPCXSTR lpszClientAddr, XCHAR* ptszMsgBuffer, int* pInt_MsgLen)
{
	Session_IsErrorOccur = false;

	if ((NULL == lpszClientAddr) || (NULL == ptszMsgBuffer) || (NULL == pInt_MsgLen))
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_STORAGE_MODULE_SESSION_PARAMENT;
		return false;
	}

	st_Locker.lock_shared();
	unordered_map<string, SESSION_STORAGEINFO>::iterator stl_MapIterator = stl_MapStroage.find(lpszClientAddr);
	if (stl_MapIterator == stl_MapStroage.end())
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_STORAGE_MODULE_SESSION_NOTFOUND;
		st_Locker.unlock_shared();
		return false;
	}
	if (stl_MapIterator->second.ullRWLen >= stl_MapIterator->second.ullRWCount)
	{
		*pInt_MsgLen = 0;
	}
	else
	{
		if (*pInt_MsgLen > (stl_MapIterator->second.ullRWCount - stl_MapIterator->second.ullRWLen))
		{
			*pInt_MsgLen = int(stl_MapIterator->second.ullRWCount - stl_MapIterator->second.ullRWLen);
		}
		*pInt_MsgLen = fread(ptszMsgBuffer, 1, *pInt_MsgLen, stl_MapIterator->second.pSt_File);
		stl_MapIterator->second.ullRWLen += *pInt_MsgLen;
	}
	st_Locker.unlock_shared();
	return true;
}
/********************************************************************
函数名称：Session_DLStroage_GetInfo
函数功能：获取下载信息
 参数.一：lpszClientAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要操作的客户端
 参数.二：pSt_StorageInfo
  In/Out：Out
  类型：数据结构指针
  可空：N
  意思：输出内容
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CSession_DLStroage::Session_DLStroage_GetInfo(LPCXSTR lpszClientAddr, SESSION_STORAGEINFO* pSt_StorageInfo)
{
	Session_IsErrorOccur = false;

	if ((NULL == pSt_StorageInfo))
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_STORAGE_MODULE_SESSION_PARAMENT;
		return false;
	}

	st_Locker.lock_shared();
	unordered_map<string, SESSION_STORAGEINFO>::iterator stl_MapIterator = stl_MapStroage.find(lpszClientAddr);
	if (stl_MapIterator == stl_MapStroage.end())
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_STORAGE_MODULE_SESSION_NOTFOUND;
		st_Locker.unlock_shared();
		return false;
	}
	*pSt_StorageInfo = stl_MapIterator->second;
	st_Locker.unlock_shared();
	return true;
}
/********************************************************************
函数名称：Session_DLStroage_GetCount
函数功能：获取队列拥有的个数
 参数.一：pStl_ListClient
  In/Out：Out
  类型：STL容器指针
  可空：N
  意思：输出要发送的队列个数
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CSession_DLStroage::Session_DLStroage_GetCount(int* pInt_ListCount)
{
	Session_IsErrorOccur = false;

	if (NULL == pInt_ListCount)
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_STORAGE_MODULE_SESSION_PARAMENT;
		return false;
	}
	st_Locker.lock_shared();
	*pInt_ListCount = stl_MapStroage.size();
	st_Locker.unlock_shared();
	return true;
}
/********************************************************************
函数名称：Session_DLStorage_SetSeek
函数功能：移动文件指针
 参数.一：lpszClientAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要操作的客户端
 参数.二：nSeek
  In/Out：In
  类型：整数型
  可空：N
  意思：输入文件位置
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CSession_DLStroage::Session_DLStorage_SetSeek(LPCXSTR lpszClientAddr, int nSeek)
{
	Session_IsErrorOccur = false;

	st_Locker.lock_shared();
	unordered_map<string, SESSION_STORAGEINFO>::iterator stl_MapIterator = stl_MapStroage.find(lpszClientAddr);
	if (stl_MapIterator == stl_MapStroage.end())
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_STORAGE_MODULE_SESSION_NOTFOUND;
		st_Locker.unlock_shared();
		return false;
	}
	//移动文件指针
	fseek(stl_MapIterator->second.pSt_File, nSeek, SEEK_CUR);
	stl_MapIterator->second.ullRWLen += nSeek;
	st_Locker.unlock_shared();
	return true;
}
/********************************************************************
函数名称：Session_DLStorage_GetAll
函数功能：获取下载池的任务列表
 参数.一：pppSt_StorageInfo
  In/Out：Out
  类型：三级指针
  可空：N
  意思：输出获取到的下载信息列表
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
bool CSession_DLStroage::Session_DLStorage_GetAll(SESSION_STORAGEINFO*** pppSt_StorageInfo, int* pInt_ListCount)
{
	Session_IsErrorOccur = false;

	if ((NULL == pppSt_StorageInfo) || (NULL == pInt_ListCount))
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_STORAGE_MODULE_SESSION_PARAMENT;
		return false;
	}

	st_Locker.lock_shared();
	*pInt_ListCount = stl_MapStroage.size();
	BaseLib_OperatorMemory_Malloc((XPPPMEM)pppSt_StorageInfo, *pInt_ListCount, sizeof(SESSION_STORAGEINFO));

	unordered_map<string, SESSION_STORAGEINFO>::iterator stl_MapIterator = stl_MapStroage.begin();
	for (int i = 0; stl_MapIterator != stl_MapStroage.end(); stl_MapIterator++, i++)
	{
		*(*pppSt_StorageInfo)[i] = stl_MapIterator->second;
	}
	st_Locker.unlock_shared();

	if (*pInt_ListCount <= 0)
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_STORAGE_MODULE_SESSION_EMPTY;
		return false;
	}
	return true;
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
bool CSession_DLStroage::Session_DLStroage_Delete(LPCXSTR lpszClientAddr)
{
	Session_IsErrorOccur = false;

	st_Locker.lock_shared();
	unordered_map<string, SESSION_STORAGEINFO>::iterator stl_MapIterator = stl_MapStroage.find(lpszClientAddr);
	if (stl_MapIterator != stl_MapStroage.end())
	{
		fclose(stl_MapIterator->second.pSt_File);
		stl_MapStroage.erase(stl_MapIterator);
	}
	st_Locker.unlock_shared();
	return true;
}
/********************************************************************
函数名称：Session_DLStroage_MaxConnect
函数功能：判断一个地址是否超过连接数限制
 参数.一：lpszClientAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要处理的地址
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CSession_DLStroage::Session_DLStroage_MaxConnect(LPCXSTR lpszClientAddr)
{
	Session_IsErrorOccur = false;

	int nExistNumber = 0;
	st_Locker.lock_shared();
	unordered_map<string, SESSION_STORAGEINFO>::iterator stl_MapIterator = stl_MapStroage.begin();
	for (; stl_MapIterator != stl_MapStroage.end(); stl_MapIterator++)
	{
		XCHAR tszIPSource[128];
		XCHAR tszIPDest[128];

		memset(tszIPSource, '\0', sizeof(tszIPSource));
		memset(tszIPDest, '\0', sizeof(tszIPDest));

		_tcsxcpy(tszIPSource, stl_MapIterator->first.c_str());
		_tcsxcpy(tszIPDest, lpszClientAddr);

		BaseLib_OperatorIPAddr_SegAddr(tszIPSource);
		BaseLib_OperatorIPAddr_SegAddr(tszIPDest);

		if (0 == _tcsxcmp(tszIPSource, tszIPDest))
		{
			nExistNumber++;
		}
	}
	st_Locker.unlock_shared();

	if (nExistNumber >= m_nMaxConnect)
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_STORAGE_MODULE_SESSION_MAXCONNECT;
		return false;
	}
	return true;
}