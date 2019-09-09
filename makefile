all:	bin/gba

bin/gba: bin/libBase.so bin/libDecompiler.so bin/libDebugger.so 
	@echo "Building GBA Emulator"
	@$(MAKE) -C "GBA"

bin/libBase.so:
	@echo "Building libBase.so"
	@$(MAKE) -C "Base"

bin/libDecompiler.so:
	@echo "Building libDecompiler.so"
	@$(MAKE) -C "Decompiler"

bin/libDebugger.so:
	@echo "Building libDebugger.so"
	@$(MAKE) -C "Debugger"

clean:
	@rm -rf bin/*
	@$(MAKE) -C "Debugger" clean
	@$(MAKE) -C "Decompiler" clean
	@$(MAKE) -C "Base" clean
