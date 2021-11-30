# Superbly Horrible, Inefficient & Tiresome Rendering Difficulty Reliever
...or **shitrndr** for short, is a boilerplate header for single-window SDL2 applications.

It provides an SDL\_Window\* and SDL\_Renderer\* created using the provided parameters, as well as a **render loop**, **input manager** and **event handling**.
It also provides a simple way to handle rendering at a set resolution in a re-scalable window, in the form of a pixScale and lockTypes.

## Setup
As mentioned before, shitrndr is a header. That's it.  
To use it in your project, simply include it and make sure the right compiler flags are on.  
See the [demo files](https://github.com/ArgonBeryllium/shitrndr/tree/master/src) for a simple usage demonstration.

## The aforementioned compiler flags

### General <sub>(clang, gcc)</sub>
If you're using clang++/g++, you're going to need these:
- `--std=c++17` -- for inline functions and variables, that are there in case you're insane enough to use this in a project consisting of more than a single file
- `-lSDL2` -- I'm guessing you already have this one, considering you're looking at obscure SDL2 program aids

...and that's it if you're compiling natively on Linux with clang++/g++. If you're using something else, however, here are the additional flags you might need to add:

### MINGW flags
Aside from the flags mentioned [above](#general-clang-gcc), you'll need to add the flags listed below if you don't want to ship your executable with a bunch of DLLs:
- `-static-libgcc -static-libstdc++ -Wl,-Bstatic -lstdc++` -- with MINGW, you have to specify these standard libraries for it to link them statically.

## Emscripten flags
Emscripten won't need any of the above to compile, but to include & link the SDL2 port to your application, add:
- `-s USE_SDL=2`

---

Hopefully that was helpful, if not, I can't really say I'm surprised.
