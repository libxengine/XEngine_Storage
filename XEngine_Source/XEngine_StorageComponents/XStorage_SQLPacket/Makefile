CC = g++ -Wall -std=c++17 -fPIC
RELEASE = 0
UNICODE = 0
INCLUDE = -I ./
FILECENTOS = /etc/redhat-release
LOADBIN = -L /usr/local/lib/XEngine_Release/XEngine_BaseLib -L /usr/local/lib/XEngine_Release/XEngine_HelpComponents
LIB = -lXEngine_BaseLib -lHelpComponents_DataBase
LIBEX = -static-libgcc -lrt -ldl 
LOADSO = -Wl,-rpath=./,--disable-new-dtags
LIBINCLUDE = XStorage_SQLHelp.o XStorage_MySql.o XStorage_SQLite.o pch.o

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
	$(CC) $(DEBUG) $(LIBINCLUDE)  -o libXStorage_SQLPacket.so -shared $(LOADBIN) $(LIB) $(LIBEX) $(LOADSO)

XStorage_SQLHelp.o:./XStorage_SQLHelp/XStorage_SQLHelp.cpp                                 
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(INCLUDE) ./XStorage_SQLHelp/XStorage_SQLHelp.cpp
XStorage_MySql.o:./XStorage_MySql/XStorage_MySql.cpp                                 
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(INCLUDE) ./XStorage_MySql/XStorage_MySql.cpp  
XStorage_SQLite.o:./XStorage_SQLite/XStorage_SQLite.cpp                                 
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(INCLUDE) ./XStorage_SQLite/XStorage_SQLite.cpp 
	
pch.o:pch.cpp             
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) pch.cpp


InstallAll:InstallSo
InstallSo:./libXStorage_SQLPacket.so
	cp ./libXStorage_SQLPacket.so ../../../XEngine_Release/libXStorage_SQLPacket.so


CleanAll:CleanObj CleanMk
CleanObj:
	rm *.o                                           
CleanMk:
	rm *.so                                        
