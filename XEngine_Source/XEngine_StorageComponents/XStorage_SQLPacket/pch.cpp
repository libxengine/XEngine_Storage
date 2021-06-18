#include "pch.h"
#include "XStorageSQL_File/XStorageSQL_File.h"
#include "XStorageSQL_Info/XStorageSQL_Info.h"
/********************************************************************
//    Created:     2021/06/10  11:10:16
//    File Name:   D:\XEngine_Storage\XEngine_Source\XEngine_StorageComponents\XStorage_SQLPacket\pch.cpp
//    File Path:   D:\XEngine_Storage\XEngine_Source\XEngine_StorageComponents\XStorage_SQLPacket
//    File Base:   pch
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     导出函数实现
//    History:
*********************************************************************/
BOOL XStorage_IsErrorOccur = FALSE;
DWORD XStorage_dwErrorCode = 0;
//////////////////////////////////////////////////////////////////////////
CXStorageSQL_File m_DBFile;
CXStorageSQL_Info m_DBInfo;
//////////////////////////////////////////////////////////////////////////
///                        导出的函数
//////////////////////////////////////////////////////////////////////////
extern "C" DWORD XStorageDB_GetLastError(int* pInt_SysError)
{
	if (NULL != pInt_SysError)
	{
		*pInt_SysError = errno;
	}
	return XStorage_dwErrorCode;
}
/************************************************************************/
/*                         导出的数据库操作函数                         */
/************************************************************************/
extern "C" BOOL XStorageSQL_Init(DATABASE_MYSQL_CONNECTINFO * pSt_DBConnector, int nTimeMonth)
{
	if (!m_DBFile.XStorageSQL_File_Init(pSt_DBConnector, nTimeMonth))
	{
		return FALSE;
	}
	if (!m_DBInfo.XStorageSQL_Info_Init(pSt_DBConnector, nTimeMonth))
	{
		return FALSE;
	}
	return TRUE;
}
extern "C" BOOL XStorageSQL_Destory()
{
	m_DBFile.XStorageSQL_File_Destory();
	m_DBInfo.XStorageSQL_Info_Destory();
	return TRUE;
}
/************************************************************************/
/*                         导出的信息数据库函数                         */
/************************************************************************/
extern "C" BOOL XStorageSQL_File_FileInsert(XSTORAGECORE_DBFILE * pSt_DBManage)
{
	return m_DBFile.XStorageSQL_File_FileInsert(pSt_DBManage);
}
extern "C" BOOL XStorageSQL_File_FileDelete(LPCTSTR lpszFile, LPCTSTR lpszHash)
{
	return m_DBFile.XStorageSQL_File_FileDelete(lpszFile, lpszHash);
}
extern "C" BOOL XStorageSQL_File_FileQuery(XSTORAGECORE_DBFILE * **pppSt_ListFile, int* pInt_ListCount, LPCTSTR lpszTimeStart, LPCTSTR lpszTimeEnd, LPCTSTR lpszFile, LPCTSTR lpszHash)
{
	return m_DBFile.XStorageSQL_File_FileQuery(pppSt_ListFile, pInt_ListCount, lpszTimeStart, lpszTimeEnd, lpszFile, lpszHash);
}
extern "C" BOOL XStorageSQL_File_FileQueryForTable(XSTORAGECORE_DBFILE * **pppSt_ListFile, int* pInt_ListCount, LPCTSTR lpszTableName)
{
	return m_DBFile.XStorageSQL_File_FileQueryForTable(pppSt_ListFile, pInt_ListCount, lpszTableName);
}
extern "C" BOOL XStorageSQL_File_FileQueryForHash(XSTORAGECORE_DBFILE * pSt_FileInfo, LPCTSTR lpszFileMD5, LPCTSTR lpszUser, LPCTSTR lpszTimeStart, LPCTSTR lpszTimeEnd)
{
	return m_DBFile.XStorageSQL_File_FileQueryForHash(pSt_FileInfo, lpszFileMD5, lpszUser, lpszTimeStart, lpszTimeEnd);
}
extern "C" BOOL XStorageSQL_File_FileGetCount(__int64x * pInt_Count, __int64x * pInt_Size)
{
	return m_DBFile.XStorageSQL_File_FileGetCount(pInt_Count, pInt_Size);
}
/************************************************************************/
/*                         导出的信息数据库函数                         */
/************************************************************************/
extern "C" BOOL XStorageSQL_Info_UserInsert(XSTORAGECORE_USERINFO * pSt_DBUser)
{
	return m_DBInfo.XStorageSQL_Info_UserInsert(pSt_DBUser);
}
extern "C" BOOL XStorageSQL_Info_UserDelete(XSTORAGECORE_USERINFO * pSt_DBUser)
{
	return m_DBInfo.XStorageSQL_Info_UserDelete(pSt_DBUser);
}
extern "C" BOOL XStorageSQL_Info_UserQuery(XSTORAGECORE_USERINFO * pSt_DBUser)
{
	return m_DBInfo.XStorageSQL_Info_UserQuery(pSt_DBUser);
}
extern "C" BOOL XStorageSQL_Info_UserDBFileSet(LPCTSTR lpszUserName, __int64x nFileSize, BOOL bDel)
{
	return m_DBInfo.XStorageSQL_Info_UserDBFileSet(lpszUserName, nFileSize, bDel);
}
extern "C" BOOL XStorageSQL_Info_UserQueryForToken(XNETHANDLE xhToken, XSTORAGECORE_USERINFO * pSt_DBUser)
{
	return m_DBInfo.XStorageSQL_Info_UserQueryForToken(xhToken, pSt_DBUser);
}
extern "C" BOOL XStorageSQL_Info_UserSet(LPCTSTR lpszUser, XNETHANDLE xhToken, int nStatus, LPCTSTR lpszClientAddr)
{
	return m_DBInfo.XStorageSQL_Info_UserSet(lpszUser, xhToken, nStatus, lpszClientAddr);
}
extern "C" BOOL XStorageSQL_Info_UPCount(__int64x nFileSize, BOOL bAdd)
{
	return m_DBInfo.XStorageSQL_Info_UPCount(nFileSize, bAdd);
}