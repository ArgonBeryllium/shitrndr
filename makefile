CXX=clang++
CFLAGS=--std=c++17
MINGW=x86_64-w64-mingw32-g++
MINGWFLAGS=-static-libgcc -static-libstdc++ -Wl,-Bstatic -lstdc++ -lpthread

LIBS=-lSDL2
DEMO_NO=0

OUT_FILE=bin/demo
OUT_FILE_WIN=bin/demo.exe

demo: src/* src/**/*
	$(CXX) src/demos/demo_$(DEMO_NO).cpp $(LIBS) $(CFLAGS) -o $(OUT_FILE)

demo-win: src/* src/**/*
	$(MINGW) src/demos/demo_$(DEMO_NO).cpp $(LIBS) $(CFLAGS) $(MINGWFLAGS) -o $(OUT_FILE_WIN)
