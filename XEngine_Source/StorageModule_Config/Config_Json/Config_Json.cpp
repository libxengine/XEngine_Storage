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
/********************************************************************
函数名称：Config_Json_File
函数功能：读取基本配置文件
 参数.一：lpszConfigFile
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入配置文件
 参数.二：pSt_ServerConfig
  In/Out：Out
  类型：数据结构指针
  可空：N
  意思：输出读取到的信息
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CConfig_Json::Config_Json_File(LPCXSTR lpszConfigFile, XENGINE_SERVERCONFIG* pSt_ServerConfig)
{
	Config_IsErrorOccur = false;

	if ((NULL == lpszConfigFile) || (NULL == pSt_ServerConfig))
	{
		Config_IsErrorOccur = true;
		Config_dwErrorCode = ERROR_XENGINE_BLOGIC_CONFIG_JSON_PARAMENT;
		return false;
	}
	JSONCPP_STRING st_JsonError;
	Json::Value st_JsonRoot;
	Json::CharReaderBuilder st_JsonBuilder;

	FILE* pSt_File = _xtfopen(lpszConfigFile, _X("rb"));
	if (NULL == pSt_File)
	{
		Config_IsErrorOccur = true;
		Config_dwErrorCode = ERROR_XENGINE_BLOGIC_CONFIG_JSON_PARAMENT;
		return false;
	}
	int nCount = 0;
	XCHAR tszMsgBuffer[4096];
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
		Config_IsErrorOccur = true;
		Config_dwErrorCode = ERROR_XENGINE_BLOGIC_CONFIG_JSON_PARSE;
		return false;
	}
	_tcsxcpy(pSt_ServerConfig->tszIPAddr, st_JsonRoot["tszIPAddr"].asCString());
	pSt_ServerConfig->bDeamon = st_JsonRoot["bDeamon"].asInt();
	pSt_ServerConfig->nCenterPort = st_JsonRoot["nCenterPort"].asInt();
	pSt_ServerConfig->nWebdavPort = st_JsonRoot["nWebdavPort"].asInt();
	pSt_ServerConfig->nStorageDLPort = st_JsonRoot["nStorageDLPort"].asInt();
	pSt_ServerConfig->nStorageUPPort = st_JsonRoot["nStorageUPPort"].asInt();

	if (st_JsonRoot["XMax"].empty() || (7 != st_JsonRoot["XMax"].size()))
	{
		Config_IsErrorOccur = true;
		Config_dwErrorCode = ERROR_XENGINE_BLOGIC_CONFIG_JSON_XMAX;
		return false;
	}
	Json::Value st_JsonXMax = st_JsonRoot["XMax"];
	pSt_ServerConfig->st_XMax.nMaxClient = st_JsonXMax["MaxClient"].asInt();
	pSt_ServerConfig->st_XMax.nMaxQueue = st_JsonXMax["MaxQueue"].asInt();
	pSt_ServerConfig->st_XMax.nIOThread = st_JsonXMax["IOThread"].asInt();
	pSt_ServerConfig->st_XMax.nCenterThread = st_JsonXMax["CenterThread"].asInt();
	pSt_ServerConfig->st_XMax.nStorageUPThread = st_JsonXMax["nStorageUPThread"].asInt();
	pSt_ServerConfig->st_XMax.nStorageDLThread = st_JsonXMax["nStorageDLThread"].asInt();
	pSt_ServerConfig->st_XMax.nWebdavThread = st_JsonXMax["nWebdavThread"].asInt();

	if (st_JsonRoot["XTime"].empty() || (6 != st_JsonRoot["XTime"].size()))
	{
		Config_IsErrorOccur = true;
		Config_dwErrorCode = ERROR_XENGINE_BLOGIC_CONFIG_JSON_XTIME;
		return false;
	}
	Json::Value st_JsonXTime = st_JsonRoot["XTime"];
	pSt_ServerConfig->st_XTime.bHBTime = st_JsonXTime["bHBTime"].asInt();
	pSt_ServerConfig->st_XTime.nDBMonth = st_JsonXTime["nDBMonth"].asInt();
	pSt_ServerConfig->st_XTime.nTimeCheck = st_JsonXTime["nTimeCheck"].asInt();
	pSt_ServerConfig->st_XTime.nCenterTimeOut = st_JsonXTime["nCenterTimeOut"].asInt();
	pSt_ServerConfig->st_XTime.nStorageTimeOut = st_JsonXTime["nStorageTimeOut"].asInt();
	pSt_ServerConfig->st_XTime.nWebdavTimeOut = st_JsonXTime["nWebdavTimeOut"].asInt();

	if (st_JsonRoot["XLog"].empty() || (3 != st_JsonRoot["XLog"].size()))
	{
		Config_IsErrorOccur = true;
		Config_dwErrorCode = ERROR_XENGINE_BLOGIC_CONFIG_JSON_XLOG;
		return false;
	}
	Json::Value st_JsonXLog = st_JsonRoot["XLog"];
	pSt_ServerConfig->st_XLog.nMaxSize = st_JsonXLog["MaxSize"].asInt();
	pSt_ServerConfig->st_XLog.nMaxCount = st_JsonXLog["MaxCount"].asInt();
	pSt_ServerConfig->st_XLog.nLogLeave = st_JsonXLog["LogLeave"].asInt();

	if (st_JsonRoot["XSql"].empty() || (5 != st_JsonRoot["XSql"].size()))
	{
		Config_IsErrorOccur = true;
		Config_dwErrorCode = ERROR_XENGINE_BLOGIC_CONFIG_JSON_XSQL;
		return false;
	}
	Json::Value st_JsonXSql = st_JsonRoot["XSql"];

	pSt_ServerConfig->st_XSql.bEnable = st_JsonXSql["SQLEnable"].asBool();
	pSt_ServerConfig->st_XSql.nSQLPort = st_JsonXSql["SQLPort"].asInt();
	_tcsxcpy(pSt_ServerConfig->st_XSql.tszSQLAddr, st_JsonXSql["SQLAddr"].asCString());
	_tcsxcpy(pSt_ServerConfig->st_XSql.tszSQLUser, st_JsonXSql["SQLUser"].asCString());
	_tcsxcpy(pSt_ServerConfig->st_XSql.tszSQLPass, st_JsonXSql["SQLPass"].asCString());

	if (st_JsonRoot["XStorage"].empty() || (3 != st_JsonRoot["XStorage"].size()))
	{
		Config_IsErrorOccur = true;
		Config_dwErrorCode = ERROR_XENGINE_BLOGIC_CONFIG_JSON_XSTORAGE;
		return false;
	}
	Json::Value st_JsonXStorage = st_JsonRoot["XStorage"];
	pSt_ServerConfig->st_XStorage.nHashMode = st_JsonXStorage["nHashMode"].asInt();
	pSt_ServerConfig->st_XStorage.bUPHash = st_JsonXStorage["bUPHash"].asBool();
	pSt_ServerConfig->st_XStorage.bResumable = st_JsonXStorage["bResumable"].asBool();

	if (st_JsonRoot["XProxy"].empty() || (6 != st_JsonRoot["XProxy"].size()))
	{
		Config_IsErrorOccur = true;
		Config_dwErrorCode = ERROR_XENGINE_BLOGIC_CONFIG_JSON_XPROXY;
		return false;
	}
	Json::Value st_JsonXProxy = st_JsonRoot["XProxy"];
	pSt_ServerConfig->st_XProxy.bDLPass = st_JsonXProxy["bDLPass"].asBool();
	pSt_ServerConfig->st_XProxy.bUPPass = st_JsonXProxy["bUPPass"].asBool();
	pSt_ServerConfig->st_XProxy.bAuthPass = st_JsonXProxy["bAuthPass"].asBool();
	_tcsxcpy(pSt_ServerConfig->st_XProxy.tszDLPass, st_JsonXProxy["tszDLPass"].asCString());
	_tcsxcpy(pSt_ServerConfig->st_XProxy.tszUPPass, st_JsonXProxy["tszUPPass"].asCString());
	_tcsxcpy(pSt_ServerConfig->st_XProxy.tszAuthPass, st_JsonXProxy["tszAuthPass"].asCString());

	if (st_JsonRoot["XAuth"].empty() || (4 != st_JsonRoot["XAuth"].size()))
	{
		Config_IsErrorOccur = true;
		Config_dwErrorCode = ERROR_XENGINE_BLOGIC_CONFIG_JSON_XAUTH;
		return false;
	}
	Json::Value st_JsonXAuth = st_JsonRoot["XAuth"];
	pSt_ServerConfig->st_XAuth.bUPAuth = st_JsonXAuth["bUPAuth"].asInt();
	pSt_ServerConfig->st_XAuth.bDLAuth = st_JsonXAuth["bDLAuth"].asInt();
	pSt_ServerConfig->st_XAuth.bCHAuth = st_JsonXAuth["bCHAuth"].asInt();
	_tcsxcpy(pSt_ServerConfig->st_XAuth.tszUserList, st_JsonXAuth["tszUserList"].asCString());

	if (st_JsonRoot["XLimit"].empty() || (5 != st_JsonRoot["XLimit"].size()))
	{
		Config_IsErrorOccur = true;
		Config_dwErrorCode = ERROR_XENGINE_BLOGIC_CONFIG_JSON_XSTORAGE;
		return false;
	}
	Json::Value st_JsonXLimit = st_JsonRoot["XLimit"];
	pSt_ServerConfig->st_XLimit.bLimitMode = st_JsonXLimit["bLimitMode"].asBool();
	pSt_ServerConfig->st_XLimit.nMaxDNLoader = st_JsonXLimit["nMaxDNLoad"].asInt64();
	pSt_ServerConfig->st_XLimit.nMaxUPLoader = st_JsonXLimit["nMaxUPLoad"].asInt64();
	pSt_ServerConfig->st_XLimit.nMaxUPConnect = st_JsonXLimit["nMaxUPConnect"].asInt();
	pSt_ServerConfig->st_XLimit.nMaxDNConnect = st_JsonXLimit["nMaxDNConnect"].asInt();

	if (st_JsonRoot["XP2xp"].empty() || (4 != st_JsonRoot["XP2xp"].size()))
	{
		Config_IsErrorOccur = true;
		Config_dwErrorCode = ERROR_XENGINE_BLOGIC_CONFIG_JSON_P2XP;
		return false;
	}
	Json::Value st_JsonP2xp = st_JsonRoot["XP2xp"];

	pSt_ServerConfig->st_P2xp.bEnable = st_JsonP2xp["bEnable"].asBool();
	pSt_ServerConfig->st_P2xp.nTime = st_JsonP2xp["nTime"].asInt();
	pSt_ServerConfig->st_P2xp.nSDPort = st_JsonP2xp["nSDPort"].asInt();
	pSt_ServerConfig->st_P2xp.nRVPort = st_JsonP2xp["nRVPort"].asInt();

	if (st_JsonRoot["XCert"].empty() || (7 != st_JsonRoot["XCert"].size()))
	{
		Config_IsErrorOccur = true;
		Config_dwErrorCode = ERROR_XENGINE_BLOGIC_CONFIG_JSON_CERT;
		return false;
	}
	Json::Value st_JsonCert = st_JsonRoot["XCert"];
	pSt_ServerConfig->st_XCert.bDLEnable = st_JsonCert["bDLEnable"].asBool();
	pSt_ServerConfig->st_XCert.bUPEnable = st_JsonCert["bUPEnable"].asBool();
	pSt_ServerConfig->st_XCert.bCHEnable = st_JsonCert["bCHEnable"].asBool();
	pSt_ServerConfig->st_XCert.bWDEnable = st_JsonCert["bWDEnable"].asBool();

	if (!st_JsonCert["tszCertChain"].isNull())
	{
		_tcsxcpy(pSt_ServerConfig->st_XCert.tszCertChain, st_JsonCert["tszCertChain"].asCString());
	}
	if (!st_JsonCert["tszCertServer"].isNull())
	{
		_tcsxcpy(pSt_ServerConfig->st_XCert.tszCertServer, st_JsonCert["tszCertServer"].asCString());
	}
	if (!st_JsonCert["tszCertKey"].isNull())
	{
		_tcsxcpy(pSt_ServerConfig->st_XCert.tszCertKey, st_JsonCert["tszCertKey"].asCString());
	}

	if (st_JsonRoot["XReport"].empty() || (3 != st_JsonRoot["XReport"].size()))
	{
		Config_IsErrorOccur = true;
		Config_dwErrorCode = ERROR_XENGINE_BLOGIC_CONFIG_JSON_CREPORT;
		return false;
	}
	Json::Value st_JsonReport = st_JsonRoot["XReport"];
	pSt_ServerConfig->st_XReport.bEnable = st_JsonReport["bEnable"].asBool();
	_tcsxcpy(pSt_ServerConfig->st_XReport.tszServiceName, st_JsonReport["tszServiceName"].asCString());
	_tcsxcpy(pSt_ServerConfig->st_XReport.tszAPIUrl, st_JsonReport["tszAPIUrl"].asCString());

	if (st_JsonRoot["XAction"].empty() || (3 != st_JsonRoot["XAction"].size()))
	{
		Config_IsErrorOccur = true;
		Config_dwErrorCode = ERROR_XENGINE_BLOGIC_CONFIG_JSON_CREPORT;
		return false;
	}
	Json::Value st_JsonXAction = st_JsonRoot["XAction"];
	pSt_ServerConfig->st_XAction.bEnable = st_JsonXAction["bEnable"].asBool();
	pSt_ServerConfig->st_XAction.nSDMax = st_JsonXAction["nSDMax"].asInt();
	pSt_ServerConfig->st_XAction.nRVMax = st_JsonXAction["nRVMax"].asInt();
	return true;
}
/********************************************************************
函数名称：Config_Json_Version
函数功能：读取版本配置文件
 参数.一：lpszConfigFile
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入配置文件
 参数.二：pSt_ServerConfig
  In/Out：Out
  类型：数据结构指针
  可空：N
  意思：输出读取到的信息
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CConfig_Json::Config_Json_Version(LPCXSTR lpszConfigFile, XENGINE_SERVERCONFIG* pSt_ServerConfig)
{
	Config_IsErrorOccur = false;

	if ((NULL == lpszConfigFile) || (NULL == pSt_ServerConfig))
	{
		Config_IsErrorOccur = true;
		Config_dwErrorCode = ERROR_XENGINE_BLOGIC_CONFIG_JSON_PARAMENT;
		return false;
	}
	JSONCPP_STRING st_JsonError;
	Json::Value st_JsonRoot;
	Json::CharReaderBuilder st_JsonBuilder;

	FILE* pSt_File = _xtfopen(lpszConfigFile, _X("rb"));
	if (NULL == pSt_File)
	{
		Config_IsErrorOccur = true;
		Config_dwErrorCode = ERROR_XENGINE_BLOGIC_CONFIG_JSON_PARAMENT;
		return false;
	}
	int nCount = 0;
	XCHAR tszMsgBuffer[4096];
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
		Config_IsErrorOccur = true;
		Config_dwErrorCode = ERROR_XENGINE_BLOGIC_CONFIG_JSON_PARSE;
		return false;
	}
	if (st_JsonRoot["XVer"].empty())
	{
		Config_IsErrorOccur = true;
		Config_dwErrorCode = ERROR_XENGINE_BLOGIC_CONFIG_JSON_XVER;
		return false;
	}
	Json::Value st_JsonXVer = st_JsonRoot["XVer"];

	pSt_ServerConfig->st_XVer.pStl_ListStorage = new list<string>;
	for (unsigned int i = 0; i < st_JsonXVer.size(); i++)
	{
		pSt_ServerConfig->st_XVer.pStl_ListStorage->push_back(st_JsonXVer[i].asCString());
	}
	return true;
}
/********************************************************************
函数名称：Config_Json_LoadBalance
函数功能：读取负载均衡配置文件
 参数.一：lpszConfigFile
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入配置文件
 参数.二：pSt_ServerConfig
  In/Out：Out
  类型：数据结构指针
  可空：N
  意思：输出读取到的信息
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CConfig_Json::Config_Json_LoadBalance(LPCXSTR lpszConfigFile, XENGINE_LBCONFIG* pSt_ServerConfig)
{
	Config_IsErrorOccur = false;

	if ((NULL == lpszConfigFile) || (NULL == pSt_ServerConfig))
	{
		Config_IsErrorOccur = true;
		Config_dwErrorCode = ERROR_XENGINE_BLOGIC_CONFIG_JSON_PARAMENT;
		return false;
	}
	JSONCPP_STRING st_JsonError;
	Json::Value st_JsonRoot;
	Json::CharReaderBuilder st_JsonBuilder;

	FILE* pSt_File = _xtfopen(lpszConfigFile, _X("rb"));
	if (NULL == pSt_File)
	{
		Config_IsErrorOccur = true;
		Config_dwErrorCode = ERROR_XENGINE_BLOGIC_CONFIG_JSON_PARAMENT;
		return false;
	}
	int nCount = 0;
	XCHAR tszMsgBuffer[4096];
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
		Config_IsErrorOccur = true;
		Config_dwErrorCode = ERROR_XENGINE_BLOGIC_CONFIG_JSON_PARSE;
		return false;
	}

	if (st_JsonRoot["LBDistributed"].empty() || (3 != st_JsonRoot["LBDistributed"].size()))
	{
		Config_IsErrorOccur = true;
		Config_dwErrorCode = ERROR_XENGINE_BLOGIC_CONFIG_JSON_LBDISTRIBUTED;
		return false;
	}
	Json::Value st_JsonLBDistributed = st_JsonRoot["LBDistributed"];
	pSt_ServerConfig->st_LBDistributed.nUPLoadMode = st_JsonLBDistributed["nUPLoadMode"].asInt();
	pSt_ServerConfig->st_LBDistributed.nDownldMode = st_JsonLBDistributed["nDownldMode"].asInt();
	pSt_ServerConfig->st_LBDistributed.nStorageMode = st_JsonLBDistributed["nStorageMode"].asInt();

	if (st_JsonRoot["LBLocation"].empty() || (3 != st_JsonRoot["LBLocation"].size()))
	{
		Config_IsErrorOccur = true;
		Config_dwErrorCode = ERROR_XENGINE_BLOGIC_CONFIG_JSON_LBLOCATION;
		return false;
	}
	Json::Value st_JsonLBLocation = st_JsonRoot["LBLocation"];
	pSt_ServerConfig->st_LBLocation.nUPLoadMode = st_JsonLBLocation["nUPLoadMode"].asInt();
	pSt_ServerConfig->st_LBLocation.nDownldMode = st_JsonLBLocation["nDownldMode"].asInt();
	pSt_ServerConfig->st_LBLocation.nStorageMode = st_JsonLBLocation["nStorageMode"].asInt();

	if (st_JsonRoot["LoadBalance"].empty() || (3 != st_JsonRoot["LoadBalance"].size()))
	{
		Config_IsErrorOccur = true;
		Config_dwErrorCode = ERROR_XENGINE_BLOGIC_CONFIG_JSON_LAODBALANCE;
		return false;
	}
	Json::Value st_JsonLoadBalance = st_JsonRoot["LoadBalance"];

	pSt_ServerConfig->st_LoadBalance.pStl_ListDownload = new list<string>;
	for (unsigned int i = 0; i < st_JsonLoadBalance["DownloadAddr"].size(); i++)
	{
		pSt_ServerConfig->st_LoadBalance.pStl_ListDownload->push_back(st_JsonLoadBalance["DownloadAddr"][i].asCString());
	}

	pSt_ServerConfig->st_LoadBalance.pStl_ListUPLoader = new list<string>;
	for (unsigned int i = 0; i < st_JsonLoadBalance["UPLoaderAddr"].size(); i++)
	{
		pSt_ServerConfig->st_LoadBalance.pStl_ListUPLoader->push_back(st_JsonLoadBalance["UPLoaderAddr"][i].asCString());
	}

	Json::Value st_JsonBucket = st_JsonLoadBalance["StorageAddr"];
	pSt_ServerConfig->st_LoadBalance.pStl_ListBucket = new list<XENGINE_STORAGEBUCKET>;
	for (unsigned int i = 0; i < st_JsonBucket.size(); i++)
	{
		XENGINE_STORAGEBUCKET st_Bucket;
		memset(&st_Bucket, '\0', sizeof(XENGINE_STORAGEBUCKET));

		st_Bucket.bEnable = st_JsonBucket[i]["bEnable"].asBool();
		st_Bucket.nLevel = st_JsonBucket[i]["nLevel"].asInt();
		_tcsxcpy(st_Bucket.tszBuckSize, st_JsonBucket[i]["Size"].asCString());
		_tcsxcpy(st_Bucket.tszBuckKey, st_JsonBucket[i]["XEngine_Key"].asCString());
		_tcsxcpy(st_Bucket.tszFilePath, st_JsonBucket[i]["XEngine_Path"].asCString());

		Json::Value st_JsonPermission = st_JsonBucket[i]["PermissionFlags"];
		st_Bucket.st_PermissionFlags.bCreateDir = st_JsonPermission["CreateDir"].asBool();
		st_Bucket.st_PermissionFlags.bRewrite = st_JsonPermission["Rewrite"].asBool();
		st_Bucket.st_PermissionFlags.bUPLimit = st_JsonPermission["UPLimit"].asBool();
		st_Bucket.st_PermissionFlags.bUPReady = st_JsonPermission["UPReady"].asBool();

		pSt_ServerConfig->st_LoadBalance.pStl_ListBucket->push_back(st_Bucket);
	}
	return true;
}