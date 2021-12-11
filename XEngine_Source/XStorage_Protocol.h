#pragma once
/********************************************************************
//    Created:     2021/07/19  13:41:43
//    File Name:   D:\XEngine_Storage\XEngine_Source\XStorage_Protocol.h
//    File Path:   D:\XEngine_Storage\XEngine_Source
//    File Base:   XStorage_Protocol
//    File Ext:    h
//    Project:     XEngine(����ͨ������)
//    Author:      qyt
//    Purpose:     �洢����-��׼Э��ͷ�ļ�
//    History:
*********************************************************************/
//////////////////////////////////////////////////////////////////////////
//                         Э�鶨��
//////////////////////////////////////////////////////////////////////////
//�洢����
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_STORAGE_REQQUERY 0x4001     //��ѯ����
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_STORAGE_REPQUERY 0x4002     //��ѯ�ظ�
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_STORAGE_REQREPORT 0x4003    //�����ѯ���
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_STORAGE_REPREPORT 0x4004    //ȷ�ϲ�ѯ���
//P2XPЭ��
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_P2XP_REQLOGIN 0x6001        //��¼
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_P2XP_REPLOGIN 0x6002
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_P2XP_REQLANLIST 0x6003      //ͬ���б�Э��,ͬ�����ؾ�����IP�еĿͻ���
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_P2XP_REPLANLIST 0x6004      //ͬ���б�ظ�ȷ��Э��
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_P2XP_REQCONNECT 0x6005      //�����û����ӵ��������
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_P2XP_REPCONNECT 0x6006      //ȷ������Э��
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_P2XP_REQUSERQUERY 0x6007    //�û���ѯ
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_P2XP_REPUSERQUERY 0x6008    //�û���ѯȷ��
//////////////////////////////////////////////////////////////////////////
//                         Э��ṹ��
//////////////////////////////////////////////////////////////////////////
//�ļ�����ͷ�ļ�,������P2XP�ļ�������������ģ���ļ�����
typedef struct tag_XEngine_Protocol_File
{
	CHAR tszFilePath[MAX_PATH];                                           //�ļ�·��
	CHAR tszFileName[MAX_PATH];                                           //�ļ�����,�ļ������ʱ�����Ҫ,����ʱ����Ҫ
	CHAR tszFileHash[MAX_PATH];                                           //�ļ�HASHֵ,MD5����HASH1 256��...���ܳ���260�ֽڴ�С
	CHAR tszFileUser[128];                                                //�ļ������û�
	CHAR tszFileTime[64];                                                 //�ļ�����ʱ�䣬��������������,��ô��������������Ϊ���ܵ����ļ�ʱ��
	__int64x nFileSize;                                                   //�ļ���С
}XENGINE_PROTOCOL_FILE, * LPXENGINE_PROTOCOL_FILE;
//������Ϣ
typedef struct tag_XEngine_P2XPPeer_Protocol
{
	CHAR tszUserName[64];                                                 //�û���Ϣ
	CHAR tszPrivateAddr[32];                                              //˽�б��ص�ַ��������ַ
	CHAR tszPublicAddr[32];                                               //������ַ
	CHAR tszConnectAddr[32];                                              //���ӵ�ַ
	CHAR tszUserLocation[32];                                             //λ����Ϣ
	CHAR tszUserISP[20];                                                  //�û�ISP
	WORD dwConnectType;                                                   //��������
	WORD dwPeerType;                                                      //�ڵ�����
}XENGINE_P2XPPEER_PROTOCOL, * LPXENGINE_P2XPPEER_PROTOCOL;
//P2XPͨ����������
typedef struct tag_XEngine_P2XPIO_Protocol
{
	CHAR tszSourceUser[32];                                               //�������ӵ��û�
	CHAR tszDestUser[32];                                                 //Ҫ���ӵ��û�
	CHAR tszConnectAddr[32];                                              //���ӵ�IP��ַ
	int nDestPort;                                                        //Ҫ���ӵĶ˿�
	BOOL bIsTcp;                                                          //��������TCP,����ΪUDP
}XENGINE_P2XPIO_PROTOCOL, * LPXENGINE_P2XPIO_PROTOCOL;