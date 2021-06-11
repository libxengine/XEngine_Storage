CC = g++ -Wall
RELEASE = 0
UNICODE = 0  
LOADBIN = -L /usr/local/lib/XEngine_Release/XEngine_BaseLib -L /usr/local/lib/XEngine_Release/XEngine_Core -L /usr/local/lib/XEngine_Release/XEngine_HelpComponents -L /usr/local/lib/XEngine_Release/XEngine_SystemSdk -L /usr/local/lib/XEngine_Release/XEngine_RfcComponents -L ../XEngine_StorageComponents/XStorage_SQLPacket -L ../XEngine_StorageComponents/XStorage_Protocol -L ../StorageModule_Config -L ../StorageModule_Session 
LIB = -lXEngine_BaseLib -lXEngine_Algorithm -lXEngine_Core -lXEngine_ManagePool -lXEngine_OPenSsl -lHelpComponents_XLog -lXEngine_ProcSdk -lXEngine_SystemApi -lRfcComponents_HttpServer -lXStorage_SQLPacket -lXStorage_Protocol -lStorageModule_Config -lStorageModule_Session
LIBEX = -static-libgcc -ldl -lrt -lpthread
LOADSO = -Wl,-rpath=./,--disable-new-dtags
LIBINCLUDE = StorageApp_Config.o StorageApp_Download.o StorageApp_Network.o StorageApp_Task.o StorageApp_UPLoader.o XEngine_StorageApp.o

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
StorageApp_Task.o:./StorageApp_Task.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) ./StorageApp_Task.cpp
StorageApp_UPLoader.o:./StorageApp_UPLoader.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) ./StorageApp_UPLoader.cpp
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
