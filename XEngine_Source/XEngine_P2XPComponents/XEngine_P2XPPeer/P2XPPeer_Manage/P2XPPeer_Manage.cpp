#include "pch.h"
#include "P2XPPeer_Manage.h"
/********************************************************************
//    Created:     2021/07/08  13:11:20
//    File Name:   D:\XEngine_Storage\XEngine_Source\XEngine_P2XPComponents\XEngine_P2XPPeer\P2XPPeer_Manage\P2XPPeer_Manage.cpp
//    File Path:   D:\XEngine_Storage\XEngine_Source\XEngine_P2XPComponents\XEngine_P2XPPeer\P2XPPeer_Manage
//    File Base:   P2XPPeer_Manage
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     P2P节点管理器
//    History:
*********************************************************************/
CNetEngine_P2XPPeerManage::CNetEngine_P2XPPeerManage()
{
}
CNetEngine_P2XPPeerManage::~CNetEngine_P2XPPeerManage()
{
}
//////////////////////////////////////////////////////////////////////////
//                          公有函数
//////////////////////////////////////////////////////////////////////////
/********************************************************************
函数名称：P2XPPeer_Manage_Add
函数功能：添加一个客户到一个节点信息
 参数.一：pSt_PeerInfo
  In/Out：In
  类型：结构体指针
  可空：N
  意思：要添加的节点信息
返回值
  类型：逻辑型
  意思：是否添加成功
备注：
*********************************************************************/
BOOL CNetEngine_P2XPPeerManage::P2XPPeer_Manage_Add(XENGINE_P2XP_PEERINFO st_PeerInfo)
{
    PeerManage_IsErrorOccur = FALSE;

    st_rwLocker.lock();
    unordered_map<tstring, LPNETENGINE_P2XP_PEERINFO>::const_iterator stl_MapIterator = stl_MapPeerAddr.find(st_PeerInfo.st_PeerAddr.tszConnectAddr);
    if (stl_MapIterator != stl_MapPeerAddr.end())
    {
        PeerManage_IsErrorOccur = TRUE;
        PeerManage_dwErrorCode = ERROR_XENGINE_P2XP_PEER_MANAGE_ADD_ISEXIST;
        st_rwLocker.unlock();
        return FALSE;
    }
    stl_MapIterator = stl_MapPeerAddr.begin();
    for (;stl_MapIterator != stl_MapPeerAddr.end();stl_MapIterator++)
    {
        if (0 == _tcsncmp(stl_MapIterator->second->st_PeerAddr.tszUserName, st_PeerInfo.st_PeerAddr.tszUserName, _tcslen(stl_MapIterator->second->st_PeerAddr.tszUserName)))
        {
            PeerManage_IsErrorOccur = TRUE;
            PeerManage_dwErrorCode = ERROR_XENGINE_P2XP_PEER_MANAGE_ADD_ISEXIST;
            st_rwLocker.unlock();
            return FALSE;
        }
    }
    //添加到表中
    XENGINE_P2XP_PEERINFO *pSt_PeerInfo = new XENGINE_P2XP_PEERINFO;
    if (NULL == pSt_PeerInfo)
    {
        PeerManage_IsErrorOccur = TRUE;
        PeerManage_dwErrorCode = ERROR_XENGINE_P2XP_PEER_MANAGE_ADD_MALLOC;
        st_rwLocker.unlock();
        return FALSE;
    }
    memcpy(pSt_PeerInfo,&st_PeerInfo,sizeof(XENGINE_P2XP_PEERINFO));

    stl_MapPeerAddr.insert(make_pair(pSt_PeerInfo->st_PeerAddr.tszConnectAddr,pSt_PeerInfo));
    if (!P2XPPeer_Manage_AddLan(st_PeerInfo.st_PeerAddr))
    {
        st_rwLocker.unlock();
        return FALSE;
    }
    st_rwLocker.unlock();
    return TRUE;
}
/********************************************************************
函数名称：P2XPPeer_Manage_Get
函数功能：获取P2P节点对应信息
 参数.一：lpszAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：要查询的地址
 参数.二：pSt_PeerInfo
  In/Out：Out
  类型：数据结构指针
  可空：Y
  意思：如果为空将不导出对应的数据
返回值
  类型：逻辑型
  意思：是否查找成功
备注：
*********************************************************************/
BOOL CNetEngine_P2XPPeerManage::P2XPPeer_Manage_Get(LPCTSTR lpszAddr,XENGINE_P2XP_PEERINFO *pSt_PeerInfo /* = NULL */)
{
    PeerManage_IsErrorOccur = FALSE;

    if (NULL == lpszAddr)
    {
        PeerManage_IsErrorOccur = TRUE;
        PeerManage_dwErrorCode = ERROR_XENGINE_P2XP_PEER_MANAGE_GET_PARAMENT;
        return FALSE;
    }
    //查找元素
    st_rwLocker.lock_shared();
    unordered_map<tstring,LPNETENGINE_P2XP_PEERINFO>::const_iterator stl_MapIterator = stl_MapPeerAddr.find(lpszAddr);
    if (stl_MapIterator == stl_MapPeerAddr.end())
    {
        PeerManage_IsErrorOccur = TRUE;
        PeerManage_dwErrorCode = ERROR_XENGINE_P2XP_PEER_MANAGE_GET_NOTFOUND;
        st_rwLocker.unlock_shared();
        return FALSE;
    }
    //判断是否导出节点信息
    if (NULL != pSt_PeerInfo)
    {
        memcpy(pSt_PeerInfo,stl_MapIterator->second,sizeof(XENGINE_P2XP_PEERINFO));
    }
    st_rwLocker.unlock_shared();
    return TRUE;
}
/********************************************************************
函数名称：P2XPPeer_Manage_GetUser
函数功能：通过用户名获取节点信息
 参数.一：lpszUser
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：要搜索的用户的 标识符
 参数.二：pSt_PeerInfo
  In/Out：Out
  类型：数据结构指针
  可空：Y
  意思：导出获取到的信息
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CNetEngine_P2XPPeerManage::P2XPPeer_Manage_GetUser(LPCTSTR lpszUser, XENGINE_P2XP_PEERINFO *pSt_PeerInfo /* = NULL */)
{
    PeerManage_IsErrorOccur = FALSE;

    if (NULL == lpszUser)
    {
        PeerManage_IsErrorOccur = TRUE;
        PeerManage_dwErrorCode = ERROR_XENGINE_P2XP_PEER_MANAGE_GETUSER_PARAMENT;
        return FALSE;
    }
    //查找元素
    st_rwLocker.lock_shared();
    BOOL bIsFound = FALSE;
    unordered_map<tstring, LPNETENGINE_P2XP_PEERINFO>::const_iterator stl_MapIterator = stl_MapPeerAddr.begin();
    for (;stl_MapIterator != stl_MapPeerAddr.end();stl_MapIterator++)
    {
        if (0 == _tcsncmp(lpszUser,stl_MapIterator->second->st_PeerAddr.tszUserName,_tcslen(lpszUser)))
        {
            bIsFound = TRUE;
            break;
        }
    }
    //是否找到用户
    if (!bIsFound)
    {
        PeerManage_IsErrorOccur = TRUE;
        PeerManage_dwErrorCode = ERROR_XENGINE_P2XP_PEER_MANAGE_GETUSER_NOTFOUND;
        st_rwLocker.unlock_shared();
        return FALSE;
    }

    //判断是否导出节点信息
    if (NULL != pSt_PeerInfo)
    {
        memcpy(pSt_PeerInfo, stl_MapIterator->second, sizeof(XENGINE_P2XP_PEERINFO));
        //pSt_PeerInfo = stl_MapIterator->second;
    }
    st_rwLocker.unlock_shared();
    return TRUE;
}
/********************************************************************
函数名称：P2XPPeer_Manage_GetLan
函数功能：获取局域网IP地址列表
 参数.一：lpszPubAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：要查找的局域网相同公网的IP地址
 参数.二：lpszPriAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要获取同一公网下不同私有网段的IP列表
 参数.三：pppSt_P2XPClient
  In/Out：Out
  类型：三级指针
  可空：N
  意思：导出局域网IP地址列表
 参数.四：pInt_ListCount
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出地址列表个数
返回值
  类型：逻辑型
  意思：是否成功
备注：参数三需要调用基础库的BaseLib_OperatorMemory_Free函数进行内存释放
*********************************************************************/
BOOL CNetEngine_P2XPPeerManage::P2XPPeer_Manage_GetLan(LPCTSTR lpszPubAddr, LPCTSTR lpszPriAddr, XENGINE_P2XPPEER_PROTOCOL*** pppSt_P2XPClient, int* pInt_ListCount)
{
    PeerManage_IsErrorOccur = FALSE;

    if ((NULL == lpszPubAddr) || (NULL == lpszPriAddr) || (NULL == pppSt_P2XPClient) || (NULL == pInt_ListCount))
    {
        PeerManage_IsErrorOccur = TRUE;
        PeerManage_dwErrorCode = ERROR_XENGINE_P2XP_PEER_MANAGE_GETLAN_PARAMENT;
        return FALSE;
    }
    //查找公网IP地址
    st_rwLocker.lock_shared();
    unordered_map<tstring, unordered_map<tstring, list<XENGINE_P2XPPEER_PROTOCOL> > >::const_iterator stl_MapIterator = stl_MapClients.find(lpszPubAddr);
    if (stl_MapIterator == stl_MapClients.end())
    {
        PeerManage_IsErrorOccur = TRUE;
        PeerManage_dwErrorCode = ERROR_XENGINE_P2XP_PEER_MANAGE_GETLAN_NOTPUBADDRR;
        st_rwLocker.unlock_shared();
        return FALSE;
    }
    XENGINE_LIBADDR st_LibAddr;
    TCHAR tszPrivateAddr[64];

    memset(tszPrivateAddr, '\0', sizeof(tszPrivateAddr));
    memset(&st_LibAddr, '\0', sizeof(XENGINE_LIBADDR));
    //如果输入的是192.168.1.1 IP,就我们自己分割,否则认为输入的是 192.168.1
    if (BaseLib_OperatorIPAddr_IsIPV4Addr(lpszPriAddr, &st_LibAddr))
    {
        _stprintf_s(tszPrivateAddr, _T("%d.%d.%d"), st_LibAddr.nIPAddr1, st_LibAddr.nIPAddr2, st_LibAddr.nIPAddr3);
        unordered_map<tstring, list<XENGINE_P2XPPEER_PROTOCOL> >::const_iterator stl_MapSubIterator = stl_MapIterator->second.find(tszPrivateAddr);
        if (stl_MapSubIterator == stl_MapIterator->second.end())
        {
            PeerManage_IsErrorOccur = TRUE;
            PeerManage_dwErrorCode = ERROR_XENGINE_P2XP_PEER_MANAGE_GETLAN_NOTPRIADDR;
            st_rwLocker.unlock_shared();
            return FALSE;
        }
        if (stl_MapSubIterator->second.empty())
        {
            PeerManage_IsErrorOccur = TRUE;
            PeerManage_dwErrorCode = ERROR_XENGINE_P2XP_PEER_MANAGE_GETLAN_NOLIST;
            st_rwLocker.unlock_shared();
            return FALSE;
        }
		BaseLib_OperatorMemory_Malloc((XPPPMEM)pppSt_P2XPClient, stl_MapSubIterator->second.size(), sizeof(XENGINE_P2XPPEER_PROTOCOL));
		list<XENGINE_P2XPPEER_PROTOCOL>::const_iterator stl_ListIterator = stl_MapSubIterator->second.begin();
		for (int i = 0; stl_ListIterator != stl_MapSubIterator->second.end(); stl_ListIterator++, i++)
		{
			*(*pppSt_P2XPClient)[i] = *stl_ListIterator;
		}
		*pInt_ListCount = stl_MapSubIterator->second.size();
    }
    else
    {
        unordered_map < tstring, list<XENGINE_P2XPPEER_PROTOCOL> >::const_iterator stl_MapSubIterator = stl_MapIterator->second.find(lpszPriAddr);
        if (stl_MapSubIterator == stl_MapIterator->second.end())
        {
            PeerManage_IsErrorOccur = TRUE;
            PeerManage_dwErrorCode = ERROR_XENGINE_P2XP_PEER_MANAGE_GETLAN_NOTPRIADDR;
            st_rwLocker.unlock_shared();
            return FALSE;
        }
        if (stl_MapSubIterator->second.empty())
        {
            PeerManage_IsErrorOccur = TRUE;
            PeerManage_dwErrorCode = ERROR_XENGINE_P2XP_PEER_MANAGE_GETLAN_NOLIST;
            st_rwLocker.unlock_shared();
            return FALSE;
        }
		BaseLib_OperatorMemory_Malloc((XPPPMEM)pppSt_P2XPClient, stl_MapSubIterator->second.size(), sizeof(XENGINE_P2XPPEER_PROTOCOL));
		list<XENGINE_P2XPPEER_PROTOCOL>::const_iterator stl_ListIterator = stl_MapSubIterator->second.begin();
		for (int i = 0; stl_ListIterator != stl_MapSubIterator->second.end(); stl_ListIterator++, i++)
		{
			*(*pppSt_P2XPClient)[i] = *stl_ListIterator;
		}
		*pInt_ListCount = stl_MapSubIterator->second.size();
    }
    st_rwLocker.unlock_shared();
    return TRUE;
}
/********************************************************************
函数名称：P2XPPeer_Manage_GetLList
函数功能：获取同一公网下所有局域网主IP列表
 参数.一：lpszPubAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要获取的IP地址
 参数.二：pppszP2XPClient
  In/Out：Out
  类型：三级指针
  可空：N
  意思：输出内网主IP地址列表
 参数.三：pInt_ListCount
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出列表个数
返回值
  类型：逻辑型
  意思：是否成功
备注：参数二需要调用基础库的BaseLib_OperatorMemory_Free函数进行内存释放
*********************************************************************/
BOOL CNetEngine_P2XPPeerManage::P2XPPeer_Manage_GetLList(LPCTSTR lpszPubAddr, TCHAR*** pppszP2XPClient, int* pInt_ListCount)
{
    PeerManage_IsErrorOccur = FALSE;

    if ((NULL == lpszPubAddr) || (NULL == pppszP2XPClient) || (NULL == pInt_ListCount))
    {
        PeerManage_IsErrorOccur = TRUE;
        PeerManage_dwErrorCode = ERROR_XENGINE_P2XP_PEER_MANAGE_GETLLIST_PARAMENT;
        return FALSE;
    }
    st_rwLocker.lock_shared();
    unordered_map<tstring, unordered_map<tstring, list<XENGINE_P2XPPEER_PROTOCOL> > >::const_iterator stl_MapIterator = stl_MapClients.find(lpszPubAddr);
    if (stl_MapIterator == stl_MapClients.end())
    {
        PeerManage_IsErrorOccur = TRUE;
        PeerManage_dwErrorCode = ERROR_XENGINE_P2XP_PEER_MANAGE_GETLLIST_NOTFOUND;
        st_rwLocker.unlock_shared();
        return FALSE;
    }
    BaseLib_OperatorMemory_Malloc((XPPPMEM)pppszP2XPClient, stl_MapIterator->second.size(), 128);
    unordered_map<tstring, list<XENGINE_P2XPPEER_PROTOCOL> >::const_iterator stl_MapListIterator = stl_MapIterator->second.begin();
    for (int i = 0; stl_MapListIterator != stl_MapIterator->second.end(); stl_MapListIterator++, i++)
    {
        _tcscpy((*pppszP2XPClient)[i], stl_MapListIterator->first.c_str());
    }
    *pInt_ListCount = stl_MapIterator->second.size();
    st_rwLocker.unlock_shared();
    return TRUE;
}
/********************************************************************
函数名称：P2XPPeer_Manage_GetWList
函数功能：获取所有连接到的公网地址列表
 参数.一：pppszP2XPClient
  In/Out：Out
  类型：三级指针
  可空：N
  意思：输出地址列表
 参数.二：pInt_ListCount
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出列表个数
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CNetEngine_P2XPPeerManage::P2XPPeer_Manage_GetWList(TCHAR*** pppszP2XPClient, int* pInt_ListCount)
{
	PeerManage_IsErrorOccur = FALSE;

	if ((NULL == pppszP2XPClient) || (NULL == pInt_ListCount))
	{
		PeerManage_IsErrorOccur = TRUE;
		PeerManage_dwErrorCode = ERROR_XENGINE_P2XP_PEER_MANAGE_GETWLIST_PARAMENT;
		return FALSE;
	}
	if (stl_MapClients.size() <= 0)
	{
		PeerManage_IsErrorOccur = TRUE;
		PeerManage_dwErrorCode = ERROR_XENGINE_P2XP_PEER_MANAGE_GETWLIST_NOTFOUND;
		return FALSE;
	}
	BaseLib_OperatorMemory_Malloc((XPPPMEM)pppszP2XPClient, stl_MapClients.size(), 128);

	st_rwLocker.lock_shared();
	unordered_map<tstring, unordered_map<tstring, list<XENGINE_P2XPPEER_PROTOCOL> > >::const_iterator stl_MapIterator = stl_MapClients.begin();
    for (int i = 0; stl_MapIterator != stl_MapClients.end(); stl_MapIterator++, i++)
    {
        _tcscpy((*pppszP2XPClient)[i], stl_MapIterator->first.c_str());
    }
	*pInt_ListCount = stl_MapClients.size();
    st_rwLocker.unlock_shared();
	return TRUE;
}
/********************************************************************
函数名称：P2XPPeer_Manage_Set
函数功能：设置指定客户的节点信息
 参数.一：lpszAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：要设置的客户
 参数.二：st_PeerInfo
  In/Out：In
  类型：结构体
  可空：N
  意思：要设置的节点信息
返回值
  类型：逻辑型
  意思：是否设置成功
备注：
*********************************************************************/
BOOL CNetEngine_P2XPPeerManage::P2XPPeer_Manage_Set(LPCTSTR lpszAddr,XENGINE_P2XP_PEERINFO st_PeerInfo)
{
    PeerManage_IsErrorOccur = FALSE;

    if (NULL == lpszAddr)
    {
        PeerManage_IsErrorOccur = TRUE;
        PeerManage_dwErrorCode = ERROR_XENGINE_P2XP_PEER_MANAGE_SET_PARAMENT;
        return FALSE;
    }
    st_rwLocker.lock_shared();
    //看下有没这个客户存在
    unordered_map<tstring,LPNETENGINE_P2XP_PEERINFO>::const_iterator stl_MapIterator = stl_MapPeerAddr.find(lpszAddr);
    if (stl_MapIterator == stl_MapPeerAddr.end())
    {
        PeerManage_IsErrorOccur = TRUE;
        PeerManage_dwErrorCode = ERROR_XENGINE_P2XP_PEER_MANAGE_SET_NOTFOUND;
        st_rwLocker.unlock_shared();
        return FALSE;
    }
    memcpy(stl_MapIterator->second,&st_PeerInfo,sizeof(st_PeerInfo));
    st_rwLocker.unlock_shared();
    return TRUE;
}
/********************************************************************
函数名称：P2XPPeer_Manage_Delete
函数功能：删除一个指定的节点
 参数.一：lpszClientAddr
  In/Out：In
  类型：字符串指针
  可空：N
  意思：要删除的节点地址
返回值
  类型：逻辑型
  意思：是否成功删除
备注：
*********************************************************************/
BOOL CNetEngine_P2XPPeerManage::P2XPPeer_Manage_Delete(LPCTSTR lpszClientAddr)
{
    PeerManage_IsErrorOccur = FALSE;

    if (NULL == lpszClientAddr)
    {
        PeerManage_IsErrorOccur = TRUE;
        PeerManage_dwErrorCode = ERROR_XENGINE_P2XP_PEER_MANAGE_DELETE_PARAMENT;
        return FALSE;
    }
    st_rwLocker.lock();
    unordered_map<tstring,LPNETENGINE_P2XP_PEERINFO>::iterator stl_MapIterator = stl_MapPeerAddr.find(lpszClientAddr);
    if (stl_MapIterator == stl_MapPeerAddr.end())
    {
        PeerManage_IsErrorOccur = TRUE;
        PeerManage_dwErrorCode = ERROR_XENGINE_P2XP_PEER_MANAGE_DELETE_NOTFOUND;
        st_rwLocker.unlock();
        return FALSE;
    }
    P2XPPeer_Manage_DelLan(stl_MapIterator->second->st_PeerAddr);

    delete stl_MapIterator->second;
    stl_MapIterator->second = NULL;
    stl_MapPeerAddr.erase(stl_MapIterator);
    st_rwLocker.unlock();
    return TRUE;
}

/********************************************************************
函数名称：P2XPPeer_Manage_DelAll
函数功能：清空节点列表数据
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CNetEngine_P2XPPeerManage::P2XPPeer_Manage_DelAll()
{
    PeerManage_IsErrorOccur = FALSE;

    st_rwLocker.lock();
    unordered_map<tstring, LPNETENGINE_P2XP_PEERINFO>::iterator stl_MapIterator = stl_MapPeerAddr.begin();
    for (;stl_MapIterator != stl_MapPeerAddr.end();stl_MapIterator++)
    {
        delete stl_MapIterator->second;
        stl_MapIterator->second = NULL;
    }
    stl_MapPeerAddr.clear();
    stl_MapClients.clear();
    st_rwLocker.unlock();

    return TRUE;
}
/********************************************************************
函数名称：P2XPPeer_Manage_GetCount
函数功能：获取节点个数
 参数.一：pInt_PeerCount
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：导出获取到的个数
返回值
  类型：逻辑型
  意思：是否成功获取到节点个数
备注：
*********************************************************************/
BOOL CNetEngine_P2XPPeerManage::P2XPPeer_Manage_GetCount(int *pInt_PeerCount)
{
    PeerManage_IsErrorOccur = FALSE;

    *pInt_PeerCount = stl_MapPeerAddr.size();

    return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////
//                                      保护函数
///////////////////////////////////////////////////////////////////////////////////////
/********************************************************************
函数名称：P2XPPeer_Manage_AddLan
函数功能：添加一个客户到一个局域网IP地址列表中
 参数.一：st_ClientAddr
  In/Out：In
  类型：结构体
  可空：N
  意思：要添加的节点信息
返回值
  类型：逻辑型
  意思：是否添加成功
备注：
*********************************************************************/
BOOL CNetEngine_P2XPPeerManage::P2XPPeer_Manage_AddLan(XENGINE_P2XPPEER_PROTOCOL st_ClientAddr)
{
    PeerManage_IsErrorOccur = FALSE;

    unordered_map<tstring, unordered_map<tstring, list<XENGINE_P2XPPEER_PROTOCOL> > >::iterator stl_MapIteratorAddr = stl_MapClients.find(st_ClientAddr.tszPublicAddr);
    if (stl_MapIteratorAddr == stl_MapClients.end())
    {
        //没有找到,一个一个加入,首先加入私有同步网络
        list<XENGINE_P2XPPEER_PROTOCOL> stl_ListSecond;
        stl_ListSecond.push_back(st_ClientAddr);
        //获取私有网络的路由地址
        XENGINE_LIBADDR st_LibAddr;
        TCHAR tszPrivateAddr[64];

        memset(tszPrivateAddr, '\0', sizeof(tszPrivateAddr));
        memset(&st_LibAddr, '\0', sizeof(XENGINE_LIBADDR));
        if (!BaseLib_OperatorIPAddr_IsIPV4Addr(st_ClientAddr.tszPrivateAddr, &st_LibAddr))
        {
            //没有内网地址不加入.
            PeerManage_IsErrorOccur = TRUE;
            PeerManage_dwErrorCode = BaseLib_GetLastError();
            return FALSE;
        }
        _stprintf_s(tszPrivateAddr, _T("%d.%d.%d"), st_LibAddr.nIPAddr1, st_LibAddr.nIPAddr2, st_LibAddr.nIPAddr3);
        //然后加入到
        unordered_map<tstring, list<XENGINE_P2XPPEER_PROTOCOL> > stl_MapSecond;
        stl_MapSecond.insert(make_pair(tszPrivateAddr, stl_ListSecond));
        stl_MapClients.insert(make_pair(st_ClientAddr.tszPublicAddr, stl_MapSecond));
    }
    else
    {
        //找到了这个公网的地址
        XENGINE_LIBADDR st_LibAddr;
        TCHAR tszPrivateAddr[64];

        memset(tszPrivateAddr, '\0', sizeof(tszPrivateAddr));
        memset(&st_LibAddr, '\0', sizeof(XENGINE_LIBADDR));
        //获取私有网络的路由地址
        if (!BaseLib_OperatorIPAddr_IsIPV4Addr(st_ClientAddr.tszPrivateAddr, &st_LibAddr))
        {
            //没有内网地址不加入.
            PeerManage_IsErrorOccur = TRUE;
            PeerManage_dwErrorCode = BaseLib_GetLastError();
            return FALSE;
        }
        //内网地址192.168.1.2  内网出口192.168.1.1 链接地址10.0.1.0:13444
        _stprintf_s(tszPrivateAddr, _T("%d.%d.%d"), st_LibAddr.nIPAddr1, st_LibAddr.nIPAddr2, st_LibAddr.nIPAddr3);
        unordered_map<tstring, list<XENGINE_P2XPPEER_PROTOCOL> >::iterator stl_MapIteartorPrivate = stl_MapIteratorAddr->second.find(tszPrivateAddr);
        if (stl_MapIteartorPrivate == stl_MapIteratorAddr->second.end())
        {
            list<XENGINE_P2XPPEER_PROTOCOL> stl_ListClient;
            stl_ListClient.push_back(st_ClientAddr);
            stl_MapIteratorAddr->second.insert(make_pair(tszPrivateAddr, stl_ListClient));
        }
        else
        {
            BOOL bIsFound = FALSE;
            //查找链接地址是否存在.存在表示已经加入过了,那么就什么都不做
            list<XENGINE_P2XPPEER_PROTOCOL>::iterator stl_ListIterator = stl_MapIteartorPrivate->second.begin();
            for (;stl_ListIterator != stl_MapIteartorPrivate->second.end();stl_ListIterator++)
            {
                if (0 == _tcscmp(stl_ListIterator->tszConnectAddr, st_ClientAddr.tszConnectAddr))
                {
                    bIsFound = TRUE;
                    break;
                }
            }
            //没有找到就加入
            if (!bIsFound)
            {
                stl_MapIteartorPrivate->second.push_back(st_ClientAddr);
            }
        }
    }

    return TRUE;
}
/********************************************************************
函数名称：P2XPPeer_Manage_DelLan
函数功能：从局域网列表中删除一个地址
 参数.一：st_ClientAddr
  In/Out：In
  类型：数据结构
  可空：N
  意思：要删除的节点
返回值
  类型：逻辑型
  意思：是否成功删除
备注：
*********************************************************************/
BOOL CNetEngine_P2XPPeerManage::P2XPPeer_Manage_DelLan(XENGINE_P2XPPEER_PROTOCOL st_ClientAddr)
{
    PeerManage_IsErrorOccur = FALSE;
    //查找公网地址
    unordered_map<tstring, unordered_map<tstring, list<XENGINE_P2XPPEER_PROTOCOL> > >::iterator stl_MapPubIteartor = stl_MapClients.find(st_ClientAddr.tszPublicAddr);
    if (stl_MapPubIteartor != stl_MapClients.end())
    {
        XENGINE_LIBADDR st_LibAddr;
        TCHAR tszPrivateAddr[64];

        memset(tszPrivateAddr, '\0', sizeof(tszPrivateAddr));
        memset(&st_LibAddr, '\0', sizeof(XENGINE_LIBADDR));
        if (BaseLib_OperatorIPAddr_IsIPV4Addr(st_ClientAddr.tszPrivateAddr, &st_LibAddr))
        {
            _stprintf_s(tszPrivateAddr, _T("%d.%d.%d"), st_LibAddr.nIPAddr1, st_LibAddr.nIPAddr2, st_LibAddr.nIPAddr3);
            //查找私有路由地址
            unordered_map<tstring, list<XENGINE_P2XPPEER_PROTOCOL> >::iterator stl_MapSubIterator = stl_MapPubIteartor->second.find(tszPrivateAddr);
            if (stl_MapSubIterator != stl_MapPubIteartor->second.end())
            {
                //查找局域网自身的IP地址是否存在
                list<XENGINE_P2XPPEER_PROTOCOL>::iterator stl_ListIterator = stl_MapSubIterator->second.begin();
                for (;stl_ListIterator != stl_MapSubIterator->second.end();stl_ListIterator++)
                {
                    if (0 == _tcscmp(stl_ListIterator->tszConnectAddr, st_ClientAddr.tszConnectAddr))
                    {
                        //找到了删除
                        stl_MapSubIterator->second.erase(stl_ListIterator);
                        break;
                    }
                }
                if (stl_MapSubIterator->second.empty())
                {
                    //如果这个私有网络列表为空,那么就清楚他
                    stl_MapPubIteartor->second.erase(stl_MapSubIterator);
                }
            }
        }
        if (stl_MapPubIteartor->second.empty())
        {
            stl_MapClients.erase(stl_MapPubIteartor);        //如果这个公有网络IP地址列表为空,那么就删除他.
        }
    }
    return TRUE;
}
