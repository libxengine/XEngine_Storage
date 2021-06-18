RELEASE = 0
UNICODE = 0

FLAGS = RELEASE=$(RELEASE)

COMPONENTS_SQL_PATH = ./XEngine_StorageComponents/XStorage_SQLPacket
COMPONENTS_PROTOCOL_PATH = ./XEngine_StorageComponents/XStorage_Protocol

BASE_CONFIG_PATH = ./StorageModule_Config
BASE_SESSION_PATH = ./StorageModule_Session

APPSERVICE_STORAGE_PATH = ./XEngine_StorageApp

NETENGINE_LIB = libXStorage_SQLPacket.so libXStorage_Protocol.so                       \
                libStorageModule_Config.so libStorageModule_Session.so                 \
		 XEngine_StorageApp.exe 

.PHONY:MakeAll 
MakeAll:$(NETENGINE_LIB)
	
libXStorage_SQLPacket.so:
	make -C $(COMPONENTS_SQL_PATH) $(FLAGS)
libXStorage_Protocol.so:
	make -C $(COMPONENTS_PROTOCOL_PATH) $(FLAGS)

libStorageModule_Config.so:
	make -C $(BASE_CONFIG_PATH) $(FLAGS)
libStorageModule_Session.so:
	make -C $(BASE_SESSION_PATH) $(FLAGS)

XEngine_StorageApp.exe:
	make -C $(APPSERVICE_STORAGE_PATH) $(FLAGS)

#安装开发包文件 
SetFlags_Install:
	export FLAGS=InstallAll
InstallAll:SetFlags_Install $(NETENGINE_LIB) 
#清除临时文件   
SetFlags_Clean:
	export FLAGS=CleanAll
CleanAll:SetFlags_Clean $(NETENGINE_LIB) 
	
	
	
