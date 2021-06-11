#pragma once
/********************************************************************
//    Created:     2021/06/10  11:09:31
//    File Name:   D:\XEngine_Storage\XEngine_Source\XEngine_StorageComponents\XStorage_SQLPacket\XStorageSQL_Info\XStorageSQL_Info.h
//    File Path:   D:\XEngine_Storage\XEngine_Source\XEngine_StorageComponents\XStorage_SQLPacket\XStorageSQL_Info
//    File Base:   XStorageSQL_Info
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     数据库管理器
//    History:
*********************************************************************/

class CXStorageSQL_Info
{
public:
    CXStorageSQL_Info();
    ~CXStorageSQL_Info();
public:
    BOOL XStorageSQL_Info_Init(DATABASE_MYSQL_CONNECTINFO *pSt_DBConnector, int nTimeDay = 30);
    BOOL XStorageSQL_Info_Destory();
public:
    BOOL XStorageSQL_Info_UserInsert(XSTORAGECORE_USERINFO *pSt_DBUser);
    BOOL XStorageSQL_Info_UserDelete(XSTORAGECORE_USERINFO *pSt_DBUser);
    BOOL XStorageSQL_Info_UserQuery(XSTORAGECORE_USERINFO *pSt_DBUser);
    BOOL XStorageSQL_Info_UserDBFileSet(LPCTSTR lpszUserName, __int64x nFileSize, BOOL bDel = FALSE);
    BOOL XStorageSQL_Info_UserQueryForToken(XNETHANDLE xhToken, XSTORAGECORE_USERINFO* pSt_DBUser);
    BOOL XStorageSQL_Info_UserSet(LPCTSTR lpszUser, XNETHANDLE xhToken = 0, int nStatus = 0, LPCTSTR lpszClientAddr = NULL);
    BOOL XStorageSQL_Info_UPCount(__int64x nFileSize, BOOL bAdd);
private:
    XHDATA xhDBSQL;
};
