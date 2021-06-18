#include "pch.h"
#include "Config_Json.h"
/********************************************************************
//    Created:     2021/06/02  15:00:45
//    File Name:   D:\XEngine_Storage\StorageModule_Config\Config_Json\Config_Json.cpp
//    File Path:   D:\XEngine_Storage\StorageModule_Config\Config_Json
//    File Base:   Config_Json
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     文件配置信息操作
//    History:
*********************************************************************/
CConfig_Json::CConfig_Json()
{

}
CConfig_Json::~CConfig_Json()
{

}
//////////////////////////////////////////////////////////////////////////
//                        公用函数
//////////////////////////////////////////////////////////////////////////
BOOL CConfig_Json::Config_Json_File(LPCTSTR lpszConfigFile,XENGINE_SERVERCONFIG *pSt_ServerConfig)
{
    Config_IsErrorOccur = FALSE;

    if ((NULL == lpszConfigFile) || (NULL == pSt_ServerConfig))
    {
        Config_IsErrorOccur = TRUE;
        Config_dwErrorCode = ERROR_XENGINE_BLOGIC_CONFIG_JSON_PARAMENT;
        return FALSE;
    }
    JSONCPP_STRING st_JsonError;
    Json::Value st_JsonRoot;
    Json::CharReaderBuilder st_JsonBuilder;

    FILE* pSt_File = _tfopen(lpszConfigFile, _T("rb"));
    if (NULL == pSt_File)
    {
        Config_IsErrorOccur = TRUE;
        Config_dwErrorCode = ERROR_XENGINE_BLOGIC_CONFIG_JSON_PARAMENT;
        return FALSE;
    }
    int nCount = 0;
    TCHAR tszMsgBuffer[4096];
    while (1)
    {
        int nRet = fread(tszMsgBuffer + nCount, 1, 2048, pSt_File);
        if (nRet <= 0)
        {
            break;
        }
        nCount += nRet;
    }
    fclose(pSt_File);

    std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_JsonBuilder.newCharReader());
    if (!pSt_JsonReader->parse(tszMsgBuffer, tszMsgBuffer + nCount, &st_JsonRoot, &st_JsonError))
    {
        Config_IsErrorOccur = TRUE;
        Config_dwErrorCode = ERROR_XENGINE_BLOGIC_CONFIG_JSON_PARSE;
        return FALSE;
    }
    _tcscpy(pSt_ServerConfig->tszIPAddr,st_JsonRoot["tszIPAddr"].asCString());
    pSt_ServerConfig->bDeamon = st_JsonRoot["bDeamon"].asInt();
    pSt_ServerConfig->nCenterPort = st_JsonRoot["nCenterPort"].asInt();
    pSt_ServerConfig->nStorageDLPort = st_JsonRoot["nStorageDLPort"].asInt();
    pSt_ServerConfig->nStorageUPPort = st_JsonRoot["nStorageUPPort"].asInt();

    if (st_JsonRoot["XMax"].empty() || (6 != st_JsonRoot["XMax"].size()))
    {
        Config_IsErrorOccur = TRUE;
        Config_dwErrorCode = ERROR_XENGINE_BLOGIC_CONFIG_JSON_XMAX;
        return FALSE;
    }
    Json::Value st_JsonXMax = st_JsonRoot["XMax"];
    pSt_ServerConfig->st_XMax.nMaxClient = st_JsonXMax["MaxClient"].asInt();
    pSt_ServerConfig->st_XMax.nMaxQueue = st_JsonXMax["MaxQueue"].asInt();
    pSt_ServerConfig->st_XMax.nIOThread = st_JsonXMax["IOThread"].asInt();
    pSt_ServerConfig->st_XMax.nCenterThread = st_JsonXMax["CenterThread"].asInt();
    pSt_ServerConfig->st_XMax.nStorageUPThread = st_JsonXMax["nStorageUPThread"].asInt();
    pSt_ServerConfig->st_XMax.nStorageDLThread = st_JsonXMax["nStorageDLThread"].asInt();

    if (st_JsonRoot["XTime"].empty() || (5 != st_JsonRoot["XTime"].size()))
    {
        Config_IsErrorOccur = TRUE;
        Config_dwErrorCode = ERROR_XENGINE_BLOGIC_CONFIG_JSON_XTIME;
        return FALSE;
    }
    Json::Value st_JsonXTime = st_JsonRoot["XTime"];
    pSt_ServerConfig->st_XTime.bHBTime = st_JsonXTime["bHBTime"].asInt();
    pSt_ServerConfig->st_XTime.nDBMonth = st_JsonXTime["nDBMonth"].asInt();
    pSt_ServerConfig->st_XTime.nTimeCheck = st_JsonXTime["nTimeCheck"].asInt();
    pSt_ServerConfig->st_XTime.nCenterTimeOut = st_JsonXTime["nCenterTimeOut"].asInt();
    pSt_ServerConfig->st_XTime.nStorageTimeOut = st_JsonXTime["nStorageTimeOut"].asInt();
    
    if (st_JsonRoot["XLog"].empty() || (3 != st_JsonRoot["XLog"].size()))
    {
        Config_IsErrorOccur = TRUE;
        Config_dwErrorCode = ERROR_XENGINE_BLOGIC_CONFIG_JSON_XLOG;
        return FALSE;
    }
    Json::Value st_JsonXLog = st_JsonRoot["XLog"];
    pSt_ServerConfig->st_XLog.nMaxSize = st_JsonXLog["MaxSize"].asInt();
    pSt_ServerConfig->st_XLog.nMaxCount = st_JsonXLog["MaxCount"].asInt();
    pSt_ServerConfig->st_XLog.nLogLeave = st_JsonXLog["LogLeave"].asInt();

    if (st_JsonRoot["XSql"].empty() || (4 != st_JsonRoot["XSql"].size()))
    {
        Config_IsErrorOccur = TRUE;
        Config_dwErrorCode = ERROR_XENGINE_BLOGIC_CONFIG_JSON_XSQL;
        return FALSE;
    }
    Json::Value st_JsonXSql = st_JsonRoot["XSql"];
    pSt_ServerConfig->st_XSql.nSQLPort = st_JsonXSql["SQLPort"].asInt();
    _tcscpy(pSt_ServerConfig->st_XSql.tszSQLAddr,st_JsonXSql["SQLAddr"].asCString());
    _tcscpy(pSt_ServerConfig->st_XSql.tszSQLUser,st_JsonXSql["SQLUser"].asCString());
    _tcscpy(pSt_ServerConfig->st_XSql.tszSQLPass,st_JsonXSql["SQLPass"].asCString());

    if (st_JsonRoot["XStorage"].empty() || (6 != st_JsonRoot["XStorage"].size()))
    {
        Config_IsErrorOccur = TRUE;
        Config_dwErrorCode = ERROR_XENGINE_BLOGIC_CONFIG_JSON_XSTORAGE;
        return FALSE;
    }
    Json::Value st_JsonXStorage = st_JsonRoot["XStorage"];
	pSt_ServerConfig->st_XStorage.nUseMode = st_JsonXStorage["nUseMode"].asInt();
    pSt_ServerConfig->st_XStorage.nHashMode = st_JsonXStorage["nHashMode"].asInt();
    pSt_ServerConfig->st_XStorage.bRename = st_JsonXStorage["bRename"].asInt();
	_tcscpy(pSt_ServerConfig->st_XStorage.tszHttpAddr, st_JsonXStorage["tszHttpAddr"].asCString());
	_tcscpy(pSt_ServerConfig->st_XStorage.tszNginAddr, st_JsonXStorage["tszNginAddr"].asCString());
    _tcscpy(pSt_ServerConfig->st_XStorage.tszFileDir, st_JsonXStorage["tszFileDir"].asCString());

	if (st_JsonRoot["XLimit"].empty() || (2 != st_JsonRoot["XLimit"].size()))
	{
		Config_IsErrorOccur = TRUE;
		Config_dwErrorCode = ERROR_XENGINE_BLOGIC_CONFIG_JSON_XSTORAGE;
		return FALSE;
	}
	Json::Value st_JsonXLimit = st_JsonRoot["XLimit"];
	pSt_ServerConfig->st_XLimit.nMaxDNLoader = st_JsonXLimit["nMaxDNLoad"].asInt64();
    pSt_ServerConfig->st_XLimit.nMaxUPLoader = st_JsonXLimit["nMaxUPLoad"].asInt64();

	if (st_JsonRoot["XVer"].empty() || (1 != st_JsonRoot["XVer"].size()))
	{
		Config_IsErrorOccur = TRUE;
		Config_dwErrorCode = ERROR_XENGINE_BLOGIC_CONFIG_JSON_XVER;
		return FALSE;
	}
	Json::Value st_JsonXVer = st_JsonRoot["XVer"];
    
    pSt_ServerConfig->st_XVer.pStl_ListStorage = new list<tstring>;

    for (unsigned int i = 0; i < st_JsonXVer["StorageVersion"].size(); i++)
    {
        pSt_ServerConfig->st_XVer.pStl_ListStorage->push_back(st_JsonXVer["StorageVersion"][i].asCString());
    }
    return TRUE;
}
