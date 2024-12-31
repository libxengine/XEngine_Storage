#include "pch.h"
#include "Database_Help.h"
/********************************************************************
//    Created:     2022/03/30  11:20:17
//    File Name:   D:\XEngine_Storage\XEngine_Source\StorageModule_Database\Database_Help\Database_Help.cpp
//    File Path:   D:\XEngine_Storage\XEngine_Source\StorageModule_Database\Database_Help
//    File Base:   Database_Help
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     数据库语法帮助类
//    History:
*********************************************************************/
CDatabase_Help::CDatabase_Help()
{
}
CDatabase_Help::~CDatabase_Help()
{
}
//////////////////////////////////////////////////////////////////////////
//                         公有函数
//////////////////////////////////////////////////////////////////////////
/********************************************************************
函数名称：Database_Help_Insert
函数功能：数据插入函数
 参数.一：ptszSQLBuffer
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出打好包的插入语句
 参数.二：pSt_DBFile
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：输入要插入的数据
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CDatabase_Help::Database_Help_Insert(XCHAR* ptszSQLBuffer, XSTORAGECORE_DBFILE* pSt_DBFile)
{
	Database_IsErrorOccur = false;

	XCHAR tszTableName[64];
	XENGINE_LIBTIME st_LibTimer;

	memset(tszTableName, '\0', sizeof(tszTableName));
	memset(&st_LibTimer, '\0', sizeof(XENGINE_LIBTIME));
	//获得插入日期表
	BaseLib_Time_GetSysTime(&st_LibTimer);

	if (_tcsxlen(pSt_DBFile->tszTableName) > 0)
	{
		_tcsxcpy(tszTableName, pSt_DBFile->tszTableName);
	}
	else
	{
		_xstprintf(tszTableName, _X("%04d%02d"), st_LibTimer.wYear, st_LibTimer.wMonth);
	}

	_xstprintf(ptszSQLBuffer, _X("INSERT INTO `%s` (BuckKey,FilePath,FileName,FileHash,FileUser,FileSize,FileTime) VALUES('%s','%s','%s','%s','%s',%lld,'%04d-%02d-%02d %02d:%02d:%02d')"), tszTableName, pSt_DBFile->tszBuckKey, pSt_DBFile->st_ProtocolFile.tszFilePath, pSt_DBFile->st_ProtocolFile.tszFileName, pSt_DBFile->st_ProtocolFile.tszFileHash, pSt_DBFile->st_ProtocolFile.tszFileUser, pSt_DBFile->st_ProtocolFile.nFileSize, st_LibTimer.wYear, st_LibTimer.wMonth, st_LibTimer.wDay, st_LibTimer.wHour, st_LibTimer.wMinute, st_LibTimer.wSecond);
	return true;
}
/********************************************************************
函数名称：Database_Help_Delete
函数功能：删除语句
 参数.一：ptszSQLBuffer
  In/Out：In
  类型：字符指针
  可空：N
  意思：输出打好包的查询语句
 参数.二：lpszTableName
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入要查询的表名称
 参数.三：lpszBuckKey
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入BUCKET名
 参数.四：lpszFilePath
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入文件路径
 参数.五：lpszFileName
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入文件名称
 参数.六：lpszFileHash
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入文件HASH值
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CDatabase_Help::Database_Help_Delete(XCHAR* ptszSQLBuffer, LPCXSTR lpszTableName, LPCXSTR lpszBuckKey /* = NULL */, LPCXSTR lpszFilePath /* = NULL */, LPCXSTR lpszFileName /* = NULL */, LPCXSTR lpszFileHash /* = NULL */)
{
	Database_IsErrorOccur = false;

	XCHAR tszSQLQuery[1024];
	memset(tszSQLQuery, '\0', sizeof(tszSQLQuery));

	Database_Help_Packet(tszSQLQuery, lpszBuckKey, lpszFilePath, lpszFileName, lpszFileHash);
	_xstprintf(ptszSQLBuffer, _X("DELETE FROM `%s` %s"), lpszTableName, tszSQLQuery);
	return true;
}
/********************************************************************
函数名称：Database_Help_Query
函数功能：数据库查询打包函数
 参数.一：ptszSQLBuffer
  In/Out：In
  类型：字符指针
  可空：N
  意思：输出打好包的查询语句
 参数.二：lpszTableName
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入要查询的表名称
 参数.三：lpszBuckKey
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入BUCKET名
 参数.四：lpszFilePath
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入文件路径
 参数.五：lpszFileName
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入文件名称
 参数.六：lpszFileHash
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入文件HASH值
 参数.七：lpszFileUser
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入文件所属用户
 参数.八：lpszTimeStart
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入文件开始时间范围
 参数.九：lpszTimeEnd
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入文件结束时间范围
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CDatabase_Help::Database_Help_Query(XCHAR* ptszSQLBuffer, LPCXSTR lpszTableName, LPCXSTR lpszBuckKey /* = NULL */, LPCXSTR lpszFilePath /* = NULL */, LPCXSTR lpszFileName /* = NULL */, LPCXSTR lpszFileHash /* = NULL */, LPCXSTR lpszFileUser /* = NULL */, LPCXSTR lpszTimeStart /* = NULL */, LPCXSTR lpszTimeEnd /* = NULL */)
{
	Database_IsErrorOccur = false;

	XCHAR tszSQLQuery[1024];
	memset(tszSQLQuery, '\0', sizeof(tszSQLQuery));

	_xstprintf(ptszSQLBuffer, _X("SELECT * FROM `%s`"), lpszTableName);
	Database_Help_Packet(tszSQLQuery, lpszBuckKey, lpszFilePath, lpszFileName, lpszFileHash, lpszFileUser, lpszTimeStart, lpszTimeEnd);
	_tcsxcat(ptszSQLBuffer, tszSQLQuery);
	return true;
}
//////////////////////////////////////////////////////////////////////////
//                          保护函数
//////////////////////////////////////////////////////////////////////////
/********************************************************************
函数名称：Database_Help_Packet
函数功能：数据库查询语句打包函数
 参数.一：ptszSQLBuffer
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出数据库语法
 参数.二：lpszBuckKey
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入BUCKET名
 参数.三：lpszFilePath
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入路径
 参数.四：lpszFileName
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入文件名
 参数.五：lpszFileHash
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入HASH
 参数.六：lpszFileUser
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入文件所属用户
 参数.七：lpszTimeStart
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入查找文件的起始时间
 参数.八：lpszTimeEnd
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入查找文件的结束时间
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CDatabase_Help::Database_Help_Packet(XCHAR* ptszSQLBuffer, LPCXSTR lpszBuckKey /* = NULL */, LPCXSTR lpszFilePath /* = NULL */, LPCXSTR lpszFileName /* = NULL */, LPCXSTR lpszFileHash /* = NULL */, LPCXSTR lpszFileUser /* = NULL */, LPCXSTR lpszTimeStart /* = NULL */, LPCXSTR lpszTimeEnd /* = NULL */)
{
	Database_IsErrorOccur = false;

	bool bInit = false;
	XCHAR tszSQLQuery[MAX_PATH];
	memset(tszSQLQuery, '\0', MAX_PATH);
	//文件所属BUCKET
	if (NULL != lpszBuckKey)
	{
		if (_tcsxlen(lpszBuckKey) > 0)
		{
			if (bInit)
			{
				_tcsxcat(ptszSQLBuffer, _X(" AND "));
			}
			else
			{
				_tcsxcat(ptszSQLBuffer, _X(" WHERE "));
			}
			memset(tszSQLQuery, '\0', MAX_PATH);
			_xstprintf(tszSQLQuery, _X("BuckKey = '%s'"), lpszBuckKey);
			_tcsxcat(ptszSQLBuffer, tszSQLQuery);
			bInit = true;
		}
	}
	//文件路径
	if (NULL != lpszFilePath)
	{
		if (_tcsxlen(lpszFilePath) > 0)
		{
			if (bInit)
			{
				_tcsxcat(ptszSQLBuffer, _X(" AND "));
			}
			else
			{
				_tcsxcat(ptszSQLBuffer, _X("WHERE "));
			}
			memset(tszSQLQuery, '\0', MAX_PATH);
			_xstprintf(tszSQLQuery, _X("FilePath = '%s'"), lpszFilePath);
			_tcsxcat(ptszSQLBuffer, tszSQLQuery);
			bInit = true;
		}
	}
	//文件名称
	if (NULL != lpszFileName)
	{
		if (_tcsxlen(lpszFileName) > 0)
		{
			if (bInit)
			{
				_tcsxcat(ptszSQLBuffer, _X(" AND "));
			}
			else
			{
				_tcsxcat(ptszSQLBuffer, _X("WHERE "));
			}
			memset(tszSQLQuery, '\0', MAX_PATH);
			_xstprintf(tszSQLQuery, _X("FileName = '%s'"), lpszFileName);
			_tcsxcat(ptszSQLBuffer, tszSQLQuery);
			bInit = true;
		}
	}
	//文件HASH
	if (NULL != lpszFileHash)
	{
		if (_tcsxlen(lpszFileHash) > 0)
		{
			if (bInit)
			{
				_tcsxcat(ptszSQLBuffer, _X(" AND "));
			}
			else
			{
				_tcsxcat(ptszSQLBuffer, _X("WHERE "));
			}
			memset(tszSQLQuery, '\0', MAX_PATH);
			_xstprintf(tszSQLQuery, _X("FileHash = '%s'"), lpszFileHash);
			_tcsxcat(ptszSQLBuffer, tszSQLQuery);
			bInit = true;
		}
	}
	//文件所属用户
	if (NULL != lpszFileUser)
	{
		if (_tcsxlen(lpszFileUser) > 0)
		{
			if (bInit)
			{
				_tcsxcat(ptszSQLBuffer, _X(" AND "));
			}
			else
			{
				_tcsxcat(ptszSQLBuffer, _X("WHERE "));
			}
			memset(tszSQLQuery, '\0', MAX_PATH);
			_xstprintf(tszSQLQuery, _X("FileUser = '%s'"), lpszFileUser);
			_tcsxcat(ptszSQLBuffer, tszSQLQuery);
			bInit = true;
		}
	}
	//时间范围
	if ((NULL != lpszTimeStart) && (NULL != lpszTimeEnd))
	{
		if ((_tcsxlen(lpszTimeStart) > 0) && (_tcsxlen(lpszTimeEnd) > 0))
		{
			if (bInit)
			{
				_tcsxcat(ptszSQLBuffer, _X(" AND "));
			}
			else
			{
				_tcsxcat(ptszSQLBuffer, _X("WHERE "));
			}
			memset(tszSQLQuery, '\0', MAX_PATH);
			_xstprintf(tszSQLQuery, _X("BETWEEN '%s' AND '%s'"), lpszTimeStart, lpszTimeEnd);
			_tcsxcat(ptszSQLBuffer, tszSQLQuery);
			bInit = true;
		}
	}

	return true;
}