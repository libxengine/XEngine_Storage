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
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_STORAGE_TRANSMISSION 0x4000 //�ļ�����
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_STORAGE_REQUPDATE 0x4001    //�ϴ��ļ�����
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_STORAGE_REPUPDATE 0x4002    //�ϴ��ļ�����ȷ��
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_STORAGE_REQDOWN 0x4003      //�����ļ�����
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_STORAGE_REPDOWN 0x4004      //�����ļ�ȷ��
//P2XPЭ��
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_P2XP_REQLANLIST 0x6001      //ͬ���б�Э��,ͬ�����ؾ�����IP�еĿͻ���
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_P2XP_REPLANLIST 0x6002      //ͬ���б�ظ�ȷ��Э��
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_P2XP_REQWLANLIST 0x6003     //ͬ���б�Э��,ͬ��ͬһ����IP�о�����
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_P2XP_REPWLANLIST 0x6004     //ͬ����ά��IPȷ��Э��
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_P2XP_REQCONNECT 0x6005      //�����û����ӵ��������
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_P2XP_REPCONNECT 0x6006      //ȷ������Э��
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_P2XP_REQUSERQUERY 0x6007    //�û���ѯ
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_P2XP_REPUSERQUERY 0x6008    //�û���ѯȷ��
//////////////////////////////////////////////////////////////////////////
//                         Э��ṹ��
//////////////////////////////////////////////////////////////////////////
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