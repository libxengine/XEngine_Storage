#include "pch.h"
#include "BTorrent_Creator.h"
/********************************************************************
//    Created:     2023/02/02  16:26:25
//    File Name:   D:\XEngine\XEngine_SourceCode\XEngine_NetHelp\NetHelp_BTorrent\BTorrent_Creator\BTorrent_Creator.cpp
//    File Path:   D:\XEngine\XEngine_SourceCode\XEngine_NetHelp\NetHelp_BTorrent\BTorrent_Creator
//    File Base:   BTorrent_Creator
//    File Ext:    cpp
//    Project:     XEngine
//    Author:      qyt
//    Purpose:     BT创建器
//    History:
*********************************************************************/
CBTorrent_Creator::CBTorrent_Creator()
{
}
CBTorrent_Creator::~CBTorrent_Creator()
{
}
//////////////////////////////////////////////////////////////////////////
//                        公有函数
//////////////////////////////////////////////////////////////////////////
/********************************************************************
函数名称：BTorrent_Creator_Init
函数功能：初始化一个制作工具
 参数.一：pxhToken
  In/Out：In
  类型：句柄
  可空：N
  意思：导出一个句柄
 参数.二：lpszBTPath
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：要制作的文件地址路径
 参数.三：nPieceSize
  In/Out：In
  类型：整数型
  可空：N
  意思：块大小,默认16K
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CBTorrent_Creator::BTorrent_Creator_Init(XNETHANDLE* pxhToken, LPCXSTR lpszBTPath, int nPieceSize)
{
    BTDload_IsErrorOccur = false;

    if ((NULL == pxhToken) || (NULL == lpszBTPath))
    {
        BTDload_IsErrorOccur = true;
        BTDload_dwErrorCode = ERROR_STORAGE_MODULE_BTORRENT_CREATOR_INIT_PARAMENT;
        return false;
    }
	//申请空间
	BTORRENT_CREATORINFO st_BTCreator;
	memset(&st_BTCreator, '\0', sizeof(BTORRENT_CREATORINFO));

	//申请内存
	st_BTCreator.pStl_ListNode = new list<BTORRENT_PARSEMAP>;
	st_BTCreator.pStl_ListSeeds = new list<BTORRENT_PARSEMAP>;
	st_BTCreator.pStl_ListTracker = new list<BTORRENT_PARSEMAP>;

	if ((NULL == st_BTCreator.pStl_ListNode) || (NULL == st_BTCreator.pStl_ListSeeds) || (NULL == st_BTCreator.pStl_ListTracker))
	{
		BTDload_IsErrorOccur = true;
		BTDload_dwErrorCode = ERROR_STORAGE_MODULE_BTORRENT_CREATOR_INIT_MALLOC;
		return false;
	}

	st_BTCreator.m_nPieceSize = nPieceSize;
	_tcsxcpy(st_BTCreator.tszBTPath, lpszBTPath);

	BaseLib_Handle_Create(pxhToken);
    st_Locker.lock();
    stl_MapBTCreator.insert(make_pair(*pxhToken, st_BTCreator));
    st_Locker.unlock();
    return true;
}
/********************************************************************
函数名称：BTorrent_Creator_AddNode
函数功能：添加种子DHT节点地址
 参数.一：xhToken
  In/Out：In
  类型：句柄
  可空：N
  意思：输入要操作的句柄
 参数.二：lpszAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入节点地址
 参数.三：nIndex
  In/Out：In
  类型：整数型
  可空：N
  意思：当前索引
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CBTorrent_Creator::BTorrent_Creator_AddNode(XNETHANDLE xhToken, LPCXSTR lpszAddr, int nIndex)
{
	BTDload_IsErrorOccur = false;

	if (NULL == lpszAddr)
	{
		BTDload_IsErrorOccur = true;
		BTDload_dwErrorCode = ERROR_STORAGE_MODULE_BTORRENT_CREATOR_NODE_PARAMENT;
		return false;
	}
	st_Locker.lock_shared();
	unordered_map<XNETHANDLE, BTORRENT_CREATORINFO>::const_iterator stl_MapIterator = stl_MapBTCreator.find(xhToken);
	if (stl_MapIterator == stl_MapBTCreator.end())
	{
		BTDload_IsErrorOccur = true;
		BTDload_dwErrorCode = ERROR_STORAGE_MODULE_BTORRENT_CREATOR_NODE_NOTFOUND;
		st_Locker.unlock_shared();
		return false;
	}
	BTORRENT_PARSEMAP st_BTInfo;
	memset(&st_BTInfo, '\0', sizeof(BTORRENT_PARSEMAP));

	st_BTInfo.nValue = nIndex;
	_tcsxcpy(st_BTInfo.tszValue, lpszAddr);
	stl_MapIterator->second.pStl_ListNode->push_back(st_BTInfo);
	st_Locker.unlock_shared();
	return true;
}
/********************************************************************
函数名称：BTorrent_Creator_AddTracker
函数功能：设置TRACKER服务器地址
 参数.一：xhToken
  In/Out：In
  类型：句柄
  可空：N
  意思：输入要操作的句柄
 参数.二：lpszAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入地址
 参数.三：nIndex
  In/Out：In
  类型：整数型
  可空：Y
  意思：输入服务器索引
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CBTorrent_Creator::BTorrent_Creator_AddTracker(XNETHANDLE xhToken, LPCXSTR lpszAddr, int nIndex)
{
	BTDload_IsErrorOccur = false;

	if (NULL == lpszAddr)
	{
		BTDload_IsErrorOccur = true;
		BTDload_dwErrorCode = ERROR_STORAGE_MODULE_BTORRENT_CREATOR_TRACKER_PARAMENT;
		return false;
	}
	st_Locker.lock_shared();
	unordered_map<XNETHANDLE, BTORRENT_CREATORINFO>::const_iterator stl_MapIterator = stl_MapBTCreator.find(xhToken);
	if (stl_MapIterator == stl_MapBTCreator.end())
	{
		BTDload_IsErrorOccur = true;
		BTDload_dwErrorCode = ERROR_STORAGE_MODULE_BTORRENT_CREATOR_TRACKER_NOTFOUND;
		st_Locker.unlock_shared();
		return false;
	}
	BTORRENT_PARSEMAP st_BTInfo;
	memset(&st_BTInfo, '\0', sizeof(BTORRENT_PARSEMAP));

	st_BTInfo.nValue = nIndex;
	_tcsxcpy(st_BTInfo.tszValue, lpszAddr);
	stl_MapIterator->second.pStl_ListTracker->push_back(st_BTInfo);
	st_Locker.unlock_shared();
	return true;
}
/********************************************************************
函数名称：BTorrent_Creator_AddSeeds
函数功能：添加URL种子到制作器 
 参数.一：xhToken
  In/Out：In
  类型：句柄
  可空：N
  意思：输入要操作的句柄
 参数.二：lpszAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入地址
 参数.三：bSingle
  In/Out：In
  类型：逻辑型
  可空：N
  意思：多种子文件还是单种子文件
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CBTorrent_Creator::BTorrent_Creator_AddSeeds(XNETHANDLE xhToken, LPCXSTR lpszAddr, bool bSingle)
{
	BTDload_IsErrorOccur = false;

	if (NULL == lpszAddr)
	{
		BTDload_IsErrorOccur = true;
		BTDload_dwErrorCode = ERROR_STORAGE_MODULE_BTORRENT_CREATOR_SEEDS_PARAMENT;
		return false;
	}
	st_Locker.lock_shared();
	unordered_map<XNETHANDLE, BTORRENT_CREATORINFO>::const_iterator stl_MapIterator = stl_MapBTCreator.find(xhToken);
	if (stl_MapIterator == stl_MapBTCreator.end())
	{
		BTDload_IsErrorOccur = true;
		BTDload_dwErrorCode = ERROR_STORAGE_MODULE_BTORRENT_CREATOR_SEEDS_NOTFOUND;
		st_Locker.unlock_shared();
		return false;
	}
	BTORRENT_PARSEMAP st_BTInfo;
	memset(&st_BTInfo, '\0', sizeof(BTORRENT_PARSEMAP));

	st_BTInfo.nValue = bSingle;
	_tcsxcpy(st_BTInfo.tszValue, lpszAddr);
	stl_MapIterator->second.pStl_ListSeeds->push_back(st_BTInfo);
	st_Locker.unlock_shared();
	return true;
}
/********************************************************************
函数名称：BTorrent_Creator_SetInfo
函数功能：设置种子信息
 参数.一：xhToken
  In/Out：In
  类型：句柄
  可空：N
  意思：输入要操作的句柄
 参数.二：lpszCreator
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入制作器名称
 参数.三：lpszComment
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入附加信息
 参数.四：lpszCertBuffer
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入PEM证书缓冲区
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CBTorrent_Creator::BTorrent_Creator_SetInfo(XNETHANDLE xhToken, LPCXSTR lpszCreator, LPCXSTR lpszComment /* = NULL */, LPCXSTR lpszCertBuffer /* = NULL */)
{
	BTDload_IsErrorOccur = false;

	if (NULL == lpszCreator)
	{
		BTDload_IsErrorOccur = true;
		BTDload_dwErrorCode = ERROR_STORAGE_MODULE_BTORRENT_CREATOR_INFO_PARAMENT;
		return false;
	}
	st_Locker.lock_shared();
	unordered_map<XNETHANDLE, BTORRENT_CREATORINFO>::iterator stl_MapIterator = stl_MapBTCreator.find(xhToken);
	if (stl_MapIterator == stl_MapBTCreator.end())
	{
		BTDload_IsErrorOccur = true;
		BTDload_dwErrorCode = ERROR_STORAGE_MODULE_BTORRENT_CREATOR_INFO_NOTFOUND;
		st_Locker.unlock_shared();
		return false;
	}
	_tcsxcpy(stl_MapIterator->second.tszCreate, lpszCreator);
	if (NULL != lpszComment)
	{
		_tcsxcpy(stl_MapIterator->second.tszComment, lpszComment);
	}
	if (NULL != lpszCertBuffer)
	{
		_tcsxcpy(stl_MapIterator->second.tszPEMCert, lpszCertBuffer);
	}
	st_Locker.unlock_shared();
	return true;
}
/********************************************************************
函数名称：BTorrent_Creator_GetTorrent
函数功能：获取中心信息
 参数.一：xhToken
  In/Out：In
  类型：句柄
  可空：N
  意思：输入要操作的句柄
 参数.二：lpszBTFile
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输出种子文件地址
 参数.三：ptszMsgBuffer
  In/Out：Out
  类型：字符指针
  可空：Y
  意思：输出种子信息到内存
 参数.四：pInt_MsgLen
  In/Out：Out
  类型：整数型指针
  可空：Y
  意思：输出内存大小
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CBTorrent_Creator::BTorrent_Creator_GetTorrent(XNETHANDLE xhToken, LPCXSTR lpszBTFile /* = NULL */, XCHAR* ptszMsgBuffer /* = NULL */, int* pInt_MsgLen /* = NULL */)
{
	BTDload_IsErrorOccur = false;

	if ((NULL == lpszBTFile) && (NULL == ptszMsgBuffer))
	{
		BTDload_IsErrorOccur = true;
		BTDload_dwErrorCode = ERROR_STORAGE_MODULE_BTORRENT_CREATOR_FILE_PARAMENT;
		return false;
	}
	st_Locker.lock_shared();
	unordered_map<XNETHANDLE, BTORRENT_CREATORINFO>::iterator stl_MapIterator = stl_MapBTCreator.find(xhToken);
	if (stl_MapIterator == stl_MapBTCreator.end())
	{
		BTDload_IsErrorOccur = true;
		BTDload_dwErrorCode = ERROR_STORAGE_MODULE_BTORRENT_CREATOR_FILE_NOTFOUND;
		st_Locker.unlock_shared();
		return false;
	}
#if 1 == _XENGIEN_STORAGE_BUILDSWITCH_BTORRENT
	lt::file_storage m_BTFile;
	lt::create_flags_t m_BTFlags;
	//种子的标示
	m_BTFlags |= lt::create_torrent::modification_time;
	//解析文件
	lt::add_files(m_BTFile, stl_MapIterator->second.tszBTPath, m_BTFlags);
	if (m_BTFile.num_files() <= 0)
	{
		BTDload_IsErrorOccur = true;
		BTDload_dwErrorCode = ERROR_STORAGE_MODULE_BTORRENT_CREATOR_FILE_PARSE;
		return false;
	}
	//创建BT种子,种子文件解析的数据,块大小,附加信息,种子标志
	lt::create_torrent m_BTCreator(m_BTFile, stl_MapIterator->second.m_nPieceSize, m_BTFlags);
	//给创建起赋值
	m_BTCreator.set_creator(stl_MapIterator->second.tszCreate);
	m_BTCreator.set_comment(stl_MapIterator->second.tszComment);
	m_BTCreator.set_root_cert(stl_MapIterator->second.tszPEMCert);
	//节点添加
	list<BTORRENT_PARSEMAP>::const_iterator stl_ListNodeIterator = stl_MapIterator->second.pStl_ListNode->begin();
	for (; stl_ListNodeIterator != stl_MapIterator->second.pStl_ListNode->end(); stl_MapIterator++)
	{
		m_BTCreator.add_node(make_pair(stl_ListNodeIterator->tszValue, stl_ListNodeIterator->nValue));
	}
	//添加服务
	list<BTORRENT_PARSEMAP>::const_iterator stl_ListTrackerIterator = stl_MapIterator->second.pStl_ListTracker->begin();
	for (; stl_ListTrackerIterator != stl_MapIterator->second.pStl_ListTracker->end(); stl_ListTrackerIterator++)
	{
		m_BTCreator.add_tracker(stl_ListTrackerIterator->tszValue, stl_ListTrackerIterator->nValue);
	}
	//添加SEED服务
	list<BTORRENT_PARSEMAP>::const_iterator stl_ListSeedIterator = stl_MapIterator->second.pStl_ListSeeds->begin();
	for (; stl_ListSeedIterator != stl_MapIterator->second.pStl_ListSeeds->end(); stl_ListSeedIterator++)
	{
		//添加URL种子,单文件为HTTP的URL,多文件为SEED
		if (stl_ListSeedIterator->nValue)
		{
			m_BTCreator.add_http_seed(stl_ListTrackerIterator->tszValue);
		}
		else
		{
			m_BTCreator.add_url_seed(stl_ListTrackerIterator->tszValue);
		}
	}
	//开始打包
	std::vector<char> stl_ListTorrent;
	lt::bencode(back_inserter(stl_ListTorrent), m_BTCreator.generate());
	//写到文件
	if (NULL != lpszBTFile)
	{
		std::fstream m_FStream;
		m_FStream.exceptions(std::ifstream::failbit);
		m_FStream.open(lpszBTFile, std::ios_base::out | std::ios_base::binary);
		if (!m_FStream)
		{
			BTDload_IsErrorOccur = true;
			BTDload_dwErrorCode = ERROR_STORAGE_MODULE_BTORRENT_CREATOR_FILE_FAILED;
			st_Locker.unlock_shared();
			return false;
		}
		m_FStream.write(stl_ListTorrent.data(), stl_ListTorrent.size());
		m_FStream.close();
	}
	//输出到内存
	if (NULL != ptszMsgBuffer)
	{
		*pInt_MsgLen = stl_ListTorrent.size();
		memcpy(ptszMsgBuffer, stl_ListTorrent.data(), *pInt_MsgLen);
	}
#endif
	st_Locker.unlock_shared();

	return true;
}
/********************************************************************
函数名称：BTorrent_Creator_Destory
函数功能：销毁
 参数.一：xhToken
  In/Out：In
  类型：句柄
  可空：N
  意思：输入要操作的句柄
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CBTorrent_Creator::BTorrent_Creator_Destory(XNETHANDLE xhToken)
{
	BTDload_IsErrorOccur = false;

	st_Locker.lock();
	unordered_map<XNETHANDLE, BTORRENT_CREATORINFO>::iterator stl_MapIterator = stl_MapBTCreator.find(xhToken);
	if (stl_MapIterator != stl_MapBTCreator.end())
	{
		stl_MapIterator->second.pStl_ListNode->clear();
		stl_MapIterator->second.pStl_ListSeeds->clear();
		stl_MapIterator->second.pStl_ListTracker->clear();

		delete stl_MapIterator->second.pStl_ListNode;
		delete stl_MapIterator->second.pStl_ListSeeds;
		delete stl_MapIterator->second.pStl_ListTracker;

		stl_MapIterator->second.pStl_ListNode = NULL;
		stl_MapIterator->second.pStl_ListSeeds = NULL;
		stl_MapIterator->second.pStl_ListTracker = NULL;

		stl_MapBTCreator.erase(stl_MapIterator);
	}
	st_Locker.unlock();
	return true;
}