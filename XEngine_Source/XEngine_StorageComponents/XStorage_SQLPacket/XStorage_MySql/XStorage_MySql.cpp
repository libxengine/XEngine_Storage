#include "pch.h"
#include "XStorage_MySql.h"
/********************************************************************
//    Created:     2021/09/28  15:50:22
//    File Name:   D:\XEngine_Storage\XEngine_Source\XEngine_StorageComponents\XStorage_SQLPacket\XStorage_MySql\XStorage_MySql.cpp
//    File Path:   D:\XEngine_Storage\XEngine_Source\XEngine_StorageComponents\XStorage_SQLPacket\XStorage_MySql
//    File Base:   XStorage_MySql
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     MYSQL数据库管理器
//    History:
*********************************************************************/
CXStorage_MySql::CXStorage_MySql()
{
    bIsRun = FALSE;
    m_nTimeMonth = 0;
    xhDBSQL = 0;
}
CXStorage_MySql::~CXStorage_MySql()
{

}
//////////////////////////////////////////////////////////////////////////
//                         公有函数
//////////////////////////////////////////////////////////////////////////
/********************************************************************
函数名称：XStorage_MySql_Init
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
BOOL CXStorage_MySql::XStorage_MySql_Init(DATABASE_MYSQL_CONNECTINFO *pSt_DBConnector, int nTimeDay)
{
    XStorage_IsErrorOccur = FALSE;

    if (NULL == pSt_DBConnector)
    {
        XStorage_IsErrorOccur = TRUE;
        XStorage_dwErrorCode = ERROR_XENGINE_XSTROGE_CORE_DB_INIT_PARAMENT;
        return FALSE;
    }
    m_nTimeMonth = nTimeDay;
#ifdef _WINDOWS
    LPCTSTR lpszStrCharset = _T("gbk");
#else
    LPCTSTR lpszStrCharset = _T("utf8");
#endif
    //连接数据库
    _tcscpy(pSt_DBConnector->tszDBName, _T("XEngine_Storage"));
    if (!DataBase_MySQL_Connect(&xhDBSQL, pSt_DBConnector, 5, TRUE, lpszStrCharset))
    {
        XStorage_IsErrorOccur = TRUE;
        XStorage_dwErrorCode = DataBase_GetLastError();
        return FALSE;
    }
    bIsRun = TRUE;
    pSTDThread = make_shared<std::thread>(XStorage_MySql_Thread, this);
    if (!pSTDThread->joinable())
    {
        XStorage_IsErrorOccur = TRUE;
        XStorage_dwErrorCode = ERROR_XENGINE_XSTROGE_CORE_DB_INIT_THREAD;
        return FALSE;
    }
    return TRUE;
}
/********************************************************************
函数名称：XStorage_MySql_Destory
函数功能：销毁数据库管理器
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CXStorage_MySql::XStorage_MySql_Destory()
{
    XStorage_IsErrorOccur = FALSE;

    if (!bIsRun)
    {
        return TRUE;
    }
    bIsRun = FALSE;
    pSTDThread->join();

    DataBase_MySQL_Close(xhDBSQL);

    return TRUE;
}
/********************************************************************
函数名称：XStorage_MySql_FileInsert
函数功能：插入一个文件数据到数据库中
 参数.一：pSt_DBManage
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：要插入的数据信息
返回值
  类型：逻辑型
  意思：是否成功
备注：这个结构所有值都必须填充
*********************************************************************/
BOOL CXStorage_MySql::XStorage_MySql_FileInsert(XSTORAGECORE_DBFILE *pSt_DBFile)
{
    XStorage_IsErrorOccur = FALSE;

    if (NULL == pSt_DBFile)
    {
        XStorage_IsErrorOccur = TRUE;
        XStorage_dwErrorCode = ERROR_XENGINE_XSTROGE_CORE_DB_INSERTFILE_PARAMENT;
        return FALSE;
    }
    int nListCount = 0;
    XSTORAGECORE_DBFILE **ppSt_ListFile;
    if (XStorage_MySql_FileQuery(&ppSt_ListFile, &nListCount, NULL, NULL, NULL, pSt_DBFile->st_ProtocolFile.tszFileHash))
    {
        BaseLib_OperatorMemory_Free((void***)&ppSt_ListFile, nListCount);
        return TRUE;
    }
    BaseLib_OperatorMemory_Free((void***)&ppSt_ListFile, nListCount);
    TCHAR tszSQLStatement[2048];
    TCHAR tszTableName[64];
    XENGINE_LIBTIMER st_LibTimer;

    memset(tszSQLStatement, '\0', sizeof(tszSQLStatement));
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
        _stprintf_s(tszTableName, _T("%04d%02d"), st_LibTimer.wYear, st_LibTimer.wMonth);
    }
    //插入语句
    _stprintf_s(tszSQLStatement, _T("INSERT INTO `%s` (FilePath,FileName,FileHash,FileUser,FileSize,FileTime) VALUES('%s','%s','%s','%s',%lld,now())"), tszTableName, pSt_DBFile->st_ProtocolFile.tszFilePath, pSt_DBFile->st_ProtocolFile.tszFileName, pSt_DBFile->st_ProtocolFile.tszFileHash, pSt_DBFile->st_ProtocolFile.tszFileUser, pSt_DBFile->st_ProtocolFile.nFileSize);
    if (!DataBase_MySQL_Execute(xhDBSQL, tszSQLStatement))
    {
        XStorage_IsErrorOccur = TRUE;
        XStorage_dwErrorCode = DataBase_GetLastError();
        return FALSE;
    }
    return TRUE;
}
/********************************************************************
函数名称：XStorage_MySql_FileDelete
函数功能：删除一个数据库文件信息
 参数.一：lpszFile
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：要删除的文件全路径
 参数.二：lpszHash
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：要删除的文件HASH
返回值
  类型：逻辑型
  意思：是否成功
备注：参数不能全为空,不会删除文件
*********************************************************************/
BOOL CXStorage_MySql::XStorage_MySql_FileDelete(LPCTSTR lpszFile, LPCTSTR lpszHash)
{
    XStorage_IsErrorOccur = FALSE;

    if ((NULL == lpszFile) && (NULL == lpszHash))
    {
        XStorage_IsErrorOccur = TRUE;
        XStorage_dwErrorCode = ERROR_XENGINE_XSTROGE_CORE_DB_DELETEFILE_PARAMENT;
        return FALSE;
    }
    TCHAR tszSQLStatement[1024];
    int nListCount = 0;
    XSTORAGECORE_DBFILE **ppSt_ListFile;
    if (!XStorage_MySql_FileQuery(&ppSt_ListFile, &nListCount, NULL, NULL, lpszFile, lpszHash))
    {
        return FALSE;
    }
    //轮训查找删除
    for (int i = 0; i < nListCount; i++)
    {
        memset(tszSQLStatement, '\0', sizeof(tszSQLStatement));
        if (NULL == lpszHash)
        {
            _stprintf_s(tszSQLStatement, _T("DELETE FROM `%s` WHERE FileName = '%s'"), ppSt_ListFile[i]->tszTableName, ppSt_ListFile[i]->st_ProtocolFile.tszFileName);
        }
        else
        {
            _stprintf_s(tszSQLStatement, _T("DELETE FROM `%s` WHERE FileHash = '%s'"), ppSt_ListFile[i]->tszTableName, ppSt_ListFile[i]->st_ProtocolFile.tszFileHash);
        }
        if (!DataBase_MySQL_Execute(xhDBSQL, tszSQLStatement))
        {
            XStorage_IsErrorOccur = TRUE;
            XStorage_dwErrorCode = DataBase_GetLastError();
            return FALSE;
        }
    }

    return TRUE;
}
/********************************************************************
函数名称：XStorage_MySql_FileQuery
函数功能：查询文件信息
 参数.一：pppSt_ListFile
  In/Out：Out
  类型：三级指针
  可空：N
  意思：导出查询到的文件列表,此函数需要调用基础库的内存释放函数
 参数.二：pInt_ListCount
  In/Out：Out
  类型：三级指针
  可空：N
  意思：导出文件个数
 参数.三：lpszTimeStart
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：查找开始时间,20190701
 参数.四：lpszTimeEnd
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：查找结束时间,20190730
 参数.五：lpszFile
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：要查询的名称
 参数.六：lpszHash
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：要查询的文件HASH
返回值
  类型：逻辑型
  意思：是否成功
备注：返回假可能没有查找到,这条记录不存在.参数lpszFile和lpszHash不能全为空
*********************************************************************/
BOOL CXStorage_MySql::XStorage_MySql_FileQuery(XSTORAGECORE_DBFILE*** pppSt_ListFile, int* pInt_ListCount, LPCTSTR lpszTimeStart /* = NULL */, LPCTSTR lpszTimeEnd /* = NULL */, LPCTSTR lpszFile /* = NULL */, LPCTSTR lpszHash /* = NULL */)
{
    XStorage_IsErrorOccur = FALSE;

    if ((NULL == lpszHash) && (NULL == lpszFile))
    {
        XStorage_IsErrorOccur = TRUE;
        XStorage_dwErrorCode = ERROR_XENGINE_XSTROGE_CORE_DB_QUERYFILE_PARAMENT;
        return FALSE;
    }
    //查询
    XHDATA xhTable = 0;
    __int64u nllLine = 0;
    __int64u nllRow = 0;
    TCHAR tszSQLStatement[1024];
    memset(tszSQLStatement, '\0', sizeof(tszSQLStatement));
    //检查是否时间范围检索
    if ((NULL != lpszTimeStart) && (NULL != lpszTimeEnd))
    {
        if (_tcslen(lpszTimeStart) > 0 && _tcslen(lpszTimeEnd) > 0)
        {
            _stprintf_s(tszSQLStatement, _T("SELECT TABLE_NAME FROM INFORMATION_SCHEMA.TABLES WHERE TABLE_SCHEMA = 'XEngine_Storage' AND TABLE_NAME BETWEEN '%s' AND '%s'"), lpszTimeStart, lpszTimeEnd);
        }
        else
        {
            _stprintf_s(tszSQLStatement, _T("SELECT TABLE_NAME FROM INFORMATION_SCHEMA.TABLES WHERE TABLE_SCHEMA = 'XEngine_Storage'"));
        }
    }
    else
    {
        _stprintf_s(tszSQLStatement, _T("SELECT TABLE_NAME FROM INFORMATION_SCHEMA.TABLES WHERE TABLE_SCHEMA = 'XEngine_Storage'"));
    }
    if (!DataBase_MySQL_ExecuteQuery(xhDBSQL, &xhTable, tszSQLStatement, &nllLine, &nllRow))
    {
        XStorage_IsErrorOccur = TRUE;
        XStorage_dwErrorCode = DataBase_GetLastError();
        return FALSE;
	}
	list<XSTORAGECORE_DBFILE> stl_ListFile;
    //轮训
    for (__int64u i = 0; i < nllLine; i++)
    {
        TCHAR **pptszResult = DataBase_MySQL_GetResult(xhDBSQL, xhTable);
        if (NULL == pptszResult[0])
        {
            continue;
        }
        __int64u dwLineResult = 0;
        __int64u dwFieldResult = 0;
        XNETHANDLE xhResult;

        memset(tszSQLStatement, '\0', sizeof(tszSQLStatement));
        //判断查询方式
        if (NULL != lpszFile)
        {
            if (_tcslen(lpszFile) > 0)
            {
                TCHAR tszFilePath[MAX_PATH];
                TCHAR tszFileName[MAX_PATH];

                memset(tszFilePath, '\0', MAX_PATH);
                memset(tszFileName, '\0', MAX_PATH);

                BaseLib_OperatorString_GetFileAndPath(lpszFile, tszFilePath, tszFileName);
                _stprintf_s(tszSQLStatement, _T("SELECT * FROM `%s` WHERE FilePath = '%s' AND FileName = '%s'"), pptszResult[0], tszFilePath, tszFileName);
            }
            else
            {
                _stprintf_s(tszSQLStatement, _T("SELECT * FROM `%s`"), pptszResult[0]);
            }
        }
        if (NULL != lpszHash)
        {
			if (_tcslen(lpszHash) > 0)
			{
				_stprintf_s(tszSQLStatement, _T("SELECT * FROM `%s` WHERE FileHash = '%s'"), pptszResult[0], lpszHash);
			}
			else
			{
				_stprintf_s(tszSQLStatement, _T("SELECT * FROM `%s`"), pptszResult[0]);
			}
        }
        //查询文件
        if (DataBase_MySQL_ExecuteQuery(xhDBSQL, &xhResult, tszSQLStatement, &dwLineResult, &dwFieldResult))
        {
            //循环获取所有查找到的文件
            for (__int64u j = 0; j < dwLineResult; j++)
            {
                TCHAR **pptszFileResult = DataBase_MySQL_GetResult(xhDBSQL, xhResult);

                XSTORAGECORE_DBFILE st_DBFile;
                memset(&st_DBFile, '\0', sizeof(XSTORAGECORE_DBFILE));

                _tcscpy(st_DBFile.tszTableName, pptszResult[0]);

                if (NULL != pptszFileResult[1])
                {
                    _tcscpy(st_DBFile.st_ProtocolFile.tszFilePath, pptszFileResult[1]);
                }
                if (NULL != pptszFileResult[2])
                {
                    _tcscpy(st_DBFile.st_ProtocolFile.tszFileName, pptszFileResult[2]);
                }
				if (NULL != pptszFileResult[3])
				{
					_tcscpy(st_DBFile.st_ProtocolFile.tszFileHash, pptszFileResult[3]);
				}
                if (NULL != pptszFileResult[4])
                {
                    _tcscpy(st_DBFile.st_ProtocolFile.tszFileUser, pptszFileResult[4]);
                }
                if (NULL != pptszFileResult[5])
                {
                    st_DBFile.st_ProtocolFile.nFileSize = _ttoi64(pptszFileResult[5]);
                }
                if (NULL != pptszFileResult[6])
                {
                    _tcscpy(st_DBFile.st_ProtocolFile.tszFileTime, pptszFileResult[6]);
                }
                stl_ListFile.push_back(st_DBFile);
            }
            DataBase_MySQL_FreeResult(xhDBSQL, xhResult);
        }
    }
    DataBase_MySQL_FreeResult(xhDBSQL, xhTable);

    if (stl_ListFile.empty())
    {
        XStorage_IsErrorOccur = TRUE;
        XStorage_dwErrorCode = ERROR_XENGINE_XSTROGE_CORE_DB_QUERYFILE_EMPTY;
        return FALSE;
    }
    BaseLib_OperatorMemory_Malloc((XPPPMEM)pppSt_ListFile, stl_ListFile.size(), sizeof(XSTORAGECORE_DBFILE));

    list<XSTORAGECORE_DBFILE>::const_iterator stl_ListIterator = stl_ListFile.begin();
    for (int i = 0; stl_ListIterator != stl_ListFile.end(); stl_ListIterator++, i++)
    {
        *(*pppSt_ListFile)[i] = *stl_ListIterator;
    }
    *pInt_ListCount = stl_ListFile.size();
    stl_ListFile.clear();
    return TRUE;
}
/********************************************************************
函数名称：XStorage_MySql_FileQueryForTable
函数功能：通过指定表名称查询所有文件
 参数.一：pppSt_ListFile
  In/Out：Out
  类型：三级指针
  可空：N
  意思：输出查询到的文件信息
 参数.二：pInt_ListCount
  In/Out：Out
  类型：三级指针
  可空：N
  意思：导出获取到的列表个数
 参数.三：lpszTableName
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要查询的表名称
返回值
  类型：逻辑型
  意思：是否成功
备注：参数一需要调用基础库的内存释放函数进行释放内存
*********************************************************************/
BOOL CXStorage_MySql::XStorage_MySql_FileQueryForTable(XSTORAGECORE_DBFILE*** pppSt_ListFile, int* pInt_ListCount, LPCTSTR lpszTableName)
{
    XStorage_IsErrorOccur = FALSE;

    if ((NULL == pppSt_ListFile) || (NULL == pInt_ListCount) || (NULL == lpszTableName))
    {
        XStorage_IsErrorOccur = TRUE;
        XStorage_dwErrorCode = ERROR_XENGINE_XSTROGE_CORE_DB_QUERYFILETABLE_PARAMENT;
        return FALSE;
    }
    //查询
    XHDATA xhTable = 0;
    __int64u nllLine = 0;
    __int64u nllRow = 0;

    TCHAR tszSQLStatement[1024];
    memset(tszSQLStatement, '\0', sizeof(tszSQLStatement));
    //检查是否时间范围检索
    _stprintf_s(tszSQLStatement, _T("SELECT * FROM `%s`"), lpszTableName);
    DataBase_MySQL_ExecuteQuery(xhDBSQL, &xhTable, tszSQLStatement, &nllLine, &nllRow);
    if (nllLine <= 0)
	{
		XStorage_IsErrorOccur = TRUE;
		XStorage_dwErrorCode = ERROR_XENGINE_XSTROGE_CORE_DB_QUERYFILETABLE_EMPTY;
		return FALSE;
	}
	BaseLib_OperatorMemory_Malloc((XPPPMEM)pppSt_ListFile, (int)nllLine, sizeof(XSTORAGECORE_DBFILE));
	//循环获取所有查找到的文件
	for (__int64u i = 0; i < nllLine; i++)
	{
		TCHAR** pptszFileResult = DataBase_MySQL_GetResult(xhDBSQL, xhTable);

		_tcscpy((*pppSt_ListFile)[i]->tszTableName, lpszTableName);
		if (NULL != pptszFileResult[1])
		{
			_tcscpy((*pppSt_ListFile)[i]->st_ProtocolFile.tszFilePath, pptszFileResult[1]);
		}
		if (NULL != pptszFileResult[2])
		{
			_tcscpy((*pppSt_ListFile)[i]->st_ProtocolFile.tszFileName, pptszFileResult[2]);
		}
		if (NULL != pptszFileResult[3])
		{
			_tcscpy((*pppSt_ListFile)[i]->st_ProtocolFile.tszFileHash, pptszFileResult[3]);
		}
		if (NULL != pptszFileResult[4])
		{
			_tcscpy((*pppSt_ListFile)[i]->st_ProtocolFile.tszFileUser, pptszFileResult[4]);
		}
		if (NULL != pptszFileResult[5])
		{
            (*pppSt_ListFile)[i]->st_ProtocolFile.nFileSize = _ttoi64(pptszFileResult[5]);
		}
		if (NULL != pptszFileResult[6])
		{
			_tcscpy((*pppSt_ListFile)[i]->st_ProtocolFile.tszFileTime, pptszFileResult[6]);
		}
	}
    DataBase_MySQL_FreeResult(xhDBSQL, xhTable);
    *pInt_ListCount = (int)nllLine;
    return TRUE;
}
/********************************************************************
函数名称：XStorage_MySql_FileQueryForHash
函数功能：通过MD5查询文件信息
 参数.一：pSt_FileInfo
  In/Out：Out
  类型：数据结构指针
  可空：N
  意思：输出查询到的文件信息
 参数.二：lpszFileHash
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要查询的文件MD5
 参数.三：lpszUser
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入文件所属用户
 参数.四：lpszTimeStart
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入开始时间
 参数.五：lpszTimeEnd
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入结束时间
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CXStorage_MySql::XStorage_MySql_FileQueryForHash(XSTORAGECORE_DBFILE* pSt_FileInfo, LPCTSTR lpszFileHash, LPCTSTR lpszUser, LPCTSTR lpszTimeStart, LPCTSTR lpszTimeEnd)
{
    XStorage_IsErrorOccur = FALSE;

    if ((NULL == pSt_FileInfo) || (NULL == lpszFileHash))
    {
        XStorage_IsErrorOccur = TRUE;
        XStorage_dwErrorCode = ERROR_XENGINE_XSTROGE_CORE_DB_QUERYMD5_PARAMENT;
        return FALSE;
    }
    //查询
    XHDATA xhTable = 0;
    __int64u nllLine = 0;
    __int64u nllRow = 0;
    BOOL bFound = FALSE;
    TCHAR tszSQLStatement[1024];
    memset(tszSQLStatement, '\0', sizeof(tszSQLStatement));
    //检查是否时间范围检索
    if ((NULL != lpszTimeStart) && (NULL != lpszTimeEnd))
    {
        _stprintf_s(tszSQLStatement, _T("SELECT TABLE_NAME FROM INFORMATION_SCHEMA.TABLES WHERE TABLE_SCHEMA = 'XStorage_Storage' AND TABLE_NAME BETWEEN '%s' AND '%s'"), lpszTimeStart, lpszTimeEnd);
    }
    else
    {
        _stprintf_s(tszSQLStatement, _T("SELECT TABLE_NAME FROM INFORMATION_SCHEMA.TABLES WHERE TABLE_SCHEMA = 'XStorage_Storage'"));
    }
    if (!DataBase_MySQL_ExecuteQuery(xhDBSQL, &xhTable, tszSQLStatement, &nllLine, &nllRow))
    {
        XStorage_IsErrorOccur = TRUE;
        XStorage_dwErrorCode = DataBase_GetLastError();
        return FALSE;
    }
    //轮训
    for (__int64u i = 0; i < nllLine; i++)
    {
        TCHAR** pptszResult = DataBase_MySQL_GetResult(xhDBSQL, xhTable);
        if (NULL == pptszResult[0])
        {
            continue;
        }
        __int64u dwLineResult = 0;
        __int64u dwFieldResult = 0;
        XNETHANDLE xhResult;

        memset(tszSQLStatement, '\0', sizeof(tszSQLStatement));
        //判断查询方式
        if (NULL == lpszUser)
        {
            _stprintf_s(tszSQLStatement, _T("SELECT * FROM `%s` WHERE FileHash = '%s'"), pptszResult[0], lpszFileHash);
        }
        else
        {
            _stprintf_s(tszSQLStatement, _T("SELECT * FROM `%s` WHERE FileHash = '%s' AND FileUser = '%s'"), pptszResult[0], lpszFileHash, lpszUser);
        }
        //查询文件
        if (DataBase_MySQL_ExecuteQuery(xhDBSQL, &xhResult, tszSQLStatement, &dwLineResult, &dwFieldResult))
        {
            //循环获取所有查找到的文件
            for (__int64u j = 0; j < dwLineResult; j++)
            {
                TCHAR** pptszFileResult = DataBase_MySQL_GetResult(xhDBSQL, xhResult);

                _tcscpy(pSt_FileInfo->tszTableName, pptszResult[0]);
                if (NULL != pptszFileResult[1])
                {
                    _tcscpy(pSt_FileInfo->st_ProtocolFile.tszFilePath, pptszFileResult[1]);
                }
                if (NULL != pptszFileResult[2])
                {
                    _tcscpy(pSt_FileInfo->st_ProtocolFile.tszFileName, pptszFileResult[2]);
                }
                if (NULL != pptszFileResult[3])
                {
                    _tcscpy(pSt_FileInfo->st_ProtocolFile.tszFileUser, pptszFileResult[3]);
                }
                if (NULL != pptszFileResult[4])
                {
                    pSt_FileInfo->st_ProtocolFile.nFileSize = _ttoi64(pptszFileResult[4]);
                }
                if (NULL != pptszFileResult[5])
                {
                    _tcscpy(pSt_FileInfo->st_ProtocolFile.tszFileHash, pptszFileResult[5]);
                }
                if (NULL != pptszFileResult[6])
                {
                    _tcscpy(pSt_FileInfo->st_ProtocolFile.tszFileTime, pptszFileResult[6]);
                }
                bFound = TRUE;
                break;
            }
            DataBase_MySQL_FreeResult(xhDBSQL, xhResult);
        }
    }
    DataBase_MySQL_FreeResult(xhDBSQL, xhTable);

    if (!bFound)
    {
        XStorage_IsErrorOccur = TRUE;
        XStorage_dwErrorCode = ERROR_XENGINE_XSTROGE_CORE_DB_QUERYMD5_NOTFOUND;
        return FALSE;
    }
    return TRUE;
}
/********************************************************************
函数名称：XStorage_MySql_FileGetCount
函数功能：获取数据库中文件总个数和总大小
 参数.一：pInt_Count
  In/Out：Out
  类型：64位整数型指针
  可空：N
  意思：导出获取到的文件个数
 参数.二：pInt_Size
  In/Out：Out
  类型：64位整数型指针
  可空：N
  意思：导出获取到的文件大小,单位字节(BYTE)
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CXStorage_MySql::XStorage_MySql_FileGetCount(__int64x *pInt_Count, __int64x *pInt_Size)
{
    XStorage_IsErrorOccur = FALSE;

    if ((NULL == pInt_Count) || (NULL == pInt_Size))
    {
        XStorage_IsErrorOccur = TRUE;
        XStorage_dwErrorCode = ERROR_XENGINE_XSTROGE_CORE_DB_GETCOUNT_PARAMENT;
        return FALSE;
    }
	__int64u nllRow = 0;
	__int64u nllColumn = 0;
    TCHAR tszSQLStatement[1024];    //SQL语句
    memset(tszSQLStatement, '\0', sizeof(tszSQLStatement));

    _stprintf_s(tszSQLStatement, _T("SELECT * FROM `XStorage_Count`"));
    //查询
    XHDATA xhResult = 0;
    if (!DataBase_MySQL_ExecuteQuery(xhDBSQL, &xhResult, tszSQLStatement, &nllRow, &nllColumn))
    {
        XStorage_IsErrorOccur = TRUE;
        XStorage_dwErrorCode = DataBase_GetLastError();
        return FALSE;
    }
    if (nllRow <= 0)
    {
        XStorage_IsErrorOccur = TRUE;
        XStorage_dwErrorCode = ERROR_XENGINE_XSTROGE_CORE_DB_GETCOUNT_BROKE;
        return FALSE;
    }
    //获取记录集
    TCHAR **pptszResult = DataBase_MySQL_GetResult(xhDBSQL, xhResult);
    if (NULL == pptszResult[0])
    {
        return FALSE;
    }
    if (NULL != pptszResult[1])
    {
        *pInt_Count = _ttoi64(pptszResult[1]);
    }
    if (NULL != pptszResult[2])
    {
        *pInt_Size = _ttoi64(pptszResult[2]);
    }
    DataBase_MySQL_FreeResult(xhDBSQL, xhResult);

    return TRUE;
}
//////////////////////////////////////////////////////////////////////////
//                    保护函数
//////////////////////////////////////////////////////////////////////////
/********************************************************************
函数名称：XStorage_MySql_CreateTable
函数功能：创建按照月份的文件表
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CXStorage_MySql::XStorage_MySql_CreateTable()
{
    XStorage_IsErrorOccur = FALSE;

    for (int i = 0; i < 2; i++)
    {
		TCHAR tszTableName[64];
		TCHAR tszSQLQuery[2048];

		memset(tszTableName, '\0', sizeof(tszTableName));
		memset(tszSQLQuery, '\0', sizeof(tszSQLQuery));

		XENGINE_LIBTIMER st_DBTime;
		memset(&st_DBTime, 0, sizeof(XENGINE_LIBTIMER));

		BaseLib_OperatorTime_GetSysTime(&st_DBTime);

		if (12 == st_DBTime.wMonth)
		{
            _stprintf_s(tszTableName, _T("%04d01"), st_DBTime.wYear);
		}
        else
        {
            _stprintf_s(tszTableName, _T("%04d%02d"), st_DBTime.wYear, st_DBTime.wMonth + i);
        }

		_stprintf_s(tszSQLQuery, _T("CREATE TABLE IF NOT EXISTS `%s` ("
			"`ID` int NOT NULL AUTO_INCREMENT COMMENT 'ID序号',"
			"`FilePath` varchar(260) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL COMMENT '文件路径',"
			"`FileName` varchar(260) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL COMMENT '文件名称',"
			"`FileHash` varchar(260) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '文件HASH',"
			"`FileUser` varchar(260) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '文件所属用户',"
			"`FileSize` bigint NOT NULL COMMENT '文件大小',"
			"`FileTime` datetime NOT NULL COMMENT '插入时间',"
			"PRIMARY KEY (`ID`) USING BTREE"
			") ENGINE = InnoDB AUTO_INCREMENT = 1 CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;"
		), tszTableName);

		if (!DataBase_MySQL_Execute(xhDBSQL, tszSQLQuery))
		{
			XStorage_IsErrorOccur = TRUE;
			XStorage_dwErrorCode = DataBase_GetLastError();
			return FALSE;
		}
    }
    return TRUE;
}
/********************************************************************
函数名称：XStorage_MySql_TimeDay
函数功能：计算指定时间与当前时间间隔天数
 参数.一：lpszStartTime
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入开始计算的时间
 参数.二：pInt_Month
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出相差月份
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CXStorage_MySql::XStorage_MySql_TimeMonth(LPCTSTR lpszStartTime, int* pInt_Month)
{
    XStorage_IsErrorOccur = FALSE;

    XENGINE_LIBTIMER st_EndTime;
    memset(&st_EndTime, '\0', sizeof(XENGINE_LIBTIMER));

    BaseLib_OperatorTime_GetSysTime(&st_EndTime);

    TCHAR tszTimeStr[64];
    memset(tszTimeStr, '\0', sizeof(tszTimeStr));

    _stprintf(tszTimeStr, _T("%04d%02d"), st_EndTime.wYear, st_EndTime.wMonth);
    int nEndTime = _ttoi(tszTimeStr);
    int nStartTime = _ttoi(lpszStartTime);
    if (nStartTime > 0)
    {
        *pInt_Month = nEndTime - nStartTime;
    }
    
    return TRUE;
}
/********************************************************************
函数名称：XStorage_MySql_TimeDel
函数功能：删除过期日期表数据
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CXStorage_MySql::XStorage_MySql_TimeDel()
{
    XStorage_IsErrorOccur = FALSE;

    __int64u dwLine = 0;
    __int64u dwField = 0;
    XNETHANDLE xhTableResult;
    TCHAR tszSQLQuery[2048];

    memset(tszSQLQuery, '\0', sizeof(tszSQLQuery));
    _stprintf_s(tszSQLQuery, _T("SELECT TABLE_NAME FROM INFORMATION_SCHEMA.TABLES WHERE TABLE_SCHEMA='XEngine_Storage' AND TABLE_TYPE='BASE TABLE'"));
    if (DataBase_MySQL_ExecuteQuery(xhDBSQL, &xhTableResult, tszSQLQuery, &dwLine, &dwField))
    {
        for (__int64u i = 0; i < dwLine; i++)
        {
            TCHAR** pptszResult = DataBase_MySQL_GetResult(xhDBSQL, xhTableResult);
            if (NULL == pptszResult[0])
            {
                continue;
            }
            int nTimeMonth = 0;
            //只有在处理正确的情况下才进行删除操作
            if (XStorage_MySql_TimeMonth(pptszResult[0], &nTimeMonth))
            {
                if (nTimeMonth > m_nTimeMonth)
                {
                    //删除文件
                    int nListCount = 0;
                    XSTORAGECORE_DBFILE **ppSt_ListFile;
                    XStorage_MySql_FileQueryForTable(&ppSt_ListFile, &nListCount, pptszResult[0]);

                    for (int i = 0; i < nListCount; i++)
                    {
                        //更新文件个数和大小
                        memset(tszSQLQuery, '\0', sizeof(tszSQLQuery));
                        _stprintf_s(tszSQLQuery, _T("UPDATE `XStorage_Count` SET FileCount = FileCount - 1,FileSize = FileSize - %lld"), ppSt_ListFile[i]->st_ProtocolFile.nFileSize);
                        if (!DataBase_MySQL_Execute(xhDBSQL, tszSQLQuery))
                        {
                            XStorage_IsErrorOccur = TRUE;
                            XStorage_dwErrorCode = DataBase_GetLastError();
                            return FALSE;
                        }
                        //删除文件
                        TCHAR tszFilePath[2048];
                        memset(tszFilePath, '\0', sizeof(tszFilePath));

                        _stprintf(tszFilePath, _T("%s/%s"), ppSt_ListFile[i]->st_ProtocolFile.tszFilePath, ppSt_ListFile[i]->st_ProtocolFile.tszFileName);
                        _tremove(tszFilePath);
                    }
                    //删除数据库
                    memset(tszSQLQuery, '\0', sizeof(tszSQLQuery));
                    _stprintf_s(tszSQLQuery, _T("DROP TABLE `%s`"), pptszResult[0]);
                    DataBase_MySQL_Execute(xhDBSQL, tszSQLQuery);
                    BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_ListFile, nListCount);
                }
            }
        }
    }
    DataBase_MySQL_FreeResult(xhDBSQL, xhTableResult);
    return TRUE;
}
//////////////////////////////////////////////////////////////////////////
//                      线程函数
//////////////////////////////////////////////////////////////////////////
XHTHREAD CXStorage_MySql::XStorage_MySql_Thread(LPVOID lParam)
{
    CXStorage_MySql *pClass_This = (CXStorage_MySql *)lParam;
    time_t nTimeStart = time(NULL);
    time_t nTimeEnd = 0;
    BOOL bFirst = TRUE;
    int nTime = 60 * 60 * 12;

    while (pClass_This->bIsRun)
    {
		if (((nTimeEnd - nTimeStart) > nTime) || bFirst)
		{
			pClass_This->XStorage_MySql_TimeDel();
			pClass_This->XStorage_MySql_CreateTable();
            bFirst = FALSE;
		}
        nTimeEnd = time(NULL);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    return 0;
}

