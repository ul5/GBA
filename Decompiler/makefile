CXX=g++
INCLUDE=-I "../Base/"
OBJS=ARM/ARMDecompiler.o THUMB/THUMBDecompiler.o

all: ../bin/libDecompiler.so

../bin/libDecompiler.so: $(OBJS)
	$(CXX) -shared $(OBJS) -o ../bin/libDecompiler.so
	
%.o: %.cpp
	$(CXX) -c -fPIC $(INCLUDE) -o $*.o $<

clean:
	rm -rf $(OBJS)
	rm -f ../bin/libDecompiler.so
