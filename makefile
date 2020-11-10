CXX=clang++
MINGW=x86_64-w64-mingw32-g++
EMXX=em++

LIBS=-lSDL2
CXXFLAGS=--std=c++17
MINGWFLAGS=-static-libgcc -static-libstdc++ -Wl,-Bstatic -lstdc++ -lpthread
EMXXFLAGS=-s WASM=1 -s USE_SDL=2

SOURCE=src/*
OUT_FILE=bin/demo
OUT_FILE_WIN=bin/win/demo.exe
OUT_FILE_WEB=bin/web/demo.html

demo: $(SOURCE)
	$(CXX) src/demo.cpp $(LIBS) $(CXXFLAGS) -o $(OUT_FILE)

demo-win: $(SOURCE) 
	$(MINGW) src/demo.cpp $(LIBS) $(CXXFLAGS) $(MINGWFLAGS) -o $(OUT_FILE_WIN)

demo-web: $(SOURCE)
	$(EMXX) src/demo.cpp $(CXXFLAGS) $(EMXXFLAGS) -o $(OUT_FILE_WEB) 
