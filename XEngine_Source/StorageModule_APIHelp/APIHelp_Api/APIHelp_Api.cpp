#include "pch.h"
#include "APIHelp_Api.h"
/********************************************************************
//    Created:     2022/01/07  14:36:59
//    File Name:   D:\XEngine_Storage\XEngine_Source\StorageModule_APIHelp\APIHelp_Api\APIHelp_Api.cpp
//    File Path:   D:\XEngine_Storage\XEngine_Source\StorageModule_APIHelp\APIHelp_Api
//    File Base:   APIHelp_Api
//    File Ext:    cpp
//    Project:     XEngine(����ͨ������)
//    Author:      qyt
//    Purpose:     ��������
//    History:
*********************************************************************/
CAPIHelp_Api::CAPIHelp_Api()
{

}
CAPIHelp_Api::~CAPIHelp_Api()
{

}
//////////////////////////////////////////////////////////////////////////
//                               ���к���
//////////////////////////////////////////////////////////////////////////
/********************************************************************
�������ƣ�APIHelp_Api_ProxyAuth
�������ܣ�������֤
 ����.һ��ptszUser
  In/Out��Out
  ���ͣ��ַ�ָ��
  �ɿգ�N
  ��˼������û���
 ����.����ptszPass
  In/Out��Out
  ���ͣ��ַ�ָ��
  �ɿգ�N
  ��˼���������
 ����.����pptszListHdr
  In/Out��In
  ���ͣ�ָ��ָ���ָ��
  �ɿգ�N
  ��˼������Ҫ������HTTPͷ
 ����.�ģ�nHdrCount
  In/Out��In
  ���ͣ�������
  �ɿգ�N
  ��˼������Ҫ������HTTPͷ�б����
����ֵ
  ���ͣ��߼���
  ��˼���Ƿ�ɹ�
��ע��
*********************************************************************/
BOOL CAPIHelp_Api::APIHelp_Api_ProxyAuth(TCHAR* ptszUser, TCHAR* ptszPass, TCHAR** pptszListHdr, int nHdrCount)
{
	APIHelp_IsErrorOccur = FALSE;

	int nSDLen = 1024;
	int nAuthType = 0;
	int nAuthLen = MAX_PATH;

	TCHAR tszAuthStr[MAX_PATH];
	TCHAR tszSDBuffer[1024];

	memset(tszAuthStr, '\0', MAX_PATH);
	memset(tszSDBuffer, '\0', sizeof(tszSDBuffer));
	//�Ƿ�����֤��Ϣ
	if (!RfcComponents_HttpHelp_GetAuthInfo(&pptszListHdr, nHdrCount, tszAuthStr, &nAuthLen, &nAuthType))
	{
		APIHelp_IsErrorOccur = TRUE;
		APIHelp_dwErrorCode = ERROR_STORAGE_MODULE_APIHELP_NOTAUTH;
		return FALSE;
	}
	//�Ƿ���BASIC
	if (1 != nAuthType)
	{
		APIHelp_IsErrorOccur = TRUE;
		APIHelp_dwErrorCode = ERROR_STORAGE_MODULE_APIHELP_NOTSUPPORT;
		return FALSE;
	}
	OPenSsl_Help_BasicDecoder(tszAuthStr, ptszUser, ptszPass);
	return TRUE;
}
/********************************************************************
�������ƣ�APIHelp_Api_RangeFile
�������ܣ���ȡHTTP�ķ�Χ
 ����.һ��pInt_SPos
  In/Out��Out
  ���ͣ�������ָ��
  �ɿգ�N
  ��˼�������ʼ��Χ
 ����.����pInt_EPos
  In/Out��Out
  ���ͣ�������ָ��
  �ɿգ�N
  ��˼�����������Χ
 ����.����pInt_Count
  In/Out��Out
  ���ͣ�������ָ��
  �ɿգ�N
  ��˼������ܴ�С
 ����.�ģ�pptszListHdr
  In/Out��In
  ���ͣ�ָ��ָ���ָ��
  �ɿգ�N
  ��˼������HTTPЭ��ͷ�б�
 ����.�壺nHdrCount
  In/Out��In
  ���ͣ�ָ��ָ���ָ��
  �ɿգ�N
  ��˼�������б����
����ֵ
  ���ͣ��߼���
  ��˼���Ƿ�ɹ�
��ע��
*********************************************************************/
BOOL CAPIHelp_Api::APIHelp_Api_RangeFile(int* pInt_SPos, int* pInt_EPos, __int64x* pInt_Count, TCHAR** pptszListHdr, int nHdrCount)
{
	APIHelp_IsErrorOccur = FALSE;

	BOOL bFound = FALSE;
	LPCTSTR lpszRangeStr = _T("Range");
	LPCTSTR lpszLengthStr = _T("Content-Length");
	TCHAR tszKeyStr[128];
	TCHAR tszValueStr[128];
	TCHAR tszFieldStr[128];

	memset(tszKeyStr, '\0', sizeof(tszKeyStr));
	memset(tszValueStr, '\0', sizeof(tszValueStr));
	memset(tszFieldStr, '\0', sizeof(tszFieldStr));
	//�Ƿ��з�Χ
	if (RfcComponents_HttpHelp_GetField(&pptszListHdr, nHdrCount, lpszRangeStr, tszFieldStr))
	{
		//�Ƿ�û���ҵ�
		int nBPos = 0;  //ĳЩʱ���и�BYTE   
		if (NULL != _tcsstr(tszFieldStr, _T("bytes=")))
		{
			nBPos = 6;
		}
		if (!BaseLib_OperatorString_GetKeyValue(tszFieldStr + nBPos, "-", tszKeyStr, tszValueStr))
		{
			APIHelp_IsErrorOccur = TRUE;
			APIHelp_dwErrorCode = ERROR_STORAGE_MODULE_APIHELP_PARSELEN;
			return FALSE;
		}
		//�õ� 1/2 ����еĻ�
		TCHAR tszRangeEnd[128];
		TCHAR tszRangeCount[128];

		memset(tszRangeEnd, '\0', sizeof(tszRangeEnd));
		memset(tszRangeCount, '\0', sizeof(tszRangeCount));
		if (BaseLib_OperatorString_GetKeyValue(tszValueStr, "/", tszRangeEnd, tszRangeCount))
		{
			*pInt_SPos = _ttoi(tszKeyStr);
			*pInt_EPos = _ttoi(tszRangeEnd);
			*pInt_Count = _ttoi64(tszRangeCount);
		}
		else
		{
			*pInt_SPos = _ttoi(tszKeyStr);
			*pInt_EPos = _ttoi(tszValueStr);
		}
		bFound = TRUE;
	}

	if (0 == *pInt_Count)
	{
		if (RfcComponents_HttpHelp_GetField(&pptszListHdr, nHdrCount, lpszLengthStr, tszFieldStr))
		{
			*pInt_Count = _ttoi64(tszFieldStr);
			bFound = TRUE;
		}
	}
	
	if (!bFound)
	{
		APIHelp_IsErrorOccur = TRUE;
		APIHelp_dwErrorCode = ERROR_STORAGE_MODULE_APIHELP_NOTLENGTH;
		return FALSE;
	}
	
	return TRUE;
}
/********************************************************************
�������ƣ�APIHelp_Api_VerHash
�������ܣ���֤HASHֵ
 ����.һ��lpszFileHash
  In/Out��In
  ���ͣ������ַ�ָ��
  �ɿգ�N
  ��˼������Ҫ�ȶԵ�HASHֵ
 ����.����pptszListHdr
  In/Out��In
  ���ͣ�ָ��ָ���ָ��
  �ɿգ�N
  ��˼������HTTPЭ��ͷ�б�
 ����.����nHdrCount
  In/Out��In
  ���ͣ�ָ��ָ���ָ��
  �ɿգ�N
  ��˼�������б����
����ֵ
  ���ͣ��߼���
  ��˼���Ƿ�ɹ�
��ע��
*********************************************************************/
BOOL CAPIHelp_Api::APIHelp_Api_VerHash(LPCTSTR lpszFileHash, TCHAR** pptszListHdr, int nHdrCount)
{
	APIHelp_IsErrorOccur = FALSE;

	LPCTSTR lpszKeyStr = _T("FileHash");
	TCHAR tszValueStr[MAX_PATH];
	memset(tszValueStr, '\0', MAX_PATH);

	if (!RfcComponents_HttpHelp_GetField(&pptszListHdr, nHdrCount, lpszKeyStr, tszValueStr))
	{
		APIHelp_IsErrorOccur = TRUE;
		APIHelp_dwErrorCode = ERROR_STORAGE_MODULE_APIHELP_NOTHASH;
		return FALSE;
	}
	if (0 != _tcsnicmp(lpszFileHash, tszValueStr, _tcslen(lpszFileHash)))
	{
		APIHelp_IsErrorOccur = TRUE;
		APIHelp_dwErrorCode = ERROR_STORAGE_MODULE_APIHELP_NOTMATCH;
		return FALSE;
	}
	return TRUE;
}