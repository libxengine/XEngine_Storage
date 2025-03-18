#pragma once
/********************************************************************
//    Created:     2023/02/02  16:25:58
//    File Name:   D:\XEngine\XEngine_SourceCode\XEngine_NetHelp\NetHelp_BTorrent\BTorrent_Parse\BTorrent_Parse.h
//    File Path:   D:\XEngine\XEngine_SourceCode\XEngine_NetHelp\NetHelp_BTorrent\BTorrent_Parse
//    File Base:   BTorrent_Parse
//    File Ext:    h
//    Project:     XEngine
//    Author:      qyt
//    Purpose:     种子解析工具
//    History:
*********************************************************************/
typedef struct  
{
#if 1 == _XENGIEN_STORAGE_BUILDSWITCH_BTORRENT
	lt::bdecode_node m_BTBNode;
	shared_ptr<lt::torrent_info> m_BTInfo;
#endif
}BTORRENT_PARSEINFO;

class CBTorrent_Parse
{
public:
	CBTorrent_Parse();
	~CBTorrent_Parse();
public:
	XHANDLE BTorrent_Parse_Init(LPCXSTR lpszAddr);
	bool BTorrent_Parse_GetNode(XHANDLE xhToken, BTORRENT_PARSEMAP*** pppSt_Parse, int* pInt_ListCount);
	bool BTorrent_Parse_GetTracker(XHANDLE xhToken, BTORRENT_PARSEMAP*** pppSt_Parse, int* pInt_ListCount);
	bool BTorrent_Parse_GetSeeds(XHANDLE xhToken, BTORRENT_PARSEMAP*** pppSt_Parse, int* pInt_ListCount);
	bool BTorrent_Parse_GetPiece(XHANDLE xhToken, int* pInt_PieceLen, int* pInt_PieceCount);
	bool BTorrent_Parse_GetInfo(XHANDLE xhToken, XCHAR* ptszHash = NULL, XCHAR* ptszCreator = NULL, XCHAR* ptszComment = NULL);
	bool BTorrent_Parse_GetMagnet(XHANDLE xhToken, XCHAR* ptszMagnet);
	bool BTorrent_Parse_GetFile(XHANDLE xhToken, XCHAR* ptszFilePath, BTORRENT_FILEINFO*** pppSt_FileList, int* pInt_ListCount);
	bool BTorrent_Parse_Destory(XHANDLE xhToken);
protected:
private:
};