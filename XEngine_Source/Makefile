RELEASE = 0
UNICODE = 0

FLAGS = RELEASE=$(RELEASE)

STORAGE_SQL_PATH = ./XEngine_StorageComponents/XStorage_SQLPacket
STORAGE_P2XP_PATH = ./XEngine_StorageComponents/XStorage_P2XPPeer

BASE_CONFIG_PATH = ./StorageModule_Config
BASE_SESSION_PATH = ./StorageModule_Session
BASE_APIHELP_PATH = ./StorageModule_APIHelp
BASE_PROTOCOL_PATH = ./StorageModule_Protocol

APPSERVICE_STORAGE_PATH = ./XEngine_StorageApp

NETENGINE_LIB = libXStorage_SQLPacket.so libXStorage_P2XPPeer.so                       \
                libStorageModule_Config.so libStorageModule_Session.so libStorageModule_APIHelp.so libStorageModule_Protocol.so         \
		 XEngine_StorageApp.exe 

.PHONY:MakeAll 
MakeAll:$(NETENGINE_LIB)
	
libXStorage_SQLPacket.so:
	make -C $(STORAGE_SQL_PATH) $(FLAGS)
libXStorage_P2XPPeer.so:
	make -C $(STORAGE_P2XP_PATH) $(FLAGS)

libStorageModule_Config.so:
	make -C $(BASE_CONFIG_PATH) $(FLAGS)
libStorageModule_Session.so:
	make -C $(BASE_SESSION_PATH) $(FLAGS)
libStorageModule_APIHelp.so:
	make -C $(BASE_APIHELP_PATH) $(FLAGS)
libStorageModule_Protocol.so:
	make -C $(BASE_PROTOCOL_PATH) $(FLAGS)
	
XEngine_StorageApp.exe:
	make -C $(APPSERVICE_STORAGE_PATH) $(FLAGS)


SetFlags_Install:
	export FLAGS=InstallAll
InstallAll:SetFlags_Install $(NETENGINE_LIB) 
 
SetFlags_Clean:
	export FLAGS=CleanAll
CleanAll:SetFlags_Clean $(NETENGINE_LIB) 
