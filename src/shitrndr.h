#pragma once

/*
Copyright (c) 2020 ArBe

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/


#include <iostream>
#include <map>

#if defined _WIN32 || defined __CYGWIN__ || defined __EMSCRIPTEN__
#include <cmath>
#endif
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include <SDL2/SDL.h>

namespace shitrndr::helpers
{
template <typename T>
struct vec2
{
    T x, y;
    vec2() : x(0), y(0) {}
    vec2(const T x_, const T y_) : x(x_), y(y_) {}
    T getLength() { return sqrt(x*x + y*y); }
    T getLengthSquare() { return x*x + y*y; }
    vec2<T> normalised()  { T l = getLength(); return l==0 ? vec2{} : *this / l; }

    void operator+=(const vec2& v) { x += v.x; y += v.y; }
    void operator-=(const vec2& v) { x -= v.x; y -= v.y; }
    void operator*=(const vec2& v) { x *= v.x; y *= v.y; }
    void operator/=(const vec2& v) { x /= v.x; y /= v.y; }
    void operator*=(const float s) { x *= s; y *= s; }
    void operator/=(const float s) { x /= s; y /= s; }
    static T dot(const vec2& a, const vec2& b);
};

template <typename T>
inline vec2<T> operator+(const vec2<T>& a, const vec2<T>& b) { return vec2{a.x + b.x, a.y + b.y}; }
template <typename T>
inline vec2<T> operator-(const vec2<T>& a, const vec2<T>& b) { return vec2{a.x - b.x, a.y - b.y}; }
template <typename T>
inline vec2<T> operator*(const vec2<T>& a, const vec2<T>& b) { return vec2{a.x * b.x, a.y * b.y}; }
template <typename T>
inline vec2<T> operator/(const vec2<T>& a, const vec2<T>& b) { return vec2{a.x / b.x, a.y / b.y}; }
template <typename T>
inline vec2<T> operator*(const vec2<T>& v, float s) { return vec2{v.x*s, v.y*s}; } 
template <typename T>
inline vec2<T> operator/(const vec2<T>& v, float s) { return vec2{v.x/s, v.y/s}; } 

template <typename T>
inline bool operator==(const vec2<T>& a, const vec2<T>& b) { return a.x==b.x && a.y==b.y; }

template <typename T>
inline T vec2<T>::dot(const vec2<T>& a, const vec2<T>& b) { return a.x * b.x + a.y * b.y; }

}

namespace shitrndr
{
inline SDL_Window* 	win;
inline SDL_Renderer* ren;

inline SDL_Colour bg_col = {0xCC, 0xCC, 0xCC};

inline void defOnKey(const SDL_Keycode& key)
{
	std::cout << "key action: " << SDL_GetKeyName(key) << '\n';
}
inline void (*onKeyDown)(const SDL_Keycode& key) = &defOnKey;
inline void (*onKeyHeld)(const SDL_Keycode& key) = &defOnKey;
inline void (*onKeyUp)(const SDL_Keycode& key) = &defOnKey;


inline void defOnMB(const uint8_t& but)
{
	std::cout << "mouse button action: " << but << '\n';
}
inline void (*onMBDown)(const uint8_t& but) = &defOnMB;
inline void (*onMBUp)(const uint8_t& but) = &defOnMB;

inline void defOnMM(const helpers::vec2<uint32_t>& mp)
{
	std::cout << "mouse moved: { " << mp.x << ", " << mp.y << " }\n"; 
}
inline void (*onMouseMoved)(const helpers::vec2<uint32_t>& mp) = &defOnMM;

inline void defOnEvent(SDL_Event* event)
{
	std::cout << "SDL event: " << event->type << '\n'; 
}
inline void (*onEvent)(SDL_Event* event) = &defOnEvent;

struct Input
{
private:
	inline static std::map<SDL_Keycode, bool> keys;
	inline static std::map<uint8_t, bool> mbs;
	inline static helpers::vec2<uint32_t> m_pos;
public:
	static void init()
	{
		m_pos = {};
		keys = {};
		mbs = {};
	}

	static void setKey(const SDL_Keycode& key, const bool& state)
	{
		keys[key] = state;
		if(state)	onKeyDown(key);
		else		onKeyUp(key);
	}
	static bool getKey(const SDL_Keycode& key) { return keys[key]; }

	static void setMB(const uint8_t& but, const bool& state)
	{
		mbs[but] = state;
		if(state)	onMBDown(but);
		else		onMBUp(but);
	}
	static bool getMB(const uint8_t& but) { return mbs[but]; }

	static void setMP(const int& x, const int& y) { m_pos.x = x; m_pos.y = y; onMouseMoved(m_pos); }
	static helpers::vec2<uint32_t> getMP() { return m_pos; } 
};
struct WindowProps
{
private:
	inline static uint32_t w, h, pixScale = 1;
public:
	inline static SDL_Texture* renProxy;
	inline static Uint32 format;
	static void updateSize()
	{
		SDL_SetWindowSize(win, w, h);

		format = SDL_GetWindowPixelFormat(win);
		if(renProxy) SDL_DestroyTexture(renProxy);
		renProxy = SDL_CreateTexture(ren, format, SDL_TEXTUREACCESS_STREAMING, w/pixScale, h/pixScale);
	}

	static void init(const uint32_t& w_, const uint32_t& h_)
	{
		w = w_;
		h = h_;
		updateSize();
	}
	static void setSize(const uint32_t& w_, const uint32_t& h_) { w = w_; h = h_; updateSize(); }
	static void setSize(const helpers::vec2<uint32_t>& s) { setSize(s.x, s.y); }
	static void setWidth(const uint32_t& w_) { w = w_; updateSize(); }
	static void setHeight(const uint32_t& h_) { h = h_; updateSize(); }
	static void setPixScale(const uint32_t& scale) { pixScale = scale; updateSize(); }

	static helpers::vec2<uint32_t> getSize() { return {(uint32_t)(w/pixScale), (uint32_t)(h/pixScale)}; }
	static SDL_Rect getSizeRect() { return {0, 0, (int)(w/pixScale), (int)(h/pixScale)}; }
	static SDL_Rect getRealSizeRect() { return {0, 0, (int)w, (int)h}; }
	static uint32_t getWidth() { return w/pixScale; }
	static uint32_t getHeight() { return h/pixScale; }
	static uint32_t getRealWidth() { return w; }
	static uint32_t getRealHeight() { return h; }
	static uint32_t getPixScale() { return pixScale; }
};

inline void init(const char* name, int w, int h, bool resizable)
{
	SDL_Init(SDL_INIT_VIDEO);
	win = SDL_CreateWindow(name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, resizable ? SDL_WINDOW_RESIZABLE : 0);
	ren = SDL_CreateRenderer(win, -1, 0);

	Input::init();
	WindowProps::init(w, h);
}

// default onRender void
inline void defOnRender(double d, double t)
{
	std::cout << "delta: " << d << '\n';
	std::cout << "elapsed: " << t << '\n';
}
inline void (*onRender)(double delta, double time) = &defOnRender;

#ifdef __EMSCRIPTEN__
inline SDL_Event ev;
inline Uint32 last = SDL_GetTicks();
inline double delta, elapsed = 0;

inline void loopCycle()
#else
inline void loopCycle(SDL_Event& ev, Uint32& last, double& delta, double& elapsed)
#endif
{
	// handle events
	while(SDL_PollEvent(&ev) && ev.window.event != SDL_WINDOWEVENT_CLOSE)
	{
		onEvent(&ev);
		switch (ev.type)
		{
		case SDL_MOUSEMOTION:
			int mx, my;
			SDL_GetMouseState(&mx, &my);
			Input::setMP(mx, my);
			break;
		case SDL_KEYDOWN:
			if(Input::getKey(ev.key.keysym.sym)) onKeyHeld(ev.key.keysym.sym);
			else Input::setKey(ev.key.keysym.sym, 1); break;
		case SDL_KEYUP:		Input::setKey(ev.key.keysym.sym, 0); break;
		case SDL_MOUSEBUTTONDOWN:	 Input::setMB(ev.button.button, 1); break;
		case SDL_MOUSEBUTTONUP:		 Input::setMB(ev.button.button, 0); break;
		}
		if(ev.window.event==SDL_WINDOWEVENT_RESIZED)
			WindowProps::setSize(ev.window.data1, ev.window.data2); 
	}
#ifdef __EMSCRIPTEN__
	if(ev.window.event==SDL_WINDOWEVENT_CLOSE)
		emscripten_cancel_main_loop();
#endif

	// clear render buffer
	SDL_SetRenderDrawColor(shitrndr::ren, bg_col.r, bg_col.g, bg_col.b, bg_col.a);
	SDL_RenderClear(ren);
	
	// calculate timing
	delta 	= 	(double)(SDL_GetTicks() - last)	/ 1000;
	elapsed	= 	(double)(SDL_GetTicks()) 		/ 1000;
	last = SDL_GetTicks();

	onRender(delta, elapsed);

	if(WindowProps::getPixScale()!=1)
	{
		uint32_t* pixels = new uint32_t[WindowProps::getWidth()*WindowProps::getHeight()];
		SDL_Rect r = WindowProps::getSizeRect();

		SDL_RenderReadPixels(ren, &r, WindowProps::format, pixels, r.w*4);
		SDL_UpdateTexture(WindowProps::renProxy, 0, pixels, r.w*4);
		SDL_RenderCopy(ren, WindowProps::renProxy, &r, 0);

		delete[] pixels;
	}
	SDL_RenderPresent(shitrndr::ren); // copy render buffer to window
}

// main loop
inline void loop()
{
#ifndef __EMSCRIPTEN__
	SDL_Event ev;

	Uint32 last = SDL_GetTicks();
	double delta, elapsed = 0;

	while(ev.window.event != SDL_WINDOWEVENT_CLOSE)
		loopCycle(ev, last, delta, elapsed);

	SDL_Quit();
#else
    emscripten_set_main_loop(loopCycle, 0, 1);
#endif
}

inline void silentDefs()
{
	onRender = [](double d, double t){};
	onMBUp = onMBDown = [](const uint8_t& but){}; 
	onMouseMoved = [](const helpers::vec2<uint32_t>& mp){};
	onKeyDown = onKeyHeld = onKeyUp = [](const SDL_Keycode& key) {};
	onEvent = [](SDL_Event* event) {};
}


// drawing functions

inline void RenderDrawCircle(SDL_Renderer* rend, const int& x, const int& y, const float& r)
{
	float l = 2*M_PI*r;
	for(float a = 0; a < l; a++)
		SDL_RenderDrawPoint(rend, x + std::cos(a/l * 2*M_PI) * r, y + std::sin(a/l * 2*M_PI) * r);
}
inline void DrawCircle(const int& x, const int& y, const float& r) { RenderDrawCircle(ren, x, y, r); }

inline void RenderFillCircle(SDL_Renderer* rend, const int& x, const int& y, const float& r)
{
	int rs = r*r;
	for(int py = -r; py < r; py++)
		for(int px = -r; px < r; px++)
			if(py*py + px*px < rs)
				SDL_RenderDrawPoint(rend, x + px, y + py);
}
inline void FillCircle(const int& x, const int& y, const float& r) { RenderFillCircle(ren, x, y, r); }

inline void RenderFillRect(SDL_Renderer* rend, const SDL_Rect& r)
{
	const SDL_Rect r_ = r;
	SDL_RenderFillRect(ren, &r);
}
inline void FillRect(const SDL_Rect& r) { RenderFillRect(ren, r); }

}
