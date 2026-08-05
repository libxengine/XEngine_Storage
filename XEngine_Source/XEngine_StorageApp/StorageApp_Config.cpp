#include "StorageApp_Hdr.h"

bool StorageApp_Config_Parament(int argc,char **argv)
{
    // 默认配置文件路径：基础服务配置、版本配置、负载均衡配置。
    // 当启动参数未指定替代路径时，按以下默认文件进行加载。
    LPCXSTR lpszBaseCfg = _X("./XEngine_Config/XEngine_Config.json");
    LPCXSTR lpszVerCfg = _X("./XEngine_Config/XEngine_VersionConfig.json");
    LPCXSTR lpszLoadCfg = _X("./XEngine_Config/XEngine_LBConfig.json");

    // 重新加载配置前先释放历史配置中可能已分配的动态容器，
    // 避免重复加载时发生内存泄漏或悬挂旧数据。
    if (NULL != st_ServiceCfg.st_XVer.pStl_ListStorage)
    {
        delete st_ServiceCfg.st_XVer.pStl_ListStorage;
        st_ServiceCfg.st_XVer.pStl_ListStorage = NULL;
    }
	if (NULL != st_LoadbalanceCfg.st_LoadBalance.pStl_ListBucket)
	{
		delete st_LoadbalanceCfg.st_LoadBalance.pStl_ListBucket;
        st_LoadbalanceCfg.st_LoadBalance.pStl_ListBucket = NULL;
	}
	if (NULL != st_LoadbalanceCfg.st_LoadBalance.pStl_ListDownload)
	{
		delete st_LoadbalanceCfg.st_LoadBalance.pStl_ListDownload;
        st_LoadbalanceCfg.st_LoadBalance.pStl_ListDownload = NULL;
	}
	if (NULL != st_LoadbalanceCfg.st_LoadBalance.pStl_ListUPLoader)
	{
		delete  st_LoadbalanceCfg.st_LoadBalance.pStl_ListUPLoader;
        st_LoadbalanceCfg.st_LoadBalance.pStl_ListUPLoader = NULL;
	}

    // 清空全局配置结构体，确保后续解析在干净状态下执行。
    memset(&st_ServiceCfg, '\0', sizeof(XENGINE_SERVERCONFIG));
    memset(&st_LoadbalanceCfg, '\0', sizeof(XENGINE_LBCONFIG));

    // 依次解析三类配置文件；任一步失败都立即返回并输出错误码。
    if (!Config_Json_File(lpszBaseCfg, &st_ServiceCfg))
    {
        printf("解析配置文件失败,Config_Json_File:%lX\n",Config_GetLastError());
        return false;
    }
    if (!Config_Json_Version(lpszVerCfg, &st_ServiceCfg))
    {
		printf("解析配置文件失败,Config_Json_Version:%lX\n", Config_GetLastError());
		return false;
    }
	if (!Config_Json_LoadBalance(lpszLoadCfg, &st_LoadbalanceCfg))
	{
		printf("解析配置文件失败,Config_Json_LoadBalance:%lX\n", Config_GetLastError());
		return false;
	}

    for (int i = 0;i < argc;i++)
    {
        if (0 == _tcsxicmp("-h",argv[i]))
        {
            StorageApp_Config_ParamentHelp();
            return false;
        }
        else if (0 == _tcsxicmp("-v", argv[i]))
        {
            string m_StrVersion = st_ServiceCfg.st_XVer.pStl_ListStorage->front();
            printf("Version：%s\n", m_StrVersion.c_str());
            return false;
        }
        else if (0 == _tcsxicmp("-d", argv[i]))
        {
            if (i + 1 >= argc)
            {
                printf("Parameter error: lost value\n");
                return false;
            }
            st_ServiceCfg.bDeamon = _ttxoi(argv[++i]);
        }
        else if (0 == _tcsxicmp("-r", argv[i]))
        {
            st_ServiceCfg.st_Memory.bReload = true;
        }
        else if (0 == _tcsxicmp("-u", argv[i]))
        {
            st_ServiceCfg.bReuseraddr = true;
        }
		else if (0 == _tcsxicmp("-t", argv[i]))
		{
			bIsTest = true;
		}
        else if (0 == _tcsxicmp("-lt", argv[i]))
        {
            if (i + 1 >= argc)
            {
                printf("Parameter error: lost value\n");
                return false;
            }
            st_ServiceCfg.st_XLog.nLogType = _ttxoi(argv[++i]);
        }
		else if (0 == _tcsxicmp("-l", argv[i]))
		{
            if (i + 1 >= argc)
            {
                printf("Parameter error: lost value\n");
                return false;
            }
			LPCXSTR lpszLogLevel = argv[++i];
			if (0 == _tcsxicmp("debug", lpszLogLevel))
			{
                st_ServiceCfg.st_XLog.nLogLeave = XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_DEBUG;
			}
			else if (0 == _tcsxicmp("detail", lpszLogLevel))
			{
                st_ServiceCfg.st_XLog.nLogLeave = XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_DETAIL;
			}
			else if (0 == _tcsxicmp("info", lpszLogLevel))
			{
                st_ServiceCfg.st_XLog.nLogLeave = XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO;
			}
		}
    }

    return true;
}

void StorageApp_Config_ParamentHelp()
{
    printf(_X("--------------------------启动参数帮助开始--------------------------\n"));
    printf(_X("服务启动参数：程序 参数 参数值，参数是区分大小写的。如果不指定将会加载默认的./XEngine_Config/XEngine_Config.json配置文件里面的参数\n"));
    printf(_X("-h or -H：启动参数帮助提示信息\n"));
    printf(_X("-v or -V：输出版本号\n"));
    printf(_X("-l：设置日志输出级别\n"));
    printf(_X("-d：1 启用守护进程，2不启用\n"));
    printf(_X("-r：无重启,重载配置文件\n"));
    printf(_X("--------------------------启动参数帮助结束--------------------------\n"));
}
