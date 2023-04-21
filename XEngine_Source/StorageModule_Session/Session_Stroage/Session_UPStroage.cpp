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
 参数.一：nMaxConnect
  In/Out：In
  类型：整数型
  可空：N
  意思：输入连接数限制
 参数.二：bUPResume
  In/Out：In
  类型：逻辑型
  可空：Y
  意思：是否启用断点上传
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CSession_UPStroage::Session_UPStroage_Init(int nMaxConnect, bool bUPResume /* = false */)
{
	Session_IsErrorOccur = false;

	m_bResume = bUPResume;
	m_nMaxConnect = nMaxConnect;
	return true;
}
/********************************************************************
函数名称：Session_UPStroage_Destory
函数功能：销毁下载管理器
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CSession_UPStroage::Session_UPStroage_Destory()
{
	Session_IsErrorOccur = false;

	st_Locker.lock();
	stl_MapStroage.clear();
	st_Locker.unlock();

	return true;
}
/********************************************************************
函数名称：Session_UPStroage_Insert
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
 参数.四：nFileSize
  In/Out：Out
  类型：整数型
  可空：N
  意思：输入文件大小
 参数.五：bRewrite
  In/Out：In
  类型：整数型
  可空：N
  意思：是否允许覆写
 参数.六：nPosStart
  In/Out：In
  类型：整数型
  可空：Y
  意思：输入起始位置
 参数.七：nPostEnd
  In/Out：In
  类型：整数型
  可空：Y
  意思：输入结束位置
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CSession_UPStroage::Session_UPStroage_Insert(LPCXSTR lpszClientAddr, LPCXSTR lpszBuckKey, LPCXSTR lpszFileDir, __int64x nFileSize, bool bRewrite, int nPosStart /* = 0 */, int nPostEnd /* = 0 */)
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
	unordered_map<string, SESSION_STORAGEUPLOADER>::iterator stl_MapIterator = stl_MapStroage.find(lpszClientAddr);
	if (stl_MapIterator != stl_MapStroage.end())
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_STORAGE_MODULE_SESSION_EXIST;
		st_Locker.unlock_shared();
		return false;
	}
	st_Locker.unlock_shared();

	SESSION_STORAGEUPLOADER st_Client;
	memset(&st_Client, '\0', sizeof(SESSION_STORAGEUPLOADER));
	//填充下载信息
	st_Client.st_StorageInfo.ullPosStart = nPosStart;
	st_Client.st_StorageInfo.ullPosEnd = nPostEnd;
	st_Client.st_StorageInfo.ullCount = nFileSize;
	_tcsxcpy(st_Client.st_StorageInfo.tszBuckKey, lpszBuckKey);
	_tcsxcpy(st_Client.st_StorageInfo.tszFileDir, lpszFileDir);
	_tcsxcpy(st_Client.st_StorageInfo.tszClientAddr, lpszClientAddr);
	//文件是否存在
	if ((m_bResume) && ((0 != nPosStart) || (0 != nPostEnd)) && (0 == _xtaccess(lpszFileDir, 0)))
	{
		struct _xtstat st_FStat;
		memset(&st_FStat, '\0', sizeof(struct _xtstat));
		_xtstat(st_Client.st_StorageInfo.tszFileDir, &st_FStat);
		st_Client.st_StorageInfo.ullRWLen = st_FStat.st_size;
		//追加打开
		st_Client.st_StorageInfo.pSt_File = _xtfopen(lpszFileDir, _X("rb+"));
		if (NULL == st_Client.st_StorageInfo.pSt_File)
		{
			Session_IsErrorOccur = true;
			Session_dwErrorCode = ERROR_STORAGE_MODULE_SESSION_OPENFILE;
			return false;
		}
		//是不是覆写?
		if (st_Client.st_StorageInfo.ullRWLen > nPosStart)
		{
			//是否允许覆写
			if (!bRewrite)
			{
				Session_IsErrorOccur = true;
				Session_dwErrorCode = ERROR_STORAGE_MODULE_SESSION_REWRITE;
				fclose(st_Client.st_StorageInfo.pSt_File);
				return false;
			}
			st_Client.st_StorageInfo.bRewrite = true;
			st_Client.st_StorageInfo.ullRWLen -= (nPostEnd - nPosStart);
		}
		fseek(st_Client.st_StorageInfo.pSt_File, nPosStart, SEEK_SET);
	}
	else
	{
		st_Client.st_StorageInfo.pSt_File = _xtfopen(lpszFileDir, _X("wb"));
		if (NULL == st_Client.st_StorageInfo.pSt_File)
		{
			Session_IsErrorOccur = true;
			Session_dwErrorCode = ERROR_STORAGE_MODULE_SESSION_OPENFILE;
			return false;
		}
	}
	st_Locker.lock();
	stl_MapStroage.insert(make_pair(lpszClientAddr, st_Client));
	st_Locker.unlock();
	return true;
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
bool CSession_UPStroage::Session_UPStroage_GetInfo(LPCXSTR lpszClientAddr, SESSION_STORAGEINFO* pSt_StorageInfo)
{
	Session_IsErrorOccur = false;

	if ((NULL == lpszClientAddr) || (NULL == pSt_StorageInfo))
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_STORAGE_MODULE_SESSION_PARAMENT;
		return false;
	}

	st_Locker.lock_shared();
	unordered_map<string, SESSION_STORAGEUPLOADER>::iterator stl_MapIterator = stl_MapStroage.find(lpszClientAddr);
	if (stl_MapIterator == stl_MapStroage.end())
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_STORAGE_MODULE_SESSION_NOTFOUND;
		st_Locker.unlock_shared();
		return false;
	}
	*pSt_StorageInfo = stl_MapIterator->second.st_StorageInfo;
	st_Locker.unlock_shared();
	return true;
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
bool CSession_UPStroage::Session_UPStroage_Write(LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen)
{
	Session_IsErrorOccur = false;

	if ((NULL == lpszClientAddr) || (NULL == lpszMsgBuffer))
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_STORAGE_MODULE_SESSION_PARAMENT;
		return false;
	}

	st_Locker.lock_shared();
	unordered_map<string, SESSION_STORAGEUPLOADER>::iterator stl_MapIterator = stl_MapStroage.find(lpszClientAddr);
	if (stl_MapIterator == stl_MapStroage.end())
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_STORAGE_MODULE_SESSION_NOTFOUND;
		st_Locker.unlock_shared();
		return false;
	}
	int nCount = nMsgLen;
	int nWLen = 0;
	while (true)
	{
		int nRet = fwrite(lpszMsgBuffer + nWLen, 1, nCount - nWLen, stl_MapIterator->second.st_StorageInfo.pSt_File);
		nWLen += nRet;
		//直到写完
		if (nWLen >= nCount)
		{
			break;
		}
	}
	stl_MapIterator->second.st_StorageInfo.ullRWLen += nWLen;
	st_Locker.unlock_shared();
	return true;
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
bool CSession_UPStroage::Session_UPStroage_Exist(LPCXSTR lpszClientAddr)
{
	Session_IsErrorOccur = false;

	bool bRet = false;
	st_Locker.lock();
	unordered_map<string, SESSION_STORAGEUPLOADER>::iterator stl_MapIterator = stl_MapStroage.find(lpszClientAddr);
	if (stl_MapIterator == stl_MapStroage.end())
	{
		bRet = false;
	}
	else
	{
		bRet = true;
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
bool CSession_UPStroage::Session_UPStorage_GetAll(SESSION_STORAGEINFO*** pppSt_StorageInfo, int* pInt_ListCount)
{
	Session_IsErrorOccur = false;

	if (NULL == pInt_ListCount)
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_STORAGE_MODULE_SESSION_PARAMENT;
		return false;
	}

	if (NULL == pppSt_StorageInfo)
	{
		*pInt_ListCount = stl_MapStroage.size();
		return true;
	}

	st_Locker.lock_shared();
	*pInt_ListCount = stl_MapStroage.size();
	BaseLib_OperatorMemory_Malloc((XPPPMEM)pppSt_StorageInfo, *pInt_ListCount, sizeof(SESSION_STORAGEINFO));
	unordered_map<string, SESSION_STORAGEUPLOADER>::iterator stl_MapIterator = stl_MapStroage.begin();
	for (int i = 0; stl_MapIterator != stl_MapStroage.end(); stl_MapIterator++, i++)
	{
		*(*pppSt_StorageInfo)[i] = stl_MapIterator->second.st_StorageInfo;
	}
	st_Locker.unlock_shared();

	if (0 == *pInt_ListCount)
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_STORAGE_MODULE_SESSION_EMPTY;
		return false;
	}
	return true;
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
bool CSession_UPStroage::Session_UPStroage_Delete(LPCXSTR lpszClientAddr)
{
	Session_IsErrorOccur = false;

	st_Locker.lock();
	unordered_map<string, SESSION_STORAGEUPLOADER>::iterator stl_MapIterator = stl_MapStroage.find(lpszClientAddr);
	if (stl_MapIterator != stl_MapStroage.end())
	{
		if (NULL != stl_MapIterator->second.st_StorageInfo.pSt_File)
		{
			fclose(stl_MapIterator->second.st_StorageInfo.pSt_File);
		}
		//大小是否足够
		if ((stl_MapIterator->second.st_StorageInfo.ullCount != stl_MapIterator->second.st_StorageInfo.ullRWLen) && !m_bResume)
		{
			_xtremove(stl_MapIterator->second.st_StorageInfo.tszFileDir);
		}
		stl_MapStroage.erase(stl_MapIterator);
	}
	st_Locker.unlock();
	return true;
}
/********************************************************************
函数名称：Session_UPStroage_Close
函数功能：关闭读写文件句柄
 参数.一：lpszClientAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：要关闭的客户端会话
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CSession_UPStroage::Session_UPStroage_Close(LPCXSTR lpszClientAddr)
{
	Session_IsErrorOccur = false;

	st_Locker.lock_shared();
	unordered_map<string, SESSION_STORAGEUPLOADER>::iterator stl_MapIterator = stl_MapStroage.find(lpszClientAddr);
	if (stl_MapIterator != stl_MapStroage.end())
	{
		if (NULL != stl_MapIterator->second.st_StorageInfo.pSt_File)
		{
			fclose(stl_MapIterator->second.st_StorageInfo.pSt_File);
		}
	}
	st_Locker.unlock_shared();
	return true;
}
/********************************************************************
函数名称：Session_UPStroage_MaxConnect
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
bool CSession_UPStroage::Session_UPStroage_MaxConnect(LPCXSTR lpszClientAddr)
{
	Session_IsErrorOccur = false;

	int nExistNumber = 0;
	st_Locker.lock_shared();
	unordered_map<string, SESSION_STORAGEUPLOADER>::iterator stl_MapIterator = stl_MapStroage.begin();
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