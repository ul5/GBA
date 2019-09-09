CXX=g++
INCLUDE=-I "../Base/" -I "../Decompiler/"
OBJS=debugger/Debugger.o interpreter/Interpreter.o interpreter/ARM/ARMBranch.o interpreter/ARM/ARMDataProcessing.o interpreter/ARM/ARMInstructions.o interpreter/ARM/ARMLoadStore.o interpreter/THUMB/THUMBALU.o interpreter/THUMB/THUMBDataProcessing.o interpreter/THUMB/THUMBInstructions.o

all: ../bin/libDebugger.so

../bin/libDebugger.so: $(OBJS)
	$(CXX) -shared $(OBJS) -o ../bin/libDebugger.so
	
%.o: %.cpp
	$(CXX) -c -fPIC $(INCLUDE) -o $*.o $<

clean:
	rm -rf $(OBJS)
	rm -f ../bin/libDebugger.so
