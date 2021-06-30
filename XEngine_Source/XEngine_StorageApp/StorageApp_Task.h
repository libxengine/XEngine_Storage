#pragma once

BOOL XEngine_Task_ProxyAuth(LPCTSTR lpszClientAddr, LPCTSTR lpszPostUrl, TCHAR** pptszListHdr, int nHdrCount, int nSDType = STORAGE_NETTYPE_HTTPDOWNLOAD);
BOOL XEngine_Task_RangeFile(LPCTSTR lpszClientAddr, int* pInt_SPos, int* pInt_EPos, TCHAR** pptszListHdr, int nHdrCount, int nSDType = STORAGE_NETTYPE_HTTPDOWNLOAD);
BOOL XEngine_Task_VerHash(LPCTSTR lpszClientAddr, LPCTSTR lpszFileName, LPCTSTR lpszFileHash, TCHAR** pptszListHdr, int nHdrCount);