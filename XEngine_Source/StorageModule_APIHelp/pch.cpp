#include "pch.h"
#include "APIHelp_Distributed/APIHelp_Distributed.h"
/********************************************************************
//    Created:     2021/07/08  16:40:04
//    File Name:   D:\XEngine_Storage\XEngine_Source\StorageModule_APIHelp\pch.cpp
//    File Path:   D:\XEngine_Storage\XEngine_Source\StorageModule_APIHelp
//    File Base:   pch
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     导出函数实现
//    History:
*********************************************************************/
BOOL APIHelp_IsErrorOccur = FALSE;
DWORD APIHelp_dwErrorCode = 0;
//////////////////////////////////////////////////////////////////////////
CAPIHelp_Distributed m_APIDistributed;
//////////////////////////////////////////////////////////////////////////
//                       导出函数
//////////////////////////////////////////////////////////////////////////
extern "C" DWORD StorageHelp_GetLastError(int* pInt_SysError)
{
	if (NULL != pInt_SysError)
	{
		*pInt_SysError = errno;
	}
	return APIHelp_dwErrorCode;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
extern "C" BOOL APIHelp_Distributed_IsMode(list<int>*pStl_ListMode, int nMode)
{
	return m_APIDistributed.APIHelp_Distributed_IsMode(pStl_ListMode, nMode);
}
extern "C" BOOL APIHelp_Distributed_RandomAddr(list<string>*pStl_ListAddr, TCHAR * ptszAddr)
{
	return m_APIDistributed.APIHelp_Distributed_RandomAddr(pStl_ListAddr, ptszAddr);
}
extern "C" BOOL APIHelp_Distributed_FileList(list<APIHELP_LBFILEINFO>*pStl_ListParse, XSTORAGECORE_DBFILE * **pppSt_ListPacket, int* pInt_ListCount)
{
	return m_APIDistributed.APIHelp_Distributed_FileList(pStl_ListParse, pppSt_ListPacket, pInt_ListCount);
}
extern "C" BOOL APIHelp_Distributed_DLStorage(LPCTSTR lpszMsgBuffer, list<XENGINE_STORAGEBUCKET>*pStl_ListBucket, XENGINE_STORAGEBUCKET * pSt_StorageBucket)
{
	return m_APIDistributed.APIHelp_Distributed_DLStorage(lpszMsgBuffer, pStl_ListBucket, pSt_StorageBucket);
}
extern "C" BOOL APIHelp_Distributed_UPStorage(list<XENGINE_STORAGEBUCKET>*pStl_ListBucket, XENGINE_STORAGEBUCKET * pSt_StorageBucket)
{
	return m_APIDistributed.APIHelp_Distributed_UPStorage(pStl_ListBucket, pSt_StorageBucket);
}