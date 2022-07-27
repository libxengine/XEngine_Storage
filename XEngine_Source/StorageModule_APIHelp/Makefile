CC = g++ -Wall -std=c++17 -fPIC
SHAREFLAG = -shared
SHAREDLL = so
RELEASE = 0
UNICODE = 0
INCLUDE = -I ./ -I ../XEngine_ThirdPart/jsoncpp
LOADBIN = -L /usr/local/lib/XEngine_Release/XEngine_BaseLib -L /usr/local/lib/XEngine_Release/XEngine_Core -L /usr/local/lib/XEngine_Release/XEngine_RfcComponents -L /usr/local/lib/XEngine_Release/XEngine_SystemSdk -L ../XEngine_ThirdPart/jsoncpp
LIB = -lXEngine_BaseLib -lXEngine_OPenSsl -lRfcComponents_HttpServer -lXEngine_SystemApi -ljsoncpp
LIBEX = -ldl -lpthread
LOADSO = -Wl,-rpath=./,--disable-new-dtags
LIBINCLUDE = APIHelp_Distributed.o APIHelp_Api.o pch.o

ifeq ($(shell uname),Darwin)
	SHAREDLL = dylib
	SHAREFLAG = -dynamiclib
	LOADSO = 
else
	SHAREDLL = so
	SHAREFLAG = -shared
endif

ifeq ($(RELEASE),1)
FLAGS = -c -O2 
DEBUG = 
else
FLAGS = -c
DEBUG = -g
endif

ifeq ($(UNICODE),1)
UNICODE = -D _UNICODE
else
UNICODE = 
endif


all:$(LIBINCLUDE)
	$(CC) $(DEBUG) $(LIBINCLUDE) -o libStorageModule_APIHelp.$(SHAREDLL) $(SHAREFLAG) -fPIC $(LOADBIN) $(LIB) $(LIBEX) $(LOADSO)

APIHelp_Distributed.o:./APIHelp_Distributed/APIHelp_Distributed.cpp             
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(INCLUDE) ./APIHelp_Distributed/APIHelp_Distributed.cpp 
APIHelp_Api.o:./APIHelp_Api/APIHelp_Api.cpp             
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(INCLUDE) ./APIHelp_Api/APIHelp_Api.cpp

pch.o:./pch.cpp                 		       	    
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(INCLUDE) ./pch.cpp


InstallAll:InstallSo 
InstallSo:./libStorageModule_APIHelp.$(SHAREDLL)
	cp ./libStorageModule_APIHelp.$(SHAREDLL) ../../XEngine_Release/libStorageModule_APIHelp.$(SHAREDLL)


CleanAll:CleanObj CleanMk
CleanObj:                                               
	rm *.o
CleanMk:                             
	rm *.$(SHAREDLL)                        
