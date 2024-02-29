#include "pch.h"
#include "APIHelp_Distributed/APIHelp_Distributed.h"
#include "APIHelp_Api/APIHelp_Api.h"
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
bool APIHelp_IsErrorOccur = false;
XLONG APIHelp_dwErrorCode = 0;
//////////////////////////////////////////////////////////////////////////
CAPIHelp_Distributed m_APIDistributed;
CAPIHelp_Api m_APIHelp;
//////////////////////////////////////////////////////////////////////////
//                       导出函数
//////////////////////////////////////////////////////////////////////////
extern "C" XLONG StorageHelp_GetLastError(int* pInt_SysError)
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
extern "C" bool APIHelp_Distributed_RandomAddr(list<string>*pStl_ListAddr, XCHAR * ptszAddr, int nMode)
{
	return m_APIDistributed.APIHelp_Distributed_RandomAddr(pStl_ListAddr, ptszAddr, nMode);
}
extern "C" bool APIHelp_Distributed_FileList(list<APIHELP_LBFILEINFO>*pStl_ListParse, XSTORAGECORE_DBFILE * **pppSt_ListPacket, int* pInt_ListCount)
{
	return m_APIDistributed.APIHelp_Distributed_FileList(pStl_ListParse, pppSt_ListPacket, pInt_ListCount);
}
extern "C" bool APIHelp_Distributed_DLStorage(LPCXSTR lpszMsgBuffer, list<XENGINE_STORAGEBUCKET>*pStl_ListBucket, XENGINE_STORAGEBUCKET * pSt_StorageBucket)
{
	return m_APIDistributed.APIHelp_Distributed_DLStorage(lpszMsgBuffer, pStl_ListBucket, pSt_StorageBucket);
}
extern "C" bool APIHelp_Distributed_UPStorage(list<XENGINE_STORAGEBUCKET>*pStl_ListBucket, XENGINE_STORAGEBUCKET * pSt_StorageBucket, int nMode)
{
	return m_APIDistributed.APIHelp_Distributed_UPStorage(pStl_ListBucket, pSt_StorageBucket, nMode);
}
extern "C" bool APIHelp_Distributed_GetPathKey(list<XENGINE_STORAGEBUCKET>*pStl_ListBucket, LPCXSTR lpszBuckKey, XCHAR * ptszFilePath)
{
	return m_APIDistributed.APIHelp_Distributed_GetPathKey(pStl_ListBucket, lpszBuckKey, ptszFilePath);
}
/************************************************************************/
/*                       帮助函数                                       */
/************************************************************************/
extern "C" bool APIHelp_Api_ProxyAuth(XCHAR * ptszUser, XCHAR * ptszPass, XCHAR * *pptszListHdr, int nHdrCount)
{
	return m_APIHelp.APIHelp_Api_ProxyAuth(ptszUser, ptszPass, pptszListHdr, nHdrCount);
}
extern "C" bool APIHelp_Api_RangeFile(int* pInt_SPos, int* pInt_EPos, __int64x * pInt_Count, XCHAR * *pptszListHdr, int nHdrCount)
{
	return m_APIHelp.APIHelp_Api_RangeFile(pInt_SPos, pInt_EPos, pInt_Count, pptszListHdr, nHdrCount);
}
extern "C" bool APIHelp_Api_VerHash(LPCXSTR lpszFileHash, XCHAR * *pptszListHdr, int nHdrCount)
{
	return m_APIHelp.APIHelp_Api_VerHash(lpszFileHash, pptszListHdr, nHdrCount);
}
extern "C" bool APIHelp_Api_GetIPInfo(LPCXSTR lpszMsgBuffer, int nMsgLen, XENGINE_IPADDRINFO * pSt_IPAddrInfo)
{
	return m_APIHelp.APIHelp_Api_GetIPInfo(lpszMsgBuffer, nMsgLen, pSt_IPAddrInfo);
}
extern "C" bool APIHelp_Api_UrlParse(XCHAR * **ppptszList, int nListCount, XCHAR * ptszFileName, XCHAR * ptszKeyName)
{
	return m_APIHelp.APIHelp_Api_UrlParse(ppptszList, nListCount, ptszFileName, ptszKeyName);
}
extern "C" bool APIHelp_Api_Boundary(XCHAR * **ppptszList, int nListCount, XCHAR * ptszBoundStr)
{
	return m_APIHelp.APIHelp_Api_Boundary(ppptszList, nListCount, ptszBoundStr);
}
extern "C" bool APIHelp_Api_GetDIRSize(LPCXSTR lpszDIRStr, __int64u * pInt_DIRSize)
{
	return m_APIHelp.APIHelp_Api_GetDIRSize(lpszDIRStr, pInt_DIRSize);
}