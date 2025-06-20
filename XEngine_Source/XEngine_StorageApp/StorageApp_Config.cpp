#include "StorageApp_Hdr.h"

bool StorageApp_Config_Parament(int argc,char **argv)
{
    LPCXSTR lpszBaseCfg = _X("./XEngine_Config/XEngine_Config.json");
    LPCXSTR lpszVerCfg = _X("./XEngine_Config/XEngine_VersionConfig.json");
    LPCXSTR lpszLoadCfg = _X("./XEngine_Config/XEngine_LBConfig.json");

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

    memset(&st_ServiceCfg, '\0', sizeof(XENGINE_SERVERCONFIG));
    memset(&st_LoadbalanceCfg, '\0', sizeof(XENGINE_LBCONFIG));

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
        if ((0 == _tcsxcmp("-h",argv[i])) || (0 == _tcsxcmp("-H",argv[i])))
        {
            StorageApp_Config_ParamentHelp();
            return false;
        }
        if ((0 == _tcsxcmp("-v", argv[i])) || (0 == _tcsxcmp("-V", argv[i])))
        {
            string m_StrVersion = st_ServiceCfg.st_XVer.pStl_ListStorage->front();
            printf("Version：%s\n", m_StrVersion.c_str());
            return false;
        }
        else if (0 == _tcsxcmp("-d", argv[i]))
        {
            st_ServiceCfg.bDeamon = _ttxoi(argv[++i]);
        }
        else if (0 == _tcsxcmp("-r", argv[i]))
        {
            st_ServiceCfg.st_Memory.bReload = true;
        }
        else if (0 == _tcsxcmp("-u", argv[i]))
        {
            st_ServiceCfg.bReuseraddr = true;
        }
		else if (0 == _tcsxcmp("-t", argv[i]))
		{
			bIsTest = true;
		}
        else if (0 == _tcsxcmp("-lt", argv[i]))
        {
            st_ServiceCfg.st_XLog.nLogType = _ttxoi(argv[++i]);
        }
        else if (0 == _tcsxcmp("-ll", argv[i]))
        {
            st_ServiceCfg.st_XLog.nLogLeave = _ttxoi(argv[++i]);
        }
		else if (0 == _tcsxcmp("-l", argv[i]))
		{
			LPCXSTR lpszLogLevel = argv[++i];
			if (0 == _tcsxcmp("debug", lpszLogLevel))
			{
                st_ServiceCfg.st_XLog.nLogLeave = XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_DEBUG;
			}
			else if (0 == _tcsxcmp("info", lpszLogLevel))
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
