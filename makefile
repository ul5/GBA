all:	bin/gba

bin/gba: bin/libBase.so bin/libDecompiler.so bin/libDebugger.so 
	@echo "[GBA] Building GBA Emulator"
	@$(MAKE) -C "GBA"

bin/libBase.so:
	@echo "[GBA] Building libBase.so"
	@$(MAKE) -C "Base"

bin/libDecompiler.so:
	@echo "[GBA] Building libDecompiler.so"
	@$(MAKE) -C "Decompiler"

bin/libDebugger.so:
	@echo "[GBA] Building libDebugger.so"
	@$(MAKE) -C "Debugger"

clean:
	@rm -rf bin/*
	@$(MAKE) -C "GBA" clean
	@$(MAKE) -C "Debugger" clean
	@$(MAKE) -C "Decompiler" clean
	@$(MAKE) -C "Base" clean
