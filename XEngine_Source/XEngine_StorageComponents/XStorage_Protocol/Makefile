CC = g++ -Wall -std=c++17 -fPIC
RELEASE = 0
UNICODE = 0
INCLUDE = -I ./
FILECENTOS = /etc/redhat-release
LOADBIN = -L /usr/local/lib/XEngine_Release/XEngine_BaseLib
LIB = -lXEngine_BaseLib -ljsoncpp
LIBEX = -static-libgcc -lrt -ldl 
LOADSO = -Wl,-rpath=,--disable-new-dtags
LIBINCLUDE = XStorageProtocol_Client.o XStorageProtocol_Comm.o XStorageProtocol_Core.o pch.o

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

ifeq ($(FILECENTOS), $(wildcard $(FILECENTOS)))
	UNICODE = -D _CENTOS
else
	UNICODE = -D _UBUNTU
endif

all:$(LIBINCLUDE)
	$(CC) $(DEBUG) $(LIBINCLUDE)  -o libXStorage_Protocol.so -shared $(LOADBIN) $(LIB) $(LIBEX) $(LOADSO)

XStorageProtocol_Client.o:./XStorageProtocol_Client/XStorageProtocol_Client.cpp                                 
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(INCLUDE)  ./XStorageProtocol_Client/XStorageProtocol_Client.cpp  
XStorageProtocol_Comm.o:./XStorageProtocol_Comm/XStorageProtocol_Comm.cpp                                 
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(INCLUDE)  ./XStorageProtocol_Comm/XStorageProtocol_Comm.cpp 
XStorageProtocol_Core.o:./XStorageProtocol_Core/XStorageProtocol_Core.cpp                                 
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(INCLUDE)  ./XStorageProtocol_Core/XStorageProtocol_Core.cpp 

pch.o:pch.cpp             
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) pch.cpp


InstallAll:InstallSo 
InstallSo:./libXStorage_Protocol.so
	cp ./libXStorage_Protocol.so ../../../XEngine_Release/libXStorage_Protocol.so


CleanAll:CleanObj CleanMk
CleanObj:
	rm *.o                                           
CleanMk:
	rm *.so                                        
