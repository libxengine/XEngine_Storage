CC = g++ -Wall -std=c++17 -fPIC
SHAREFLAG = -shared
SHAREDLL = so
RELEASE = 0
UNICODE = 0
INCLUDE = -I ./
FILECENTOS = /etc/redhat-release
LOADBIN = -L /usr/local/lib/XEngine_Release/XEngine_BaseLib -L /usr/local/lib/XEngine_Release/XEngine_HelpComponents
LIB = -lXEngine_BaseLib -lHelpComponents_DataBase
LIBEX = -ldl 
LOADSO = -Wl,-rpath=./,--disable-new-dtags
LIBINCLUDE = Database_File.o Database_Help.o pch.o

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
	$(CC) $(DEBUG) $(LIBINCLUDE)  -o libStorageModule_Database.$(SHAREDLL) $(SHAREFLAG) $(LOADBIN) $(LIB) $(LIBEX) $(LOADSO)

Database_File.o:./Database_File/Database_File.cpp                                 
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(INCLUDE) ./Database_File/Database_File.cpp
Database_Help.o:./Database_Help/Database_Help.cpp                                 
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(INCLUDE) ./Database_Help/Database_Help.cpp

pch.o:pch.cpp             
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) pch.cpp


InstallAll:InstallSo
InstallSo:./libStorageModule_Database.$(SHAREDLL)
	cp ./libStorageModule_Database.$(SHAREDLL) ../../XEngine_Release/libStorageModule_Database.$(SHAREDLL)


CleanAll:CleanObj CleanMk
CleanObj:
	rm *.o                                           
CleanMk:
	rm *.$(SHAREDLL)                                        
