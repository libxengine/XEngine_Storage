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
	pSt_ServerConfig->nP2XPPort = st_JsonRoot["nP2XPPort"].asInt();

    if (st_JsonRoot["XMax"].empty() || (7 != st_JsonRoot["XMax"].size()))
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
    pSt_ServerConfig->st_XMax.nP2XPThread = st_JsonXMax["nP2XPThread"].asInt();

    if (st_JsonRoot["XTime"].empty() || (6 != st_JsonRoot["XTime"].size()))
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
    pSt_ServerConfig->st_XTime.nP2XPTimeOut = st_JsonXTime["nP2XPTimeOut"].asInt();

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

    if (st_JsonRoot["XSql"].empty() || (6 != st_JsonRoot["XSql"].size()))
    {
        Config_IsErrorOccur = TRUE;
        Config_dwErrorCode = ERROR_XENGINE_BLOGIC_CONFIG_JSON_XSQL;
        return FALSE;
    }
    Json::Value st_JsonXSql = st_JsonRoot["XSql"];

    pSt_ServerConfig->st_XSql.nSQLType = st_JsonXSql["SQLType"].asInt();
    pSt_ServerConfig->st_XSql.nSQLPort = st_JsonXSql["SQLPort"].asInt();
	_tcscpy(pSt_ServerConfig->st_XSql.tszSQLAddr, st_JsonXSql["SQLAddr"].asCString());
	_tcscpy(pSt_ServerConfig->st_XSql.tszSQLUser, st_JsonXSql["SQLUser"].asCString());
	_tcscpy(pSt_ServerConfig->st_XSql.tszSQLPass, st_JsonXSql["SQLPass"].asCString());
	_tcscpy(pSt_ServerConfig->st_XSql.tszSQLFile, st_JsonXSql["SQLFile"].asCString());

    if (st_JsonRoot["XStorage"].empty() || (3 != st_JsonRoot["XStorage"].size()))
    {
        Config_IsErrorOccur = TRUE;
        Config_dwErrorCode = ERROR_XENGINE_BLOGIC_CONFIG_JSON_XSTORAGE;
        return FALSE;
    }
    Json::Value st_JsonXStorage = st_JsonRoot["XStorage"];
    pSt_ServerConfig->st_XStorage.nHashMode = st_JsonXStorage["nHashMode"].asInt();
    pSt_ServerConfig->st_XStorage.bRename = st_JsonXStorage["bRename"].asInt();
    _tcscpy(pSt_ServerConfig->st_XStorage.tszFileDir, st_JsonXStorage["tszFileDir"].asCString());

	if (st_JsonRoot["XProxy"].empty() || (2 != st_JsonRoot["XProxy"].size()))
	{
		Config_IsErrorOccur = TRUE;
		Config_dwErrorCode = ERROR_XENGINE_BLOGIC_CONFIG_JSON_XPROXY;
		return FALSE;
	}
    Json::Value st_JsonXProxy = st_JsonRoot["XProxy"];
    Json::Value st_JsonXProxyAuth = st_JsonXProxy["XProxyAuth"];
    Json::Value st_JsonXProxyPass = st_JsonXProxy["XProxyPass"];

    pSt_ServerConfig->st_XProxy.st_XProxyAuth.bAuth = st_JsonXProxyAuth["bAuth"].asInt();
    _tcscpy(pSt_ServerConfig->st_XProxy.st_XProxyAuth.tszAuthProxy, st_JsonXProxyAuth["tszAuthProxy"].asCString());
    _tcscpy(pSt_ServerConfig->st_XProxy.st_XProxyAuth.tszUserList, st_JsonXProxyAuth["tszUserList"].asCString());

    pSt_ServerConfig->st_XProxy.st_XProxyPass.bDLPass = st_JsonXProxyPass["bDLGet"].asInt();
    pSt_ServerConfig->st_XProxy.st_XProxyPass.bUPPass = st_JsonXProxyPass["bUPGet"].asInt();
    _tcscpy(pSt_ServerConfig->st_XProxy.st_XProxyPass.tszDLPass, st_JsonXProxyPass["tszDLPass"].asCString());
    _tcscpy(pSt_ServerConfig->st_XProxy.st_XProxyPass.tszUPPass, st_JsonXProxyPass["tszUPPass"].asCString());

	if (st_JsonRoot["XLimit"].empty() || (4 != st_JsonRoot["XLimit"].size()))
	{
		Config_IsErrorOccur = TRUE;
		Config_dwErrorCode = ERROR_XENGINE_BLOGIC_CONFIG_JSON_XSTORAGE;
		return FALSE;
	}
	Json::Value st_JsonXLimit = st_JsonRoot["XLimit"];
    pSt_ServerConfig->st_XLimit.nDLTry = st_JsonXLimit["nDLTry"].asUInt();
    pSt_ServerConfig->st_XLimit.nDLError = st_JsonXLimit["nDLError"].asUInt();
	pSt_ServerConfig->st_XLimit.nMaxDNLoader = st_JsonXLimit["nMaxDNLoad"].asInt64();
    pSt_ServerConfig->st_XLimit.nMaxUPLoader = st_JsonXLimit["nMaxUPLoad"].asInt64();

	if (st_JsonRoot["XP2xp"].empty() || (5 != st_JsonRoot["XP2xp"].size()))
	{
		Config_IsErrorOccur = TRUE;
		Config_dwErrorCode = ERROR_XENGINE_BLOGIC_CONFIG_JSON_P2XP;
		return FALSE;
	}
	Json::Value st_JsonP2xp = st_JsonRoot["XP2xp"];
    pSt_ServerConfig->st_P2xp.nMode = st_JsonP2xp["nMode"].asInt();
    pSt_ServerConfig->st_P2xp.nTime = st_JsonP2xp["nTime"].asInt();
    pSt_ServerConfig->st_P2xp.nRVPort = st_JsonP2xp["nRVPort"].asInt();
    pSt_ServerConfig->st_P2xp.nSDPort = st_JsonP2xp["nSDPort"].asInt();
    _tcscpy(pSt_ServerConfig->st_P2xp.tszQQWryFile, st_JsonP2xp["tszQQWryFile"].asCString());

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
BOOL CConfig_Json::Config_Json_LoadBalance(LPCTSTR lpszConfigFile, XENGINE_LBCONFIG* pSt_ServerConfig)
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
	_tcscpy(pSt_ServerConfig->tszIPAddr, st_JsonRoot["tszIPAddr"].asCString());
	pSt_ServerConfig->bDistributed = st_JsonRoot["bDistributed"].asInt();

	if (st_JsonRoot["LBConfig"].empty() || (1 != st_JsonRoot["LBConfig"].size()))
	{
		Config_IsErrorOccur = TRUE;
		Config_dwErrorCode = ERROR_XENGINE_BLOGIC_CONFIG_JSON_LBCONFIG;
		return FALSE;
	}
	Json::Value st_JsonLBConfig = st_JsonRoot["LBConfig"];
	pSt_ServerConfig->st_LBConfig.nServerMode = st_JsonLBConfig["nUseMode"].asInt();

	if (st_JsonRoot["LoadBalance"].empty() || (4 != st_JsonRoot["LoadBalance"].size()))
	{
		Config_IsErrorOccur = TRUE;
		Config_dwErrorCode = ERROR_XENGINE_BLOGIC_CONFIG_JSON_LAODBALANCE;
		return FALSE;
	}
	Json::Value st_JsonLoadBalance = st_JsonRoot["LoadBalance"];

	pSt_ServerConfig->st_LoadBalance.pStl_ListUseMode = new list<int>;
	for (unsigned int i = 0; i < st_JsonLoadBalance["nUseMode"].size(); i++)
	{
		pSt_ServerConfig->st_LoadBalance.pStl_ListUseMode->push_back(st_JsonLoadBalance["nUseMode"][i].asInt());
	}
	pSt_ServerConfig->st_LoadBalance.pStl_ListCenter = new list<tstring>;
	for (unsigned int i = 0; i < st_JsonLoadBalance["CenterAddr"].size(); i++)
	{
		pSt_ServerConfig->st_LoadBalance.pStl_ListCenter->push_back(st_JsonLoadBalance["CenterAddr"][i].asCString());
	}
	pSt_ServerConfig->st_LoadBalance.pStl_ListDownload = new list<tstring>;
	for (unsigned int i = 0; i < st_JsonLoadBalance["DownloadAddr"].size(); i++)
	{
		pSt_ServerConfig->st_LoadBalance.pStl_ListDownload->push_back(st_JsonLoadBalance["DownloadAddr"][i].asCString());
	}
	pSt_ServerConfig->st_LoadBalance.pStl_ListUPLoader = new list<tstring>;
	for (unsigned int i = 0; i < st_JsonLoadBalance["UPLoaderAddr"].size(); i++)
	{
		pSt_ServerConfig->st_LoadBalance.pStl_ListUPLoader->push_back(st_JsonLoadBalance["UPLoaderAddr"][i].asCString());
	}
	return TRUE;
}