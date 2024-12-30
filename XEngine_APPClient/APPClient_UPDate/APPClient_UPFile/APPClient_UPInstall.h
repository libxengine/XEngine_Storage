#pragma once
/********************************************************************
函数名称：FileParser_ReadVer_GetRemote
函数功能：获取更新内容
 参数.一：lpszJsonMsg
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：通过获取到的新版本JSON信息来获取更新内容
 参数.二：nMsgLen
  In/Out：In
  类型：整数型
  可空：N
  意思：输入JSON缓冲区大小
 参数.三：pppSt_FileList
  In/Out：Out
  类型：三级指针
  可空：N
  意思：导出获取到的文件列表
 参数.四：pInt_Count
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出获取到的列表个数
 参数.五：pInt_Version
  In/Out：Out
  类型：整数型指针
  可空：Y
  意思：输出新版本值
 参数.六：pStrDes
  In/Out：Out
  类型：string类
  可空：Y
  意思：输出获取到的更新描述
返回值
  类型：逻辑型
  意思：是否获取成功
备注：
*********************************************************************/
bool FileParser_ReadVer_GetRemote(LPCXSTR lpszJsonMsg, int nMsgLen, FILEPARSER_VERSIONINFO*** pppSt_FileList, int* pInt_Count, __int64x* pInt_Version, string* pStrDes)
{
	Json::Value st_JsonRoot;
	Json::Value st_JsonArray;
	Json::CharReaderBuilder st_JsonBuild;
	Json::CharReader* pSt_JsonReader(st_JsonBuild.newCharReader());

	JSONCPP_STRING st_JsonError;
	//先解析JSON是否正确
	if (!pSt_JsonReader->parse(lpszJsonMsg, lpszJsonMsg + nMsgLen, &st_JsonRoot, &st_JsonError))
	{
		return FALSE;
	}
	delete pSt_JsonReader;
	pSt_JsonReader = NULL;

	if (NULL != pInt_Version)
	{
		*pInt_Version = st_JsonRoot["MainVersion"].asInt64();
	}
	if (NULL != pStrDes)
	{
		*pStrDes = st_JsonRoot["MainDescription"].asString();
	}
	st_JsonArray = st_JsonRoot["UPList"];

	//判断是否有文件存在
	if (st_JsonArray.size() == 0)
	{
		return FALSE;
	}
	*pInt_Count = st_JsonArray.size();
	BaseLib_Memory_Malloc((XPPPMEM)pppSt_FileList, *pInt_Count, sizeof(FILEPARSER_VERSIONINFO));

	for (unsigned int i = 0; i < st_JsonArray.size(); i++)
	{
		//开始读取文件列表内容
		(*pppSt_FileList)[i]->bIsRun = st_JsonArray[i]["ModuleRun"].asBool();
		(*pppSt_FileList)[i]->nModuleVersion = st_JsonArray[i]["ModuleVersion"].asInt64();
		_tcsxcpy((*pppSt_FileList)[i]->tszModuleCode, st_JsonArray[i]["ModuleCode"].asCString());
		if (!st_JsonArray[i]["ModulePath"].isNull())
		{
			_tcsxcpy((*pppSt_FileList)[i]->tszModulePath, st_JsonArray[i]["ModulePath"].asCString());
		}
		_tcsxcpy((*pppSt_FileList)[i]->tszModuleName, st_JsonArray[i]["ModuleName"].asCString());
		_tcsxcpy((*pppSt_FileList)[i]->tszModuleDownload, st_JsonArray[i]["ModuleDownload"].asCString());
	}
	return true;
}
/********************************************************************
函数名称：FileParser_ReadVer_GetLocal
函数功能：获取本地内容
 参数.一：lpszFileList
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：本地列表地址
 参数.二：pppSt_FileList
  In/Out：Out
  类型：三级指针
  可空：N
  意思：导出获取到的文件列表
 参数.三：pInt_Count
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出获取到的列表个数
 参数.四：pInt_Version
  In/Out：Out
  类型：整数型指针
  可空：Y
  意思：输出版本值
返回值
  类型：逻辑型
  意思：是否获取成功
备注：
*********************************************************************/
bool FileParser_ReadVer_GetLocal(LPCXSTR lpszFileList, FILEPARSER_VERSIONINFO*** pppSt_FileList, int* pInt_Count, __int64x* pInt_Version /* = NULL */)
{
	int nCount = 0;
	XCHAR* ptszJsonFile = (XCHAR*)malloc(1024 * 1024 * 10);
	if (NULL == ptszJsonFile)
	{
		return FALSE;
	}
	memset(ptszJsonFile, '\0', 1024 * 1024 * 10);
	FILE* pSt_File = _xtfopen(lpszFileList, _X("rb"));
	if (NULL == pSt_File)
	{
		free(ptszJsonFile);
		ptszJsonFile = NULL;
		return FALSE;
	}
	while (true)
	{
		XCHAR tszJsonMsg[2048];
		memset(tszJsonMsg, '\0', sizeof(tszJsonMsg));
		size_t nRet = fread(tszJsonMsg, sizeof(XCHAR), sizeof(tszJsonMsg), pSt_File);
		if (nRet <= 0)
		{
			break;
		}
		memcpy(ptszJsonFile + nCount, tszJsonMsg, nRet);
		nCount += (int)nRet;
	}
	fclose(pSt_File);

	Json::Value st_JsonRoot;
	Json::Value st_JsonArray;
	Json::CharReaderBuilder st_JsonBuild;
	Json::CharReader* pSt_JsonReader(st_JsonBuild.newCharReader());

	JSONCPP_STRING st_JsonError;

	if (!pSt_JsonReader->parse(ptszJsonFile, ptszJsonFile + nCount, &st_JsonRoot, &st_JsonError))
	{
		return FALSE;
	}
	delete pSt_JsonReader;
	pSt_JsonReader = NULL;

	if (NULL != pInt_Version)
	{
		*pInt_Version = st_JsonRoot["MainVersion"].asInt64();
	}
	st_JsonArray = st_JsonRoot["FileList"];
	//判断是否有文件存在
	if (st_JsonArray.size() == 0)
	{
		return FALSE;
	}
	*pInt_Count = st_JsonArray.size();
	BaseLib_Memory_Malloc((XPPPMEM)pppSt_FileList, *pInt_Count, sizeof(FILEPARSER_VERSIONINFO));
	for (unsigned int i = 0; i < st_JsonArray.size(); i++)
	{
		(*pppSt_FileList)[i]->st_LocalVersion.nModuleVersion = st_JsonArray[i]["ModuleVersion"].asInt64();
		_tcsxcpy((*pppSt_FileList)[i]->st_LocalVersion.tszMoudelCode, st_JsonArray[i]["ModuleCode"].asCString());
		_tcsxcpy((*pppSt_FileList)[i]->st_LocalVersion.tszMoudelName, st_JsonArray[i]["ModuleName"].asCString());
		_tcsxcpy((*pppSt_FileList)[i]->st_LocalVersion.tszMoudelPath, st_JsonArray[i]["ModulePath"].asCString());
	}
	return true;
}
/********************************************************************
函数名称：FileParser_Match_Start
函数功能：开始匹配新版本
 参数.一：pppSt_ListRemote
  In/Out：In
  类型：三级指针
  可空：N
  意思：远程更新文件列表信息
 参数.二：nRemoteCount
  In/Out：In
  类型：整数型
  可空：N
  意思：输入远程文件个数
 参数.三：pppSt_ListLocal
  In/Out：In
  类型：三级指针
  可空：N
  意思：输入本地文件列表信息
 参数.四：nLocalCount
  In/Out：In
  类型：整数型
  可空：N
  意思：输入本地列表文件信息
 参数.五：pStl_ListUPDate
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出要更新的文件列表信息
返回值
  类型：逻辑型
  意思：是否有新版本
备注：
*********************************************************************/
bool FileParser_Match_Start(FILEPARSER_VERSIONINFO*** pppSt_ListRemote, int nRemoteCount, FILEPARSER_VERSIONINFO*** pppSt_ListLocal, int nLocalCount, list<FILEPARSER_VERSIONINFO>* pStl_ListUPDate)
{
	for (int i = 0; i < nRemoteCount; i++)
	{
		//版本比较
		for (int j = 0; j < nLocalCount; j++)
		{
			if (0 == _tcsxncmp((*pppSt_ListLocal)[j]->st_LocalVersion.tszMoudelCode, (*pppSt_ListRemote)[i]->tszModuleCode, _tcsxlen((*pppSt_ListRemote)[i]->tszModuleCode)))
			{
				if ((*pppSt_ListRemote)[i]->nModuleVersion > (*pppSt_ListLocal)[j]->st_LocalVersion.nModuleVersion)
				{
					(*pppSt_ListRemote)[i]->st_LocalVersion = (*pppSt_ListLocal)[j]->st_LocalVersion;
					pStl_ListUPDate->push_back(*(*pppSt_ListRemote)[i]);
					break;
				}
				else
				{
					break;
				}
			}
		}
	}
	return true;
}