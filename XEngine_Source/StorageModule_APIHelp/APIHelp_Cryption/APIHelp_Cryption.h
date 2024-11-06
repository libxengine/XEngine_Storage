#pragma once
/********************************************************************
//    Created:     2024/10/29  11:43:31
//    File Name:   D:\XEngine_Storage\XEngine_Source\StorageModule_APIHelp\APIHelp_Cryption\APIHelp_Cryption.h
//    File Path:   D:\XEngine_Storage\XEngine_Source\StorageModule_APIHelp\APIHelp_Cryption
//    File Base:   APIHelp_Cryption
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     加解密帮助函数
//    History:
*********************************************************************/

class CAPIHelp_Cryption
{
public:
    CAPIHelp_Cryption();
    ~CAPIHelp_Cryption();
public:
    bool APIHelp_Cryption_BasicEncoder(LPCXSTR lpszUser, LPCXSTR lpszPass, XCHAR* ptszMsgBuffer, bool bADD = true);
	bool APIHelp_Cryption_BasicDecoder(LPCXSTR lpszMsgBuffer, XCHAR* ptszUser, XCHAR* ptszPass);
    bool APIHelp_Cryption_Digest(XCHAR* ptszResponseStr, LPCXSTR lpszUser, LPCXSTR lpszPass, LPCXSTR lpszRealm, LPCXSTR lpszMethod, LPCXSTR lpszUrl, LPCXSTR lpszNonce, LPCXSTR lpszCNonce, LPCXSTR lpszNC = NULL);
};
