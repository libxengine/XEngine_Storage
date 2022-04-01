CC = g++ -Wall -std=c++17 -fPIC
RELEASE = 0
UNICODE = 0
INCLUDE = -I ./
FILECENTOS = /etc/redhat-release
LOADBIN = -L /usr/local/lib/XEngine_Release/XEngine_BaseLib
LIB = -lXEngine_BaseLib
LIBEX = -static-libgcc -lrt -ldl -lpthread
LOADSO = -Wl,-rpath=./,--disable-new-dtags
LIBINCLUDE = P2XPPeer_Manage.o pch.o

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
	$(CC) $(DEBUG) $(LIBINCLUDE)  -o libStorageModule_P2XPPeer.so -shared -fPIC $(LOADBIN) $(LIB) $(LIBEX) $(LOADSO)

    
P2XPPeer_Manage.o:./P2XPPeer_Manage/P2XPPeer_Manage.cpp                                 
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(INCLUDE) ./P2XPPeer_Manage/P2XPPeer_Manage.cpp 

pch.o:pch.cpp             
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) pch.cpp


InstallAll:InstallSo 
InstallSo:./libStorageModule_P2XPPeer.so 
	cp ./libStorageModule_P2XPPeer.so ../../XEngine_Release/libStorageModule_P2XPPeer.so


CleanAll:CleanObj CleanMk
CleanObj:
	rm *.o                                           
CleanMk:
	rm *.so                                       
