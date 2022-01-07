#pragma once
/********************************************************************
//    Created:     2022/01/06  10:23:13
//    File Name:   D:\XEngine_Storage\XEngine_Source\XEngine_StorageComponents\XStorage_SQLPacket\XStorage_SQLHelp\XStorage_SQLHelp.h
//    File Path:   D:\XEngine_Storage\XEngine_Source\XEngine_StorageComponents\XStorage_SQLPacket\XStorage_SQLHelp
//    File Base:   XStorage_SQLHelp
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     SQL数据库语法帮助类
//    History:
*********************************************************************/

class CXStorage_SQLHelp
{
public:
    CXStorage_SQLHelp();
    ~CXStorage_SQLHelp();
public:
    BOOL XStorage_SQLHelp_Insert(TCHAR* ptszSQLBuffer, XSTORAGECORE_DBFILE* pSt_DBFile);
    BOOL XStorage_SQLHelp_Delete(TCHAR* ptszSQLBuffer, LPCTSTR lpszTableName, LPCTSTR lpszBuckKey = NULL, LPCTSTR lpszFileName = NULL, LPCTSTR lpszFileHash = NULL);
    BOOL XStorage_SQLHelp_Query(TCHAR* ptszSQLBuffer, LPCTSTR lpszTableName, LPCTSTR lpszBuckKey = NULL, LPCTSTR lpszFilePath = NULL, LPCTSTR lpszFileName = NULL, LPCTSTR lpszFileHash = NULL, LPCTSTR lpszFileUser = NULL, LPCTSTR lpszTimeStart = NULL, LPCTSTR lpszTimeEnd = NULL);
protected:
    BOOL XStorage_SQLHelp_Packet(TCHAR* ptszSQLBuffer, LPCTSTR lpszBuckKey = NULL, LPCTSTR lpszFilePath = NULL, LPCTSTR lpszFileName = NULL, LPCTSTR lpszFileHash = NULL, LPCTSTR lpszFileUser = NULL, LPCTSTR lpszTimeStart = NULL, LPCTSTR lpszTimeEnd = NULL);
};
