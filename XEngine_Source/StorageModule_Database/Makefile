CC = g++ -Wall -std=c++17 -fPIC
PLATFORM = linux
PLATVER = 
PLATDIR = 
FILEEXT = 
LIBFLAG = 
RELEASE = 0
UNICODE = 0
LOADHDR = -I ./ -I ../XEngine_ThirdPart/jsoncpp
LOADSO = 
LIB = -lXEngine_BaseLib -lHelpComponents_DataBase
LIBEX = 
OBJECTS = Database_File.o Database_Memory.o Database_Help.o pch.o

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
	$(CC) $(DEBUG) $(OBJECTS)  -o libStorageModule_Database.$(FILEEXT) $(LIBFLAG) $(LIB) $(LIBEX) $(LOADSO) $(LOADBIN)

Database_File.o:./Database_File/Database_File.cpp                                 
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) ./Database_File/Database_File.cpp
Database_Memory.o:./Database_Memory/Database_Memory.cpp                                 
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) ./Database_Memory/Database_Memory.cpp
Database_Help.o:./Database_Help/Database_Help.cpp                                 
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) ./Database_Help/Database_Help.cpp

pch.o:pch.cpp             
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) pch.cpp


InstallAll:InstallSo
InstallSo:./libStorageModule_Database.$(FILEEXT)
	cp ./libStorageModule_Database.$(FILEEXT) ../../XEngine_Release/libStorageModule_Database.$(FILEEXT)


CleanAll:CleanObj CleanMk
CleanObj:
	rm *.o                                           
CleanMk:
	rm *.$(FILEEXT)                                        
