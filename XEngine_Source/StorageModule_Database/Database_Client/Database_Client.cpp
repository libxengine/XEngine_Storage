#include "pch.h"
#include "Database_Client.h"
/********************************************************************
//    Created:     2022/03/29  15:36:56
//    File Name:   D:\XEngine_Storage\XEngine_Source\StorageModule_Database\Database_Client\Database_Client.cpp
//    File Path:   D:\XEngine_Storage\XEngine_Source\StorageModule_Database\Database_Client
//    File Base:   Database_Client
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     客户端数据库操作类
//    History:
*********************************************************************/
CDatabase_Client::CDatabase_Client()
{
}
CDatabase_Client::~CDatabase_Client()
{

}
//////////////////////////////////////////////////////////////////////////
//                             公有函数
//////////////////////////////////////////////////////////////////////////
/********************************************************************
函数名称：Database_Client_Init
函数功能：初始化客户端数据库系统
 参数.一：lpszSQLFile
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要操作的SQL文件
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CDatabase_Client::Database_Client_Init(LPCTSTR lpszSQLFile)
{
    Database_IsErrorOccur = FALSE;

	memset(tszTableName, '\0', sizeof(tszTableName));
	_tcscpy(tszTableName, _T("FileList"));
    //创建数据库
    if (DataBase_SQLite_Create(lpszSQLFile))
    {
		//打开数据库
		if (!DataBase_SQLite_Open(&xhSQL, lpszSQLFile))
		{
			Database_IsErrorOccur = TRUE;
			Database_dwErrorCode = DataBase_GetLastError();
			return FALSE;
		}
		Database_Client_CreateTable(tszTableName);
    }
	else
	{
		//如果不是这个错误，说明创建数据库失败了
		if (ERROR_HELPCOMPONENTS_DATABASE_SQLITE_CREATE_ISEXIST != DataBase_GetLastError())
		{
			Database_IsErrorOccur = TRUE;
			Database_dwErrorCode = DataBase_GetLastError();
			return FALSE;
		}
		//打开数据库
		if (!DataBase_SQLite_Open(&xhSQL, lpszSQLFile))
		{
			Database_IsErrorOccur = TRUE;
			Database_dwErrorCode = DataBase_GetLastError();
			return FALSE;
		}
	}
    return TRUE;
}
/********************************************************************
函数名称：Database_Client_Destory
函数功能：销毁
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CDatabase_Client::Database_Client_Destory()
{
    Database_IsErrorOccur = FALSE;

    DataBase_SQLite_Close(xhSQL);
    return TRUE;
}
/********************************************************************
函数名称：Database_Client_FileInsert
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
BOOL CDatabase_Client::Database_Client_FileInsert(XSTORAGECORE_DBFILE* pSt_DBFile)
{
    Database_IsErrorOccur = FALSE;

    if (NULL == pSt_DBFile)
    {
        Database_IsErrorOccur = TRUE;
        Database_dwErrorCode = ERROR_XENGINE_XSTROGE_CORE_DB_INSERTFILE_PARAMENT;
        return FALSE;
    }
    int nListCount = 0;
    XSTORAGECORE_DBFILE** ppSt_ListFile;
    if (Database_Client_FileQuery(&ppSt_ListFile, &nListCount, NULL, NULL, NULL, pSt_DBFile->st_ProtocolFile.tszFileHash))
    {
        BaseLib_OperatorMemory_Free((void***)&ppSt_ListFile, nListCount);
        return TRUE;
    }
    BaseLib_OperatorMemory_Free((void***)&ppSt_ListFile, nListCount);

	TCHAR tszSQLStatement[2048];
	memset(tszSQLStatement, '\0', sizeof(tszSQLStatement));

    Database_Help_Insert(tszSQLStatement, pSt_DBFile);
    if (!DataBase_SQLite_Exec(xhSQL, tszSQLStatement))
    {
        Database_IsErrorOccur = TRUE;
        Database_dwErrorCode = DataBase_GetLastError();
        return FALSE;
    }
    return TRUE;
}
/********************************************************************
函数名称：Database_Client_FileDelete
函数功能：删除一个数据库文件信息
 参数.一：lpszBuckKey
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：所属BUCK名称
 参数.二：lpszFile
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：要删除的文件全路径
 参数.三：lpszHash
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：要删除的文件HASH
返回值
  类型：逻辑型
  意思：是否成功
备注：参数不能全为空,不会删除文件
*********************************************************************/
BOOL CDatabase_Client::Database_Client_FileDelete(LPCTSTR lpszBuckKey /* = NULL */, LPCTSTR lpszFile /* = NULL */, LPCTSTR lpszHash /* = NULL */)
{
    Database_IsErrorOccur = FALSE;

    if ((NULL == lpszFile) && (NULL == lpszHash))
    {
        Database_IsErrorOccur = TRUE;
        Database_dwErrorCode = ERROR_XENGINE_XSTROGE_CORE_DB_DELETEFILE_PARAMENT;
        return FALSE;
    }
    int nListCount = 0;
    XSTORAGECORE_DBFILE** ppSt_ListFile;
    if (!Database_Client_FileQuery(&ppSt_ListFile, &nListCount, NULL, NULL, lpszBuckKey, lpszFile, lpszHash))
    {
        return FALSE;
    }
    //轮训查找删除
    for (int i = 0; i < nListCount; i++)
    {
		TCHAR tszSQLStatement[1024];
		memset(tszSQLStatement, '\0', sizeof(tszSQLStatement));
        Database_Help_Delete(tszSQLStatement, ppSt_ListFile[i]->tszTableName, lpszBuckKey, lpszFile, lpszHash);

        if (!DataBase_SQLite_Exec(xhSQL, tszSQLStatement))
        {
            Database_IsErrorOccur = TRUE;
            Database_dwErrorCode = DataBase_GetLastError();
            return FALSE;
        }
    }

    return TRUE;
}
/********************************************************************
函数名称：Database_Client_FileQuery
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
 参数.五：lpszBuckKey
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：查询的BUCK名
 参数.六：lpszFile
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：要查询的名称
 参数.七：lpszHash
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：要查询的文件HASH
 参数.八：lpszTableName
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入要查询的表明,为NULL所有
返回值
  类型：逻辑型
  意思：是否成功
备注：返回假可能没有查找到,这条记录不存在.参数lpszFile和lpszHash不能全为空
*********************************************************************/
BOOL CDatabase_Client::Database_Client_FileQuery(XSTORAGECORE_DBFILE*** pppSt_ListFile, int* pInt_ListCount, LPCTSTR lpszTimeStart /* = NULL */, LPCTSTR lpszTimeEnd /* = NULL */, LPCTSTR lpszBuckKey /* = NULL */, LPCTSTR lpszFile /* = NULL */, LPCTSTR lpszHash /* = NULL */)
{
    Database_IsErrorOccur = FALSE;

    if ((NULL == lpszHash) && (NULL == lpszFile))
    {
        Database_IsErrorOccur = TRUE;
        Database_dwErrorCode = ERROR_XENGINE_XSTROGE_CORE_DB_QUERYFILE_PARAMENT;
        return FALSE;
    }
    //查询
    int nLine = 0;
    int nRow = 0;
    TCHAR** pptszResult;
	TCHAR tszSQLStatement[1024];
	list<XSTORAGECORE_DBFILE> stl_ListFile;

    memset(tszSQLStatement, '\0', sizeof(tszSQLStatement));

    Database_Help_Query(tszSQLStatement, tszTableName, lpszBuckKey, NULL, lpszFile, lpszHash, NULL, lpszTimeStart, lpszTimeEnd);
	if (DataBase_SQLite_GetTable(xhSQL, tszSQLStatement, &pptszResult, &nLine, &nRow))
	{
		int nFiled = nRow;
		//循环获取所有查找到的文件
		for (int i = 0; i < nLine; i++)
		{
			XSTORAGECORE_DBFILE st_DBFile;
			memset(&st_DBFile, '\0', sizeof(XSTORAGECORE_DBFILE));

			_tcscpy(st_DBFile.tszTableName, tszTableName);
			nFiled++;

			_tcscpy(st_DBFile.tszBuckKey, pptszResult[nFiled]);
			nFiled++;

			_tcscpy(st_DBFile.st_ProtocolFile.tszFilePath, pptszResult[nFiled]);
			nFiled++;

			_tcscpy(st_DBFile.st_ProtocolFile.tszFileName, pptszResult[nFiled]);
			nFiled++;

			_tcscpy(st_DBFile.st_ProtocolFile.tszFileHash, pptszResult[nFiled]);
			nFiled++;

			_tcscpy(st_DBFile.st_ProtocolFile.tszFileUser, pptszResult[nFiled]);
			nFiled++;

			st_DBFile.st_ProtocolFile.nFileSize = _ttoi64(pptszResult[nFiled]);
			nFiled++;

			_tcscpy(st_DBFile.st_ProtocolFile.tszFileTime, pptszResult[nFiled]);
			nFiled++;
			stl_ListFile.push_back(st_DBFile);
		}
		DataBase_SQLite_FreeTable(pptszResult);
	}
    if (stl_ListFile.empty())
    {
        Database_IsErrorOccur = TRUE;
        Database_dwErrorCode = ERROR_XENGINE_XSTROGE_CORE_DB_QUERYFILE_EMPTY;
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
//////////////////////////////////////////////////////////////////////////
//                    保护函数
//////////////////////////////////////////////////////////////////////////
/********************************************************************
函数名称：Database_Client_CreateTable
函数功能：创建表
 参数.一：lpszTableName
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入表名称
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CDatabase_Client::Database_Client_CreateTable(LPCTSTR lpszTableName)
{
    Database_IsErrorOccur = FALSE;

	TCHAR tszSQLQuery[2048];
	memset(tszSQLQuery, '\0', sizeof(tszSQLQuery));

	_stprintf_s(tszSQLQuery, _T(
		"CREATE TABLE IF NOT EXISTS \"%s\" ("
		"  \"ID\" INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,"
		"  \"BuckKey\" TEXT,"
		"  \"FilePath\" TEXT NOT NULL,"
		"  \"FileName\" TEXT NOT NULL,"
		"  \"FileHash\" TEXT,"
		"  \"FileUser\" TEXT,"
		"  \"FileSize\" integer NOT NULL,"
		"  \"FileTime\" TEXT NOT NULL"
		");"
	), lpszTableName);

	if (!DataBase_SQLite_Exec(xhSQL, tszSQLQuery))
	{
		Database_IsErrorOccur = TRUE;
		Database_dwErrorCode = DataBase_GetLastError();
		return FALSE;
	}
    return TRUE;
}