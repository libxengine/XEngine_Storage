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