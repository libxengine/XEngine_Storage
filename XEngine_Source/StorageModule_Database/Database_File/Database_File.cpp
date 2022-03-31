#include "pch.h"
#include "Database_File.h"
/********************************************************************
//    Created:     2022/03/29  14:07:14
//    File Name:   D:\XEngine_Storage\XEngine_Source\StorageModule_Database\Database_File\Database_File.cpp
//    File Path:   D:\XEngine_Storage\XEngine_Source\StorageModule_Database\Database_File
//    File Base:   Database_File
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     文件数据库管理器
//    History:
*********************************************************************/
CDatabase_File::CDatabase_File()
{
    bIsRun = FALSE;
    m_nTimeMonth = 0;
    xhDBSQL = 0;
}
CDatabase_File::~CDatabase_File()
{

}
//////////////////////////////////////////////////////////////////////////
//                         公有函数
//////////////////////////////////////////////////////////////////////////
/********************************************************************
函数名称：Database_File_Init
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
BOOL CDatabase_File::Database_File_Init(DATABASE_MYSQL_CONNECTINFO *pSt_DBConnector, int nTimeDay)
{
    Database_IsErrorOccur = FALSE;

    if (NULL == pSt_DBConnector)
    {
        Database_IsErrorOccur = TRUE;
        Database_dwErrorCode = ERROR_XENGINE_XSTROGE_CORE_DB_INIT_PARAMENT;
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
        Database_IsErrorOccur = TRUE;
        Database_dwErrorCode = DataBase_GetLastError();
        return FALSE;
    }
    bIsRun = TRUE;
   
    pSTDThread = make_shared<std::thread>(Database_File_Thread, this);
    if (!pSTDThread->joinable())
    {
        Database_IsErrorOccur = TRUE;
        Database_dwErrorCode = ERROR_XENGINE_XSTROGE_CORE_DB_INIT_THREAD;
        return FALSE;
    }
    return TRUE;
}
/********************************************************************
函数名称：Database_File_Destory
函数功能：销毁数据库管理器
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CDatabase_File::Database_File_Destory()
{
    Database_IsErrorOccur = FALSE;

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
        }
    }
    DataBase_MySQL_Close(xhDBSQL);
    return TRUE;
}
/********************************************************************
函数名称：Database_File_FileInsert
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
BOOL CDatabase_File::Database_File_FileInsert(XSTORAGECORE_DBFILE *pSt_DBFile)
{
    Database_IsErrorOccur = FALSE;

    if (NULL == pSt_DBFile)
    {
        Database_IsErrorOccur = TRUE;
        Database_dwErrorCode = ERROR_XENGINE_XSTROGE_CORE_DB_INSERTFILE_PARAMENT;
        return FALSE;
    }
    int nListCount = 0;
    XSTORAGECORE_DBFILE **ppSt_ListFile;
    if (Database_File_FileQuery(&ppSt_ListFile, &nListCount, NULL, NULL, NULL, pSt_DBFile->st_ProtocolFile.tszFileHash))
    {
        BaseLib_OperatorMemory_Free((void***)&ppSt_ListFile, nListCount);
        return TRUE;
    }
    BaseLib_OperatorMemory_Free((void***)&ppSt_ListFile, nListCount);

	TCHAR tszSQLStatement[2048];
	memset(tszSQLStatement, '\0', sizeof(tszSQLStatement));

    Database_Help_Insert(tszSQLStatement, pSt_DBFile);

    if (!DataBase_MySQL_Execute(xhDBSQL, tszSQLStatement))
    {
        Database_IsErrorOccur = TRUE;
        Database_dwErrorCode = DataBase_GetLastError();
        return FALSE;
    }
    return TRUE;
}
/********************************************************************
函数名称：Database_File_FileDelete
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
BOOL CDatabase_File::Database_File_FileDelete(LPCTSTR lpszBuckKey /* = NULL */, LPCTSTR lpszFile /* = NULL */, LPCTSTR lpszHash /* = NULL */)
{
    Database_IsErrorOccur = FALSE;

    if ((NULL == lpszFile) && (NULL == lpszHash))
    {
        Database_IsErrorOccur = TRUE;
        Database_dwErrorCode = ERROR_XENGINE_XSTROGE_CORE_DB_DELETEFILE_PARAMENT;
        return FALSE;
    }
    int nListCount = 0;
    XSTORAGECORE_DBFILE **ppSt_ListFile;
    if (!Database_File_FileQuery(&ppSt_ListFile, &nListCount, NULL, NULL, lpszBuckKey, lpszFile, lpszHash))
    {
        return FALSE;
    }
    //轮训查找删除
    for (int i = 0; i < nListCount; i++)
    {
		TCHAR tszSQLStatement[1024];
		memset(tszSQLStatement, '\0', sizeof(tszSQLStatement));
        Database_Help_Delete(tszSQLStatement, ppSt_ListFile[i]->tszTableName, lpszBuckKey, lpszFile, lpszHash);

        if (!DataBase_MySQL_Execute(xhDBSQL, tszSQLStatement))
        {
            Database_IsErrorOccur = TRUE;
            Database_dwErrorCode = DataBase_GetLastError();
            return FALSE;
        }
    }

    return TRUE;
}
/********************************************************************
函数名称：Database_File_FileQuery
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
BOOL CDatabase_File::Database_File_FileQuery(XSTORAGECORE_DBFILE*** pppSt_ListFile, int* pInt_ListCount, LPCTSTR lpszTimeStart /* = NULL */, LPCTSTR lpszTimeEnd /* = NULL */, LPCTSTR lpszBuckKey /* = NULL */, LPCTSTR lpszFile /* = NULL */, LPCTSTR lpszHash /* = NULL */, LPCTSTR lpszTableName /* = NULL */)
{
    Database_IsErrorOccur = FALSE;

    if ((NULL == lpszHash) && (NULL == lpszFile))
    {
        Database_IsErrorOccur = TRUE;
        Database_dwErrorCode = ERROR_XENGINE_XSTROGE_CORE_DB_QUERYFILE_PARAMENT;
        return FALSE;
    }
    //查询
    XHDATA xhTable = 0;
    __int64u nllLine = 0;
    __int64u nllRow = 0;
    list<XSTORAGECORE_DBFILE> stl_ListFile;

    TCHAR tszSQLStatement[1024];
    memset(tszSQLStatement, '\0', sizeof(tszSQLStatement));

    if (NULL == lpszTableName)
    {
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
			Database_IsErrorOccur = TRUE;
			Database_dwErrorCode = DataBase_GetLastError();
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

            Database_Help_Query(tszSQLStatement, pptszResult[0], lpszBuckKey, NULL, lpszFile, lpszHash, NULL, lpszTimeStart, lpszTimeEnd);
			//查询文件
			if (DataBase_MySQL_ExecuteQuery(xhDBSQL, &xhResult, tszSQLStatement, &dwLineResult, &dwFieldResult))
			{
				//循环获取所有查找到的文件
				for (__int64u j = 0; j < dwLineResult; j++)
				{
					TCHAR** pptszFileResult = DataBase_MySQL_GetResult(xhDBSQL, xhResult);

					XSTORAGECORE_DBFILE st_DBFile;
					memset(&st_DBFile, '\0', sizeof(XSTORAGECORE_DBFILE));

					_tcscpy(st_DBFile.tszTableName, pptszResult[0]);

					if (NULL != pptszFileResult[1])
					{
						_tcscpy(st_DBFile.tszBuckKey, pptszFileResult[1]);
					}
					if (NULL != pptszFileResult[2])
					{
						_tcscpy(st_DBFile.st_ProtocolFile.tszFilePath, pptszFileResult[2]);
					}
					if (NULL != pptszFileResult[3])
					{
						_tcscpy(st_DBFile.st_ProtocolFile.tszFileName, pptszFileResult[3]);
					}
					if (NULL != pptszFileResult[4])
					{
						_tcscpy(st_DBFile.st_ProtocolFile.tszFileHash, pptszFileResult[4]);
					}
					if (NULL != pptszFileResult[5])
					{
						_tcscpy(st_DBFile.st_ProtocolFile.tszFileUser, pptszFileResult[5]);
					}
					if (NULL != pptszFileResult[6])
					{
						st_DBFile.st_ProtocolFile.nFileSize = _ttoi64(pptszFileResult[6]);
					}
					if (NULL != pptszFileResult[7])
					{
						_tcscpy(st_DBFile.st_ProtocolFile.tszFileTime, pptszFileResult[7]);
					}
					stl_ListFile.push_back(st_DBFile);
				}
				DataBase_MySQL_FreeResult(xhDBSQL, xhResult);
			}
		}
		DataBase_MySQL_FreeResult(xhDBSQL, xhTable);
    }
    else
    {
        Database_Help_Query(tszSQLStatement, lpszTableName, lpszBuckKey, NULL, lpszFile, lpszHash, NULL, lpszTimeStart, lpszTimeEnd);
		//查询文件
		if (DataBase_MySQL_ExecuteQuery(xhDBSQL, &xhTable, tszSQLStatement, &nllLine, &nllRow))
		{
			//循环获取所有查找到的文件
			for (__int64u i = 0; i < nllLine; i++)
			{
				TCHAR** pptszFileResult = DataBase_MySQL_GetResult(xhDBSQL, xhTable);

				XSTORAGECORE_DBFILE st_DBFile;
				memset(&st_DBFile, '\0', sizeof(XSTORAGECORE_DBFILE));

				_tcscpy(st_DBFile.tszTableName, lpszTableName);

				if (NULL != pptszFileResult[1])
				{
					_tcscpy(st_DBFile.tszBuckKey, pptszFileResult[1]);
				}
				if (NULL != pptszFileResult[2])
				{
					_tcscpy(st_DBFile.st_ProtocolFile.tszFilePath, pptszFileResult[2]);
				}
				if (NULL != pptszFileResult[3])
				{
					_tcscpy(st_DBFile.st_ProtocolFile.tszFileName, pptszFileResult[3]);
				}
				if (NULL != pptszFileResult[4])
				{
					_tcscpy(st_DBFile.st_ProtocolFile.tszFileHash, pptszFileResult[4]);
				}
				if (NULL != pptszFileResult[5])
				{
					_tcscpy(st_DBFile.st_ProtocolFile.tszFileUser, pptszFileResult[5]);
				}
				if (NULL != pptszFileResult[6])
				{
					st_DBFile.st_ProtocolFile.nFileSize = _ttoi64(pptszFileResult[6]);
				}
				if (NULL != pptszFileResult[7])
				{
					_tcscpy(st_DBFile.st_ProtocolFile.tszFileTime, pptszFileResult[7]);
				}
				stl_ListFile.push_back(st_DBFile);
			}
			DataBase_MySQL_FreeResult(xhDBSQL, xhTable);
		}
    }
    //是否为空
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
函数名称：Database_File_CreateTable
函数功能：创建按照月份的文件表
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CDatabase_File::Database_File_CreateTable()
{
    Database_IsErrorOccur = FALSE;

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
            "`BuckKey` varchar(260) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '路径KEY',"
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
			Database_IsErrorOccur = TRUE;
			Database_dwErrorCode = DataBase_GetLastError();
			return FALSE;
		}
    }
    return TRUE;
}
/********************************************************************
函数名称：Database_File_TimeDay
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
BOOL CDatabase_File::Database_File_TimeMonth(LPCTSTR lpszStartTime, int* pInt_Month)
{
    Database_IsErrorOccur = FALSE;

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
函数名称：Database_File_TimeDel
函数功能：删除过期日期表数据
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CDatabase_File::Database_File_TimeDel()
{
    Database_IsErrorOccur = FALSE;

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
            if (Database_File_TimeMonth(pptszResult[0], &nTimeMonth))
            {
                if (nTimeMonth > m_nTimeMonth)
                {
                    //删除文件
                    int nListCount = 0;
                    XSTORAGECORE_DBFILE **ppSt_ListFile;
                    Database_File_FileQuery(&ppSt_ListFile, &nListCount, NULL, NULL, NULL, NULL, NULL, pptszResult[0]);
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
XHTHREAD CDatabase_File::Database_File_Thread(LPVOID lParam)
{
    CDatabase_File *pClass_This = (CDatabase_File *)lParam;
    time_t nTimeStart = time(NULL);
    time_t nTimeEnd = 0;
    int nTime = 60 * 60 * 12;

	pClass_This->Database_File_TimeDel();
	pClass_This->Database_File_CreateTable();

	while (pClass_This->bIsRun)
	{
		if ((nTimeEnd - nTimeStart) > nTime)
		{
			pClass_This->Database_File_TimeDel();
			pClass_This->Database_File_CreateTable();
		}
		nTimeEnd = time(NULL);
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
    return 0;
}