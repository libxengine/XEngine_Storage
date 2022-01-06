#include "pch.h"
#include "XStorage_SQLite.h"
/********************************************************************
//    Created:     2021/07/12  09:32:21
//    File Name:   D:\XEngine_Storage\XEngine_Source\XEngine_StorageComponents\XStorage_SQLPacket\XStorage_SQLite\XStorage_SQLite.cpp
//    File Path:   D:\XEngine_Storage\XEngine_Source\XEngine_StorageComponents\XStorage_SQLPacket\XStorage_SQLite
//    File Base:   XStorage_SQLite
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     sqlite操作
//    History:
*********************************************************************/
CXStorage_SQLite::CXStorage_SQLite()
{
    bIsRun = FALSE;
    m_nTimeMonth = 0;
}
CXStorage_SQLite::~CXStorage_SQLite()
{

}
//////////////////////////////////////////////////////////////////////////
//                             公有函数
//////////////////////////////////////////////////////////////////////////
/********************************************************************
函数名称：XStorage_SQLite_Init
函数功能：初始化SQLITE文件系统
 参数.一：lpszSQLFile
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要操作的SQL文件
 参数.二：nTimeMonth
  In/Out：In
  类型：整数型
  可空：Y
  意思：输入允许保存的时间,单位月
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CXStorage_SQLite::XStorage_SQLite_Init(LPCTSTR lpszSQLFile, int nTimeMonth /* = 1 */)
{
	XStorage_IsErrorOccur = FALSE;
	//创建数据库
	if (!DataBase_SQLite_Create(lpszSQLFile))
	{
		//如果不是这个错误，说明创建数据库失败了
		if (ERROR_HELPCOMPONENTS_DATABASE_SQLITE_CREATE_ISEXIST != DataBase_GetLastError())
		{
			XStorage_IsErrorOccur = TRUE;
			XStorage_dwErrorCode = DataBase_GetLastError();
			return FALSE;
		}
	}
	//打开数据库
	if (!DataBase_SQLite_Open(&xhSQL, lpszSQLFile))
	{
		XStorage_IsErrorOccur = TRUE;
		XStorage_dwErrorCode = DataBase_GetLastError();
		return FALSE;
	}
    bIsRun = TRUE;
	m_nTimeMonth = nTimeMonth;
	
    pSTDThread = new std::thread(XStorage_SQLite_Thread, this);
    //pSTDThread = make_shared<std::thread>(XStorage_SQLite_Thread, this);
	if (!pSTDThread->joinable())
	{
		XStorage_IsErrorOccur = TRUE;
		XStorage_dwErrorCode = ERROR_XENGINE_XSTROGE_CORE_DB_INIT_THREAD;
		return FALSE;
	}
	return TRUE;
}
/********************************************************************
函数名称：XStorage_SQLite_Destory
函数功能：销毁
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CXStorage_SQLite::XStorage_SQLite_Destory()
{
	XStorage_IsErrorOccur = FALSE;

    if (!bIsRun)
    {
        return TRUE;
    }
    bIsRun = FALSE;

    if (NULL != pSTDThread)
    {
        if (pSTDThread->joinable())
        {
            pSTDThread->join();
            delete pSTDThread;
            pSTDThread = NULL;
        }
    }
	DataBase_SQLite_Close(xhSQL);
	return TRUE;
}
/********************************************************************
函数名称：XStorage_SQLite_FileInsert
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
BOOL CXStorage_SQLite::XStorage_SQLite_FileInsert(XSTORAGECORE_DBFILE* pSt_DBFile)
{
    XStorage_IsErrorOccur = FALSE;

    if (NULL == pSt_DBFile)
    {
        XStorage_IsErrorOccur = TRUE;
        XStorage_dwErrorCode = ERROR_XENGINE_XSTROGE_CORE_DB_INSERTFILE_PARAMENT;
        return FALSE;
    }
    int nListCount = 0;
    XSTORAGECORE_DBFILE** ppSt_ListFile;
    if (XStorage_SQLite_FileQuery(&ppSt_ListFile, &nListCount, NULL, NULL, NULL, pSt_DBFile->st_ProtocolFile.tszFileHash))
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
    _stprintf_s(tszSQLStatement, _T("INSERT INTO `%s` (BuckKey,FilePath,FileName,FileHash,FileUser,FileSize,FileTime) VALUES('%s','%s','%s','%s','%s',%lld,DATETIME('now', 'localtime'))"), tszTableName, pSt_DBFile->tszBuckKey, pSt_DBFile->st_ProtocolFile.tszFilePath, pSt_DBFile->st_ProtocolFile.tszFileName, pSt_DBFile->st_ProtocolFile.tszFileHash, pSt_DBFile->st_ProtocolFile.tszFileUser, pSt_DBFile->st_ProtocolFile.nFileSize);
    if (!DataBase_SQLite_Exec(xhSQL, tszSQLStatement))
    {
        XStorage_IsErrorOccur = TRUE;
        XStorage_dwErrorCode = DataBase_GetLastError();
        return FALSE;
    }
    return TRUE;
}
/********************************************************************
函数名称：XStorage_SQLite_FileDelete
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
BOOL CXStorage_SQLite::XStorage_SQLite_FileDelete(LPCTSTR lpszFile, LPCTSTR lpszHash)
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
    XSTORAGECORE_DBFILE** ppSt_ListFile;
    if (!XStorage_SQLite_FileQuery(&ppSt_ListFile, &nListCount, NULL, NULL, lpszFile, lpszHash))
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
        if (!DataBase_SQLite_Exec(xhSQL, tszSQLStatement))
        {
            XStorage_IsErrorOccur = TRUE;
            XStorage_dwErrorCode = DataBase_GetLastError();
            return FALSE;
        }
    }

    return TRUE;
}
/********************************************************************
函数名称：XStorage_SQLite_FileQuery
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
BOOL CXStorage_SQLite::XStorage_SQLite_FileQuery(XSTORAGECORE_DBFILE*** pppSt_ListFile, int* pInt_ListCount, LPCTSTR lpszTimeStart /* = NULL */, LPCTSTR lpszTimeEnd /* = NULL */, LPCTSTR lpszFile /* = NULL */, LPCTSTR lpszHash /* = NULL */)
{
    XStorage_IsErrorOccur = FALSE;

    if ((NULL == lpszHash) && (NULL == lpszFile))
    {
        XStorage_IsErrorOccur = TRUE;
        XStorage_dwErrorCode = ERROR_XENGINE_XSTROGE_CORE_DB_QUERYFILE_PARAMENT;
        return FALSE;
    }
    //查询
    int nLine = 0;
    int nRow = 0;
    TCHAR** pptszResult;
    TCHAR tszSQLStatement[1024];
    memset(tszSQLStatement, '\0', sizeof(tszSQLStatement));
    //检查是否时间范围检索
    if ((NULL != lpszTimeStart) && (NULL != lpszTimeEnd))
    {
        if (_tcslen(lpszTimeStart) > 0 && _tcslen(lpszTimeEnd) > 0)
        {
            _stprintf_s(tszSQLStatement, _T("SELECT NAME FROM SQLITE_MASTER WHERE TYPE = 'table' AND NAME BETWEEN '%s' AND '%s'"), lpszTimeStart, lpszTimeEnd);
        }
        else
        {
            _stprintf_s(tszSQLStatement, _T("SELECT NAME FROM SQLITE_MASTER WHERE TYPE = 'table'"));
        }
    }
    else
    {
        _stprintf_s(tszSQLStatement, _T("SELECT NAME FROM SQLITE_MASTER WHERE TYPE = 'table'"));
    }
    if (!DataBase_SQLite_GetTable(xhSQL, tszSQLStatement, &pptszResult, &nLine, &nRow))
    {
        XStorage_IsErrorOccur = TRUE;
        XStorage_dwErrorCode = DataBase_GetLastError();
        return FALSE;
    }
	list<XSTORAGECORE_DBFILE> stl_ListFile;
	//轮训
	for (int i = 0; i < nLine; i++)
	{
		if (!XStorage_SQLite_IsNumber(pptszResult[i]))
		{
			continue;
		}
		int nLineResult = 0;
		int nFieldResult = 0;
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
                _stprintf_s(tszSQLStatement, _T("SELECT * FROM `%s` WHERE FilePath = '%s' AND FileName = '%s'"), pptszResult[i], tszFilePath, tszFileName);
			}
			else
			{
				_stprintf_s(tszSQLStatement, _T("SELECT * FROM `%s`"), pptszResult[i]);
			}
		}
		if (NULL != lpszHash)
		{
			if (_tcslen(lpszHash) > 0)
			{
				_stprintf_s(tszSQLStatement, _T("SELECT * FROM `%s` WHERE FileHash = '%s'"), pptszResult[i], lpszHash);
			}
			else
			{
				_stprintf_s(tszSQLStatement, _T("SELECT * FROM `%s`"), pptszResult[i]);
			}
		}
		//查询文件
        TCHAR** pptszFileResult;
        if (DataBase_SQLite_GetTable(xhSQL, tszSQLStatement, &pptszFileResult, &nLineResult, &nFieldResult))
		{
            int nFiled = nFieldResult;
			//循环获取所有查找到的文件
			for (int j = 0; j < nLineResult; j++)
			{
				XSTORAGECORE_DBFILE st_DBFile;
				memset(&st_DBFile, '\0', sizeof(XSTORAGECORE_DBFILE));

				_tcscpy(st_DBFile.tszTableName, pptszResult[i]);
                nFiled++;

				_tcscpy(st_DBFile.tszBuckKey, pptszFileResult[nFiled]);
				nFiled++;

                _tcscpy(st_DBFile.st_ProtocolFile.tszFilePath, pptszFileResult[nFiled]);
                nFiled++;

				_tcscpy(st_DBFile.st_ProtocolFile.tszFileName, pptszFileResult[nFiled]);
				nFiled++;

				_tcscpy(st_DBFile.st_ProtocolFile.tszFileHash, pptszFileResult[nFiled]);
				nFiled++;
    
				_tcscpy(st_DBFile.st_ProtocolFile.tszFileUser, pptszFileResult[nFiled]);
				nFiled++;

				st_DBFile.st_ProtocolFile.nFileSize = _ttoi64(pptszFileResult[nFiled]);
				nFiled++;
				
				_tcscpy(st_DBFile.st_ProtocolFile.tszFileTime, pptszFileResult[nFiled]);
				nFiled++;
				stl_ListFile.push_back(st_DBFile);
			}
			DataBase_SQLite_FreeTable(pptszFileResult);
		}
	}
    DataBase_SQLite_FreeTable(pptszResult);
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
函数名称：XStorage_SQLite_FileQueryForTable
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
BOOL CXStorage_SQLite::XStorage_SQLite_FileQueryForTable(XSTORAGECORE_DBFILE*** pppSt_ListFile, int* pInt_ListCount, LPCTSTR lpszTableName)
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
    int nLine = 0;
    int nRow = 0;
    TCHAR** pptszResulte;
    TCHAR tszSQLStatement[1024];
    memset(tszSQLStatement, '\0', sizeof(tszSQLStatement));
    //检查是否时间范围检索
    _stprintf_s(tszSQLStatement, _T("SELECT * FROM `%s`"), lpszTableName);
    DataBase_SQLite_GetTable(xhSQL, tszSQLStatement, &pptszResulte, &nLine, &nRow);
    if (nLine <= 0)
    {
        XStorage_IsErrorOccur = TRUE;
        XStorage_dwErrorCode = ERROR_XENGINE_XSTROGE_CORE_DB_QUERYFILETABLE_EMPTY;
        return FALSE;
    }
    *pInt_ListCount = nLine;
    BaseLib_OperatorMemory_Malloc((XPPPMEM)pppSt_ListFile, nLine, sizeof(XSTORAGECORE_DBFILE));
    //循环获取所有查找到的文件
    int nFiled = nRow;
    for (int i = 0; i < nLine; i++)
    {
        nFiled++;
        _tcscpy((*pppSt_ListFile)[i]->tszTableName, lpszTableName);

        _tcscpy((*pppSt_ListFile)[i]->tszBuckKey, pptszResulte[nFiled]);
        nFiled++;

		_tcscpy((*pppSt_ListFile)[i]->st_ProtocolFile.tszFilePath, pptszResulte[nFiled]);
		nFiled++;

        _tcscpy((*pppSt_ListFile)[i]->st_ProtocolFile.tszFileName, pptszResulte[nFiled]);
        nFiled++;

        _tcscpy((*pppSt_ListFile)[i]->st_ProtocolFile.tszFileHash, pptszResulte[nFiled]);
        nFiled++;

        _tcscpy((*pppSt_ListFile)[i]->st_ProtocolFile.tszFileUser, pptszResulte[nFiled]);
        nFiled++;

        (*pppSt_ListFile)[i]->st_ProtocolFile.nFileSize = _ttoi64(pptszResulte[nFiled]);
        nFiled++;

        _tcscpy((*pppSt_ListFile)[i]->st_ProtocolFile.tszFileTime, pptszResulte[nFiled]);
        nFiled++;
    }
    DataBase_MySQL_FreeResult(xhSQL, xhTable);
    return TRUE;
}
//////////////////////////////////////////////////////////////////////////
//                    保护函数
//////////////////////////////////////////////////////////////////////////
/********************************************************************
函数名称：XStorage_SQLite_CreateTable
函数功能：创建按照月份的文件表
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CXStorage_SQLite::XStorage_SQLite_CreateTable()
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

		_stprintf_s(tszSQLQuery, _T("PRAGMA foreign_keys = false;"
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
			"PRAGMA foreign_keys = true;"
        ), tszTableName);

        if (!DataBase_SQLite_Exec(xhSQL, tszSQLQuery))
        {
            XStorage_IsErrorOccur = TRUE;
            XStorage_dwErrorCode = DataBase_GetLastError();
            return FALSE;
        }
    }
    return TRUE;
}
/********************************************************************
函数名称：XStorage_SQLite_TimeDay
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
BOOL CXStorage_SQLite::XStorage_SQLite_TimeMonth(LPCTSTR lpszStartTime, int* pInt_Month)
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
函数名称：XStorage_SQLite_TimeDel
函数功能：删除过期日期表数据
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CXStorage_SQLite::XStorage_SQLite_TimeDel()
{
    XStorage_IsErrorOccur = FALSE;

    int nLine = 0;
    int nField = 0;
    TCHAR** pptszResult;
    TCHAR tszSQLQuery[2048];

    memset(tszSQLQuery, '\0', sizeof(tszSQLQuery));
    _stprintf_s(tszSQLQuery, _T("SELECT NAME FROM SQLITE_MASTER WHERE TYPE = 'table' ORDER BY NAME"));
    if (DataBase_SQLite_GetTable(xhSQL, tszSQLQuery, &pptszResult, &nLine, &nField))
    {
        for (int i = 0; i < nLine; i++)
        {
            int nTimeMonth = 0;
			if (!XStorage_SQLite_IsNumber(pptszResult[i]))
			{
				continue;
			}
            //只有在处理正确的情况下才进行删除操作
            if (XStorage_SQLite_TimeMonth(pptszResult[i], &nTimeMonth))
            {
                if (nTimeMonth > m_nTimeMonth)
                {
                    //删除文件
                    int nListCount = 0;
                    XSTORAGECORE_DBFILE** ppSt_ListFile;
                    XStorage_SQLite_FileQueryForTable(&ppSt_ListFile, &nListCount, pptszResult[i]);
                    for (int i = 0; i < nListCount; i++)
                    {
                        //删除文件
                        TCHAR tszFilePath[2048];
                        memset(tszFilePath, '\0', sizeof(tszFilePath));

                        _stprintf(tszFilePath, _T("%s/%s"), ppSt_ListFile[i]->st_ProtocolFile.tszFilePath, ppSt_ListFile[i]->st_ProtocolFile.tszFileName);
                        _tremove(tszFilePath);
                    }
                    //删除数据库
                    memset(tszSQLQuery, '\0', sizeof(tszSQLQuery));
                    _stprintf_s(tszSQLQuery, _T("DROP TABLE `%s`"), pptszResult[0]);
                    DataBase_SQLite_Exec(xhSQL, tszSQLQuery);
                    BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_ListFile, nListCount);
                }
            }
        }
    }
    DataBase_SQLite_FreeTable(pptszResult);
    return TRUE;
}
BOOL CXStorage_SQLite::XStorage_SQLite_IsNumber(LPCTSTR lpszStr)
{
    int nLen = _tcslen(lpszStr);
    for (int i = 0; i < nLen; i++)
    {
        if (0 == isdigit(lpszStr[i]))
        {
            return FALSE;
        }
    }
    return TRUE;
}
//////////////////////////////////////////////////////////////////////////
//                      线程函数
//////////////////////////////////////////////////////////////////////////
XHTHREAD CXStorage_SQLite::XStorage_SQLite_Thread(LPVOID lParam)
{
    CXStorage_SQLite* pClass_This = (CXStorage_SQLite*)lParam;
    time_t nTimeStart = time(NULL);
    time_t nTimeEnd = 0;
    int nTime = 60 * 60 * 12;

    while (pClass_This->bIsRun)
    {
        if ((nTimeEnd - nTimeStart) > nTime)
        {
            pClass_This->XStorage_SQLite_TimeDel();
            pClass_This->XStorage_SQLite_CreateTable();
        }
        nTimeEnd = time(NULL);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    return 0;
}