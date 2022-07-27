CC = g++ -Wall -std=c++17 -fPIC
SHAREFLAG = -shared
SHAREDLL = so
RELEASE = 0
UNICODE = 0
INCLUDE = -I ./
LOADBIN = -L /usr/local/lib/XEngine_Release/XEngine_BaseLib
LIB = -lXEngine_BaseLib
LIBEX = -ldl -lpthread
LOADSO = -Wl,-rpath=
LIBINCLUDE = Session_DLStroage.o Session_UPStroage.o Session_User.o pch.o

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
	$(CC) $(DEBUG) $(LIBINCLUDE) -o libStorageModule_Session.$(SHAREDLL) $(SHAREFLAG) -fPIC $(LOADBIN) $(LIB) $(LIBEX) $(LOADSO)

Session_DLStroage.o:./Session_Stroage/Session_DLStroage.cpp             
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(INCLUDE) ./Session_Stroage/Session_DLStroage.cpp 
Session_UPStroage.o:./Session_Stroage/Session_UPStroage.cpp             
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(INCLUDE) ./Session_Stroage/Session_UPStroage.cpp 
Session_User.o:./Session_User/Session_User.cpp             
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(INCLUDE) ./Session_User/Session_User.cpp
							              
pch.o:./pch.cpp                 		       	    
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(INCLUDE) ./pch.cpp


InstallAll:InstallSo 
InstallSo:./libStorageModule_Session.$(SHAREDLL)
	cp ./libStorageModule_Session.$(SHAREDLL) ../../XEngine_Release/libStorageModule_Session.$(SHAREDLL)


CleanAll:CleanObj CleanMk
CleanObj:                                               
	rm *.o
CleanMk:                             
	rm *.$(SHAREDLL)                        
