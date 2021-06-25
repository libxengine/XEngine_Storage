CC = g++ -Wall -std=c++17 -fPIC
RELEASE = 0
UNICODE = 0
INCLUDE = -I ./
LOADBIN = 
LIB = 
LIBEX = -static-libgcc -lrt -ldl -lpthread
LOADSO = -Wl,-rpath=
LIBINCLUDE = Session_DLStroage.o Session_UPStroage.o Session_User.o pch.o

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
	$(CC) $(DEBUG) $(LIBINCLUDE) -o libStorageModule_Session.so -shared -fPIC $(LOADBIN) $(LIB) $(LIBEX) $(LOADSO)

Session_DLStroage.o:./Session_Stroage/Session_DLStroage.cpp             
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(INCLUDE) ./Session_Stroage/Session_DLStroage.cpp 
Session_UPStroage.o:./Session_Stroage/Session_UPStroage.cpp             
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(INCLUDE) ./Session_Stroage/Session_UPStroage.cpp 
Session_User.o:./Session_User/Session_User.cpp             
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(INCLUDE) ./Session_User/Session_User.cpp
							              
pch.o:./pch.cpp                 		       	    
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(INCLUDE) ./pch.cpp


InstallAll:InstallSo 
InstallSo:./libStorageModule_Session.so
	cp ./libStorageModule_Session.so ../../XEngine_Release/libStorageModule_Session.so


CleanAll:CleanObj CleanMk
CleanObj:                                               
	rm *.o
CleanMk:                             
	rm *.so                        
