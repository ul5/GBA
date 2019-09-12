CXX=g++
INCLUDE=
OBJS=Base/Logic.o CPU/CPU.o MMU/MMU.o Register/Register.o Register/RegisterSet.o

all: ../bin/libBase.so

../bin/libBase.so: $(OBJS)
	$(CXX) -shared $(OBJS) -o ../bin/libBase.so
	
%.o: %.cpp
	$(CXX) -c -fPIC $(INCLUDE) -o $*.o $<

clean:
	rm -rf $(OBJS)
	rm -f ../bin/libBase.so
