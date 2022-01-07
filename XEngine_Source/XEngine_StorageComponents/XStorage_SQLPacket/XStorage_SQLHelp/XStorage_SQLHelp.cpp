#include "pch.h"
#include "XStorage_SQLHelp.h"
/********************************************************************
//    Created:     2022/01/06  10:24:07
//    File Name:   D:\XEngine_Storage\XEngine_Source\XEngine_StorageComponents\XStorage_SQLPacket\XStorage_SQLHelp\XStorage_SQLHelp.cpp
//    File Path:   D:\XEngine_Storage\XEngine_Source\XEngine_StorageComponents\XStorage_SQLPacket\XStorage_SQLHelp
//    File Base:   XStorage_SQLHelp
//    File Ext:    cpp
//    Project:     XEngine(����ͨ������)
//    Author:      qyt
//    Purpose:     SQL���ݿ��﷨������
//    History:
*********************************************************************/
CXStorage_SQLHelp::CXStorage_SQLHelp()
{
}
CXStorage_SQLHelp::~CXStorage_SQLHelp()
{
}
//////////////////////////////////////////////////////////////////////////
//                         ���к���
//////////////////////////////////////////////////////////////////////////
/********************************************************************
�������ƣ�XStorage_SQLHelp_Insert
�������ܣ����ݲ��뺯��
 ����.һ��ptszSQLBuffer
  In/Out��Out
  ���ͣ��ַ�ָ��
  �ɿգ�N
  ��˼�������ð��Ĳ������
 ����.����pSt_DBFile
  In/Out��In
  ���ͣ����ݽṹָ��
  �ɿգ�N
  ��˼������Ҫ���������
����ֵ
  ���ͣ��߼���
  ��˼���Ƿ�ɹ�
��ע��
*********************************************************************/
BOOL CXStorage_SQLHelp::XStorage_SQLHelp_Insert(TCHAR* ptszSQLBuffer, XSTORAGECORE_DBFILE* pSt_DBFile)
{
	XStorage_IsErrorOccur = FALSE;

	TCHAR tszTableName[64];
	XENGINE_LIBTIMER st_LibTimer;

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
		_stprintf(tszTableName, _T("%04d%02d"), st_LibTimer.wYear, st_LibTimer.wMonth);
	}

	_stprintf(ptszSQLBuffer, _T("INSERT INTO `%s` (BuckKey,FilePath,FileName,FileHash,FileUser,FileSize,FileTime) VALUES('%s','%s','%s','%s','%s',%lld,%04d-%02d-%02d %02d:%02d:%02d)"), tszTableName, pSt_DBFile->tszBuckKey, pSt_DBFile->st_ProtocolFile.tszFilePath, pSt_DBFile->st_ProtocolFile.tszFileName, pSt_DBFile->st_ProtocolFile.tszFileHash, pSt_DBFile->st_ProtocolFile.tszFileUser, pSt_DBFile->st_ProtocolFile.nFileSize, st_LibTimer.wYear, st_LibTimer.wMonth, st_LibTimer.wDay, st_LibTimer.wHour, st_LibTimer.wMinute, st_LibTimer.wSecond);
	return TRUE;
}
/********************************************************************
�������ƣ�XStorage_SQLHelp_Delete
�������ܣ�ɾ�����
 ����.һ��ptszSQLBuffer
  In/Out��In
  ���ͣ��ַ�ָ��
  �ɿգ�N
  ��˼�������ð��Ĳ�ѯ���
 ����.����lpszTableName
  In/Out��In
  ���ͣ������ַ�ָ��
  �ɿգ�Y
  ��˼������Ҫ��ѯ�ı�����
 ����.����lpszBuckKey
  In/Out��In
  ���ͣ������ַ�ָ��
  �ɿգ�Y
  ��˼������BUCKET��
 ����.�ģ�lpszFileName
  In/Out��In
  ���ͣ������ַ�ָ��
  �ɿգ�Y
  ��˼�������ļ�����
 ����.�壺lpszFileHash
  In/Out��In
  ���ͣ������ַ�ָ��
  �ɿգ�Y
  ��˼�������ļ�HASHֵ
����ֵ
  ���ͣ��߼���
  ��˼���Ƿ�ɹ�
��ע��
*********************************************************************/
BOOL CXStorage_SQLHelp::XStorage_SQLHelp_Delete(TCHAR* ptszSQLBuffer, LPCTSTR lpszTableName, LPCTSTR lpszBuckKey /* = NULL */, LPCTSTR lpszFileName /* = NULL */, LPCTSTR lpszFileHash /* = NULL */)
{
	XStorage_IsErrorOccur = FALSE;

	TCHAR tszSQLQuery[1024];
	memset(tszSQLQuery, '\0', sizeof(tszSQLQuery));

	XStorage_SQLHelp_Packet(tszSQLQuery, lpszBuckKey, NULL, lpszFileName, lpszFileHash);
	_stprintf(ptszSQLBuffer, _T("DELETE FROM `%s` %s"), lpszTableName, tszSQLQuery);
	return TRUE;
}
/********************************************************************
�������ƣ�XStorage_SQLHelp_Query
�������ܣ����ݿ��ѯ�������
 ����.һ��ptszSQLBuffer
  In/Out��In
  ���ͣ��ַ�ָ��
  �ɿգ�N
  ��˼�������ð��Ĳ�ѯ���
 ����.����lpszTableName
  In/Out��In
  ���ͣ������ַ�ָ��
  �ɿգ�Y
  ��˼������Ҫ��ѯ�ı�����
 ����.����lpszBuckKey
  In/Out��In
  ���ͣ������ַ�ָ��
  �ɿգ�Y
  ��˼������BUCKET��
 ����.�ģ�lpszFilePath
  In/Out��In
  ���ͣ������ַ�ָ��
  �ɿգ�Y
  ��˼�������ļ�·��
 ����.�壺lpszFileName
  In/Out��In
  ���ͣ������ַ�ָ��
  �ɿգ�Y
  ��˼�������ļ�����
 ����.����lpszFileHash
  In/Out��In
  ���ͣ������ַ�ָ��
  �ɿգ�Y
  ��˼�������ļ�HASHֵ
 ����.�ߣ�lpszFileUser
  In/Out��In
  ���ͣ������ַ�ָ��
  �ɿգ�Y
  ��˼�������ļ������û�
 ����.�ˣ�lpszTimeStart
  In/Out��In
  ���ͣ������ַ�ָ��
  �ɿգ�Y
  ��˼�������ļ���ʼʱ�䷶Χ
 ����.�ţ�lpszTimeEnd
  In/Out��In
  ���ͣ������ַ�ָ��
  �ɿգ�Y
  ��˼�������ļ�����ʱ�䷶Χ
����ֵ
  ���ͣ��߼���
  ��˼���Ƿ�ɹ�
��ע��
*********************************************************************/
BOOL CXStorage_SQLHelp::XStorage_SQLHelp_Query(TCHAR* ptszSQLBuffer, LPCTSTR lpszTableName, LPCTSTR lpszBuckKey /* = NULL */, LPCTSTR lpszFilePath /* = NULL */, LPCTSTR lpszFileName /* = NULL */, LPCTSTR lpszFileHash /* = NULL */, LPCTSTR lpszFileUser /* = NULL */, LPCTSTR lpszTimeStart /* = NULL */, LPCTSTR lpszTimeEnd /* = NULL */)
{
    XStorage_IsErrorOccur = FALSE;

    BOOL bInit = FALSE;
    TCHAR tszSQLQuery[1024];
    memset(tszSQLQuery, '\0', sizeof(tszSQLQuery));

	_stprintf(ptszSQLBuffer, _T("SELECT * FROM `%s`"), lpszTableName);
	XStorage_SQLHelp_Packet(tszSQLQuery, lpszBuckKey, lpszFilePath, lpszFileName, lpszFileHash, lpszFileUser, lpszTimeStart, lpszTimeEnd);
	_tcscat(ptszSQLBuffer, tszSQLQuery);
    return TRUE;
}
//////////////////////////////////////////////////////////////////////////
//                          ��������
//////////////////////////////////////////////////////////////////////////
BOOL CXStorage_SQLHelp::XStorage_SQLHelp_Packet(TCHAR* ptszSQLBuffer, LPCTSTR lpszBuckKey /* = NULL */, LPCTSTR lpszFilePath /* = NULL */, LPCTSTR lpszFileName /* = NULL */, LPCTSTR lpszFileHash /* = NULL */, LPCTSTR lpszFileUser /* = NULL */, LPCTSTR lpszTimeStart /* = NULL */, LPCTSTR lpszTimeEnd /* = NULL */)
{
	XStorage_IsErrorOccur = FALSE;

	BOOL bInit = FALSE;
	TCHAR tszSQLQuery[MAX_PATH];
	memset(tszSQLQuery, '\0', MAX_PATH);
	//�ļ�����BUCKET
	if (NULL != lpszBuckKey)
	{
		if (_tcslen(lpszBuckKey) > 0)
		{
			if (bInit)
			{
				_tcscat(ptszSQLBuffer, _T(" AND "));
			}
			else
			{
				_tcscat(ptszSQLBuffer, _T("WHERE "));
			}
			memset(tszSQLQuery, '\0', MAX_PATH);
			_stprintf(tszSQLQuery, _T("BuckKey = '%s'"), lpszBuckKey);
			_tcscat(ptszSQLBuffer, tszSQLQuery);
			bInit = TRUE;
		}
	}
	//�ļ�·��
	if (NULL != lpszFilePath)
	{
		if (_tcslen(lpszFilePath) > 0)
		{
			if (bInit)
			{
				_tcscat(ptszSQLBuffer, _T(" AND "));
			}
			else
			{
				_tcscat(ptszSQLBuffer, _T("WHERE "));
			}
			memset(tszSQLQuery, '\0', MAX_PATH);
			_stprintf(tszSQLQuery, _T("FilePath = '%s'"), lpszFilePath);
			_tcscat(ptszSQLBuffer, tszSQLQuery);
			bInit = TRUE;
		}
	}
	//�ļ�����
	if (NULL != lpszFileName)
	{
		if (_tcslen(lpszFileName) > 0)
		{
			if (bInit)
			{
				_tcscat(ptszSQLBuffer, _T(" AND "));
			}
			else
			{
				_tcscat(ptszSQLBuffer, _T("WHERE "));
			}
			memset(tszSQLQuery, '\0', MAX_PATH);
			_stprintf(tszSQLQuery, _T("FileName = '%s'"), lpszFileName);
			_tcscat(ptszSQLBuffer, tszSQLQuery);
			bInit = TRUE;
		}
	}
	//�ļ�HASH
	if (NULL != lpszFileHash)
	{
		if (_tcslen(lpszFileHash) > 0)
		{
			if (bInit)
			{
				_tcscat(ptszSQLBuffer, _T(" AND "));
			}
			else
			{
				_tcscat(ptszSQLBuffer, _T("WHERE "));
			}
			memset(tszSQLQuery, '\0', MAX_PATH);
			_stprintf(tszSQLQuery, _T("FileHash = '%s'"), lpszFileHash);
			_tcscat(ptszSQLBuffer, tszSQLQuery);
			bInit = TRUE;
		}
	}
	//�ļ������û�
	if (NULL != lpszFileUser)
	{
		if (_tcslen(lpszFileUser) > 0)
		{
			if (bInit)
			{
				_tcscat(ptszSQLBuffer, _T(" AND "));
			}
			else
			{
				_tcscat(ptszSQLBuffer, _T("WHERE "));
			}
			memset(tszSQLQuery, '\0', MAX_PATH);
			_stprintf(tszSQLQuery, _T("FileUser = '%s'"), lpszFileUser);
			_tcscat(ptszSQLBuffer, tszSQLQuery);
			bInit = TRUE;
		}
	}
	//ʱ�䷶Χ
	if ((NULL != lpszTimeStart) && (NULL != lpszTimeEnd))
	{
		if ((_tcslen(lpszTimeStart) > 0) && (_tcslen(lpszTimeEnd) > 0))
		{
			if (bInit)
			{
				_tcscat(ptszSQLBuffer, _T(" AND "));
			}
			else
			{
				_tcscat(ptszSQLBuffer, _T("WHERE "));
			}
			memset(tszSQLQuery, '\0', MAX_PATH);
			_stprintf(tszSQLQuery, _T("BETWEEN '%s' AND '%s'"), lpszTimeStart, lpszTimeEnd);
			_tcscat(ptszSQLBuffer, tszSQLQuery);
			bInit = TRUE;
		}
	}

	return TRUE;
}