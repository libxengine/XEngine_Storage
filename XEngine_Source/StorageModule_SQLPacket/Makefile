CC = g++ -Wall -std=c++17 -fPIC
RELEASE = 0
UNICODE = 0
INCLUDE = -I ./
LOADBIN = -L /usr/local/lib/XEngine_Release/XEngine_BaseLib -L /usr/local/lib/XEngine_Release/XEngine_HelpComponents
LIB = -lXEngine_BaseLib -lHelpComponents_Database
LIBEX = -static-libgcc -lrt -ldl -lpthread
LOADSO = -Wl,-rpath=,--disable-new-dtags
LIBINCLUDE = SQLPacket_File.o pch.o

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
	$(CC) $(DEBUG) $(LIBINCLUDE) -o libStorageModule_SQLPacket.so -shared -fPIC $(LOADBIN) $(LIB) $(LIBEX) $(LOADSO)

SQLPacket_File.o:./SQLPacket_File/SQLPacket_File.cpp             
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(INCLUDE) ./SQLPacket_File/SQLPacket_File.cpp  
						              
pch.o:./pch.cpp                 		       	    
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(INCLUDE) ./pch.cpp


InstallAll:InstallSo 
InstallSo:./libStorageModule_SQLPacket.so
	cp ./libStorageModule_SQLPacket.so ../../XEngine_Release/libStorageModule_SQLPacket.so


CleanAll:CleanObj CleanMk
CleanObj:                                               
	rm *.o
CleanMk:                             
	rm *.so                        
