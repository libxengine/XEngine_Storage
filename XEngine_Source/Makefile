RELEASE = 0
UNICODE = 0
PLATFORM=linux
FLAGS= 

THIRD_JSONCPP_PATH = ./XEngine_Depend/XEngine_Module/jsoncpp
THIRD_TINYXML_PATH = ./XEngine_Depend/XEngine_Module/tinyxml2
THIRD_REPORT_PATH = ./XEngine_Depend/XEngine_Module/XEngine_InfoReport

BASE_SQL_PATH = ./StorageModule_Database
BASE_CONFIG_PATH = ./StorageModule_Config
BASE_SESSION_PATH = ./StorageModule_Session
BASE_APIHELP_PATH = ./StorageModule_APIHelp
BASE_PROTOCOL_PATH = ./StorageModule_Protocol
BASE_BTORRENT_PATH = ./StorageModule_BTorrent

APPSERVICE_STORAGE_PATH = ./XEngine_StorageApp

XENGINE_MODULES = libjsoncpp.so libtinyxml2.so libXEngine_InfoReport.so                                   \
                libStorageModule_Database.so libStorageModule_Config.so libStorageModule_Session.so libStorageModule_APIHelp.so libStorageModule_Protocol.so libStorageModule_BTorrent.so    \
		 		XEngine_StorageApp.exe 

.PHONY:MakeAll 
MakeAll:$(XENGINE_MODULES)
	
ifeq ($(PLATFORM),linux)
	FILEEXT = so
else ifeq ($(PLATFORM),mac)
	FILEEXT = dylib
endif

libjsoncpp.so:
ifeq ($(FLAGS), InstallAll)
	cp $(THIRD_JSONCPP_PATH)/libjsoncpp.$(FILEEXT) ../XEngine_Release/
else
	make -C $(THIRD_JSONCPP_PATH) PLATFORM=$(PLATFORM) UNICODE=$(UNICODE) RELEASE=$(RELEASE) $(FLAGS)
endif
libtinyxml2.so:
ifeq ($(FLAGS), InstallAll)
	cp $(THIRD_TINYXML_PATH)/libtinyxml2.$(FILEEXT) ../XEngine_Release/
else
	make -C $(THIRD_TINYXML_PATH) PLATFORM=$(PLATFORM) UNICODE=$(UNICODE) RELEASE=$(RELEASE) $(FLAGS)
endif
libXEngine_InfoReport.so:
ifeq ($(FLAGS), InstallAll)
	cp $(THIRD_REPORT_PATH)/libXEngine_InfoReport.$(FILEEXT) ../XEngine_Release/
else
	make -C $(THIRD_REPORT_PATH) PLATFORM=$(PLATFORM) UNICODE=$(UNICODE) RELEASE=$(RELEASE) $(FLAGS)
endif

libStorageModule_Database.so:
	make -C $(BASE_SQL_PATH) PLATFORM=$(PLATFORM) UNICODE=$(UNICODE) RELEASE=$(RELEASE) $(FLAGS)
libStorageModule_Config.so:
	make -C $(BASE_CONFIG_PATH) PLATFORM=$(PLATFORM) UNICODE=$(UNICODE) RELEASE=$(RELEASE) $(FLAGS)
libStorageModule_Session.so:
	make -C $(BASE_SESSION_PATH) PLATFORM=$(PLATFORM) UNICODE=$(UNICODE) RELEASE=$(RELEASE) $(FLAGS)
libStorageModule_APIHelp.so:
	make -C $(BASE_APIHELP_PATH) PLATFORM=$(PLATFORM) UNICODE=$(UNICODE) RELEASE=$(RELEASE) $(FLAGS)
libStorageModule_Protocol.so:
	make -C $(BASE_PROTOCOL_PATH) PLATFORM=$(PLATFORM) UNICODE=$(UNICODE) RELEASE=$(RELEASE) $(FLAGS)
libStorageModule_BTorrent.so:
	make -C $(BASE_BTORRENT_PATH) PLATFORM=$(PLATFORM) UNICODE=$(UNICODE) RELEASE=$(RELEASE) $(FLAGS)

XEngine_StorageApp.exe:
	make -C $(APPSERVICE_STORAGE_PATH) PLATFORM=$(PLATFORM) UNICODE=$(UNICODE) RELEASE=$(RELEASE) $(FLAGS)
