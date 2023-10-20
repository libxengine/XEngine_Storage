#pragma once
/********************************************************************
//    Created:     2022/03/29  14:21:46
//    File Name:   D:\XEngine_Storage\XEngine_Source\StorageModule_Database\Database_Define.h
//    File Path:   D:\XEngine_Storage\XEngine_Source\StorageModule_Database
//    File Base:   Database_Define
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     数据库操作导出函数
//    History:
*********************************************************************/
//////////////////////////////////////////////////////////////////////////
///                        导出数据结构
//////////////////////////////////////////////////////////////////////////
typedef struct
{
    XENGINE_PROTOCOL_FILE st_ProtocolFile;
    XCHAR tszBuckKey[MAX_PATH];                                             //路径的关键字
    XCHAR tszTableName[64];                                                 //日期表名称,插入:表示自定义插入日期表,获取:表示导出这个文件所属日期表
}XSTORAGECORE_DBFILE, *LPXSTORAGECORE_DBFILE;
typedef struct
{
    XENGINE_PROTOCOL_USERINFO st_ProtocolUser;
    __int64x nFileCount;                                                   //拥有的文件个数
    __int64x nFileSize;                                                    //文件占用大小
    XNETHANDLE xhToken;                                                    //用户临时会话句柄
    XCHAR tszIPAddr[64];                                                    //最后登录IP地址
}XSTORAGECORE_USERINFO, *LPXSTORAGECORE_USERINFO;
//////////////////////////////////////////////////////////////////////////
///                        导出的函数
//////////////////////////////////////////////////////////////////////////
extern "C" XLONG Database_GetLastError(int *pInt_SysError = NULL);
/************************************************************************/
/*                         导出的数据库帮助函数                         */
/************************************************************************/
/********************************************************************
函数名称：Database_Help_Insert
函数功能：数据插入函数
 参数.一：ptszSQLBuffer
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出打好包的插入语句
 参数.二：pSt_DBFile
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：输入要插入的数据
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool Database_Help_Insert(XCHAR* ptszSQLBuffer, XSTORAGECORE_DBFILE* pSt_DBFile);
/********************************************************************
函数名称：Database_Help_Delete
函数功能：删除语句
 参数.一：ptszSQLBuffer
  In/Out：In
  类型：字符指针
  可空：N
  意思：输出打好包的查询语句
 参数.二：lpszTableName
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入要查询的表名称
 参数.三：lpszBuckKey
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入BUCKET名
 参数.四：lpszFilePath
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入文件路径
 参数.五：lpszFileName
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入文件名称
 参数.六：lpszFileHash
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入文件HASH值
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool Database_Help_Delete(XCHAR* ptszSQLBuffer, LPCXSTR lpszTableName, LPCXSTR lpszBuckKey = NULL, LPCXSTR lpszFilePath = NULL, LPCXSTR lpszFileName = NULL, LPCXSTR lpszFileHash = NULL);
/********************************************************************
函数名称：Database_Help_Query
函数功能：数据库查询打包函数
 参数.一：ptszSQLBuffer
  In/Out：In
  类型：字符指针
  可空：N
  意思：输出打好包的查询语句
 参数.二：lpszTableName
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入要查询的表名称
 参数.三：lpszBuckKey
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入BUCKET名
 参数.四：lpszFilePath
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入文件路径
 参数.五：lpszFileName
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入文件名称
 参数.六：lpszFileHash
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入文件HASH值
 参数.七：lpszFileUser
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入文件所属用户
 参数.八：lpszTimeStart
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入文件开始时间范围
 参数.九：lpszTimeEnd
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入文件结束时间范围
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool Database_Help_Query(XCHAR* ptszSQLBuffer, LPCXSTR lpszTableName, LPCXSTR lpszBuckKey = NULL, LPCXSTR lpszFilePath = NULL, LPCXSTR lpszFileName = NULL, LPCXSTR lpszFileHash = NULL, LPCXSTR lpszFileUser = NULL, LPCXSTR lpszTimeStart = NULL, LPCXSTR lpszTimeEnd = NULL);
/************************************************************************/
/*                         导出的数据库操作函数                         */
/************************************************************************/
/********************************************************************
函数名称：Database_File_Init
函数功能：初始化存储服务数据库管理器
 参数.一：pSt_DBConnector
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：数据MYSQL数据库连接信息
 参数.二：nTimeDay
  In/Out：In
  类型：整数型
  可空：N
  意思：输入文件数据保留时间
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool Database_File_Init(DATABASE_MYSQL_CONNECTINFO * pSt_DBConnector, int nTimeMonth = 1);
/********************************************************************
函数名称：Database_File_Destory
函数功能：销毁数据库管理器
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool Database_File_Destory();
/********************************************************************
函数名称：Database_File_FileInsert
函数功能：插入一个文件数据到数据库中
 参数.一：pSt_DBManage
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：要插入的数据信息
 参数.二：bRewrite
  In/Out：In
  类型：逻辑型
  可空：N
  意思：是否覆写数据
返回值
  类型：逻辑型
  意思：是否成功
备注：这个结构所有值都必须填充
*********************************************************************/
extern "C" bool Database_File_FileInsert(XSTORAGECORE_DBFILE *pSt_DBManage, bool bRewrite = false);
/********************************************************************
函数名称：Database_File_FileDelete
函数功能：删除一个数据库文件信息
 参数.一：lpszBuckKey
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：所属BUCK名称
 参数.二：lpszFilePath
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：要删除的文件路径
 参数.三：lpszFileName
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：要删除的文件名称
 参数.四：lpszHash
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：要删除的文件HASH
返回值
  类型：逻辑型
  意思：是否成功
备注：参数不能全为空,不会删除文件
*********************************************************************/
extern "C" bool Database_File_FileDelete(LPCXSTR lpszBuckKey = NULL, LPCXSTR lpszFilePath = NULL, LPCXSTR lpszFileName = NULL, LPCXSTR lpszHash = NULL);
/********************************************************************
函数名称：Database_File_FileQuery
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
 参数.三：lpszTimeStart
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：查找开始时间,20190701
 参数.四：lpszTimeEnd
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：查找结束时间,20190730
 参数.五：lpszBuckKey
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：查询的BUCK名
 参数.六：lpszFilePath
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：要查询的路径
 参数.七：lpszFileName
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：要查询的名称
 参数.八：lpszHash
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：要查询的文件HASH
 参数.九：lpszTableName
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入要查询的表明,为NULL所有
返回值
  类型：逻辑型
  意思：是否成功
备注：返回假可能没有查找到,这条记录不存在.参数lpszFile和lpszMD5不能全为空
*********************************************************************/
extern "C" bool Database_File_FileQuery(XSTORAGECORE_DBFILE * **pppSt_ListFile, int* pInt_ListCount, LPCXSTR lpszTimeStart = NULL, LPCXSTR lpszTimeEnd = NULL, LPCXSTR lpszBuckKey = NULL, LPCXSTR lpszFilePath = NULL, LPCXSTR lpszFileName = NULL, LPCXSTR lpszHash = NULL, LPCXSTR lpszTableName = NULL);
/************************************************************************/
/*                         导出的内存数据库函数                         */
/************************************************************************/
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
extern "C" bool Database_Memory_Init(list<XENGINE_STORAGEBUCKET>* pStl_ListBucket, int nHashMode);
/********************************************************************
函数名称：Database_Memory_Destory
函数功能：销毁数据库管理器
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool Database_Memory_Destory();
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
extern "C" bool Database_Memory_FileInsert(XSTORAGECORE_DBFILE* pSt_DBManage);
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
extern "C" bool Database_Memory_FileDelete(LPCXSTR lpszFilePath = NULL, LPCXSTR lpszFileName = NULL, LPCXSTR lpszHash = NULL);
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
 参数.三：lpszFilePath
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：要查询的路径
 参数.四：lpszFileName
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：要查询的名称
 参数.五：lpszHash
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：要查询的文件HASH
返回值
  类型：逻辑型
  意思：是否成功
备注：返回假可能没有查找到,这条记录不存在.参数lpszFile和lpszHash不能全为空
*********************************************************************/
extern "C" bool Database_Memory_FileQuery(XSTORAGECORE_DBFILE*** pppSt_ListFile, int* pInt_ListCount, LPCXSTR lpszFilePath = NULL, LPCXSTR lpszFileName = NULL, LPCXSTR lpszHash = NULL);