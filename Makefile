CXX = g++
CXXFLAGS = -W -Wall -std=c++11 -fPIC  
LDFLAGS = -pthread

EXEC = simplequery.exe
LIB = libsimplequery.a  

generateExe = main.cpp
SRC = $(wildcard *.cpp) 
SRCEXE =  $(SRC) $(generateExe) 
SRCLIB = SQUtils.cpp $(SRC)  
OBJEXE = $(SRC:.cpp=.o)
OBJLIB = $(SRCLIB:.cpp=.o)

HEADERS = SQEvents.h SQUtils.h SQException.h SQDataHeaders.h SQPacket.h  SQStringPacket.h SQCommunicator.h SQNetEntity.h SQClient.h SQServer.h
HEADERNAME = simplequery.h


all : clean lib #exe

.PHONY : clean cleanapp cleanlib lib exe client server

exe : $(OBJEXE)  
	#@echo Compiling the executable ...
	#$(CXX) -o $(EXEC) $^ $(LDFLAGS) 

client : 
	cd client/ && \
	make && \
	cd ../ ;
	
server : 
	cd server/ && \
	make && \
	cd ../ ;

lib : $(OBJLIB)
	ar rv -o $(LIB) $^
	cat $(HEADERS) > $(HEADERNAME)
	awk '!/(include)([\ ]+)\"/' $(HEADERS) > temp && mv temp $(HEADERNAME) 
	cp $(LIB) client/
	cp $(HEADERNAME) client/
	cp $(LIB) server/
	cp $(HEADERNAME) server/

%.o : %.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS) $(LDFLAGS) 

clean : cleanapp cleanlib
	rm -rf *.o
	rm -rf *~

cleanapp :
	rm -rf $(EXEC)

cleanlib : 
	rm -rf $(LIB)
