CC = g++ -Wall -std=c++20 -fPIC
PLATVER = 
FILEEXT = 
LIBFLAG = 
LOADHDR = -I ./
LIB = -lXEngine_BaseLib
LIBEX = 
LOADSO = 
OBJECTS = BTorrent_Creator.o BTorrent_DLoader.o BTorrent_Parse.o pch.o 

MACRO_VALUE := $(shell gcc -E -dM ../XStorage_BuildSwitch.h | grep _XENGIEN_STORAGE_BUILDSWITCH_BTORRENT | cut -d ' ' -f 3)

ifeq ($(MACRO_VALUE),1)
    LIB += -ltorrent-rasterbar
endif

ifeq ($(RELEASE),1)
FLAGS = -c -D _RELEASE
DEBUG = -O2
else
	ifeq ($(PLATFORM),mac)
		FLAGS = -c
		DEBUG = -g -D _DEBUG
	else
		FLAGS = -c -lc_p
		DEBUG = -g -pg -D _DEBUG
	endif
endif

ifeq ($(UNICODE),1)
override UNICODE := -D __UNICODE__
else
override UNICODE := -D __ANSI__
endif

ifeq ($(PLATFORM),linux)
	ifeq (/etc/redhat-release,$(wildcard /etc/redhat-release))
		PLATVER = -D __CENTOS__
	else
		PLATVER = -D __UBUNTU__
	endif
	FILEEXT = so
	LIBFLAG = -shared
	LIBEX = -lpthread -lrt -ldl
	LOADBIN = -Wl,-rpath=../XEngine_BaseLib,--disable-new-dtags
else ifeq ($(PLATFORM),mac)
	CC += -Wno-deprecated-declarations
	PLATVER = -D __MACOS__
	FILEEXT = dylib
	LIBFLAG = -dynamiclib
	LIBEX = -lpthread -ldl
	LOADBIN = -Wl,-rpath,@loader_path/../XEngine_BaseLib/
endif

all:$(OBJECTS)
	$(CC) $(DEBUG) $(OBJECTS) -o libStorageModule_BTorrent.$(FILEEXT) $(LIBFLAG) $(LIB) $(LIBEX) $(LOADSO) $(LOADBIN)

BTorrent_Creator.o:./BTorrent_Creator/BTorrent_Creator.cpp             
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) ./BTorrent_Creator/BTorrent_Creator.cpp 
BTorrent_DLoader.o:./BTorrent_DLoader/BTorrent_DLoader.cpp              	    
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) ./BTorrent_DLoader/BTorrent_DLoader.cpp   
BTorrent_Parse.o:./BTorrent_Parse/BTorrent_Parse.cpp              	    
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) ./BTorrent_Parse/BTorrent_Parse.cpp
	             
pch.o:./pch.cpp            
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) ./pch.cpp

InstallAll:InstallSo 
InstallSo:./libStorageModule_BTorrent.$(FILEEXT)
	cp ./libStorageModule_BTorrent.$(FILEEXT) ../../XEngine_Release/libStorageModule_BTorrent.$(FILEEXT)

CleanAll:CleanObj CleanMk
CleanObj:                                                   
	rm *.o
CleanMk:                                   
	rm *.$(FILEEXT)                        
