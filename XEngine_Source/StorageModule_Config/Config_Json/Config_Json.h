#pragma once
/********************************************************************
//    Created:     2021/06/02  15:00:50
//    File Name:   D:\XEngine_Storage\StorageModule_Config\Config_Json\Config_Json.h
//    File Path:   D:\XEngine_Storage\StorageModule_Config\Config_Json
//    File Base:   Config_Json
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     文件配置信息操作
//    History:
*********************************************************************/

class CConfig_Json
{
public:
	CConfig_Json();
	~CConfig_Json();
public:
	BOOL Config_Json_File(LPCTSTR lpszConfigFile, XENGINE_SERVERCONFIG* pSt_ServerConfig);
	BOOL Config_Json_LoadBalance(LPCTSTR lpszConfigFile, XENGINE_LBCONFIG* pSt_ServerConfig);
};
