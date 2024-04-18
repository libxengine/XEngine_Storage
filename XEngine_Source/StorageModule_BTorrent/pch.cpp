#include "pch.h"
#include "BTorrent_DLoader/BTorrent_DLoader.h"
#include "BTorrent_Parse/BTorrent_Parse.h"
#include "BTorrent_Creator/BTorrent_Creator.h"
/********************************************************************
//    Created:     2023/02/02  16:28:57
//    File Name:   D:\XEngine\XEngine_SourceCode\XEngine_NetHelp\NetHelp_BTorrent\pch.cpp
//    File Path:   D:\XEngine\XEngine_SourceCode\XEngine_NetHelp\NetHelp_BTorrent
//    File Base:   pch
//    File Ext:    cpp
//    Project:     XEngine
//    Author:      qyt
//    Purpose:     导出实现
//    History:
*********************************************************************/
bool BTDload_IsErrorOccur = false;
XLONG BTDload_dwErrorCode = 0;
//////////////////////////////////////////////////////////////////////////
CBTorrent_DLoader m_BTDLoader;
CBTorrent_Parse m_BTParse;
CBTorrent_Creator m_BTCreator;
//////////////////////////////////////////////////////////////////////////
//                          导出函数实现
//////////////////////////////////////////////////////////////////////////
extern "C" XLONG BTorrent_GetLastError(int* pInt_SysError)
{
	if (NULL != pInt_SysError)
	{
		*pInt_SysError = errno;
	}
	return BTDload_dwErrorCode;
}
/************************************************************************/
/*                          BT下载器导出函数                            */
/************************************************************************/
extern "C" bool BTorrent_DLoader_Create(XNETHANDLE * pxhToken, LPCXSTR lpszAddr, LPCXSTR lpszSavePath, LPCXSTR lpszTempFile)
{
	return m_BTDLoader.BTorrent_DLoader_Create(pxhToken, lpszAddr, lpszSavePath, lpszTempFile);
}
extern "C" bool BTorrent_DLoader_Query(XNETHANDLE xhToken, ENUM_BTORRENT_EVENT_TYPE * **pppenEventList, int* pInt_ListCount)
{
	return m_BTDLoader.BTorrent_DLoader_Query(xhToken, pppenEventList, pInt_ListCount);
}
extern "C" bool BTorrent_DLoader_GetStatus(XNETHANDLE xhToken, BTORRENT_DLOADER * pSt_DLStatus)
{
	return m_BTDLoader.BTorrent_DLoader_GetStatus(xhToken, pSt_DLStatus);
}
extern "C" bool BTorrent_DLoader_SaveResume(XNETHANDLE xhToken)
{
	return m_BTDLoader.BTorrent_DLoader_SaveResume(xhToken);
}
extern "C" bool BTorrent_DLoader_Close(XNETHANDLE xhToken)
{
	return m_BTDLoader.BTorrent_DLoader_Close(xhToken);
}
extern "C" bool BTorrent_DLoader_SetPause(XNETHANDLE xhToken, bool bPause)
{
	return m_BTDLoader.BTorrent_DLoader_SetPause(xhToken, bPause);
}
extern "C" bool BTorrent_DLoader_UPNPEnable(XNETHANDLE xhToken, bool bEnable)
{
	return m_BTDLoader.BTorrent_DLoader_UPNPEnable(xhToken, bEnable);
}
/************************************************************************/
/*                          BT解析器导出函数                            */
/************************************************************************/
extern "C" bool BTorrent_Parse_Init(XNETHANDLE * pxhToken, LPCXSTR lpszAddr)
{
	return m_BTParse.BTorrent_Parse_Init(pxhToken, lpszAddr);
}
extern "C" bool BTorrent_Parse_GetNode(XNETHANDLE xhToken, BTORRENT_PARSEMAP * **pppSt_Parse, int* pInt_ListCount)
{
	return m_BTParse.BTorrent_Parse_GetNode(xhToken, pppSt_Parse, pInt_ListCount);
}
extern "C" bool BTorrent_Parse_GetTracker(XNETHANDLE xhToken, BTORRENT_PARSEMAP * **pppSt_Parse, int* pInt_ListCount)
{
	return m_BTParse.BTorrent_Parse_GetTracker(xhToken, pppSt_Parse, pInt_ListCount);
}
extern "C" bool BTorrent_Parse_GetSeeds(XNETHANDLE xhToken, BTORRENT_PARSEMAP * **pppSt_Parse, int* pInt_ListCount)
{
	return m_BTParse.BTorrent_Parse_GetSeeds(xhToken, pppSt_Parse, pInt_ListCount);
}
extern "C" bool BTorrent_Parse_GetPiece(XNETHANDLE xhToken, int* pInt_PieceLen, int* pInt_PieceCount)
{
	return m_BTParse.BTorrent_Parse_GetPiece(xhToken, pInt_PieceLen, pInt_PieceCount);
}
extern "C" bool BTorrent_Parse_GetInfo(XNETHANDLE xhToken, XCHAR * ptszHash, XCHAR * ptszCreator, XCHAR * ptszComment)
{
	return m_BTParse.BTorrent_Parse_GetInfo(xhToken, ptszHash, ptszCreator, ptszComment);
}
extern "C" bool BTorrent_Parse_GetMagnet(XNETHANDLE xhToken, XCHAR * ptszMagnet)
{
	return m_BTParse.BTorrent_Parse_GetMagnet(xhToken, ptszMagnet);
}
extern "C" bool BTorrent_Parse_GetFile(XNETHANDLE xhToken, XCHAR * pszFilePath, BTORRENT_FILEINFO * **pppSt_FileList, int* pInt_ListCount)
{
	return m_BTParse.BTorrent_Parse_GetFile(xhToken, pszFilePath, pppSt_FileList, pInt_ListCount);
}
extern "C" bool BTorrent_Parse_Destory(XNETHANDLE xhToken)
{
	return m_BTParse.BTorrent_Parse_Destory(xhToken);
}
/************************************************************************/
/*                          BT种子制作导出函数                          */
/************************************************************************/
extern "C" bool BTorrent_Creator_Init(XNETHANDLE * pxhToken, LPCXSTR lpszBTPath, int nPieceSize)
{
	return m_BTCreator.BTorrent_Creator_Init(pxhToken, lpszBTPath, nPieceSize);
}
extern "C" bool BTorrent_Creator_AddNode(XNETHANDLE xhToken, LPCXSTR lpszAddr, int nIndex)
{
	return m_BTCreator.BTorrent_Creator_AddNode(xhToken, lpszAddr, nIndex);
}
extern "C" bool BTorrent_Creator_AddTracker(XNETHANDLE xhToken, LPCXSTR lpszAddr, int nIndex)
{
	return m_BTCreator.BTorrent_Creator_AddTracker(xhToken, lpszAddr, nIndex);
}
extern "C" bool BTorrent_Creator_AddSeeds(XNETHANDLE xhToken, LPCXSTR lpszAddr, bool bSingle)
{
	return m_BTCreator.BTorrent_Creator_AddSeeds(xhToken, lpszAddr, bSingle);
}
extern "C" bool BTorrent_Creator_SetInfo(XNETHANDLE xhToken, LPCXSTR lpszCreator, LPCXSTR lpszComment, LPCXSTR lpszCertBuffer)
{
	return m_BTCreator.BTorrent_Creator_SetInfo(xhToken, lpszCreator, lpszComment, lpszCertBuffer);
}
extern "C" bool BTorrent_Creator_GetTorrent(XNETHANDLE xhToken, LPCXSTR lpszBTFile, XCHAR * ptszMsgBuffer, int* pInt_MsgLen)
{
	return m_BTCreator.BTorrent_Creator_GetTorrent(xhToken, lpszBTFile, ptszMsgBuffer, pInt_MsgLen);
}
extern "C" bool BTorrent_Creator_Destory(XNETHANDLE xhToken)
{
	return m_BTCreator.BTorrent_Creator_Destory(xhToken);
}