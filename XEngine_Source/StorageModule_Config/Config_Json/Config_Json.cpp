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
BOOL CConfig_Json::Config_Json_File(LPCTSTR lpszConfigFile, XENGINE_SERVERCONFIG* pSt_ServerConfig)
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

	if (st_JsonRoot["XSql"].empty() || (5 != st_JsonRoot["XSql"].size()))
	{
		Config_IsErrorOccur = TRUE;
		Config_dwErrorCode = ERROR_XENGINE_BLOGIC_CONFIG_JSON_XSQL;
		return FALSE;
	}
	Json::Value st_JsonXSql = st_JsonRoot["XSql"];

	pSt_ServerConfig->st_XSql.bEnable = st_JsonXSql["SQLEnable"].asBool();
	pSt_ServerConfig->st_XSql.nSQLPort = st_JsonXSql["SQLPort"].asInt();
	_tcscpy(pSt_ServerConfig->st_XSql.tszSQLAddr, st_JsonXSql["SQLAddr"].asCString());
	_tcscpy(pSt_ServerConfig->st_XSql.tszSQLUser, st_JsonXSql["SQLUser"].asCString());
	_tcscpy(pSt_ServerConfig->st_XSql.tszSQLPass, st_JsonXSql["SQLPass"].asCString());

	if (st_JsonRoot["XStorage"].empty() || (3 != st_JsonRoot["XStorage"].size()))
	{
		Config_IsErrorOccur = TRUE;
		Config_dwErrorCode = ERROR_XENGINE_BLOGIC_CONFIG_JSON_XSTORAGE;
		return FALSE;
	}
	Json::Value st_JsonXStorage = st_JsonRoot["XStorage"];
	pSt_ServerConfig->st_XStorage.nHashMode = st_JsonXStorage["nHashMode"].asInt();
	pSt_ServerConfig->st_XStorage.bUPHash = st_JsonXStorage["bUPHash"].asInt();
	pSt_ServerConfig->st_XStorage.bResumable = st_JsonXStorage["bResumable"].asInt();

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

	if (st_JsonRoot["XLimit"].empty() || (3 != st_JsonRoot["XLimit"].size()))
	{
		Config_IsErrorOccur = TRUE;
		Config_dwErrorCode = ERROR_XENGINE_BLOGIC_CONFIG_JSON_XSTORAGE;
		return FALSE;
	}
	Json::Value st_JsonXLimit = st_JsonRoot["XLimit"];
	pSt_ServerConfig->st_XLimit.nDLTry = st_JsonXLimit["nDLTry"].asUInt();
	pSt_ServerConfig->st_XLimit.nMaxDNLoader = st_JsonXLimit["nMaxDNLoad"].asInt64();
	pSt_ServerConfig->st_XLimit.nMaxUPLoader = st_JsonXLimit["nMaxUPLoad"].asInt64();

	if (st_JsonRoot["XP2xp"].empty() || (3 != st_JsonRoot["XP2xp"].size()))
	{
		Config_IsErrorOccur = TRUE;
		Config_dwErrorCode = ERROR_XENGINE_BLOGIC_CONFIG_JSON_P2XP;
		return FALSE;
	}
	Json::Value st_JsonP2xp = st_JsonRoot["XP2xp"];
	pSt_ServerConfig->st_P2xp.nTime = st_JsonP2xp["nTime"].asInt();
	pSt_ServerConfig->st_P2xp.nRVPort = st_JsonP2xp["nRVPort"].asInt();
	pSt_ServerConfig->st_P2xp.nSDPort = st_JsonP2xp["nSDPort"].asInt();

	if (st_JsonRoot["XCert"].empty() || (6 != st_JsonRoot["XCert"].size()))
	{
		Config_IsErrorOccur = TRUE;
		Config_dwErrorCode = ERROR_XENGINE_BLOGIC_CONFIG_JSON_CERT;
		return FALSE;
	}
	Json::Value st_JsonCert = st_JsonRoot["XCert"];
	pSt_ServerConfig->st_XCert.bDLEnable = st_JsonCert["bDLEnable"].asInt();
	pSt_ServerConfig->st_XCert.bUPEnable = st_JsonCert["bUPEnable"].asInt();
	pSt_ServerConfig->st_XCert.bCHEnable = st_JsonCert["bCHEnable"].asInt();
	pSt_ServerConfig->st_XCert.nSslType = st_JsonCert["nSslType"].asInt();
	if (!st_JsonP2xp["tszCertChain"].isNull())
	{
		_tcscpy(pSt_ServerConfig->st_XCert.tszCertChain, st_JsonP2xp["tszCertChain"].asCString());
	}
	if (!st_JsonP2xp["tszCertKey"].isNull())
	{
		_tcscpy(pSt_ServerConfig->st_XCert.tszCertKey, st_JsonP2xp["tszCertKey"].asCString());
	}

	if (st_JsonRoot["XVer"].empty() || (1 != st_JsonRoot["XVer"].size()))
	{
		Config_IsErrorOccur = TRUE;
		Config_dwErrorCode = ERROR_XENGINE_BLOGIC_CONFIG_JSON_XVER;
		return FALSE;
	}
	Json::Value st_JsonXVer = st_JsonRoot["XVer"];

	pSt_ServerConfig->st_XVer.pStl_ListStorage = new list<string>;
	for (unsigned int i = 0; i < st_JsonXVer["StorageVersion"].size(); i++)
	{
		pSt_ServerConfig->st_XVer.pStl_ListStorage->push_back(st_JsonXVer["StorageVersion"][i].asCString());
	}
	return TRUE;
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
	pSt_ServerConfig->bDistributed = st_JsonRoot["bDistributed"].asInt();

	if (st_JsonRoot["LBDistributed"].empty() || (4 != st_JsonRoot["LBDistributed"].size()))
	{
		Config_IsErrorOccur = TRUE;
		Config_dwErrorCode = ERROR_XENGINE_BLOGIC_CONFIG_JSON_LBDISTRIBUTED;
		return FALSE;
	}
	Json::Value st_JsonLBDistributed = st_JsonRoot["LBDistributed"];
	pSt_ServerConfig->st_LBDistributed.nCenterMode = st_JsonLBDistributed["nCenterMode"].asInt();
	pSt_ServerConfig->st_LBDistributed.nUPLoadMode = st_JsonLBDistributed["nUPLoadMode"].asInt();
	pSt_ServerConfig->st_LBDistributed.nDownldMode = st_JsonLBDistributed["nDownldMode"].asInt();
	pSt_ServerConfig->st_LBDistributed.nStorageMode = st_JsonLBDistributed["nStorageMode"].asInt();

	if (st_JsonRoot["LBLocation"].empty() || (4 != st_JsonRoot["LBLocation"].size()))
	{
		Config_IsErrorOccur = TRUE;
		Config_dwErrorCode = ERROR_XENGINE_BLOGIC_CONFIG_JSON_LBLOCATION;
		return FALSE;
	}
	Json::Value st_JsonLBLocation = st_JsonRoot["LBLocation"];
	pSt_ServerConfig->st_LBLocation.nCenterMode = st_JsonLBLocation["nCenterMode"].asInt();
	pSt_ServerConfig->st_LBLocation.nUPLoadMode = st_JsonLBLocation["nUPLoadMode"].asInt();
	pSt_ServerConfig->st_LBLocation.nDownldMode = st_JsonLBLocation["nDownldMode"].asInt();
	pSt_ServerConfig->st_LBLocation.nStorageMode = st_JsonLBLocation["nStorageMode"].asInt();

	if (st_JsonRoot["LoadBalance"].empty() || (4 != st_JsonRoot["LoadBalance"].size()))
	{
		Config_IsErrorOccur = TRUE;
		Config_dwErrorCode = ERROR_XENGINE_BLOGIC_CONFIG_JSON_LAODBALANCE;
		return FALSE;
	}
	Json::Value st_JsonLoadBalance = st_JsonRoot["LoadBalance"];

	pSt_ServerConfig->st_LoadBalance.pStl_ListCenter = new list<string>;
	for (unsigned int i = 0; i < st_JsonLoadBalance["CenterAddr"].size(); i++)
	{
		pSt_ServerConfig->st_LoadBalance.pStl_ListCenter->push_back(st_JsonLoadBalance["CenterAddr"][i].asCString());
	}

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

		st_Bucket.bEnable = st_JsonBucket[i]["bEnable"].asInt();
		st_Bucket.nLevel = st_JsonBucket[i]["nLevel"].asInt();
		_tcscpy(st_Bucket.tszBuckSize, st_JsonBucket[i]["Size"].asCString());
		_tcscpy(st_Bucket.tszBuckKey, st_JsonBucket[i]["XEngine_Key"].asCString());
		_tcscpy(st_Bucket.tszFilePath, st_JsonBucket[i]["XEngine_Path"].asCString());

		pSt_ServerConfig->st_LoadBalance.pStl_ListBucket->push_back(st_Bucket);
	}
	return TRUE;
}