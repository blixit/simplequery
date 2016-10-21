CXX = g++
CXXFLAGS = -W -Wall -std=c++11
LDFLAGS = -pthread
LIB = -lsimplequery
LIBPATH = -L.

EXEC = SQServer.exe 

SRC = $(wildcard *.cpp)  
OBJ = $(SRC:.cpp=.o)


all : clean exe

.PHONY : clean cleanapp exe

exe : $(OBJ) 
	@echo Compiling the executable ...
	$(CXX) -o $(EXEC) $^ $(LDFLAGS) $(LIBPATH) $(LIB)
	./$(EXEC)

%.o : %.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS)

clean : cleanapp
	rm -rf *.o
	rm -rf *~
	rm -rf sqlog

cleanapp : 
	rm -rf $(EXEC)
 
