RELEASE = 0
UNICODE = 0

FLAGS = RELEASE=$(RELEASE)

STORAGE_SQL_PATH = ./XEngine_StorageComponents/XStorage_SQLPacket
STORAGE_PROTOCOL_PATH = ./XEngine_StorageComponents/XStorage_Protocol
P2XP_PEER_PATH = ./XEngine_P2XPComponents/XEngine_P2XPPeer
P2XP_PROTOCOL_PATH = ./XEngine_P2XPComponents/XEngine_P2XPProtocol

BASE_CONFIG_PATH = ./StorageModule_Config
BASE_SESSION_PATH = ./StorageModule_Session

APPSERVICE_STORAGE_PATH = ./XEngine_StorageApp

NETENGINE_LIB = libXStorage_SQLPacket.so libXStorage_Protocol.so                       \
                libXEngine_P2XPPeer.so libXEngine_P2XPProtocol.so                      \
                libStorageModule_Config.so libStorageModule_Session.so                 \
		 XEngine_StorageApp.exe 

.PHONY:MakeAll 
MakeAll:$(NETENGINE_LIB)
	
libXStorage_SQLPacket.so:
	make -C $(STORAGE_SQL_PATH) $(FLAGS)
libXStorage_Protocol.so:
	make -C $(STORAGE_PROTOCOL_PATH) $(FLAGS)

libXEngine_P2XPPeer.so:
	make -C $(P2XP_PEER_PATH) $(FLAGS)
libXEngine_P2XPProtocol.so:
	make -C $(P2XP_PROTOCOL_PATH) $(FLAGS)
	
libStorageModule_Config.so:
	make -C $(BASE_CONFIG_PATH) $(FLAGS)
libStorageModule_Session.so:
	make -C $(BASE_SESSION_PATH) $(FLAGS)

XEngine_StorageApp.exe:
	make -C $(APPSERVICE_STORAGE_PATH) $(FLAGS)


SetFlags_Install:
	export FLAGS=InstallAll
InstallAll:SetFlags_Install $(NETENGINE_LIB) 
 
SetFlags_Clean:
	export FLAGS=CleanAll
CleanAll:SetFlags_Clean $(NETENGINE_LIB) 
	
	
	
