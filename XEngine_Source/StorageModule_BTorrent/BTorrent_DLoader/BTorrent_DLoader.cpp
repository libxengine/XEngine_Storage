#include "pch.h"
#include "BTorrent_DLoader.h"
/********************************************************************
//    Created:     2023/02/02  16:26:33
//    File Name:   D:\XEngine\XEngine_SourceCode\XEngine_NetHelp\NetHelp_BTorrent\BTorrent_DLoader\BTorrent_DLoader.cpp
//    File Path:   D:\XEngine\XEngine_SourceCode\XEngine_NetHelp\NetHelp_BTorrent\BTorrent_DLoader
//    File Base:   BTorrent_DLoader
//    File Ext:    cpp
//    Project:     XEngine
//    Author:      qyt
//    Purpose:     BT下载器
//    History:
*********************************************************************/
CBTorrent_DLoader::CBTorrent_DLoader()
{
}
CBTorrent_DLoader::~CBTorrent_DLoader()
{
}
//////////////////////////////////////////////////////////////////////////
//                        公有函数
//////////////////////////////////////////////////////////////////////////
/********************************************************************
函数名称：BTorrent_DLoader_Create
函数功能：创建一个BT下载
 参数.一：lpszAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：下载的地址,可以是本地种子文件路径或者磁力链接
 参数.二：lpszSavePath
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：要保存的地址,这个地址是一个本地文件夹.而不是文件名.
 参数.三：lpszTempFile
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：断点续传临时文件路径
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
XHANDLE CBTorrent_DLoader::BTorrent_DLoader_Create(LPCXSTR lpszAddr, LPCXSTR lpszSavePath, LPCXSTR lpszTempFile /* = NULL */)
{
    BTDload_IsErrorOccur = false;

    if ((NULL == lpszAddr) || (NULL == lpszSavePath))
    {
        BTDload_IsErrorOccur = true;
        BTDload_dwErrorCode = ERROR_STORAGE_MODULE_BTORRENT_DLOAD_CREATE_PARAMENT;
        return NULL;
    }
	//申请空间
	BTORRENT_DLOADINFO* pSt_BTDLoader = new BTORRENT_DLOADINFO;
	if (NULL == pSt_BTDLoader)
	{
		BTDload_IsErrorOccur = true;
		BTDload_dwErrorCode = ERROR_STORAGE_MODULE_BTORRENT_DLOAD_CREATE_MALLOC;
		return NULL;
	}

#if 1 == _XENGIEN_STORAGE_BUILDSWITCH_BTORRENT
	//初始化要查询的事件
	pSt_BTDLoader->m_BTSetPack.set_int(lt::settings_pack::alert_mask, lt::alert_category::error | lt::alert_category::storage | lt::alert_category::status | lt::alert_category::port_mapping);
	pSt_BTDLoader->m_BTSession = std::make_shared<lt::session>(pSt_BTDLoader->m_BTSetPack);

	lt::add_torrent_params m_TorrentParam;
	//是磁力链接还是BT种子
	LPCXSTR lpszBTType = _X("magnet:?xt=urn:");
	if (NULL == _tcsxstr(lpszAddr, lpszBTType))
	{
		pSt_BTDLoader->m_BTParam.ti = std::make_shared<lt::torrent_info>(std::string(lpszAddr));
	}
	else
	{
		pSt_BTDLoader->m_BTParam = lt::parse_magnet_uri(lpszAddr);
	}
	//如果是续传，那么判断文件是否存在！
	if (NULL != lpszTempFile)
	{
		pSt_BTDLoader->bIsResume = true;
		std::ifstream m_IFStream(lpszTempFile, std::ios_base::binary);
		m_IFStream.unsetf(std::ios_base::skipws);

		std::vector<char> stl_VecFile;
		stl_VecFile = { std::istream_iterator<char>(m_IFStream), std::istream_iterator<char>() };
		if (stl_VecFile.size() > 0)
		{
			m_TorrentParam = lt::read_resume_data(stl_VecFile);
			if (m_TorrentParam.info_hashes == pSt_BTDLoader->m_BTParam.info_hashes)
			{
				pSt_BTDLoader->m_BTParam = std::move(m_TorrentParam);
			}
		}
	}
    pSt_BTDLoader->m_BTParam.save_path = lpszSavePath;
    pSt_BTDLoader->m_BTSession->async_add_torrent(std::move(pSt_BTDLoader->m_BTParam));
    //设置选项
    pSt_BTDLoader->lPClass = this;
    _tcsxcpy(pSt_BTDLoader->tszAddr, lpszAddr);
	_tcsxcpy(pSt_BTDLoader->tszFile, lpszSavePath);
#endif
    return pSt_BTDLoader;
}
/********************************************************************
函数名称：BTorrent_DLoader_Query
函数功能：查询下载状态
 参数.一：xhToken
  In/Out：In
  类型：句柄
  可空：N
  意思：输入要操作的句柄
 参数.二：pppenEventList
  In/Out：Out
  类型：三级指针
  可空：N
  意思：输出事件列表
 参数.三：pInt_ListCount
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出列表个数
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CBTorrent_DLoader::BTorrent_DLoader_Query(XHANDLE xhToken, ENUM_BTORRENT_EVENT_TYPE*** pppenEventList, int* pInt_ListCount)
{
	BTDload_IsErrorOccur = false;

	if ((NULL == pppenEventList) || (NULL == pInt_ListCount))
	{
		BTDload_IsErrorOccur = true;
		BTDload_dwErrorCode = ERROR_STORAGE_MODULE_BTORRENT_DLOAD_QUERY_PARAMENT;
		return false;
	}
	BTORRENT_DLOADINFO* pSt_BTDLoader = (BTORRENT_DLOADINFO*)xhToken;
	if (NULL == pSt_BTDLoader)
	{
		BTDload_IsErrorOccur = true;
		BTDload_dwErrorCode = ERROR_STORAGE_MODULE_BTORRENT_DLOAD_QUERY_NOTFOUND;
		return false;
	}
#if 1 == _XENGIEN_STORAGE_BUILDSWITCH_BTORRENT
	//投教一个状态更新请求,才能触发信号
	pSt_BTDLoader->m_BTSession->post_torrent_updates();
	//得到信号
	std::vector<lt::alert*> stl_VectorAlerts;
	pSt_BTDLoader->m_BTSession->pop_alerts(&stl_VectorAlerts);
	if (stl_VectorAlerts.empty())
	{
		BTDload_IsErrorOccur = true;
		BTDload_dwErrorCode = ERROR_STORAGE_MODULE_BTORRENT_DLOAD_QUERY_NONE;
		return false;
	}
	*pInt_ListCount = stl_VectorAlerts.size();
	BaseLib_Memory_Malloc((XPPPMEM)pppenEventList, stl_VectorAlerts.size(), sizeof(ENUM_BTORRENT_EVENT_TYPE));
	//轮训元素
	int i = 0;
	for (lt::alert const* stl_ListElement : stl_VectorAlerts)
	{
		//是否接受到完成的信号
		if (lt::alert_cast<lt::torrent_finished_alert>(stl_ListElement))
		{
			*((*pppenEventList)[i]) = ENUM_BTORRENT_EVENT_TYPE_COMPLETE;
		}
		//是否接受到错误的信号
		if (lt::alert_cast<lt::torrent_error_alert>(stl_ListElement))
		{
			//std::cout << stl_ListElement->message() << std::endl;
			*((*pppenEventList)[i]) = ENUM_BTORRENT_EVENT_TYPE_ERROR;
		}
		//接受到临时文件保存请求,需要保存他
		if (auto m_BTDLoader = lt::alert_cast<lt::save_resume_data_alert>(stl_ListElement))
		{
			*((*pppenEventList)[i]) = ENUM_BTORRENT_EVENT_TYPE_WRITETMP;
			std::ofstream m_OSFile(pSt_BTDLoader->tszTemp, std::ios_base::binary);
			m_OSFile.unsetf(std::ios_base::skipws);
			auto const m_BTData = write_resume_data_buf(m_BTDLoader->params);
			m_OSFile.write(m_BTData.data(), int(m_BTData.size()));
		}
		//接受到数据写入错误
		if (lt::alert_cast<lt::save_resume_data_failed_alert>(stl_ListElement))
		{
			*((*pppenEventList)[i]) = ENUM_BTORRENT_EVENT_TYPE_DATAERROR;
		}
		//UPNP信息
		if (lt::alert_cast<lt::portmap_error_alert>(stl_ListElement))
		{
			//stl_ListElement->message().c_str();
			*((*pppenEventList)[i]) = ENUM_BTORRENT_EVENT_TYPE_UPNPERROR;
		}
		else if (lt::alert_cast<lt::portmap_alert>(stl_ListElement))
		{
			*((*pppenEventList)[i]) = ENUM_BTORRENT_EVENT_TYPE_UPNPCOMPLETE;
		}
		//下载状态通知
		if (auto stl_VectorUPDate = lt::alert_cast<lt::state_update_alert>(stl_ListElement))
		{
			if (stl_VectorUPDate->status.empty())
			{
				continue;
			}
			*((*pppenEventList)[i]) = ENUM_BTORRENT_EVENT_TYPE_STATUS;
			//更新通知,查询状态
			std::vector<lt::torrent_status>::const_iterator stl_VectorIterator = stl_VectorUPDate->status.begin();
			for (int i = 0; stl_VectorIterator != stl_VectorUPDate->status.end(); stl_VectorIterator++, i++)
			{
#if _MSC_BUILD
				int nUTFLen = stl_VectorIterator->name.length();
				BaseLib_Charset_UTFToAnsi(stl_VectorIterator->name.c_str(), pSt_BTDLoader->st_DLStatus.tszFileName, &nUTFLen);
				pSt_BTDLoader->st_DLStatus.nDLoadTotal = stl_VectorIterator->total;
#else
				_tcsxcpy(pSt_BTDLoader->st_DLStatus.tszFileName, stl_VectorIterator->name.c_str());
				pSt_BTDLoader->st_DLStatus.nDLoadTotal = stl_VectorIterator->total_wanted;
#endif
				pSt_BTDLoader->st_DLStatus.nDLCount = stl_VectorIterator->total_done;
				pSt_BTDLoader->st_DLStatus.nUPCount = stl_VectorIterator->total_upload;
				pSt_BTDLoader->st_DLStatus.nDLStatus = stl_VectorIterator->state;
				pSt_BTDLoader->st_DLStatus.nDLoadRate = stl_VectorIterator->download_payload_rate;
				pSt_BTDLoader->st_DLStatus.nDLPeers = stl_VectorIterator->num_peers;
				pSt_BTDLoader->st_DLStatus.nUPPeers = stl_VectorIterator->num_seeds;
				pSt_BTDLoader->st_DLStatus.nDLoadProcess = stl_VectorIterator->progress_ppm / 10000;
			}
		}
		i++;
	}
#endif
	return true;
}
/********************************************************************
函数名称：BTorrent_DLoader_GetStatus
函数功能：获取下载状态
 参数.一：xhToken
  In/Out：In
  类型：句柄
  可空：N
  意思：输入要操作的句柄
 参数.二：pSt_DLStatus
  In/Out：Out
  类型：数据结构指针
  可空：N
  意思：输出查到的下载情况
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CBTorrent_DLoader::BTorrent_DLoader_GetStatus(XHANDLE xhToken, BTORRENT_DLOADER* pSt_DLStatus)
{
	BTDload_IsErrorOccur = false;

	if (NULL == pSt_DLStatus)
	{
		BTDload_IsErrorOccur = true;
		BTDload_dwErrorCode = ERROR_STORAGE_MODULE_BTORRENT_DLOAD_GETSTATUS_PARAMENT;
		return false;
	}
	BTORRENT_DLOADINFO* pSt_BTDLoader = (BTORRENT_DLOADINFO*)xhToken;
	if (NULL == pSt_BTDLoader)
	{
		BTDload_IsErrorOccur = true;
		BTDload_dwErrorCode = ERROR_STORAGE_MODULE_BTORRENT_DLOAD_GETSTATUS_NOTFOUND;
		return false;
	}
	*pSt_DLStatus = pSt_BTDLoader->st_DLStatus;

	return true;
}
/********************************************************************
函数名称：BTorrent_DLoader_SaveResume
函数功能：触发一次临时文件保存操作
 参数.一：xhToken
  In/Out：In
  类型：句柄
  可空：N
  意思：输入要操作的句柄
返回值
  类型：逻辑型
  意思：是否成功
备注：每调用一次,将会触发一次事件信号,通知系统保存一次临时文件
*********************************************************************/
bool CBTorrent_DLoader::BTorrent_DLoader_SaveResume(XHANDLE xhToken)
{
	BTDload_IsErrorOccur = false;

#if 1 == _XENGIEN_STORAGE_BUILDSWITCH_BTORRENT
	BTORRENT_DLOADINFO* pSt_BTDLoader = (BTORRENT_DLOADINFO*)xhToken;
	if (NULL == pSt_BTDLoader)
	{
		std::vector<lt::alert*> stl_VectorAlerts;
		pSt_BTDLoader->m_BTSession->pop_alerts(&stl_VectorAlerts);
		//轮训元素
		for (lt::alert const* stl_ListElement : stl_VectorAlerts)
		{
			//获得元素内容
			auto stl_ListIterator = lt::alert_cast<lt::add_torrent_alert>(stl_ListElement);
			stl_ListIterator->handle.save_resume_data(lt::torrent_handle::save_info_dict);
		}
	}
#endif
	return true;
}
/********************************************************************
函数名称：BTorrent_DLoader_Close
函数功能：关闭一个种子下载器
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
bool CBTorrent_DLoader::BTorrent_DLoader_Close(XHANDLE xhToken)
{
	BTDload_IsErrorOccur = false;

#if 1 == _XENGIEN_STORAGE_BUILDSWITCH_BTORRENT
	BTORRENT_DLOADINFO* pSt_BTDLoader = (BTORRENT_DLOADINFO*)xhToken;
	if (NULL == pSt_BTDLoader)
	{
		pSt_BTDLoader->m_BTSession->abort();
	}
#endif 
	return true;
}
/********************************************************************
函数名称：BTorrent_DLoader_SetPause
函数功能：设置暂停或者恢复
 参数.一：xhToken
  In/Out：In
  类型：句柄
  可空：N
  意思：输入要操作的
 参数.二：bPause
  In/Out：In
  类型：逻辑型
  可空：N
  意思：假为暂停,真为恢复下载
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CBTorrent_DLoader::BTorrent_DLoader_SetPause(XHANDLE xhToken, bool bPause)
{
	BTDload_IsErrorOccur = false;

#if 1 == _XENGIEN_STORAGE_BUILDSWITCH_BTORRENT
	BTORRENT_DLOADINFO* pSt_BTDLoader = (BTORRENT_DLOADINFO*)xhToken;
	if (NULL == pSt_BTDLoader)
	{
		if (bPause)
		{
			pSt_BTDLoader->m_BTSession->pause();
		}
		else
		{
			pSt_BTDLoader->m_BTSession->resume();
		}
	}
#endif
	return true;
}
/********************************************************************
函数名称：BTorrent_DLoader_UPNPEnable
函数功能：启用或者关闭UPNP
 参数.一：xhToken
  In/Out：In
  类型：句柄
  可空：N
  意思：输入要操作的
 参数.二：bEnable
  In/Out：In
  类型：逻辑型
  可空：N
  意思：输入启用还是关闭UPNP
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CBTorrent_DLoader::BTorrent_DLoader_UPNPEnable(XHANDLE xhToken, bool bEnable /* = false */)
{
	BTDload_IsErrorOccur = false;

#if 1 == _XENGIEN_STORAGE_BUILDSWITCH_BTORRENT
	BTORRENT_DLOADINFO* pSt_BTDLoader = (BTORRENT_DLOADINFO*)xhToken;
	if (NULL == pSt_BTDLoader)
	{
		pSt_BTDLoader->m_BTSetPack.set_bool(lt::settings_pack::enable_upnp, bEnable);
		pSt_BTDLoader->m_BTSetPack.set_bool(lt::settings_pack::enable_natpmp, bEnable);
		pSt_BTDLoader->m_BTSession->apply_settings(pSt_BTDLoader->m_BTSetPack);
	}
#endif
	return true;
}