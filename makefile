CXX=clang++
CFLAGS=--std=c++17

LIBS=-lSDL2
DEMO_NO=0

OUT_FILE=bin/demo

demo: src/*
	$(CXX) src/demos/demo_$(DEMO_NO).cpp $(LIBS) -o $(OUT_FILE)
