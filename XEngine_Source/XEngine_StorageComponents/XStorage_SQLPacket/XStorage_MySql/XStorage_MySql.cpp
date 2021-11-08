#include "pch.h"
#include "XStorage_MySql.h"
/********************************************************************
//    Created:     2021/09/28  15:50:22
//    File Name:   D:\XEngine_Storage\XEngine_Source\XEngine_StorageComponents\XStorage_SQLPacket\XStorage_MySql\XStorage_MySql.cpp
//    File Path:   D:\XEngine_Storage\XEngine_Source\XEngine_StorageComponents\XStorage_SQLPacket\XStorage_MySql
//    File Base:   XStorage_MySql
//    File Ext:    cpp
//    Project:     XEngine(����ͨ������)
//    Author:      qyt
//    Purpose:     MYSQL���ݿ������
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
//                         ���к���
//////////////////////////////////////////////////////////////////////////
/********************************************************************
�������ƣ�XStorage_MySql_Init
�������ܣ���ʼ���洢�������ݿ������
 ����.һ��pSt_DBConnector
  In/Out��In
  ���ͣ����ݽṹָ��
  �ɿգ�N
  ��˼������MYSQL���ݿ�������Ϣ
 ����.����nTimeDay
  In/Out��In
  ���ͣ�������
  �ɿգ�N
  ��˼�������ļ����ݱ���ʱ��
����ֵ
  ���ͣ��߼���
  ��˼���Ƿ�ɹ�
��ע��
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
    //�������ݿ�
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
�������ƣ�XStorage_MySql_Destory
�������ܣ��������ݿ������
����ֵ
  ���ͣ��߼���
  ��˼���Ƿ�ɹ�
��ע��
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
�������ƣ�XStorage_MySql_FileInsert
�������ܣ�����һ���ļ����ݵ����ݿ���
 ����.һ��pSt_DBManage
  In/Out��In
  ���ͣ����ݽṹָ��
  �ɿգ�N
  ��˼��Ҫ�����������Ϣ
����ֵ
  ���ͣ��߼���
  ��˼���Ƿ�ɹ�
��ע������ṹ����ֵ���������
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
    //��ò������ڱ�
    BaseLib_OperatorTime_GetSysTime(&st_LibTimer);
    if (_tcslen(pSt_DBFile->tszTableName) > 0)
    {
        _tcscpy(tszTableName, pSt_DBFile->tszTableName);
    }
    else
    {
        _stprintf_s(tszTableName, _T("%04d%02d"), st_LibTimer.wYear, st_LibTimer.wMonth);
    }
    //�������
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
�������ƣ�XStorage_MySql_FileDelete
�������ܣ�ɾ��һ�����ݿ��ļ���Ϣ
 ����.һ��lpszFile
  In/Out��In
  ���ͣ������ַ�ָ��
  �ɿգ�Y
  ��˼��Ҫɾ�����ļ�ȫ·��
 ����.����lpszHash
  In/Out��In
  ���ͣ������ַ�ָ��
  �ɿգ�Y
  ��˼��Ҫɾ�����ļ�HASH
����ֵ
  ���ͣ��߼���
  ��˼���Ƿ�ɹ�
��ע����������ȫΪ��,����ɾ���ļ�
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
    //��ѵ����ɾ��
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
�������ƣ�XStorage_MySql_FileQuery
�������ܣ���ѯ�ļ���Ϣ
 ����.һ��pppSt_ListFile
  In/Out��Out
  ���ͣ�����ָ��
  �ɿգ�N
  ��˼��������ѯ�����ļ��б�,�˺�����Ҫ���û�������ڴ��ͷź���
 ����.����pInt_ListCount
  In/Out��Out
  ���ͣ�����ָ��
  �ɿգ�N
  ��˼�������ļ�����
 ����.����lpszTimeStart
  In/Out��In
  ���ͣ������ַ�ָ��
  �ɿգ�Y
  ��˼�����ҿ�ʼʱ��,20190701
 ����.�ģ�lpszTimeEnd
  In/Out��In
  ���ͣ������ַ�ָ��
  �ɿգ�Y
  ��˼�����ҽ���ʱ��,20190730
 ����.�壺lpszFile
  In/Out��In
  ���ͣ������ַ�ָ��
  �ɿգ�Y
  ��˼��Ҫ��ѯ������
 ����.����lpszHash
  In/Out��In
  ���ͣ������ַ�ָ��
  �ɿգ�Y
  ��˼��Ҫ��ѯ���ļ�HASH
����ֵ
  ���ͣ��߼���
  ��˼���Ƿ�ɹ�
��ע�����ؼٿ���û�в��ҵ�,������¼������.����lpszFile��lpszHash����ȫΪ��
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
    //��ѯ
    XHDATA xhTable = 0;
    __int64u nllLine = 0;
    __int64u nllRow = 0;
    TCHAR tszSQLStatement[1024];
    memset(tszSQLStatement, '\0', sizeof(tszSQLStatement));
    //����Ƿ�ʱ�䷶Χ����
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
    //��ѵ
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
        //�жϲ�ѯ��ʽ
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
        //��ѯ�ļ�
        if (DataBase_MySQL_ExecuteQuery(xhDBSQL, &xhResult, tszSQLStatement, &dwLineResult, &dwFieldResult))
        {
            //ѭ����ȡ���в��ҵ����ļ�
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
�������ƣ�XStorage_MySql_FileQueryForTable
�������ܣ�ͨ��ָ�������Ʋ�ѯ�����ļ�
 ����.һ��pppSt_ListFile
  In/Out��Out
  ���ͣ�����ָ��
  �ɿգ�N
  ��˼�������ѯ�����ļ���Ϣ
 ����.����pInt_ListCount
  In/Out��Out
  ���ͣ�����ָ��
  �ɿգ�N
  ��˼��������ȡ�����б����
 ����.����lpszTableName
  In/Out��In
  ���ͣ������ַ�ָ��
  �ɿգ�N
  ��˼������Ҫ��ѯ�ı�����
����ֵ
  ���ͣ��߼���
  ��˼���Ƿ�ɹ�
��ע������һ��Ҫ���û�������ڴ��ͷź��������ͷ��ڴ�
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
    //��ѯ
    XHDATA xhTable = 0;
    __int64u nllLine = 0;
    __int64u nllRow = 0;

    TCHAR tszSQLStatement[1024];
    memset(tszSQLStatement, '\0', sizeof(tszSQLStatement));
    //����Ƿ�ʱ�䷶Χ����
    _stprintf_s(tszSQLStatement, _T("SELECT * FROM `%s`"), lpszTableName);
    DataBase_MySQL_ExecuteQuery(xhDBSQL, &xhTable, tszSQLStatement, &nllLine, &nllRow);
    if (nllLine <= 0)
	{
		XStorage_IsErrorOccur = TRUE;
		XStorage_dwErrorCode = ERROR_XENGINE_XSTROGE_CORE_DB_QUERYFILETABLE_EMPTY;
		return FALSE;
	}
	BaseLib_OperatorMemory_Malloc((XPPPMEM)pppSt_ListFile, (int)nllLine, sizeof(XSTORAGECORE_DBFILE));
	//ѭ����ȡ���в��ҵ����ļ�
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
�������ƣ�XStorage_MySql_FileQueryForHash
�������ܣ�ͨ��MD5��ѯ�ļ���Ϣ
 ����.һ��pSt_FileInfo
  In/Out��Out
  ���ͣ����ݽṹָ��
  �ɿգ�N
  ��˼�������ѯ�����ļ���Ϣ
 ����.����lpszFileHash
  In/Out��In
  ���ͣ������ַ�ָ��
  �ɿգ�N
  ��˼������Ҫ��ѯ���ļ�MD5
 ����.����lpszUser
  In/Out��In
  ���ͣ������ַ�ָ��
  �ɿգ�N
  ��˼�������ļ������û�
 ����.�ģ�lpszTimeStart
  In/Out��In
  ���ͣ������ַ�ָ��
  �ɿգ�Y
  ��˼�����뿪ʼʱ��
 ����.�壺lpszTimeEnd
  In/Out��In
  ���ͣ������ַ�ָ��
  �ɿգ�Y
  ��˼���������ʱ��
����ֵ
  ���ͣ��߼���
  ��˼���Ƿ�ɹ�
��ע��
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
    //��ѯ
    XHDATA xhTable = 0;
    __int64u nllLine = 0;
    __int64u nllRow = 0;
    BOOL bFound = FALSE;
    TCHAR tszSQLStatement[1024];
    memset(tszSQLStatement, '\0', sizeof(tszSQLStatement));
    //����Ƿ�ʱ�䷶Χ����
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
    //��ѵ
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
        //�жϲ�ѯ��ʽ
        if (NULL == lpszUser)
        {
            _stprintf_s(tszSQLStatement, _T("SELECT * FROM `%s` WHERE FileHash = '%s'"), pptszResult[0], lpszFileHash);
        }
        else
        {
            _stprintf_s(tszSQLStatement, _T("SELECT * FROM `%s` WHERE FileHash = '%s' AND FileUser = '%s'"), pptszResult[0], lpszFileHash, lpszUser);
        }
        //��ѯ�ļ�
        if (DataBase_MySQL_ExecuteQuery(xhDBSQL, &xhResult, tszSQLStatement, &dwLineResult, &dwFieldResult))
        {
            //ѭ����ȡ���в��ҵ����ļ�
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
�������ƣ�XStorage_MySql_FileGetCount
�������ܣ���ȡ���ݿ����ļ��ܸ������ܴ�С
 ����.һ��pInt_Count
  In/Out��Out
  ���ͣ�64λ������ָ��
  �ɿգ�N
  ��˼��������ȡ�����ļ�����
 ����.����pInt_Size
  In/Out��Out
  ���ͣ�64λ������ָ��
  �ɿգ�N
  ��˼��������ȡ�����ļ���С,��λ�ֽ�(BYTE)
����ֵ
  ���ͣ��߼���
  ��˼���Ƿ�ɹ�
��ע��
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
    TCHAR tszSQLStatement[1024];    //SQL���
    memset(tszSQLStatement, '\0', sizeof(tszSQLStatement));

    _stprintf_s(tszSQLStatement, _T("SELECT * FROM `XStorage_Count`"));
    //��ѯ
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
    //��ȡ��¼��
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
//                    ��������
//////////////////////////////////////////////////////////////////////////
/********************************************************************
�������ƣ�XStorage_MySql_CreateTable
�������ܣ����������·ݵ��ļ���
����ֵ
  ���ͣ��߼���
  ��˼���Ƿ�ɹ�
��ע��
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
			"`ID` int NOT NULL AUTO_INCREMENT COMMENT 'ID���',"
			"`FilePath` varchar(260) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL COMMENT '�ļ�·��',"
			"`FileName` varchar(260) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL COMMENT '�ļ�����',"
			"`FileHash` varchar(260) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '�ļ�HASH',"
			"`FileUser` varchar(260) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '�ļ������û�',"
			"`FileSize` bigint NOT NULL COMMENT '�ļ���С',"
			"`FileTime` datetime NOT NULL COMMENT '����ʱ��',"
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
�������ƣ�XStorage_MySql_TimeDay
�������ܣ�����ָ��ʱ���뵱ǰʱ��������
 ����.һ��lpszStartTime
  In/Out��In
  ���ͣ������ַ�ָ��
  �ɿգ�N
  ��˼�����뿪ʼ�����ʱ��
 ����.����pInt_Month
  In/Out��Out
  ���ͣ�������ָ��
  �ɿգ�N
  ��˼���������·�
����ֵ
  ���ͣ��߼���
  ��˼���Ƿ�ɹ�
��ע��
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
�������ƣ�XStorage_MySql_TimeDel
�������ܣ�ɾ���������ڱ�����
����ֵ
  ���ͣ��߼���
  ��˼���Ƿ�ɹ�
��ע��
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
            //ֻ���ڴ�����ȷ������²Ž���ɾ������
            if (XStorage_MySql_TimeMonth(pptszResult[0], &nTimeMonth))
            {
                if (nTimeMonth > m_nTimeMonth)
                {
                    //ɾ���ļ�
                    int nListCount = 0;
                    XSTORAGECORE_DBFILE **ppSt_ListFile;
                    XStorage_MySql_FileQueryForTable(&ppSt_ListFile, &nListCount, pptszResult[0]);

                    for (int i = 0; i < nListCount; i++)
                    {
                        //�����ļ������ʹ�С
                        memset(tszSQLQuery, '\0', sizeof(tszSQLQuery));
                        _stprintf_s(tszSQLQuery, _T("UPDATE `XStorage_Count` SET FileCount = FileCount - 1,FileSize = FileSize - %lld"), ppSt_ListFile[i]->st_ProtocolFile.nFileSize);
                        if (!DataBase_MySQL_Execute(xhDBSQL, tszSQLQuery))
                        {
                            XStorage_IsErrorOccur = TRUE;
                            XStorage_dwErrorCode = DataBase_GetLastError();
                            return FALSE;
                        }
                        //ɾ���ļ�
                        TCHAR tszFilePath[2048];
                        memset(tszFilePath, '\0', sizeof(tszFilePath));

                        _stprintf(tszFilePath, _T("%s/%s"), ppSt_ListFile[i]->st_ProtocolFile.tszFilePath, ppSt_ListFile[i]->st_ProtocolFile.tszFileName);
                        _tremove(tszFilePath);
                    }
                    //ɾ�����ݿ�
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
//                      �̺߳���
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

