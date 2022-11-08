#pragma once
/********************************************************************
函数名称：HelpModule_Api_Copy
函数功能：拷贝新文件到指定目录
 参数.一：pStl_ListUPDate
  In/Out：In
  类型：容器指针
  可空：N
  意思：更新的文件列表信息
 参数.二：lpszDlPath
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：下载的路径
 参数.三：bDelFile
  In/Out：In
  类型：逻辑型
  可空：Y
  意思：是否删除原始文件
返回值
  类型：逻辑型
  意思：是否拷贝成功
备注：
*********************************************************************/
BOOL HelpModule_Api_Copy(list<FILEPARSER_VERSIONINFO>* pStl_ListUPDate, LPCTSTR lpszDlPath = NULL, BOOL bDelFile = TRUE)
{
    TCHAR tszDlPath[1024];
    TCHAR tszCpPath[1024];
    if ((NULL == pStl_ListUPDate))
    {
        return FALSE;
    }
    list<FILEPARSER_VERSIONINFO>::const_iterator stl_ListIterator = pStl_ListUPDate->begin();
    for (; stl_ListIterator != pStl_ListUPDate->end(); stl_ListIterator++)
    {
        memset(tszDlPath, '\0', sizeof(tszDlPath));
        memset(tszCpPath, '\0', sizeof(tszCpPath));
        //获取下载的文件路径和名称 组合成可以拷贝的路径地址
        if (NULL != lpszDlPath)
        {
            _stprintf_s(tszDlPath, _T("%s%s"), lpszDlPath, stl_ListIterator->tszModuleName);
        }
        else
        {
            _tcscpy(tszDlPath, stl_ListIterator->tszModuleName);
        }
        if (0 == stl_ListIterator->st_LocalVersion.nModuleVersion)
        {
            _stprintf_s(tszCpPath, _T("%s%s"), stl_ListIterator->tszModulePath, stl_ListIterator->tszModuleName);
        }
        else
        {
            _stprintf_s(tszCpPath, _T("%s%s"), stl_ListIterator->st_LocalVersion.tszMoudelPath, stl_ListIterator->st_LocalVersion.tszMoudelName);
        }
        if (!SystemApi_File_CreateMutilFolder(stl_ListIterator->st_LocalVersion.tszMoudelPath))
        {
            return FALSE;
        }
        if (!SystemApi_File_CopyFile(tszDlPath, tszCpPath, FALSE))
        {
            return FALSE;
        }
        if (bDelFile)
        {
            //删除原始下载的文件
            if (0 != _tremove(tszDlPath))
            {
                return FALSE;
            }
        }
    }
    return TRUE;
}
/********************************************************************
函数名称：HelpModule_Api_SetVersion
函数功能：设置文件版本
 参数.一：lpszLocalListFile
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：本地文件列表名称
 参数.二：lpszMsgBuffer
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：更新文件列表的信息结构
 参数.三：nMsgLen
  In/Out：In
  类型：整数型
  可空：N
  意思：缓冲区大小
返回值
  类型：逻辑型
  意思：是否设置成功
备注：
*********************************************************************/
BOOL HelpModule_Api_SetVersion(LPCTSTR lpszLocalFile, LPCTSTR lpszMsgBuffer, int nMsgLen)
{
    //文件JSON
    Json::Value st_JsonRoot;
    Json::Value st_JsonArray;
    //以前的JSON
    Json::Value st_JsonLocalRoot;
    Json::Value st_JsonLocalArray;
    Json::CharReaderBuilder st_JsonLocalBuild;
    Json::CharReader* pSt_JsonLocalReader(st_JsonLocalBuild.newCharReader());
    //新的JSON
    Json::Value st_JsonRemoteRoot;
    Json::Value st_JsonRemoteArray;
    Json::CharReaderBuilder st_JsonRemoteBuild;
    Json::CharReader* pSt_JsonRemoteReader(st_JsonRemoteBuild.newCharReader());

    JSONCPP_STRING st_JsonError;

    if (!pSt_JsonRemoteReader->parse(lpszMsgBuffer, lpszMsgBuffer + nMsgLen, &st_JsonRemoteRoot, &st_JsonError))
    {
        return FALSE;
    }
    delete pSt_JsonRemoteReader;
    pSt_JsonRemoteReader = NULL;

    int nCount = 0;
    TCHAR* ptszJsonFile = new TCHAR[1024 * 1024 * 10];
    if (NULL == ptszJsonFile)
    {
        return FALSE;
    }
    memset(ptszJsonFile, '\0', 1024 * 1024 * 10);
    FILE* pSt_File = _tfopen(lpszLocalFile, _T("rb"));
    if (NULL == pSt_File)
    {
        delete[]ptszJsonFile;
        ptszJsonFile = NULL;
        return FALSE;
    }
    while (TRUE)
    {
        TCHAR tszJsonMsg[2048];
        memset(tszJsonMsg, '\0', sizeof(tszJsonMsg));
        int nRet = fread(tszJsonMsg, sizeof(TCHAR), 2048, pSt_File);
        if (nRet <= 0)
        {
            break;
        }
        memcpy(ptszJsonFile + nCount, tszJsonMsg, nRet);
        nCount += nRet;
    }
    fclose(pSt_File);

    if (!pSt_JsonLocalReader->parse(ptszJsonFile, ptszJsonFile + nCount, &st_JsonLocalRoot, &st_JsonError))
    {
        delete[]ptszJsonFile;
        ptszJsonFile = NULL;
        return FALSE;
    }
    delete pSt_JsonLocalReader;
    delete[]ptszJsonFile;
    ptszJsonFile = NULL;
    pSt_JsonLocalReader = NULL;

    st_JsonLocalArray = st_JsonLocalRoot["FileList"];
    for (unsigned int i = 0; i < st_JsonLocalArray.size(); i++)
    {
        BOOL bIsFound = FALSE;
        Json::Value st_JsonRemoteArray = st_JsonRemoteRoot["UPList"];
        for (unsigned int j = 0; j < st_JsonRemoteArray.size(); j++)
        {
            if (0 == _tcscmp(st_JsonLocalArray[i]["ModuleCode"].asCString(), st_JsonRemoteArray[j]["ModuleCode"].asCString()))
            {
                Json::Value st_JsonObject;
                st_JsonObject["ModuleVersion"] = st_JsonRemoteArray[j]["ModuleVersion"].asUInt64();
                st_JsonObject["ModuleCode"] = st_JsonLocalArray[i]["ModuleCode"].asCString();
                st_JsonObject["ModuleName"] = st_JsonLocalArray[i]["ModuleName"].asCString();
                st_JsonObject["ModulePath"] = st_JsonLocalArray[i]["ModulePath"].asCString();
                st_JsonArray.append(st_JsonObject);
                bIsFound = TRUE;
                break;
            }
        }
        if (!bIsFound)
        {
            //没有找到更新的文件
            BOOL bIsDelFound = FALSE;
            Json::Value st_JsonDelArray;
            st_JsonDelArray = st_JsonRemoteRoot["DelList"];
            //查找是否有删除的文件内容
            for (unsigned int k = 0; k < st_JsonDelArray.size(); k++)
            {
                if (0 == _tcscmp(st_JsonLocalArray[i]["ModuleName"].asCString(), st_JsonDelArray[k]["ModuleName"].asCString()))
                {
                    bIsDelFound = TRUE;
                    break;
                }
            }
            //如果没有删除的,那么这个JSON需要加到集合里面,如果有,那么这个JSON就不需要了
            if (!bIsDelFound)
            {
                Json::Value st_JsonObject;
                st_JsonObject["ModuleVersion"] = st_JsonLocalArray[i]["ModuleVersion"].asUInt64();
                st_JsonObject["ModuleCode"] = st_JsonLocalArray[i]["ModuleCode"].asCString();
                st_JsonObject["ModuleName"] = st_JsonLocalArray[i]["ModuleName"].asCString();
                st_JsonObject["ModulePath"] = st_JsonLocalArray[i]["ModulePath"].asCString();
                st_JsonArray.append(st_JsonObject);
            }
        }
    }
    //添加新增加的JSON
    st_JsonRemoteArray = st_JsonRemoteRoot["UPList"];
    for (unsigned int i = 0; i < st_JsonRemoteArray.size(); i++)
    {
        BOOL bIsFound = FALSE;
        Json::Value st_JsonLocalArray = st_JsonLocalRoot["FileList"];
        //查找远程元素是否在本地存在
        for (unsigned int j = 0; j < st_JsonLocalArray.size(); j++)
        {
            if (0 == _tcscmp(st_JsonRemoteArray[i]["ModuleCode"].asCString(), st_JsonLocalArray[j]["ModuleCode"].asCString()))
            {
                bIsFound = TRUE;
                break;
            }
        }
        //如果不存在,就添加
        if (!bIsFound)
        {
            Json::Value st_JsonObject;
            st_JsonObject["ModuleVersion"] = st_JsonRemoteArray[i]["ModuleVersion"].asInt64();
            st_JsonObject["ModuleCode"] = st_JsonRemoteArray[i]["ModuleCode"].asCString();
            st_JsonObject["ModuleName"] = st_JsonRemoteArray[i]["ModuleName"].asCString();
            st_JsonObject["ModulePath"] = st_JsonRemoteArray[i]["ModulePath"].asCString();
            st_JsonArray.append(st_JsonObject);
        }
    }
    st_JsonRoot["MainVersion"] = st_JsonRemoteRoot["MainVersion"].asInt64();
    st_JsonRoot["FileList"] = st_JsonArray;

    if (0 != _tremove(lpszLocalFile))
    {
        return FALSE;
    }
    if (!SystemApi_File_SaveBuffToFile(lpszLocalFile, st_JsonRoot.toStyledString().c_str(), st_JsonRoot.toStyledString().length()))
    {
        return FALSE;
    }

    return TRUE;
}
/********************************************************************
函数名称：HelpModule_Api_RunExec
函数功能：运行一个程序
 参数.一：pStl_ListUPDate
  In/Out：In
  类型：容器指针
  可空：N
  意思：文件更新运行列表
返回值
  类型：逻辑型
  意思：是否执行成功
备注：
*********************************************************************/
BOOL HelpModule_Api_RunExec(list<FILEPARSER_VERSIONINFO>* pStl_ListUPDate)
{
    list<FILEPARSER_VERSIONINFO>::const_iterator stl_ListIterator = pStl_ListUPDate->begin();
    for (; stl_ListIterator != pStl_ListUPDate->end(); stl_ListIterator++)
    {
        //判断要执行的文件
        if (stl_ListIterator->bIsRun)
        {
            DWORD dwProcessID = 0;
            TCHAR tszPath[1024];
            memset(tszPath, '\0', sizeof(tszPath));
            //判断目录时候需要改变
            _stprintf_s(tszPath, _T("%s%s"), stl_ListIterator->st_LocalVersion.tszMoudelPath, stl_ListIterator->st_LocalVersion.tszMoudelName);
            if (!SystemApi_Process_CreateProcess(&dwProcessID, tszPath))
            {
                return FALSE;
            }
        }
    }
    return TRUE;
}
/********************************************************************
函数名称：HelpModule_Api_Clear
函数功能：清理工作执行
 参数.一：lpszRemoteJson
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：清理列表
 参数.二：nMsgLen
  In/Out：In
  类型：整数型
  可空：N
  意思：输入缓冲区大小
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL HelpModule_Api_Clear(LPCTSTR lpszRemoteJson, int nMsgLen)
{
    Json::Value st_JsonRoot;
    Json::Value st_JsonArray;
    Json::CharReaderBuilder st_JsonBuild;
    Json::CharReader* pSt_JsonReader(st_JsonBuild.newCharReader());

    JSONCPP_STRING st_JsonError;

    if (!pSt_JsonReader->parse(lpszRemoteJson, lpszRemoteJson + nMsgLen, &st_JsonRoot, &st_JsonError))
    {
        return FALSE;
    }
    delete pSt_JsonReader;
    pSt_JsonReader = NULL;

    st_JsonArray = st_JsonRoot["DelList"];
    for (unsigned int i = 0; i < st_JsonArray.size(); i++)
    {
        if (0 != _tremove(st_JsonArray[i].asCString()))
        {
            return FALSE;
        }
    }
    return TRUE;
}