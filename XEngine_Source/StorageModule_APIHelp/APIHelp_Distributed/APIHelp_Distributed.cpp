#include "pch.h"
#include "APIHelp_Distributed.h"
/********************************************************************
//    Created:     2021/07/08  15:26:13
//    File Name:   D:\XEngine_Storage\XEngine_Source\StorageModule_APIHelp\APIHelp_Distributed\APIHelp_Distributed.cpp
//    File Path:   D:\XEngine_Storage\XEngine_Source\StorageModule_APIHelp\APIHelp_Distributed
//    File Base:   APIHelp_Distributed
//    File Ext:    cpp
//    Project:     XEngine(����ͨ������)
//    Author:      qyt
//    Purpose:     �ֲ�ʽ��������
//    History:
*********************************************************************/
CAPIHelp_Distributed::CAPIHelp_Distributed()
{

}
CAPIHelp_Distributed::~CAPIHelp_Distributed()
{

}
//////////////////////////////////////////////////////////////////////////
//                               ���к���
//////////////////////////////////////////////////////////////////////////
/********************************************************************
�������ƣ�APIHelp_Distributed_IsMode
�������ܣ��жϸ���ģʽ�Ƿ�Ϊָ��ģʽ
 ����.һ��pStl_ListMode
  In/Out��In
  ���ͣ�STL����ָ��
  �ɿգ�N
  ��˼������֧�ֵ�ģʽ�б�
 ����.����nMode
  In/Out��In
  ���ͣ�������
  �ɿգ�N
  ��˼������Ҫ�жϵ�ģʽ
����ֵ
  ���ͣ��߼���
  ��˼���Ƿ�ɹ�
��ע��
*********************************************************************/
BOOL CAPIHelp_Distributed::APIHelp_Distributed_IsMode(list<int>* pStl_ListMode, int nMode)
{
	APIHelp_IsErrorOccur = FALSE;

	BOOL bFound = FALSE;
	list<int>::const_iterator stl_ListIterator = pStl_ListMode->begin();
	for (; stl_ListIterator != pStl_ListMode->end(); stl_ListIterator++)
	{
		if (nMode == *stl_ListIterator)
		{
			bFound = TRUE;
			break;
		}
	}
	return bFound;
}
/********************************************************************
�������ƣ�APIHelp_Distributed_RandomAddr
�������ܣ����ѡ��һ�����ص��ض����������ַ
 ����.һ��pStl_ListAddr
  In/Out��In
  ���ͣ�STL����ָ��
  �ɿգ�N
  ��˼�����븺�ط������б�
 ����.����ptszAddr
  In/Out��Out
  ���ͣ��ַ�ָ��
  �ɿգ�N
  ��˼�������ȡ���ĸ��ص�ַ
����ֵ
  ���ͣ��߼���
  ��˼���Ƿ�ɹ�
��ע��
*********************************************************************/
BOOL CAPIHelp_Distributed::APIHelp_Distributed_RandomAddr(list<string>* pStl_ListAddr, TCHAR* ptszAddr)
{
	APIHelp_IsErrorOccur = FALSE;

	BOOL bFound = FALSE;
	XNETHANDLE xhToken = 0;

	BaseLib_OperatorHandle_Create(&xhToken, 0, pStl_ListAddr->size(), FALSE);
	if (xhToken == pStl_ListAddr->size())
	{
		xhToken--;
	}
	list<string>::const_iterator stl_ListIterator = pStl_ListAddr->begin();
	for (XNETHANDLE i = 0; stl_ListIterator != pStl_ListAddr->end(); stl_ListIterator++, i++)
	{
		if (xhToken == i)
		{
			bFound = TRUE;
			_tcscpy(ptszAddr, stl_ListIterator->c_str());
			break;
		}
	}
	return bFound;
}
/********************************************************************
�������ƣ�APIHelp_Distributed_FileList
�������ܣ��������н����������ݲ��Ҵ����ָ���ṹ
 ����.һ��pStl_ListParse
  In/Out��In
  ���ͣ�STL����ָ��
  �ɿգ�N
  ��˼������Ҫ�����������б�
 ����.����pppSt_ListPacket
  In/Out��Out
  ���ͣ�����ָ��
  �ɿգ�N
  ��˼������ļ���Ϣ�ṹ�б�
 ����.����pInt_ListCount
  In/Out��Out
  ���ͣ�������
  �ɿգ�N
  ��˼������ļ��б����
����ֵ
  ���ͣ��߼���
  ��˼���Ƿ�ɹ�
��ע��
*********************************************************************/
BOOL CAPIHelp_Distributed::APIHelp_Distributed_FileList(list<APIHELP_LBFILEINFO>* pStl_ListParse, XSTORAGECORE_DBFILE*** pppSt_ListPacket, int* pInt_ListCount)
{
	APIHelp_IsErrorOccur = FALSE;

	if ((NULL == pStl_ListParse) || (NULL == pppSt_ListPacket))
	{
		APIHelp_IsErrorOccur = TRUE;
		APIHelp_dwErrorCode = ERROR_STORAGE_MODULE_APIHELP_PARAMENT;
		return FALSE;
	}

	BaseLib_OperatorMemory_Malloc((XPPPMEM)pppSt_ListPacket, pStl_ListParse->size(), sizeof(XSTORAGECORE_DBFILE));
	list<APIHELP_LBFILEINFO>::const_iterator stl_ListIterator = pStl_ListParse->begin();
	for (int i = 0; stl_ListIterator != pStl_ListParse->end(); stl_ListIterator++, i++)
	{
		APIHelp_Distributed_FileListParse(stl_ListIterator->tszMsgBuffer, stl_ListIterator->nMsgLen, (*pppSt_ListPacket)[i]);
	}
	*pInt_ListCount = pStl_ListParse->size();
	return TRUE;
}
/********************************************************************
�������ƣ�APIHelp_Distributed_DLStorage
�������ܣ�ͨ��URLKEY�õ�һ����Ӧ���ص�ַ
 ����.һ��lpszMsgBuffer
  In/Out��In
  ���ͣ������ַ�ָ��
  �ɿգ�N
  ��˼������Ҫ������URL
 ����.����pStl_ListBucket
  In/Out��In
  ���ͣ�����ָ��
  �ɿգ�N
  ��˼������Ҫ�������б�
 ����.����pSt_StorageBucket
  In/Out��Out
  ���ͣ����ݽṹָ��
  �ɿգ�N
  ��˼�������ȡ���Ŀ��ô洢
����ֵ
  ���ͣ��߼���
  ��˼���Ƿ�ɹ�
��ע��
*********************************************************************/
BOOL CAPIHelp_Distributed::APIHelp_Distributed_DLStorage(LPCTSTR lpszMsgBuffer, list<XENGINE_STORAGEBUCKET>* pStl_ListBucket, XENGINE_STORAGEBUCKET* pSt_StorageBucket)
{
	APIHelp_IsErrorOccur = FALSE;

	if ((NULL == lpszMsgBuffer) || (NULL == pSt_StorageBucket))
	{
		APIHelp_IsErrorOccur = TRUE;
		APIHelp_dwErrorCode = ERROR_STORAGE_MODULE_APIHELP_PARAMENT;
		return FALSE;
	}
	BOOL bFound = FALSE;
	TCHAR tszKeyStr[128];
	memset(tszKeyStr, '\0', sizeof(tszKeyStr));
	//���key
	int i = 1;
	int nLen = _tcslen(lpszMsgBuffer);
	for (; i < nLen; i++)
	{
		if ('/' == lpszMsgBuffer[i])
		{
			bFound = TRUE;
			memcpy(tszKeyStr, lpszMsgBuffer + 1, i - 1);
			break;
		}
	}
	if (!bFound)
	{
		APIHelp_IsErrorOccur = TRUE;
		APIHelp_dwErrorCode = ERROR_STORAGE_MODULE_APIHELP_NOTFOUND;
		return FALSE;
	}
	bFound = FALSE;
	//��ö�Ӧ�洢
	for (auto stl_ListIterator = pStl_ListBucket->begin(); stl_ListIterator != pStl_ListBucket->end(); stl_ListIterator++)
	{
		if (0 == _tcsncmp(tszKeyStr, stl_ListIterator->tszBuckKey, _tcslen(tszKeyStr)))
		{
			bFound = TRUE;
			*pSt_StorageBucket = *stl_ListIterator;
			break;
		}
	}
	if (!bFound)
	{
		APIHelp_IsErrorOccur = TRUE;
		APIHelp_dwErrorCode = ERROR_STORAGE_MODULE_APIHELP_NOTFOUND;
		return FALSE;
	}
	_tcscpy(pSt_StorageBucket->tszFileName, lpszMsgBuffer + i);
	return TRUE;
}
/********************************************************************
�������ƣ�APIHelp_Distributed_UPStorage
�������ܣ�ͨ���ֲ�ʽ�洢�б���һ���洢��ַ
 ����.һ��pStl_ListBucket
  In/Out��In
  ���ͣ�����ָ��
  �ɿգ�N
  ��˼������Ҫ�������б�
 ����.����pSt_StorageBucket
  In/Out��Out
  ���ͣ����ݽṹָ��
  �ɿգ�N
  ��˼�������ȡ���Ŀ��ô洢
����ֵ
  ���ͣ��߼���
  ��˼���Ƿ�ɹ�
��ע��
*********************************************************************/
BOOL CAPIHelp_Distributed::APIHelp_Distributed_UPStorage(list<XENGINE_STORAGEBUCKET>* pStl_ListBucket, XENGINE_STORAGEBUCKET* pSt_StorageBucket)
{
	APIHelp_IsErrorOccur = FALSE;

	if ((NULL == pStl_ListBucket) || (NULL == pSt_StorageBucket))
	{
		APIHelp_IsErrorOccur = TRUE;
		APIHelp_dwErrorCode = ERROR_STORAGE_MODULE_APIHELP_PARAMENT;
		return FALSE;
	}
	BOOL bFound = FALSE;
	int nLastLevel = 9999;
	for (auto stl_ListIterator = pStl_ListBucket->begin(); stl_ListIterator != pStl_ListBucket->end(); stl_ListIterator++)
	{
		//ֻ�������õ�
		if (stl_ListIterator->bEnable)
		{
			//�������ȼ�
			if (stl_ListIterator->nLevel < nLastLevel)
			{
				int nListCount = 0;
				__int64u nDirCount = 0;   //��ǰĿ¼��С
				CHAR** ppListFile;
				SystemApi_File_EnumFile(stl_ListIterator->tszFilePath, &ppListFile, &nListCount, NULL, NULL, TRUE, 1);
				for (int j = 0; j < nListCount; j++)
				{
					struct __stat64 st_FStat;
					_stat64(ppListFile[j], &st_FStat);
					nDirCount += st_FStat.st_size;
				}
				BaseLib_OperatorMemory_Free((XPPPMEM)&ppListFile, nListCount);
				//�����ǰĿ¼��С�����趨�Ĵ�С.��ô����
				if (nDirCount >= APIHelp_Distributed_GetSize(stl_ListIterator->tszBuckSize))
				{
					continue;
				}
				bFound = TRUE;
				nLastLevel = stl_ListIterator->nLevel;
				*pSt_StorageBucket = *stl_ListIterator;
			}
		}
	}
	if (!bFound)
	{
		APIHelp_IsErrorOccur = TRUE;
		APIHelp_dwErrorCode = ERROR_STORAGE_MODULE_APIHELP_NOTFOUND;
		return FALSE;
	}
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////
//                               ��������
//////////////////////////////////////////////////////////////////////////
/********************************************************************
�������ƣ�APIHelp_Distributed_FileListParse
�������ܣ��ļ��б��������
 ����.һ��lpszMsgBuffer
  In/Out��In
  ���ͣ������ַ�ָ��
  �ɿգ�N
  ��˼������Ҫ������JSON
 ����.����nMsgLen
  In/Out��In
  ���ͣ�������
  �ɿգ�N
  ��˼������Ҫ�����Ĵ�С
 ����.����pSt_DBFile
  In/Out��Out
  ���ͣ����ݽṹָ��
  �ɿգ�N
  ��˼�������������ļ���Ϣ
����ֵ
  ���ͣ��߼���
  ��˼���Ƿ�ɹ�
��ע��
*********************************************************************/
BOOL CAPIHelp_Distributed::APIHelp_Distributed_FileListParse(LPCTSTR lpszMsgBuffer, int nMsgLen, XSTORAGECORE_DBFILE *pSt_DBFile)
{
	APIHelp_IsErrorOccur = FALSE;

	Json::Value st_JsonRoot;
	JSONCPP_STRING st_JsonError;
	Json::CharReaderBuilder st_JsonBuilder;

	std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_JsonBuilder.newCharReader());
	if (!pSt_JsonReader->parse(lpszMsgBuffer, lpszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
	{
		APIHelp_IsErrorOccur = TRUE;
		APIHelp_dwErrorCode = ERROR_STORAGE_MODULE_APIHELP_PARSE;
		return FALSE;
	}
	//�����ļ��б�,һ��ֻ��һ��
	int nCount = st_JsonRoot["Count"].asInt();
	Json::Value st_JsonArray = st_JsonRoot["List"];

	for (int i = 0; i < nCount; i++)
	{
		pSt_DBFile->st_ProtocolFile.nFileSize = st_JsonArray[i]["nFileSize"].asInt64();
		_tcscpy(pSt_DBFile->tszTableName, st_JsonArray[i]["tszTableName"].asCString());
		_tcscpy(pSt_DBFile->st_ProtocolFile.tszFileUser, st_JsonArray[i]["tszFileUser"].asCString());
		_tcscpy(pSt_DBFile->st_ProtocolFile.tszFileHash, st_JsonArray[i]["tszFileHash"].asCString());
		_tcscpy(pSt_DBFile->st_ProtocolFile.tszFileName, st_JsonArray[i]["tszFileName"].asCString());
		_tcscpy(pSt_DBFile->st_ProtocolFile.tszFilePath, st_JsonArray[i]["tszFilePath"].asCString());
		_tcscpy(pSt_DBFile->st_ProtocolFile.tszFileTime, st_JsonArray[i]["tszFileTime"].asCString());
		
		break;
	}
	return TRUE;
}
/********************************************************************
�������ƣ�APIHelp_Distributed_GetSize
�������ܣ���ȡ�洢���ô�С
 ����.һ��lpszMsgBuffer
  In/Out��In
  ���ͣ������ַ�ָ��
  �ɿգ�N
  ��˼������Ҫ��ȡ�Ļ�����
����ֵ
  ���ͣ�������
  ��˼����ȡ���Ĵ�С�ֽ�
��ע��
*********************************************************************/
__int64u CAPIHelp_Distributed::APIHelp_Distributed_GetSize(LPCTSTR lpszMsgBuffer)
{
	APIHelp_IsErrorOccur = FALSE;

	TCHAR tszSizeStr[64];
	TCHAR tszUnitStr[4];

	memset(tszSizeStr, '\0', sizeof(tszSizeStr));
	memset(tszUnitStr, '\0', sizeof(tszUnitStr));
	//�ֱ�õ����ֺ͵�λ
	memcpy(tszSizeStr, lpszMsgBuffer, _tcslen(lpszMsgBuffer) - 2);
	BaseLib_OperatorString_GetLastString(lpszMsgBuffer, 2, tszUnitStr);

	__int64u nllSize = _ttoi64(tszSizeStr);
	//�õ���λ��С
	if (0 == _tcsncmp(tszUnitStr, _T("KB"), 2))
	{
		nllSize = nllSize * 1024;
	}
	else if (0 == _tcsncmp(tszUnitStr, _T("MB"), 2))
	{
		nllSize = nllSize * 1024 * 1024;
	}
	else if (0 == _tcsncmp(tszUnitStr, _T("GB"), 2))
	{
		nllSize = nllSize * 1024 * 1024 * 1024;
	}

	return nllSize;
}