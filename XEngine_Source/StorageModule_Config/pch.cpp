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
BOOL Config_IsErrorOccur = FALSE;
DWORD Config_dwErrorCode = 0;
//////////////////////////////////////////////////////////////////////
CConfig_Json m_ConfigJson;
//////////////////////////////////////////////////////////////////////
//                        导出函数定义
//////////////////////////////////////////////////////////////////////
extern "C" DWORD Config_GetLastError(int* pInt_ErrorCode)
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
extern "C" BOOL Config_Json_File(LPCTSTR lpszConfigFile, XENGINE_SERVERCONFIG * pSt_ServerConfig)
{
	return m_ConfigJson.Config_Json_File(lpszConfigFile, pSt_ServerConfig);
}
