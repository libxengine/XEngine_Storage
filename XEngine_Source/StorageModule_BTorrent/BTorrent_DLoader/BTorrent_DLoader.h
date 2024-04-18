#pragma once
/********************************************************************
//    Created:     2023/02/02  16:25:51
//    File Name:   D:\XEngine\XEngine_SourceCode\XEngine_NetHelp\NetHelp_BTorrent\BTorrent_DLoader\BTorrent_DLoader.h
//    File Path:   D:\XEngine\XEngine_SourceCode\XEngine_NetHelp\NetHelp_BTorrent\BTorrent_DLoader
//    File Base:   BTorrent_DLoader
//    File Ext:    h
//    Project:     XEngine
//    Author:      qyt
//    Purpose:     BT下载器
//    History:
*********************************************************************/
typedef struct
{
#if 1 == _XENGIEN_STORAGE_BUILDSWITCH_BTORRENT
	lt::settings_pack m_BTSetPack;
	lt::add_torrent_params m_BTParam;
	std::shared_ptr<lt::session> m_BTSession;
#endif
    BTORRENT_DLOADER st_DLStatus;                                         //下载状态

    XCHAR tszAddr[MAX_PATH];                                              //下载地址
    XCHAR tszFile[MAX_PATH];                                              //保存地址
	XCHAR tszTemp[MAX_PATH];                                              //临时文件
    bool bIsResume;                                                       //是否续传
    XPVOID lPClass;                                                       //指向类的指针
}BTORRENT_DLOADINFO, * LPBTORRENT_DLOADINFO;
//////////////////////////////////////////////////////////////////////////
class CBTorrent_DLoader
{
public:
    CBTorrent_DLoader();
    ~CBTorrent_DLoader();
public:
    bool BTorrent_DLoader_Create(XNETHANDLE* pxhToken, LPCXSTR lpszAddr, LPCXSTR lpszSavePath, LPCXSTR lpszTempFile = NULL);
    bool BTorrent_DLoader_Query(XNETHANDLE xhToken, ENUM_BTORRENT_EVENT_TYPE*** pppenEventList, int* pInt_ListCount);
    bool BTorrent_DLoader_GetStatus(XNETHANDLE xhToken, BTORRENT_DLOADER* pSt_DLStatus);
    bool BTorrent_DLoader_SaveResume(XNETHANDLE xhToken);
    bool BTorrent_DLoader_Close(XNETHANDLE xhToken);
public:
    bool BTorrent_DLoader_SetPause(XNETHANDLE xhToken, bool bPause);
    bool BTorrent_DLoader_UPNPEnable(XNETHANDLE xhToken, bool bEnable = false);
private:
	shared_mutex st_Locker;
private:
    unordered_map<XNETHANDLE, BTORRENT_DLOADINFO*> stl_MapDLoader;
};
