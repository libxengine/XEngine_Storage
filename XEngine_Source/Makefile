RELEASE = 0
UNICODE = 0

FLAGS = RELEASE=$(RELEASE)

THIRD_JSONCPP_PATH = ./XEngine_ThirdPart/jsoncpp

BASE_SQL_PATH = ./StorageModule_Database
BASE_CONFIG_PATH = ./StorageModule_Config
BASE_SESSION_PATH = ./StorageModule_Session
BASE_APIHELP_PATH = ./StorageModule_APIHelp
BASE_PROTOCOL_PATH = ./StorageModule_Protocol

APPSERVICE_STORAGE_PATH = ./XEngine_StorageApp

NETENGINE_LIB = libjsoncpp.so                                                          \
                libStorageModule_Database.so libStorageModule_Config.so libStorageModule_Session.so libStorageModule_APIHelp.so libStorageModule_Protocol.so         \
		 		XEngine_StorageApp.exe 

.PHONY:MakeAll 
MakeAll:$(NETENGINE_LIB)
	
libjsoncpp.so:
	make -C $(THIRD_JSONCPP_PATH) $(FLAGS)

libStorageModule_Database.so:
	make -C $(BASE_SQL_PATH) $(FLAGS)
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
