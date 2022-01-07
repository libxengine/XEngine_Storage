#include "pch.h"
#include "XStorage_SQLHelp.h"
/********************************************************************
//    Created:     2022/01/06  10:24:07
//    File Name:   D:\XEngine_Storage\XEngine_Source\XEngine_StorageComponents\XStorage_SQLPacket\XStorage_SQLHelp\XStorage_SQLHelp.cpp
//    File Path:   D:\XEngine_Storage\XEngine_Source\XEngine_StorageComponents\XStorage_SQLPacket\XStorage_SQLHelp
//    File Base:   XStorage_SQLHelp
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     SQL数据库语法帮助类
//    History:
*********************************************************************/
CXStorage_SQLHelp::CXStorage_SQLHelp()
{
}
CXStorage_SQLHelp::~CXStorage_SQLHelp()
{
}
//////////////////////////////////////////////////////////////////////////
//                         公有函数
//////////////////////////////////////////////////////////////////////////
/********************************************************************
函数名称：XStorage_SQLHelp_Insert
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
BOOL CXStorage_SQLHelp::XStorage_SQLHelp_Insert(TCHAR* ptszSQLBuffer, XSTORAGECORE_DBFILE* pSt_DBFile)
{
	XStorage_IsErrorOccur = FALSE;

	TCHAR tszTableName[64];
	XENGINE_LIBTIMER st_LibTimer;

	memset(tszTableName, '\0', sizeof(tszTableName));
	memset(&st_LibTimer, '\0', sizeof(XENGINE_LIBTIMER));
	//获得插入日期表
	BaseLib_OperatorTime_GetSysTime(&st_LibTimer);

	if (_tcslen(pSt_DBFile->tszTableName) > 0)
	{
		_tcscpy(tszTableName, pSt_DBFile->tszTableName);
	}
	else
	{
		_stprintf(tszTableName, _T("%04d%02d"), st_LibTimer.wYear, st_LibTimer.wMonth);
	}

	_stprintf(ptszSQLBuffer, _T("INSERT INTO `%s` (BuckKey,FilePath,FileName,FileHash,FileUser,FileSize,FileTime) VALUES('%s','%s','%s','%s','%s',%lld,%04d-%02d-%02d %02d:%02d:%02d)"), tszTableName, pSt_DBFile->tszBuckKey, pSt_DBFile->st_ProtocolFile.tszFilePath, pSt_DBFile->st_ProtocolFile.tszFileName, pSt_DBFile->st_ProtocolFile.tszFileHash, pSt_DBFile->st_ProtocolFile.tszFileUser, pSt_DBFile->st_ProtocolFile.nFileSize, st_LibTimer.wYear, st_LibTimer.wMonth, st_LibTimer.wDay, st_LibTimer.wHour, st_LibTimer.wMinute, st_LibTimer.wSecond);
	return TRUE;
}
/********************************************************************
函数名称：XStorage_SQLHelp_Delete
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
 参数.四：lpszFileName
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入文件名称
 参数.五：lpszFileHash
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入文件HASH值
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CXStorage_SQLHelp::XStorage_SQLHelp_Delete(TCHAR* ptszSQLBuffer, LPCTSTR lpszTableName, LPCTSTR lpszBuckKey /* = NULL */, LPCTSTR lpszFileName /* = NULL */, LPCTSTR lpszFileHash /* = NULL */)
{
	XStorage_IsErrorOccur = FALSE;

	TCHAR tszSQLQuery[1024];
	memset(tszSQLQuery, '\0', sizeof(tszSQLQuery));

	XStorage_SQLHelp_Packet(tszSQLQuery, lpszBuckKey, NULL, lpszFileName, lpszFileHash);
	_stprintf(ptszSQLBuffer, _T("DELETE FROM `%s` %s"), lpszTableName, tszSQLQuery);
	return TRUE;
}
/********************************************************************
函数名称：XStorage_SQLHelp_Query
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
BOOL CXStorage_SQLHelp::XStorage_SQLHelp_Query(TCHAR* ptszSQLBuffer, LPCTSTR lpszTableName, LPCTSTR lpszBuckKey /* = NULL */, LPCTSTR lpszFilePath /* = NULL */, LPCTSTR lpszFileName /* = NULL */, LPCTSTR lpszFileHash /* = NULL */, LPCTSTR lpszFileUser /* = NULL */, LPCTSTR lpszTimeStart /* = NULL */, LPCTSTR lpszTimeEnd /* = NULL */)
{
    XStorage_IsErrorOccur = FALSE;

    BOOL bInit = FALSE;
    TCHAR tszSQLQuery[1024];
    memset(tszSQLQuery, '\0', sizeof(tszSQLQuery));

	_stprintf(ptszSQLBuffer, _T("SELECT * FROM `%s`"), lpszTableName);
	XStorage_SQLHelp_Packet(tszSQLQuery, lpszBuckKey, lpszFilePath, lpszFileName, lpszFileHash, lpszFileUser, lpszTimeStart, lpszTimeEnd);
	_tcscat(ptszSQLBuffer, tszSQLQuery);
    return TRUE;
}
//////////////////////////////////////////////////////////////////////////
//                          保护函数
//////////////////////////////////////////////////////////////////////////
BOOL CXStorage_SQLHelp::XStorage_SQLHelp_Packet(TCHAR* ptszSQLBuffer, LPCTSTR lpszBuckKey /* = NULL */, LPCTSTR lpszFilePath /* = NULL */, LPCTSTR lpszFileName /* = NULL */, LPCTSTR lpszFileHash /* = NULL */, LPCTSTR lpszFileUser /* = NULL */, LPCTSTR lpszTimeStart /* = NULL */, LPCTSTR lpszTimeEnd /* = NULL */)
{
	XStorage_IsErrorOccur = FALSE;

	BOOL bInit = FALSE;
	TCHAR tszSQLQuery[MAX_PATH];
	memset(tszSQLQuery, '\0', MAX_PATH);
	//文件所属BUCKET
	if (NULL != lpszBuckKey)
	{
		if (_tcslen(lpszBuckKey) > 0)
		{
			if (bInit)
			{
				_tcscat(ptszSQLBuffer, _T(" AND "));
			}
			else
			{
				_tcscat(ptszSQLBuffer, _T("WHERE "));
			}
			memset(tszSQLQuery, '\0', MAX_PATH);
			_stprintf(tszSQLQuery, _T("BuckKey = '%s'"), lpszBuckKey);
			_tcscat(ptszSQLBuffer, tszSQLQuery);
			bInit = TRUE;
		}
	}
	//文件路径
	if (NULL != lpszFilePath)
	{
		if (_tcslen(lpszFilePath) > 0)
		{
			if (bInit)
			{
				_tcscat(ptszSQLBuffer, _T(" AND "));
			}
			else
			{
				_tcscat(ptszSQLBuffer, _T("WHERE "));
			}
			memset(tszSQLQuery, '\0', MAX_PATH);
			_stprintf(tszSQLQuery, _T("FilePath = '%s'"), lpszFilePath);
			_tcscat(ptszSQLBuffer, tszSQLQuery);
			bInit = TRUE;
		}
	}
	//文件名称
	if (NULL != lpszFileName)
	{
		if (_tcslen(lpszFileName) > 0)
		{
			if (bInit)
			{
				_tcscat(ptszSQLBuffer, _T(" AND "));
			}
			else
			{
				_tcscat(ptszSQLBuffer, _T("WHERE "));
			}
			memset(tszSQLQuery, '\0', MAX_PATH);
			_stprintf(tszSQLQuery, _T("FileName = '%s'"), lpszFileName);
			_tcscat(ptszSQLBuffer, tszSQLQuery);
			bInit = TRUE;
		}
	}
	//文件HASH
	if (NULL != lpszFileHash)
	{
		if (_tcslen(lpszFileHash) > 0)
		{
			if (bInit)
			{
				_tcscat(ptszSQLBuffer, _T(" AND "));
			}
			else
			{
				_tcscat(ptszSQLBuffer, _T("WHERE "));
			}
			memset(tszSQLQuery, '\0', MAX_PATH);
			_stprintf(tszSQLQuery, _T("FileHash = '%s'"), lpszFileHash);
			_tcscat(ptszSQLBuffer, tszSQLQuery);
			bInit = TRUE;
		}
	}
	//文件所属用户
	if (NULL != lpszFileUser)
	{
		if (_tcslen(lpszFileUser) > 0)
		{
			if (bInit)
			{
				_tcscat(ptszSQLBuffer, _T(" AND "));
			}
			else
			{
				_tcscat(ptszSQLBuffer, _T("WHERE "));
			}
			memset(tszSQLQuery, '\0', MAX_PATH);
			_stprintf(tszSQLQuery, _T("FileUser = '%s'"), lpszFileUser);
			_tcscat(ptszSQLBuffer, tszSQLQuery);
			bInit = TRUE;
		}
	}
	//时间范围
	if ((NULL != lpszTimeStart) && (NULL != lpszTimeEnd))
	{
		if ((_tcslen(lpszTimeStart) > 0) && (_tcslen(lpszTimeEnd) > 0))
		{
			if (bInit)
			{
				_tcscat(ptszSQLBuffer, _T(" AND "));
			}
			else
			{
				_tcscat(ptszSQLBuffer, _T("WHERE "));
			}
			memset(tszSQLQuery, '\0', MAX_PATH);
			_stprintf(tszSQLQuery, _T("BETWEEN '%s' AND '%s'"), lpszTimeStart, lpszTimeEnd);
			_tcscat(ptszSQLBuffer, tszSQLQuery);
			bInit = TRUE;
		}
	}

	return TRUE;
}