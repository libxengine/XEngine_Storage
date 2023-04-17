CC = g++ -Wall -std=c++17 -fPIC
PLATFORM = linux
PLATVER = 
PLATDIR = 
FILEEXT = 
LIBFLAG = 
RELEASE = 0
UNICODE = 0
LOADHDR = -I ./ -I ../XEngine_ThirdPart/jsoncpp
LOADSO = -L /usr/local/lib/XEngine_Release/XEngine_BaseLib -L ../XEngine_ThirdPart/jsoncpp
LIB = -lXEngine_BaseLib -ljsoncpp
LIBEX = 
OBJECTS = Protocol_StoragePacket.o Protocol_StorageParse.o pch.o

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
	FILEEXT = so
	LIBFLAG = -shared
	LIBEX = -lpthread -lrt -ldl
	LOADBIN = -Wl,-rpath=./,--disable-new-dtags
else ifeq ($(PLATFORM),mac)
	CC += -Wno-deprecated-declarations
	PLATVER = -D __MACOS__
	PLATDIR = XEngine_Mac
	FILEEXT = dylib
	LIBFLAG = -dynamiclib
	LIBEX = -lpthread -ldl
	LOADBIN = -Wl,-rpath,@loader_path/./
endif

all:$(OBJECTS)
	$(CC) $(DEBUG) $(OBJECTS)  -o libStorageModule_Protocol.$(FILEEXT) $(LIBFLAG) $(LIB) $(LIBEX) $(LOADSO) $(LOADBIN)

Protocol_StoragePacket.o:./Protocol_Packet/Protocol_StoragePacket.cpp                                 
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) ./Protocol_Packet/Protocol_StoragePacket.cpp 
Protocol_StorageParse.o:./Protocol_Parse/Protocol_StorageParse.cpp                                 
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) ./Protocol_Parse/Protocol_StorageParse.cpp 
	
pch.o:pch.cpp             
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) pch.cpp


InstallAll:InstallSo 
InstallSo:./libStorageModule_Protocol.$(FILEEXT)
	cp ./libStorageModule_Protocol.$(FILEEXT) ../../XEngine_Release/libStorageModule_Protocol.$(FILEEXT)


CleanAll:CleanObj CleanMk
CleanObj:
	rm *.o                                           
CleanMk:
	rm *.$(FILEEXT)    
