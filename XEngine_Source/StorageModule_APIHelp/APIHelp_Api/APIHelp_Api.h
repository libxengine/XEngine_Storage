#pragma once
/********************************************************************
//    Created:     2022/01/07  14:36:50
//    File Name:   D:\XEngine_Storage\XEngine_Source\StorageModule_APIHelp\APIHelp_Api\APIHelp_Api.h
//    File Path:   D:\XEngine_Storage\XEngine_Source\StorageModule_APIHelp\APIHelp_Api
//    File Base:   APIHelp_Api
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     帮助函数
//    History:
*********************************************************************/


class CAPIHelp_Api
{
public:
	CAPIHelp_Api();
	~CAPIHelp_Api();
public:
	bool APIHelp_Api_ProxyAuth(XCHAR* ptszUser, XCHAR* ptszPass, XCHAR** pptszListHdr, int nHdrCount);
	bool APIHelp_Api_RangeFile(int* pInt_SPos, int* pInt_EPos, __int64x* pInt_Count, XCHAR** pptszListHdr, int nHdrCount);
	bool APIHelp_Api_VerHash(LPCXSTR lpszFileHash, XCHAR** pptszListHdr, int nHdrCount);
	bool APIHelp_Api_GetIPInfo(LPCXSTR lpszMsgBuffer, int nMsgLen, XENGINE_IPADDRINFO* pSt_IPAddrInfo);
	bool APIHelp_Api_UrlParse(XCHAR*** ppptszList, int nListCount, XCHAR* ptszFileName, XCHAR* ptszKeyName);
	bool APIHelp_Api_Boundary(XCHAR*** ppptszList, int nListCount, XCHAR* ptszBoundStr);
	bool APIHelp_Api_GetDIRSize(LPCXSTR lpszDIRStr, __int64u* pInt_DIRSize);
protected:
private:
};