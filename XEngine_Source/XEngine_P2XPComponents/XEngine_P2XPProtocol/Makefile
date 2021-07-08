CC = g++ -Wall -std=c++17 -fPIC
RELEASE = 0
UNICODE = 0
INCLUDE = -I ./
FILECENTOS = /etc/redhat-release
LOADBIN = 
LIB = -ljsoncpp
LIBEX = -static-libgcc -lrt -ldl 
LOADSO = -Wl,-rpath=./,--disable-new-dtags
LIBINCLUDE = P2XPProtocol_ServiceMsg.o P2XPProtocol_UserMsg.o pch.o

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
	$(CC) $(DEBUG) $(LIBINCLUDE)  -o libXEngine_P2XPProtocol.so -shared -fPIC $(LOADBIN) $(LIB) $(LIBEX) $(LOADSO)

P2XPProtocol_ServiceMsg.o:./P2XPProtocol_ServiceMsg/P2XPProtocol_ServiceMsg.cpp                 
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(INCLUDE) ./P2XPProtocol_ServiceMsg/P2XPProtocol_ServiceMsg.cpp
P2XPProtocol_UserMsg.o:./P2XPProtocol_UserMsg/P2XPProtocol_UserMsg.cpp                 
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(INCLUDE) ./P2XPProtocol_UserMsg/P2XPProtocol_UserMsg.cpp
	
pch.o:pch.cpp             
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) pch.cpp


InstallAll:InstallSo 
InstallSo:./libXEngine_P2XPProtocol.so 
	cp ./libXEngine_P2XPProtocol.so ../../../XEngine_Release/libXEngine_P2XPProtocol.so


CleanAll:CleanObj CleanMk
CleanObj:
	rm *.o                                          
CleanMk:
	rm *.so                                       
