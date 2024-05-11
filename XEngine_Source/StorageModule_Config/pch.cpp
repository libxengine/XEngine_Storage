#include "pch.h"
#include "Config_Json/Config_Json.h"
/********************************************************************
//    Created:     2021/06/02  15:00:39
//    File Name:   D:\XEngine_Storage\StorageModule_Config\pch.cpp
//    File Path:   D:\XEngine_Storage\StorageModule_Config
//    File Base:   pch
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     导出函数实现
//    History:
*********************************************************************/
bool Config_IsErrorOccur = false;
XLONG Config_dwErrorCode = 0;
//////////////////////////////////////////////////////////////////////
CConfig_Json m_ConfigJson;
//////////////////////////////////////////////////////////////////////
//                        导出函数定义
//////////////////////////////////////////////////////////////////////
extern "C" XLONG Config_GetLastError(int* pInt_ErrorCode)
{
	if (pInt_ErrorCode != NULL)
	{
		*pInt_ErrorCode = errno;
	}
	return Config_dwErrorCode;
}
/************************************************************************/
/*                        配置文件读取                                  */
/************************************************************************/
extern "C" bool Config_Json_File(LPCXSTR lpszConfigFile, XENGINE_SERVERCONFIG * pSt_ServerConfig)
{
	return m_ConfigJson.Config_Json_File(lpszConfigFile, pSt_ServerConfig);
}
extern "C" bool Config_Json_Version(LPCXSTR lpszConfigFile, XENGINE_SERVERCONFIG * pSt_ServerConfig)
{
	return m_ConfigJson.Config_Json_Version(lpszConfigFile, pSt_ServerConfig);
}
extern "C" bool Config_Json_LoadBalance(LPCXSTR lpszConfigFile, XENGINE_LBCONFIG* pSt_ServerConfig)
{
	return m_ConfigJson.Config_Json_LoadBalance(lpszConfigFile, pSt_ServerConfig);
}