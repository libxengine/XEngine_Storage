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
	BOOL APIHelp_Api_ProxyAuth(TCHAR* ptszUser, TCHAR* ptszPass, TCHAR** pptszListHdr, int nHdrCount);
	BOOL APIHelp_Api_RangeFile(int* pInt_SPos, int* pInt_EPos, __int64x* pInt_Count, TCHAR** pptszListHdr, int nHdrCount);
	BOOL APIHelp_Api_VerHash(LPCTSTR lpszFileHash, TCHAR** pptszListHdr, int nHdrCount);
protected:
private:
};