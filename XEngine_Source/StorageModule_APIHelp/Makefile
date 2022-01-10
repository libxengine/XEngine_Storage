CC = g++ -Wall -std=c++17 -fPIC
RELEASE = 0
UNICODE = 0
INCLUDE = -I ./
LOADBIN = 
LIB = 
LIBEX = -static-libgcc -lrt -ldl -lpthread
LOADSO = -Wl,-rpath=
LIBINCLUDE = APIHelp_Distributed.o APIHelp_Api.o pch.o

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
	$(CC) $(DEBUG) $(LIBINCLUDE) -o libStorageModule_APIHelp.so -shared -fPIC $(LOADBIN) $(LIB) $(LIBEX) $(LOADSO)

APIHelp_Distributed.o:./APIHelp_Distributed/APIHelp_Distributed.cpp             
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(INCLUDE) ./APIHelp_Distributed/APIHelp_Distributed.cpp 
APIHelp_Api.o:./APIHelp_Api/APIHelp_Api.cpp             
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(INCLUDE) ./APIHelp_Api/APIHelp_Api.cpp

pch.o:./pch.cpp                 		       	    
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(INCLUDE) ./pch.cpp


InstallAll:InstallSo 
InstallSo:./libStorageModule_APIHelp.so
	cp ./libStorageModule_APIHelp.so ../../XEngine_Release/libStorageModule_APIHelp.so


CleanAll:CleanObj CleanMk
CleanObj:                                               
	rm *.o
CleanMk:                             
	rm *.so                        
