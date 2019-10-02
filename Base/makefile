CXX=g++
INCLUDE=-I"../libs/include" -I"../Base"
OBJS=Base/Logic.o CPU/CPU.o GPU/GPU.o MMU/MMU.o Register/Register.o Register/RegisterSet.o
LIBS=-L$(shell pwd)/../bin -L"../libs/lib" -lSDL2 -lSDL2_ttf

all: ../bin/libBase.so

../bin/libBase.so: $(OBJS)
	$(CXX) -shared $(OBJS) -o ../bin/libBase.so $(LIBS)
	
%.o: %.cpp
	$(CXX) -c -std=c++11 -fPIC $(INCLUDE) -o $*.o $<

clean:
	rm -rf $(OBJS)
	rm -f ../bin/libBase.so
