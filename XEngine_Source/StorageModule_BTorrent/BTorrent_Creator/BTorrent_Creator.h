#pragma once
/********************************************************************
//    Created:     2023/02/02  16:25:43
//    File Name:   D:\XEngine\XEngine_SourceCode\XEngine_NetHelp\NetHelp_BTorrent\BTorrent_Creator\BTorrent_Creator.h
//    File Path:   D:\XEngine\XEngine_SourceCode\XEngine_NetHelp\NetHelp_BTorrent\BTorrent_Creator
//    File Base:   BTorrent_Creator
//    File Ext:    h
//    Project:     XEngine
//    Author:      qyt
//    Purpose:     BT创建器
//    History:
*********************************************************************/
typedef struct  
{
	XCHAR tszComment[4096];
	XCHAR tszPEMCert[4096];
	XCHAR tszBTPath[XPATH_MAX];
	XCHAR tszCreate[XPATH_MAX];
	int m_nPieceSize;

	list<BTORRENT_PARSEMAP>* pStl_ListNode;
	list<BTORRENT_PARSEMAP>* pStl_ListTracker;
	list<BTORRENT_PARSEMAP>* pStl_ListSeeds;
}BTORRENT_CREATORINFO;

class CBTorrent_Creator
{
public:
	CBTorrent_Creator();
	~CBTorrent_Creator();
public:
	XHANDLE BTorrent_Creator_Init(LPCXSTR lpszBTPath, int nPieceSize = 0);
	bool BTorrent_Creator_AddNode(XHANDLE xhToken, LPCXSTR lpszAddr, int nIndex);
	bool BTorrent_Creator_AddTracker(XHANDLE xhToken, LPCXSTR lpszAddr, int nIndex = 0);
	bool BTorrent_Creator_AddSeeds(XHANDLE xhToken, LPCXSTR lpszAddr, bool bSingle = false);
	bool BTorrent_Creator_SetInfo(XHANDLE xhToken, LPCXSTR lpszCreator, LPCXSTR lpszComment = NULL, LPCXSTR lpszCertBuffer = NULL);
	bool BTorrent_Creator_GetTorrent(XHANDLE xhToken, LPCXSTR lpszBTFile = NULL, XCHAR* ptszMsgBuffer = NULL, int* pInt_MsgLen = NULL);
	bool BTorrent_Creator_Destory(XHANDLE xhToken);
protected:
private:
};