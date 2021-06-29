#include "pch.h"
#include "XStorageSQL_Info.h"
/********************************************************************
//    Created:     2021/06/10  11:09:38
//    File Name:   D:\XEngine_Storage\XEngine_Source\XEngine_StorageComponents\XStorage_SQLPacket\XStorageSQL_Info\XStorageSQL_Info.cpp
//    File Path:   D:\XEngine_Storage\XEngine_Source\XEngine_StorageComponents\XStorage_SQLPacket\XStorageSQL_Info
//    File Base:   XStorageSQL_Info
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     数据库管理器
//    History:
*********************************************************************/
CXStorageSQL_Info::CXStorageSQL_Info()
{
    xhDBSQL = 0;
}
CXStorageSQL_Info::~CXStorageSQL_Info()
{

}
//////////////////////////////////////////////////////////////////////////
//                         公有函数
//////////////////////////////////////////////////////////////////////////
/********************************************************************
函数名称：XStorageSQL_Info_Init
函数功能：初始化存储服务数据库管理器
 参数.一：pSt_DBConnector
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：数据MYSQL数据库连接信息
 参数.二：nTimeDay
  In/Out：In
  类型：整数型
  可空：N
  意思：输入文件数据保留时间
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CXStorageSQL_Info::XStorageSQL_Info_Init(DATABASE_MYSQL_CONNECTINFO *pSt_DBConnector, int nTimeDay)
{
    XStorage_IsErrorOccur = FALSE;

    if (NULL == pSt_DBConnector)
    {
        XStorage_IsErrorOccur = TRUE;
        XStorage_dwErrorCode = ERROR_XENGINE_XSTROGE_CORE_DB_INIT_PARAMENT;
        return FALSE;
    }
#ifdef _WINDOWS
    LPCTSTR lpszStrCharset = _T("gbk");
#else
    LPCTSTR lpszStrCharset = _T("utf8");
#endif
    //连接数据库
    _tcscpy(pSt_DBConnector->tszDBName, _T("XEngine"));
    if (!DataBase_MySQL_Connect(&xhDBSQL, pSt_DBConnector, 5, TRUE, lpszStrCharset))
    {
        XStorage_IsErrorOccur = TRUE;
        XStorage_dwErrorCode = DataBase_GetLastError();
        return FALSE;
    }
    return TRUE;
}
/********************************************************************
函数名称：XStorageSQL_Info_Destory
函数功能：销毁数据库管理器
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CXStorageSQL_Info::XStorageSQL_Info_Destory()
{
    XStorage_IsErrorOccur = FALSE;

    DataBase_MySQL_Close(xhDBSQL);

    return TRUE;
}
/********************************************************************
函数名称：XStorageSQL_Info_UserInsert
函数功能：用户插入
 参数.一：pSt_DBUser
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：要插入的信息
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CXStorageSQL_Info::XStorageSQL_Info_UserInsert(XSTORAGECORE_USERINFO *pSt_DBUser)
{
    XStorage_IsErrorOccur = FALSE;

    //是否存在,存在直接返回真
    if (XStorageSQL_Info_UserQuery(pSt_DBUser))
    {
        return TRUE;
    }
    TCHAR tszSQLStatement[2048];
    memset(tszSQLStatement, '\0', sizeof(tszSQLStatement));
    //插入语句
    _stprintf_s(tszSQLStatement, _T("INSERT INTO `XStorage_User` (UserName,UserPass,FileCount,FileSize,PhoneNumber,IDNumber,PermissionLeave,Status,EMailAddr,CreateTime) VALUES('%s','%s',0,0,%lld,%lld,%d,0,'%s',now())"), pSt_DBUser->st_ProtocolUser.tszUserName, pSt_DBUser->st_ProtocolUser.tszUserPass, pSt_DBUser->st_ProtocolUser.nPhoneNumber, pSt_DBUser->st_ProtocolUser.nIDNumber, pSt_DBUser->st_ProtocolUser.nUserLevel, pSt_DBUser->st_ProtocolUser.tszEMailAddr);
    if (!DataBase_MySQL_Execute(xhDBSQL, tszSQLStatement))
    {
        XStorage_IsErrorOccur = TRUE;
        XStorage_dwErrorCode = DataBase_GetLastError();
        return FALSE;
    }
    return TRUE;
}
/********************************************************************
函数名称：XStorageSQL_Info_UserDelete
函数功能：用户删除
 参数.一：pSt_DBUser
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：要删除的信息
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CXStorageSQL_Info::XStorageSQL_Info_UserDelete(XSTORAGECORE_USERINFO *pSt_DBUser)
{
    XStorage_IsErrorOccur = FALSE;

    TCHAR tszSQLStatement[2048];
    memset(tszSQLStatement, '\0', sizeof(tszSQLStatement));
    //插入语句
    _stprintf_s(tszSQLStatement, _T("DELETE * FROM `XStorage_User` WHERE UserName = '%s'"), pSt_DBUser->st_ProtocolUser.tszUserName);
    if (!DataBase_MySQL_Execute(xhDBSQL, tszSQLStatement))
    {
        XStorage_IsErrorOccur = TRUE;
        XStorage_dwErrorCode = DataBase_GetLastError();
        return FALSE;
    }
    return TRUE;
}
/********************************************************************
函数名称：XStorageSQL_Info_UserQuery
函数功能：用户查询
 参数.一：pSt_DBUser
  In/Out：In/Out
  类型：数据结构指针
  可空：N
  意思：要查询的信息,输入的用户名不能为空,导出相关信息
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CXStorageSQL_Info::XStorageSQL_Info_UserQuery(XSTORAGECORE_USERINFO *pSt_DBUser)
{
    XStorage_IsErrorOccur = FALSE;

    if (NULL == pSt_DBUser)
    {
        XStorage_IsErrorOccur = TRUE;
        XStorage_dwErrorCode = ERROR_XENGINE_XSTROGE_CORE_DB_USERQUERY_PARAMENT;
        return FALSE;
    }
    //查询
    XHDATA xhResult = 0;
    DWORD64 nllLine = 0;
    DWORD64 nllRow = 0;
    TCHAR tszSQLStatement[1024];

    memset(tszSQLStatement, '\0', sizeof(tszSQLStatement));
    _stprintf_s(tszSQLStatement, _T("SELECT * FROM `XStorage_User` WHERE UserName = '%s'"), pSt_DBUser->st_ProtocolUser.tszUserName);
    //查询文件
    if (!DataBase_MySQL_ExecuteQuery(xhDBSQL, &xhResult, tszSQLStatement, &nllLine, &nllRow))
    {
        XStorage_IsErrorOccur = TRUE;
        XStorage_dwErrorCode = DataBase_GetLastError();
        return FALSE;
    }
    if (nllLine <= 0)
    {
        XStorage_IsErrorOccur = TRUE;
        XStorage_dwErrorCode = ERROR_XENGINE_XSTROGE_CORE_DB_USERQUERY_NOTDATA;
        return FALSE;
    }
    TCHAR **pptszResult = DataBase_MySQL_GetResult(xhDBSQL, xhResult);
    if (NULL == pptszResult[0])
    {
		XStorage_IsErrorOccur = TRUE;
		XStorage_dwErrorCode = ERROR_XENGINE_XSTROGE_CORE_DB_USERQUERY_GETRESULT;
		return FALSE;
    }
    memset(pSt_DBUser, '\0', sizeof(XSTORAGECORE_USERINFO));
    if (NULL != pptszResult[1])
    {
        _tcscpy(pSt_DBUser->st_ProtocolUser.tszUserName, pptszResult[1]);
    }
    if (NULL != pptszResult[2])
    {
        _tcscpy(pSt_DBUser->st_ProtocolUser.tszUserPass, pptszResult[2]);
    }
    if (NULL != pptszResult[3])
    {
        pSt_DBUser->nFileCount = _ttoi64(pptszResult[3]);
    }
    if (NULL != pptszResult[4])
    {
        pSt_DBUser->nFileSize = _ttoi64(pptszResult[4]);
    }
    if (NULL != pptszResult[5])
    {
        pSt_DBUser->st_ProtocolUser.nPhoneNumber = _ttoi64(pptszResult[5]);
    }
    if (NULL != pptszResult[6])
    {
        pSt_DBUser->st_ProtocolUser.nIDNumber = _ttoi64(pptszResult[6]);
    }
    if (NULL != pptszResult[7])
    {
        pSt_DBUser->st_ProtocolUser.nUserLevel = _ttoi(pptszResult[7]);
    }
    if (NULL != pptszResult[8])
    {
        pSt_DBUser->st_ProtocolUser.nUserState = _ttoi(pptszResult[8]);
    }
    if (NULL != pptszResult[9])
    {
        pSt_DBUser->xhToken = _ttoi64(pptszResult[9]);
    }
    if (NULL != pptszResult[10])
    {
        _tcscpy(pSt_DBUser->st_ProtocolUser.tszEMailAddr, pptszResult[10]);
    }
    if (NULL != pptszResult[11])
    {
        _tcscpy(pSt_DBUser->tszIPAddr, pptszResult[11]);
    }
    if (NULL != pptszResult[12])
    {
        _tcscpy(pSt_DBUser->st_ProtocolUser.tszCreateTime, pptszResult[12]);
    }
    DataBase_MySQL_FreeResult(xhDBSQL, xhResult);
    return TRUE;
}
/********************************************************************
函数名称：XStorageSQL_Info_UserDBFileSet
函数功能：设置用户文件信息数据
 参数.一：lpszUserName
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入
 参数.二：nFileSize
  In/Out：In
  类型：整数型
  可空：N
  意思：输入文件大小
 参数.三：bDel
  In/Out：In
  类型：逻辑型
  可空：Y
  意思：删除还是增加
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CXStorageSQL_Info::XStorageSQL_Info_UserDBFileSet(LPCTSTR lpszUserName, __int64x nFileSize, BOOL bDel)
{
    XStorage_IsErrorOccur = FALSE;

    if (NULL == lpszUserName)
    {
        XStorage_IsErrorOccur = TRUE;
        XStorage_dwErrorCode = ERROR_XENGINE_XSTROGE_CORE_DB_USERFILESET_PARAMENT;
        return FALSE;
    }
    TCHAR tszSQLStatement[1024];
    //更新文件个数和大小
    memset(tszSQLStatement, '\0', sizeof(tszSQLStatement));

    if (bDel)
    {
        _stprintf_s(tszSQLStatement, _T("UPDATE `XStorage_User` SET FileCount = FileCount - 1,FileSize = FileSize - %lld WHERE UserName = '%s'"), nFileSize, lpszUserName);
    }
    else
    {
        _stprintf_s(tszSQLStatement, _T("UPDATE `XStorage_User` SET FileCount = FileCount + 1,FileSize = FileSize + %lld WHERE UserName = '%s'"), nFileSize, lpszUserName);
    }
    if (!DataBase_MySQL_Execute(xhDBSQL, tszSQLStatement))
    {
        XStorage_IsErrorOccur = TRUE;
        XStorage_dwErrorCode = DataBase_GetLastError();
        return FALSE;
    }
    return TRUE;
}
/********************************************************************
函数名称：XStorageSQL_Info_UserQueryForToken
函数功能：通过TOKEN查询用户信息
 参数.一：xhToken
  In/Out：In
  类型：句柄
  可空：N
  意思：输入要查询的TOKEN
 参数.二：pSt_DBUser
  In/Out：In/Out
  类型：数据结构指针
  可空：N
  意思：要查询的信息,输入的用户名不能为空,导出相关信息
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CXStorageSQL_Info::XStorageSQL_Info_UserQueryForToken(XNETHANDLE xhToken, XSTORAGECORE_USERINFO* pSt_DBUser)
{
    XStorage_IsErrorOccur = FALSE;

    if (NULL == pSt_DBUser)
    {
        XStorage_IsErrorOccur = TRUE;
        XStorage_dwErrorCode = ERROR_XENGINE_XSTROGE_CORE_DB_USERQUERYTOKEN_PARAMENT;
        return FALSE;
    }
    //查询
    XHDATA xhResult = 0;
    DWORD64 nllLine = 0;
    DWORD64 nllRow = 0;
    TCHAR tszSQLStatement[1024];

    memset(tszSQLStatement, '\0', sizeof(tszSQLStatement));
    _stprintf_s(tszSQLStatement, _T("SELECT * FROM `XStorage_User` WHERE xhToken = '%lld'"), xhToken);
    //查询文件
    if (!DataBase_MySQL_ExecuteQuery(xhDBSQL, &xhResult, tszSQLStatement, &nllLine, &nllRow))
    {
        XStorage_IsErrorOccur = TRUE;
        XStorage_dwErrorCode = DataBase_GetLastError();
        return FALSE;
    }
    if (nllLine <= 0)
    {
        XStorage_IsErrorOccur = TRUE;
        XStorage_dwErrorCode = ERROR_XENGINE_XSTROGE_CORE_DB_USERQUERYTOKEN_NOTDATA;
        return FALSE;
    }
    TCHAR** pptszResult = DataBase_MySQL_GetResult(xhDBSQL, xhResult);
    if (NULL == pptszResult[0])
    {
        return FALSE;
    }
    memset(pSt_DBUser, '\0', sizeof(XSTORAGECORE_USERINFO));
    if (NULL != pptszResult[1])
    {
        _tcscpy(pSt_DBUser->st_ProtocolUser.tszUserName, pptszResult[1]);
    }
    if (NULL != pptszResult[2])
    {
        _tcscpy(pSt_DBUser->st_ProtocolUser.tszUserPass, pptszResult[2]);
    }
    if (NULL != pptszResult[3])
    {
        pSt_DBUser->nFileCount = _ttoi64(pptszResult[3]);
    }
    if (NULL != pptszResult[4])
    {
        pSt_DBUser->nFileSize = _ttoi64(pptszResult[4]);
    }
    if (NULL != pptszResult[5])
    {
        pSt_DBUser->st_ProtocolUser.nPhoneNumber = _ttoi64(pptszResult[5]);
    }
    if (NULL != pptszResult[6])
    {
        pSt_DBUser->st_ProtocolUser.nIDNumber = _ttoi64(pptszResult[6]);
    }
    if (NULL != pptszResult[7])
    {
        pSt_DBUser->st_ProtocolUser.nUserLevel = _ttoi(pptszResult[7]);
    }
    if (NULL != pptszResult[8])
    {
        pSt_DBUser->st_ProtocolUser.nUserState = _ttoi(pptszResult[8]);
    }
    if (NULL != pptszResult[9])
    {
        pSt_DBUser->xhToken = _ttoi64(pptszResult[9]);
    }
    if (NULL != pptszResult[10])
    {
        _tcscpy(pSt_DBUser->st_ProtocolUser.tszEMailAddr, pptszResult[10]);
    }
    if (NULL != pptszResult[11])
    {
        _tcscpy(pSt_DBUser->tszIPAddr, pptszResult[11]);
    }
    if (NULL != pptszResult[12])
    {
        _tcscpy(pSt_DBUser->st_ProtocolUser.tszCreateTime, pptszResult[12]);
    }
    DataBase_MySQL_FreeResult(xhDBSQL, xhResult);
    return TRUE;
}
/********************************************************************
函数名称：XStorageSQL_Info_UserSet
函数功能：用户表设置
 参数.一：lpszUser
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要操作的用户名
 参数.二：xhToken
  In/Out：In
  类型：句柄
  可空：Y
  意思：输入用户创建的句柄
 参数.三：nUserState
  In/Out：In
  类型：整数型
  可空：Y
  意思：输入状态,0不在线,1在线
 参数.四：lpszClientAddr
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入IP地址
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CXStorageSQL_Info::XStorageSQL_Info_UserSet(LPCTSTR lpszUser, XNETHANDLE xhToken /* = 0 */, int nUserState /* = 0 */, LPCTSTR lpszClientAddr /* = NULL */)
{
    XStorage_IsErrorOccur = FALSE;

    TCHAR tszSQLStatement[1024];
    memset(tszSQLStatement, '\0', sizeof(tszSQLStatement));

    if (NULL == lpszClientAddr)
    {
        _stprintf_s(tszSQLStatement, _T("UPDATE `XStorage_User` SET xhToken = '%lld',Status = %d WHERE UserName = '%s'"), xhToken, nUserState, lpszUser);
    }
    else
    {
        _stprintf_s(tszSQLStatement, _T("UPDATE `XStorage_User` SET xhToken = '%lld',Status = %d,IPAddr = '%s' WHERE UserName = '%s'"), xhToken, nUserState, lpszClientAddr, lpszUser);
    }
    if (!DataBase_MySQL_Execute(xhDBSQL, tszSQLStatement))
    {
        XStorage_IsErrorOccur = TRUE;
        XStorage_dwErrorCode = DataBase_GetLastError();
        return FALSE;
    }
    return TRUE;
}
/********************************************************************
函数名称：XStorageSQL_Info_UPCount
函数功能：更新统计信息
 参数.一：nFileSize
  In/Out：In
  类型：整数型
  可空：N
  意思：输入要处理的大小
 参数.二：bAdd
  In/Out：In
  类型：逻辑型
  可空：Y
  意思：输入是增加还是减少
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CXStorageSQL_Info::XStorageSQL_Info_UPCount(__int64x nFileSize, BOOL bAdd)
{
    XStorage_IsErrorOccur = FALSE;

    TCHAR tszSQLStatement[2048];
	memset(tszSQLStatement, '\0', sizeof(tszSQLStatement));

    if (bAdd)
    {
        _stprintf_s(tszSQLStatement, _T("UPDATE `XStorage_Count` SET FileCount = FileCount + 1,FileSize = FileSize + %lld"), nFileSize);
    }
    else
    {
        _stprintf_s(tszSQLStatement, _T("UPDATE `XStorage_Count` SET FileCount = FileCount - 1,FileSize = FileSize - %lld"), nFileSize);
    }

	if (!DataBase_MySQL_Execute(xhDBSQL, tszSQLStatement))
	{
		XStorage_IsErrorOccur = TRUE;
		XStorage_dwErrorCode = DataBase_GetLastError();
		return FALSE;
	}
    return TRUE;
}