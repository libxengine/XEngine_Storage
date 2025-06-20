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

    XCHAR tszAddr[XPATH_MAX];                                              //下载地址
    XCHAR tszFile[XPATH_MAX];                                              //保存地址
	XCHAR tszTemp[XPATH_MAX];                                              //临时文件
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
    XHANDLE BTorrent_DLoader_Create(LPCXSTR lpszAddr, LPCXSTR lpszSavePath, LPCXSTR lpszTempFile = NULL);
    bool BTorrent_DLoader_Query(XHANDLE xhToken, ENUM_BTORRENT_EVENT_TYPE*** pppenEventList, int* pInt_ListCount);
    bool BTorrent_DLoader_GetStatus(XHANDLE xhToken, BTORRENT_DLOADER* pSt_DLStatus);
    bool BTorrent_DLoader_SaveResume(XHANDLE xhToken);
    bool BTorrent_DLoader_Close(XHANDLE xhToken);
public:
    bool BTorrent_DLoader_SetPause(XHANDLE xhToken, bool bPause);
    bool BTorrent_DLoader_UPNPEnable(XHANDLE xhToken, bool bEnable = false);
private:
};
