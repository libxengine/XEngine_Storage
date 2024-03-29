CC = g++ -Wall -std=c++17 -fPIC -Wformat-overflow=0
PLATFORM = linux
PLATVER = 
PLATDIR = 
RELEASE = 0
UNICODE = 0
LOADHDR = -I ./ -I ../XEngine_ThirdPart/jsoncpp
LOADSO = -L ../StorageModule_Database -L ../StorageModule_Config -L ../StorageModule_Session -L ../StorageModule_APIHelp -L ../StorageModule_Protocol -L ../XEngine_ThirdPart/jsoncpp
LIB = -lXEngine_BaseLib -lXEngine_Algorithm -lXEngine_Core -lXEngine_ManagePool -lXEngine_OPenSsl -lXClient_APIHelp -lXEngine_ProcFile -lXEngine_SystemApi -lHelpComponents_XLog -lHelpComponents_Packets -lRfcComponents_HttpProtocol \
      -lStorageModule_Database -lStorageModule_Config -lStorageModule_Session -lStorageModule_APIHelp -lStorageModule_Protocol \
	  -ljsoncpp
LIBEX = 
OBJECTS = StorageApp_Config.o StorageApp_Download.o StorageApp_Network.o StorageApp_Center.o StorageApp_UPLoader.o Storage_TaskPass.o Storage_TaskManage.o Storage_TaskP2p.o XEngine_StorageApp.o

ifeq ($(RELEASE),1)
FLAGS = -c 
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

ifeq ($(PLATFORM),linux)
	ifeq (/etc/redhat-release,$(wildcard /etc/redhat-release))
		PLATVER = -D __CENTOS__
		PLATDIR = XEngine_Linux/Centos
	else
		PLATVER = -D __UBUNTU__
		PLATDIR = XEngine_Linux/Ubuntu
	endif
	LIBEX = -lpthread -lrt -ldl
	LOADBIN = -Wl,-rpath=./,--disable-new-dtags
else ifeq ($(PLATFORM),mac)
	CC += -Wno-deprecated-declarations
	PLATVER = -D __MACOS__
	PLATDIR = XEngine_Mac
	LIBEX = -lpthread -ldl
	LOADBIN = -Wl,-rpath,@loader_path/./
endif

all:$(OBJECTS)
	$(CC) $(DEBUG) $(OBJECTS) -o XEngine_XStorageApp $(LIB) $(LIBEX) $(LOADSO) $(LOADBIN)
  
StorageApp_Config.o:./StorageApp_Config.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) ./StorageApp_Config.cpp 
StorageApp_Download.o:./StorageApp_Download.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) ./StorageApp_Download.cpp
StorageApp_Network.o:./StorageApp_Network.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) ./StorageApp_Network.cpp 
StorageApp_Center.o:./StorageApp_Center.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) ./StorageApp_Center.cpp
StorageApp_UPLoader.o:./StorageApp_UPLoader.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) ./StorageApp_UPLoader.cpp

Storage_TaskPass.o:./Storage_APPTask/Storage_TaskPass.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) ./Storage_APPTask/Storage_TaskPass.cpp
Storage_TaskManage.o:./Storage_APPTask/Storage_TaskManage.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) ./Storage_APPTask/Storage_TaskManage.cpp
Storage_TaskP2p.o:./Storage_APPTask/Storage_TaskP2p.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) ./Storage_APPTask/Storage_TaskP2p.cpp
					
XEngine_StorageApp.o:./XEngine_StorageApp.cpp             
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) ./XEngine_StorageApp.cpp   


InstallAll:InstallApp
InstallApp:./XEngine_XStorageApp
	cp -f ./XEngine_XStorageApp ../../XEngine_Release/XEngine_XStorageApp
	
CleanAll:CleanObj CleanMk 
CleanObj:                                              
	rm *.o
CleanMk:                                  
	rm XEngine_XStorageApp
