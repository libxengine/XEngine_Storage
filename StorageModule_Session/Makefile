CC = g++ -Wall -std=c++17 -fPIC
RELEASE = 0
UNICODE = 0
INCLUDE = -I ./
LOADBIN = -L ../../XEngine_Lib/XEngine_BaseLib -L ../../XEngine_Lib/XEngine_Algorithm
LIB = -lXEngine_BaseLib -lXEngine_Algorithm
LIBEX = -static-libgcc -lrt -ldl -lpthread
LOADSO = -Wl,-rpath=../XEngine_BaseLib,--disable-new-dtags
LIBINCLUDE = Session_Client.o Session_DLStroage.o pch.o

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
	$(CC) $(DEBUG) $(LIBINCLUDE) -o libNetHelp_Session.so -shared -fPIC $(LOADBIN) $(LIB) $(LIBEX) $(LOADSO)

Session_DLStroage.o:./Session_Stroage/Session_DLStroage.cpp             
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(INCLUDE) ./Session_Stroage/Session_DLStroage.cpp 
					              
pch.o:./pch.cpp                 		       	    
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(INCLUDE) ./pch.cpp


InstallAll:InstallSo 
InstallSo:./libNetHelp_Session.so
	cp ./libNetHelp_Session.so ../../../XEngine_Release/XEngine_NetHelp/libNetHelp_Session.so


CleanAll:CleanObj CleanMk
CleanObj:                                               
	rm *.o
CleanMk:                             
	rm *.so                        
