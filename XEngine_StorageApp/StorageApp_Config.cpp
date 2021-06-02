#include "StorageApp_Hdr.h"

BOOL StorageApp_Config_Parament(int argc,char **argv, XENGINE_SERVERCONFIG *pSt_ServerConfig)
{
    LPCTSTR lpszCfg = _T("./XEngine_Config/XEngine_Config.json");

    if (!Config_Json_File(lpszCfg,pSt_ServerConfig))
    {
        printf("解析配置文件失败,Config_Json_File:%lX\n",Config_GetLastError());
        return FALSE;
    }

    for (int i = 0;i < argc;i++)
    {
        if ((0 == _tcscmp("-h",argv[i])) || (0 == _tcscmp("-H",argv[i])))
        {
            StorageApp_Config_ParamentHelp();
            return FALSE;
        }
        if ((0 == _tcscmp("-v",argv[i])) || (0 == _tcscmp("-V",argv[i])))
        {
            tstring m_StrVersion = st_ServiceCfg.st_XVer.pStl_ListStorage->front();
            printf("Version：%s\n", m_StrVersion.c_str());
            return FALSE;
        }
        else if (0 == _tcscmp("-l",argv[i]))
        {
            pSt_ServerConfig->st_XLog.nLogLeave = _ttoi(argv[i + 1]);
        }
        else if (0 == _tcscmp("-d",argv[i]))
        {
            pSt_ServerConfig->bDeamon = _ttoi(argv[i + 1]);
        }
    }

    return TRUE;
}

void StorageApp_Config_ParamentHelp()
{
    printf(_T("--------------------------启动参数帮助开始--------------------------\n"));
    printf(_T("服务启动参数：程序 参数 参数值，参数是区分大小写的。如果不指定将会加载默认的./XEngine_Config/XEngine_Config.json配置文件里面的参数\n"));
    printf(_T("-h or -H：启动参数帮助提示信息\n"));
    printf(_T("-v or -V：输出版本号\n"));
    printf(_T("-l：设置日志输出级别\n"));
    printf(_T("-d：1 启用守护进程，2不启用\n"));
    printf(_T("--------------------------启动参数帮助结束--------------------------\n"));
}
