#include "pch.h"
#include "XStorage_SQLite.h"
/********************************************************************
//    Created:     2021/07/12  09:32:21
//    File Name:   D:\XEngine_Storage\XEngine_Source\XEngine_StorageComponents\XStorage_SQLPacket\XStorage_SQLite\XStorage_SQLite.cpp
//    File Path:   D:\XEngine_Storage\XEngine_Source\XEngine_StorageComponents\XStorage_SQLPacket\XStorage_SQLite
//    File Base:   XStorage_SQLite
//    File Ext:    cpp
//    Project:     XEngine(����ͨ������)
//    Author:      qyt
//    Purpose:     sqlite����
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
//                             ���к���
//////////////////////////////////////////////////////////////////////////
/********************************************************************
�������ƣ�XStorage_SQLite_Init
�������ܣ���ʼ��SQLITE�ļ�ϵͳ
 ����.һ��lpszSQLFile
  In/Out��In
  ���ͣ������ַ�ָ��
  �ɿգ�N
  ��˼������Ҫ������SQL�ļ�
 ����.����nTimeMonth
  In/Out��In
  ���ͣ�������
  �ɿգ�Y
  ��˼�������������ʱ��,��λ��
����ֵ
  ���ͣ��߼���
  ��˼���Ƿ�ɹ�
��ע��
*********************************************************************/
BOOL CXStorage_SQLite::XStorage_SQLite_Init(LPCTSTR lpszSQLFile, int nTimeMonth /* = 1 */)
{
	XStorage_IsErrorOccur = FALSE;

	//�������ݿ�
	if (!DataBase_SQLite_Create(lpszSQLFile))
	{
		//��������������˵���������ݿ�ʧ����
		if (ERROR_HELPCOMPONENTS_DATABASE_SQLITE_CREATE_ISEXIST != DataBase_GetLastError())
		{
			XStorage_IsErrorOccur = TRUE;
			XStorage_dwErrorCode = DataBase_GetLastError();
			return FALSE;
		}
	}
	//�����ݿ�
	if (!DataBase_SQLite_Open(&xhSQL, lpszSQLFile))
	{
		XStorage_IsErrorOccur = TRUE;
		XStorage_dwErrorCode = DataBase_GetLastError();
		return FALSE;
	}
    bIsRun = TRUE;
	m_nTimeMonth = nTimeMonth;
	
	pSTDThread = make_shared<std::thread>(XStorage_SQLite_Thread, this);
	if (!pSTDThread->joinable())
	{
		XStorage_IsErrorOccur = TRUE;
		XStorage_dwErrorCode = ERROR_XENGINE_XSTROGE_CORE_DB_INIT_THREAD;
		return FALSE;
	}
	return TRUE;
}
/********************************************************************
�������ƣ�XStorage_SQLite_Destory
�������ܣ�����
����ֵ
  ���ͣ��߼���
  ��˼���Ƿ�ɹ�
��ע��
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
        pSTDThread->join();
    }
	DataBase_SQLite_Close(xhSQL);
	return TRUE;
}
/********************************************************************
�������ƣ�XStorage_SQLite_FileInsert
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
    if (!DataBase_SQLite_Exec(xhSQL, tszSQLStatement))
    {
        XStorage_IsErrorOccur = TRUE;
        XStorage_dwErrorCode = DataBase_GetLastError();
        return FALSE;
    }
    return TRUE;
}
/********************************************************************
�������ƣ�XStorage_SQLite_FileDelete
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
�������ƣ�XStorage_SQLite_FileQuery
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
BOOL CXStorage_SQLite::XStorage_SQLite_FileQuery(XSTORAGECORE_DBFILE*** pppSt_ListFile, int* pInt_ListCount, LPCTSTR lpszTimeStart /* = NULL */, LPCTSTR lpszTimeEnd /* = NULL */, LPCTSTR lpszFile /* = NULL */, LPCTSTR lpszHash /* = NULL */)
{
    XStorage_IsErrorOccur = FALSE;

    if ((NULL == lpszHash) && (NULL == lpszFile))
    {
        XStorage_IsErrorOccur = TRUE;
        XStorage_dwErrorCode = ERROR_XENGINE_XSTROGE_CORE_DB_QUERYFILE_PARAMENT;
        return FALSE;
    }
    //��ѯ
    int nLine = 0;
    int nRow = 0;
    TCHAR** pptszResult;
    TCHAR tszSQLStatement[1024];
    memset(tszSQLStatement, '\0', sizeof(tszSQLStatement));
    //����Ƿ�ʱ�䷶Χ����
    if ((NULL != lpszTimeStart) && (NULL != lpszTimeEnd))
    {
        if (_tcslen(lpszTimeStart) > 0 && _tcslen(lpszTimeEnd) > 0)
        {
            _stprintf_s(tszSQLStatement, _T("SELECT NAME FROM SQLITE_MASTER WHERE TYPE = 'TABLE' AND NAME BETWEEN '%s' AND '%s'"), lpszTimeStart, lpszTimeEnd);
        }
        else
        {
            _stprintf_s(tszSQLStatement, _T("SELECT NAME FROM SQLITE_MASTER WHERE TYPE = 'TABLE'"));
        }
    }
    else
    {
        _stprintf_s(tszSQLStatement, _T("SELECT NAME FROM SQLITE_MASTER WHERE TYPE = 'TABLE'"));
    }
    if (!DataBase_SQLite_GetTable(xhSQL, tszSQLStatement, &pptszResult, &nLine, &nRow))
    {
        XStorage_IsErrorOccur = TRUE;
        XStorage_dwErrorCode = DataBase_GetLastError();
        return FALSE;
    }
	list<XSTORAGECORE_DBFILE> stl_ListFile;
	//��ѵ
	for (int i = 0; i < nLine; i++)
	{
		int nLineResult = 0;
		int nFieldResult = 0;
		memset(tszSQLStatement, '\0', sizeof(tszSQLStatement));
		//�жϲ�ѯ��ʽ
		if (NULL != lpszFile)
		{
			if (_tcslen(lpszFile) > 0)
			{
				_stprintf_s(tszSQLStatement, _T("SELECT * FROM `%s` WHERE FileName = '%s'"), pptszResult[i], lpszFile);
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
		//��ѯ�ļ�
        TCHAR** pptszFileResult;
        if (DataBase_SQLite_GetTable(xhSQL, tszSQLStatement, &pptszFileResult, &nLineResult, &nFieldResult))
		{
            int nFiled = 0;
			//ѭ����ȡ���в��ҵ����ļ�
			for (int j = 0; j < nLineResult; j++)
			{
				XSTORAGECORE_DBFILE st_DBFile;
				memset(&st_DBFile, '\0', sizeof(XSTORAGECORE_DBFILE));

				_tcscpy(st_DBFile.tszTableName, pptszResult[0]);
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
�������ƣ�XStorage_SQLite_FileQueryForTable
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
BOOL CXStorage_SQLite::XStorage_SQLite_FileQueryForTable(XSTORAGECORE_DBFILE*** pppSt_ListFile, int* pInt_ListCount, LPCTSTR lpszTableName)
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
    int nLine = 0;
    int nRow = 0;
    TCHAR** pptszResulte;
    TCHAR tszSQLStatement[1024];
    memset(tszSQLStatement, '\0', sizeof(tszSQLStatement));
    //����Ƿ�ʱ�䷶Χ����
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
    //ѭ����ȡ���в��ҵ����ļ�
    int nFiled = nRow;
    for (int i = 0; i < nLine; i++)
    {
        nFiled++;
        _tcscpy((*pppSt_ListFile)[i]->tszTableName, lpszTableName);

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
//                    ��������
//////////////////////////////////////////////////////////////////////////
/********************************************************************
�������ƣ�XStorage_SQLite_CreateTable
�������ܣ����������·ݵ��ļ���
����ֵ
  ���ͣ��߼���
  ��˼���Ƿ�ɹ�
��ע��
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

		_stprintf_s(tszSQLQuery, _T("DROP TABLE IF EXISTS `%s`;"
			"CREATE TABLE `%s`  ("
			"  `ID` int NOT NULL AUTO_INCREMENT COMMENT 'ID���',"
			"  `FilePath` varchar(260) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL COMMENT '�ļ�·��',"
			"  `FileName` varchar(260) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL COMMENT '�ļ�����',"
			"  `FileHash` varchar(260) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '�ļ�HASH',"
			"  `FileUser` varchar(260) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '�ļ������û�',"
			"  `FileUser` varchar(260) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '�ļ������û�',"
			"  `FileSize` bigint NOT NULL COMMENT '�ļ���С',"
			"  `FileTime` datetime NOT NULL COMMENT '����ʱ��',"
			"  PRIMARY KEY (`ID`) USING BTREE"
			") ENGINE = InnoDB AUTO_INCREMENT = 1 CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = DYNAMIC;"
			"SET FOREIGN_KEY_CHECKS = 1;"
        ), tszTableName, tszTableName);

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
�������ƣ�XStorage_SQLite_TimeDay
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
�������ƣ�XStorage_SQLite_TimeDel
�������ܣ�ɾ���������ڱ�����
����ֵ
  ���ͣ��߼���
  ��˼���Ƿ�ɹ�
��ע��
*********************************************************************/
BOOL CXStorage_SQLite::XStorage_SQLite_TimeDel()
{
    XStorage_IsErrorOccur = FALSE;

    int nLine = 0;
    int nField = 0;
    TCHAR** pptszResult;
    TCHAR tszSQLQuery[2048];

    memset(tszSQLQuery, '\0', sizeof(tszSQLQuery));
    _stprintf_s(tszSQLQuery, _T("SELECT NAME FROM SQLITE_MASTER WHERE TYPE = 'TABLE' ORDER BY NAME"));
    if (DataBase_SQLite_GetTable(xhSQL, tszSQLQuery, &pptszResult, &nLine, &nField))
    {
        for (int i = 0; i < nLine; i++)
        {
            int nTimeMonth = 0;
            //ֻ���ڴ�����ȷ������²Ž���ɾ������
            if (XStorage_SQLite_TimeMonth(pptszResult[i], &nTimeMonth))
            {
                if (nTimeMonth > m_nTimeMonth)
                {
                    //ɾ���ļ�
                    int nListCount = 0;
                    XSTORAGECORE_DBFILE** ppSt_ListFile;
                    XStorage_SQLite_FileQueryForTable(&ppSt_ListFile, &nListCount, pptszResult[i]);
                    for (int i = 0; i < nListCount; i++)
                    {
                        //ɾ���ļ�
                        TCHAR tszFilePath[2048];
                        memset(tszFilePath, '\0', sizeof(tszFilePath));

                        _stprintf(tszFilePath, _T("%s/%s"), ppSt_ListFile[i]->st_ProtocolFile.tszFilePath, ppSt_ListFile[i]->st_ProtocolFile.tszFileName);
                        _tremove(tszFilePath);
                    }
                    //ɾ�����ݿ�
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
//////////////////////////////////////////////////////////////////////////
//                      �̺߳���
//////////////////////////////////////////////////////////////////////////
XHTHREAD CXStorage_SQLite::XStorage_SQLite_Thread(LPVOID lParam)
{
    CXStorage_SQLite* pClass_This = (CXStorage_SQLite*)lParam;
    time_t nTimeStart = time(NULL);
    time_t nTimeEnd = 0;
    BOOL bFirst = TRUE;
    int nTime = 60 * 60 * 12;

    while (pClass_This->bIsRun)
    {
        if (((nTimeEnd - nTimeStart) > nTime) || bFirst)
        {
            pClass_This->XStorage_SQLite_TimeDel();
            pClass_This->XStorage_SQLite_CreateTable();
            bFirst = FALSE;
        }
        nTimeEnd = time(NULL);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    return 0;
}

