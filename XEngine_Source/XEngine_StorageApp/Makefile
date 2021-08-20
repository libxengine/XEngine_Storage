CC = g++ -Wall
RELEASE = 0
UNICODE = 0  
LOADBIN = -L /usr/local/lib/XEngine_Release/XEngine_BaseLib -L /usr/local/lib/XEngine_Release/XEngine_Core -L /usr/local/lib/XEngine_Release/XEngine_HelpComponents -L /usr/local/lib/XEngine_Release/XEngine_SystemSdk -L /usr/local/lib/XEngine_Release/XEngine_RfcComponents -L /usr/local/lib/XEngine_Release/XEngine_NetHelp -L ../XEngine_StorageComponents/XStorage_SQLPacket -L ../XEngine_StorageComponents/XStorage_Protocol -L ../XEngine_P2XPComponents/XEngine_P2XPPeer -L ../XEngine_P2XPComponents/XEngine_P2XPProtocol -L ../StorageModule_Config -L ../StorageModule_Session -L ../StorageModule_APIHelp
LIB = -lXEngine_BaseLib -lXEngine_Algorithm -lXEngine_Core -lXEngine_ManagePool -lXEngine_NetXApi -lXEngine_OPenSsl -lHelpComponents_XLog -lHelpComponents_Packets -lXEngine_ProcSdk -lXEngine_SystemApi -lRfcComponents_HttpServer -lNetHelp_APIHelp -lXStorage_SQLPacket -lXStorage_Protocol -lXEngine_P2XPPeer -lXEngine_P2XPProtocol -lStorageModule_Config -lStorageModule_Session -lStorageModule_APIHelp
LIBEX = -static-libgcc -ldl -lrt -lpthread
LOADSO = -Wl,-rpath=./,--disable-new-dtags
LIBINCLUDE = StorageApp_Config.o StorageApp_Download.o StorageApp_Network.o StorageApp_Center.o StorageApp_UPLoader.o StorageApp_P2XPNet.o Storage_APPHelp.o Storage_TaskEvent.o Storage_TaskPass.o Storage_TaskManage.o Storage_TaskP2p.o XEngine_StorageApp.o

ifeq ($(RELEASE),1)
FLAGS = -c -O2 
DEBUG = 
else
FLAGS = -c -lc_p
DEBUG = -g -pg 
endif

ifeq ($(UNICODE),1)
UNICODE = -D _UNICODE
else
UNICODE = 
endif

all:$(LIBINCLUDE)
	$(CC) $(DEBUG) $(LIBINCLUDE) -o XEngine_XStorageApp $(LOADBIN) $(LIB) $(LIBEX) $(LOADSO)
  
StorageApp_Config.o:./StorageApp_Config.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) ./StorageApp_Config.cpp 
StorageApp_Download.o:./StorageApp_Download.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) ./StorageApp_Download.cpp
StorageApp_Network.o:./StorageApp_Network.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) ./StorageApp_Network.cpp 
StorageApp_Center.o:./StorageApp_Center.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) ./StorageApp_Center.cpp
StorageApp_UPLoader.o:./StorageApp_UPLoader.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) ./StorageApp_UPLoader.cpp
StorageApp_P2XPNet.o:./StorageApp_P2XPNet.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) ./StorageApp_P2XPNet.cpp
		
Storage_APPHelp.o:./Storage_APPHelp/Storage_APPHelp.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) ./Storage_APPHelp/Storage_APPHelp.cpp
	
Storage_TaskEvent.o:./Storage_APPTask/Storage_TaskEvent.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) ./Storage_APPTask/Storage_TaskEvent.cpp
Storage_TaskPass.o:./Storage_APPTask/Storage_TaskPass.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) ./Storage_APPTask/Storage_TaskPass.cpp
Storage_TaskManage.o:./Storage_APPTask/Storage_TaskManage.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) ./Storage_APPTask/Storage_TaskManage.cpp
Storage_TaskP2p.o:./Storage_APPTask/Storage_TaskP2p.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) ./Storage_APPTask/Storage_TaskP2p.cpp
					
XEngine_StorageApp.o:./XEngine_StorageApp.cpp             
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) ./XEngine_StorageApp.cpp   


InstallAll:InstallApp
InstallApp:./XEngine_XStorageApp
	cp -f ./XEngine_XStorageApp ../../XEngine_Release/XEngine_XStorageApp
	
CleanAll:CleanObj CleanMk 
CleanObj:                                              
	rm *.o
CleanMk:                                  
	rm XEngine_XStorageApp
