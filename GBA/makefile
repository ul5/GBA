CXX=g++
INCLUDE=-I "../Base/" -I "../Decompiler/" -I "../Debugger/" -I "../GBA/"
LD=g++
LIBS=-L$(shell pwd)/../bin -lDebugger -lDecompiler -lBase
OBJS=GBA/main.o

all: ../bin/gba

../bin/gba: $(OBJS)
	$(LD) -o ../bin/gba $(OBJS) $(LIBS)

%.o: %.cpp
	$(CXX) -c $(INCLUDE) -o $*.o $<

clean:
	rm -rf $(OBJS)
	rm -f ../bin/gba
