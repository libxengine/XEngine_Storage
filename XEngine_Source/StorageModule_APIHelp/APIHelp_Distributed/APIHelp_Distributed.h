#pragma once
/********************************************************************
//    Created:     2021/07/08  15:24:30
//    File Name:   D:\XEngine_Storage\XEngine_Source\StorageModule_APIHelp\APIHelp_Distributed\APIHelp_Distributed.h
//    File Path:   D:\XEngine_Storage\XEngine_Source\StorageModule_APIHelp\APIHelp_Distributed
//    File Base:   APIHelp_Distributed
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     分布式帮助函数
//    History:
*********************************************************************/


class CAPIHelp_Distributed
{
public:
	CAPIHelp_Distributed();
	~CAPIHelp_Distributed();
public:
	bool APIHelp_Distributed_RandomAddr(list<string>* pStl_ListAddr, XCHAR* ptszAddr, int nMode);
	bool APIHelp_Distributed_FileList(list<APIHELP_LBFILEINFO>* pStl_ListParse, XSTORAGECORE_DBFILE*** pppSt_ListPacket, int* pInt_ListCount);
	bool APIHelp_Distributed_DLStorage(LPCXSTR lpszMsgBuffer, list<XENGINE_STORAGEBUCKET>* pStl_ListBucket, XENGINE_STORAGEBUCKET* pSt_StorageBucket);
	bool APIHelp_Distributed_CTStorage(LPCXSTR lpszMsgBuffer, list<XENGINE_STORAGEBUCKET>* pStl_ListBucket, XENGINE_STORAGEBUCKET* pSt_StorageBucket);
	bool APIHelp_Distributed_UPStorage(list<XENGINE_STORAGEBUCKET>* pStl_ListBucket, XENGINE_STORAGEBUCKET* pSt_StorageBucket, int nMode);
	bool APIHelp_Distributed_GetPathKey(list<XENGINE_STORAGEBUCKET>* pStl_ListBucket, LPCXSTR lpszBuckKey, XCHAR* ptszFilePath);
protected:
	bool APIHelp_Distributed_FileListParse(LPCXSTR lpszMsgBuffer, int nMsgLen, XSTORAGECORE_DBFILE* pSt_DBFile);
	__int64u APIHelp_Distributed_GetSize(LPCXSTR lpszMsgBuffer);
private:
};