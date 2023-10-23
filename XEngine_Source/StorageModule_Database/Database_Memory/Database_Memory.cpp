#include "pch.h"
#include "Database_Memory.h"
/********************************************************************
//    Created:     2023/10/20  10:12:03
//    File Name:   D:\XEngine_Storage\XEngine_Source\StorageModule_Database\Database_Memory\Database_Memory.cpp
//    File Path:   D:\XEngine_Storage\XEngine_Source\StorageModule_Database\Database_Memory
//    File Base:   Database_Memory
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     内存数据库管理器
//    History:
*********************************************************************/
CDatabase_Memory::CDatabase_Memory()
{
}
CDatabase_Memory::~CDatabase_Memory()
{
}
//////////////////////////////////////////////////////////////////////////
//                         公有函数
//////////////////////////////////////////////////////////////////////////
/********************************************************************
函数名称：Database_Memory_Init
函数功能：初始化存储服务数据库管理器
 参数.一：pStl_ListBucket
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：BUCKET列表
 参数.二：nHashMode
  In/Out：In
  类型：整数型
  可空：N
  意思：文件HASH方法
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CDatabase_Memory::Database_Memory_Init(list<XENGINE_STORAGEBUCKET>* pStl_ListBucket, int nHashMode)
{
    Database_IsErrorOccur = false;

    m_bIsRun = true;
    st_Locker = make_unique<shared_mutex>();
    pSTDThread = make_unique<std::thread>(Database_Memory_Thread, this);
    if (!pSTDThread->joinable())
    {
        Database_IsErrorOccur = true;
        Database_dwErrorCode = ERROR_XENGINE_XSTROGE_CORE_DB_INIT_THREAD;
        return false;
    }
    m_nHashMode = nHashMode;
	stl_ListBucket = *pStl_ListBucket;
    return true;
}
/********************************************************************
函数名称：Database_Memory_Destory
函数功能：销毁数据库管理器
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CDatabase_Memory::Database_Memory_Destory()
{
    Database_IsErrorOccur = false;

    if (!m_bIsRun)
    {
        return true;
    }
    m_bIsRun = false;

    if (NULL != pSTDThread)
    {
        if (pSTDThread->joinable())
        {
            pSTDThread->join();
        }
    }
	stl_ListBucket.clear();
    return true;
}
/********************************************************************
函数名称：Database_Memory_FileInsert
函数功能：插入一个文件数据到数据库中
 参数.一：pSt_DBManage
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：要插入的数据信息
返回值
  类型：逻辑型
  意思：是否成功
备注：这个结构所有值都必须填充
*********************************************************************/
bool CDatabase_Memory::Database_Memory_FileInsert(XSTORAGECORE_DBFILE *pSt_DBManage)
{
    Database_IsErrorOccur = false;

    if (NULL == pSt_DBManage)
    {
        Database_IsErrorOccur = true;
        Database_dwErrorCode = ERROR_XENGINE_XSTROGE_CORE_DB_INSERTFILE_PARAMENT;
        return false;
    }

    st_Locker->lock();
    stl_ListFile.push_back(*pSt_DBManage);
    st_Locker->unlock();
    return true;
}
/********************************************************************
函数名称：Database_Memory_FileDelete
函数功能：删除一个数据库文件信息
 参数.一：lpszFilePath
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：要删除的文件路径
 参数.二：lpszFileName
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：要删除的文件名称
 参数.三：lpszHash
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：要删除的文件HASH
返回值
  类型：逻辑型
  意思：是否成功
备注：参数不能全为空,不会删除文件
*********************************************************************/
bool CDatabase_Memory::Database_Memory_FileDelete(LPCXSTR lpszFilePath /* = NULL */, LPCXSTR lpszFileName /* = NULL */, LPCXSTR lpszHash /* = NULL */)
{
    Database_IsErrorOccur = false;

    if ((NULL == lpszFileName) && (NULL == lpszHash))
    {
        Database_IsErrorOccur = true;
        Database_dwErrorCode = ERROR_XENGINE_XSTROGE_CORE_DB_DELETEFILE_PARAMENT;
        return false;
    }
	st_Locker->lock();
	auto stl_ListIterator = stl_ListFile.begin();
	for (; stl_ListIterator != stl_ListFile.end(); stl_ListIterator++)
	{
		if (NULL != lpszHash)
		{
			if (0 == _tcsxnicmp(lpszHash, stl_ListIterator->st_ProtocolFile.tszFileHash, _tcsxlen(lpszHash)))
			{
                stl_ListFile.erase(stl_ListIterator);
				break;
			}
		}
		else if (NULL != lpszFileName)
		{
			if (0 == _tcsxnicmp(lpszFilePath, stl_ListIterator->st_ProtocolFile.tszFilePath, _tcsxlen(lpszFilePath)) && 0 == _tcsxnicmp(lpszFileName, stl_ListIterator->st_ProtocolFile.tszFileName, _tcsxlen(lpszFileName)))
			{
                stl_ListFile.erase(stl_ListIterator);
				break;
			}
		}
	}
	st_Locker->unlock();
    return true;
}
/********************************************************************
函数名称：Database_Memory_FileQuery
函数功能：查询文件信息
 参数.一：pppSt_ListFile
  In/Out：Out
  类型：三级指针
  可空：N
  意思：导出查询到的文件列表,此函数需要调用基础库的内存释放函数
 参数.二：pInt_ListCount
  In/Out：Out
  类型：三级指针
  可空：N
  意思：导出文件个数
 参数.三：lpszBucketName
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：要查询的BUCKET名称
 参数.四：lpszFileName
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：要查询的名称
 参数.五：lpszHash
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：要查询的文件HASH
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CDatabase_Memory::Database_Memory_FileQuery(XSTORAGECORE_DBFILE*** pppSt_ListFile, int* pInt_ListCount, LPCXSTR lpszBucketName, LPCXSTR lpszFileName, LPCXSTR lpszHash)
{
    Database_IsErrorOccur = false;

    bool bFound = false;
    st_Locker->lock_shared();
    auto stl_ListIterator = stl_ListFile.begin();
    for (; stl_ListIterator != stl_ListFile.end(); stl_ListIterator++)
    {
        if (_tcsxlen(lpszHash) > 0)
        {
			if (0 == _tcsxnicmp(lpszHash, stl_ListIterator->st_ProtocolFile.tszFileHash, _tcsxlen(lpszHash)))
			{
                if (_tcsxlen(lpszBucketName) > 0)
                {
                    if (0 == _tcsxnicmp(lpszBucketName, stl_ListIterator->tszBuckKey, _tcsxlen(lpszBucketName)))
                    {
						bFound = true;
						break;
                    }
                }
			}
        }
        else if (_tcsxlen(lpszFileName) > 0)
        {
			if (0 == _tcsxnicmp(lpszFileName, stl_ListIterator->st_ProtocolFile.tszFileName, _tcsxlen(lpszFileName)))
			{
				if (_tcsxlen(lpszBucketName) > 0)
				{
					if (0 == _tcsxnicmp(lpszBucketName, stl_ListIterator->tszBuckKey, _tcsxlen(lpszBucketName)))
					{
						bFound = true;
						break;
					}
				}
				break;
			}
        }
    }
    st_Locker->unlock_shared();

    if (bFound)
    {
		*pInt_ListCount = 1;
		BaseLib_OperatorMemory_Malloc((XPPPMEM)pppSt_ListFile, 1, sizeof(XSTORAGECORE_DBFILE));

		*(*pppSt_ListFile)[0] = *stl_ListIterator;
    }
    return true;
}
//////////////////////////////////////////////////////////////////////////
//                    保护函数
//////////////////////////////////////////////////////////////////////////
/********************************************************************
函数名称：Database_Memory_TimeDel
函数功能：删除过期日期表数据
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CDatabase_Memory::Database_Memory_Flush()
{
    Database_IsErrorOccur = false;

    st_Locker->lock();
    stl_ListFile.clear();
	for (auto stl_ListIterator = stl_ListBucket.begin(); stl_ListIterator != stl_ListBucket.end(); stl_ListIterator++)
	{
		int nListCount = 0;
		XCHAR** pptszListFile;
        XCHAR tszFoundDir[MAX_PATH];

        memset(tszFoundDir, '\0', MAX_PATH);
        _tcsxcpy(tszFoundDir, stl_ListIterator->tszFilePath);
        _tcsxcat(tszFoundDir, _T("/*"));

		SystemApi_File_EnumFile(tszFoundDir, &pptszListFile, &nListCount, NULL, NULL, true, 1);
		for (int i = 0; i < nListCount; i++)
		{
			int nHashLen = 0;
			struct _xtstat st_FileStatus;
			XBYTE tszHashStr[MAX_PATH];
			XSTORAGECORE_DBFILE st_DBFile;

			memset(tszHashStr, '\0', MAX_PATH);
			memset(&st_DBFile, '\0', sizeof(XSTORAGECORE_DBFILE));

            _xtstat(pptszListFile[i], &st_FileStatus);
			
            if (st_FileStatus.st_size > 0)
            {
                st_DBFile.st_ProtocolFile.nFileSize = st_FileStatus.st_size;
				OPenSsl_Api_Digest(pptszListFile[i], tszHashStr, &nHashLen, true, m_nHashMode);
				BaseLib_OperatorString_StrToHex((char*)tszHashStr, nHashLen, st_DBFile.st_ProtocolFile.tszFileHash);
				BaseLib_OperatorString_GetFileAndPath(pptszListFile[i], st_DBFile.st_ProtocolFile.tszFilePath, st_DBFile.st_ProtocolFile.tszFileName);
				_tcsxcpy(st_DBFile.tszBuckKey, stl_ListIterator->tszBuckKey);

				stl_ListFile.push_back(st_DBFile);
            }
		}
        BaseLib_OperatorMemory_Free((XPPPMEM)&pptszListFile, nListCount);
	}
    st_Locker->unlock();
    return true;
}
//////////////////////////////////////////////////////////////////////////
//                      线程函数
//////////////////////////////////////////////////////////////////////////
XHTHREAD CDatabase_Memory::Database_Memory_Thread(XPVOID lParam)
{
    CDatabase_Memory *pClass_This = (CDatabase_Memory *)lParam;
    time_t nTimeStart = time(NULL);
    time_t nTimeEnd = 0;
    int nTime = 60;

	while (pClass_This->m_bIsRun)
	{
        nTimeEnd = time(NULL);
		if ((nTimeEnd - nTimeStart) > nTime)
		{
            nTimeStart = nTimeEnd;
			pClass_This->Database_Memory_Flush();
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
    return 0;
}