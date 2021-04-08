#pragma once
/*
Copyright (c) 2020-2021 ArBe

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

	template <typename T2>
	const vec2<T2> to() const { return {(T2)x, (T2)y}; }

	template <typename T2>
    void operator+=(const vec2<T2>& v) { x += (T)v.x; y += (T)v.y; }
	template <typename T2>
    void operator-=(const vec2<T2>& v) { x -= (T)v.x; y -= (T)v.y; }
	template <typename T2>
    void operator*=(const vec2<T2>& v) { x *= (T)v.x; y *= (T)v.y; }
	template <typename T2>
    void operator/=(const vec2<T2>& v) { x /= (T)v.x; y /= (T)v.y; }
	template <typename T2>
    void operator*=(const T2& s) { x *= (T)s; y *= (T)s; }
	template <typename T2>
    void operator/=(const T2& s) { x /= (T)s; y /= (T)s; }

	// Dot product of the two input vectors.
    static T dot(const vec2& a, const vec2& b);
};

template <typename T1, typename T2>
inline vec2<T1> operator+(const vec2<T1>& a, const vec2<T2>& b) { return vec2<T1>{a.x + (T1)b.x, a.y + (T1)b.y}; }
template <typename T1, typename T2>
inline vec2<T1> operator-(const vec2<T1>& a, const vec2<T2>& b) { return vec2<T1>{a.x - (T1)b.x, a.y - (T1)b.y}; }
template <typename T1, typename T2>
inline vec2<T1> operator*(const vec2<T1>& a, const vec2<T2>& b) { return vec2<T1>{a.x * (T1)b.x, a.y * (T1)b.y}; }
template <typename T1, typename T2>
inline vec2<T1> operator/(const vec2<T1>& a, const vec2<T2>& b) { return vec2<T1>{a.x / (T1)b.x, a.y / (T1)b.y}; }

template <typename T1, typename T2>
inline vec2<T1> operator*(const vec2<T1>& v, const T2& s) { return vec2<T1>{v.x*(T1)s, v.y*(T1)s}; } 
template <typename T1, typename T2>
inline vec2<T1> operator/(const vec2<T1>& v, const T2& s) { return vec2<T1>{v.x/(T1)s, v.y/(T1)s}; } 

template <typename T1, typename T2>
inline vec2<T1> operator*(const T1& s, const vec2<T2>& v) { return vec2<T1>{s*(T1)v.x, s*(T1)v.y}; } 
template <typename T1, typename T2>
inline vec2<T1> operator/(const T1& s, const vec2<T2>& v) { return vec2<T1>{s/(T1)v.x, s/(T1)v.y}; } 

template <typename T>
inline bool operator==(const vec2<T>& a, const vec2<T>& b) { return a.x==b.x && a.y==b.y; }
template <typename T>
inline bool operator!=(const vec2<T>& a, const vec2<T>& b) { return !a==b; }

template <typename T>
inline T vec2<T>::dot(const vec2<T>& a, const vec2<T>& b) { return a.x * b.x + a.y * b.y; }
}

namespace shitrndr
{
inline SDL_Window* win;
inline SDL_Renderer* ren;

// Clear/background colour
inline SDL_Colour bg_col = {0xCC, 0xCC, 0xCC, 0xFF};

// Side bars colour for WindowProps::lockType::BARS
inline static SDL_Colour bars_col = {0,0,0,255};

// DEFAULT EVENT VOIDS //
inline void defOnKey(const SDL_Keycode& key) { std::cout << "key action: " << SDL_GetKeyName(key) << '\n'; }
inline void defOnMB(const uint8_t& but) { std::cout << "mouse button action: " << but << '\n'; }
inline void defOnMM(const helpers::vec2<int>& mp) { std::cout << "mouse moved: { " << mp.x << ", " << mp.y << " }\n"; }
inline void defOnEvent(SDL_Event* event) { std::cout << "SDL event: " << event->type << '\n'; }
inline void defOnRender(double d, double t)
{
	std::cout << "delta: " << d << '\n';
	std::cout << "elapsed: " << t << '\n';
}
/////////////////////////

// EVENT FUNCTION POINTERS //
inline void (*onKeyDown)(const SDL_Keycode& key) = &defOnKey;
inline void (*onKeyHeld)(const SDL_Keycode& key) = &defOnKey;
inline void (*onKeyUp)(const SDL_Keycode& key) = &defOnKey;
inline void (*onMBDown)(const uint8_t& but) = &defOnMB;
inline void (*onMBUp)(const uint8_t& but) = &defOnMB;
inline void (*onMouseMoved)(const helpers::vec2<int>& mp) = &defOnMM;
inline void (*onEvent)(SDL_Event* event) = &defOnEvent;
inline void (*onRender)(double delta, double time) = &defOnRender;
/////////////////////////////

struct WindowProps
{
private:
	inline static int w, h, sw, sh;
	inline static int pixScale = 1;
	inline static bool slocked;
public:
	enum lockType { STRETCH, CUTOFF, BARS };
	inline static lockType lock_type = STRETCH;

	// For internal use // 
	inline static SDL_Texture* renProxy;
	inline static Uint32 format;

	static void updateSize()
	{
		SDL_SetWindowSize(win, w, h);

		if(!slocked)
		{
			SDL_SetRenderTarget(ren, 0);

			format = SDL_GetWindowPixelFormat(win);
			if(renProxy) SDL_DestroyTexture(renProxy);
			renProxy = SDL_CreateTexture(ren, format, SDL_TEXTUREACCESS_TARGET, sw, sh);

#if !defined _WIN32 && !defined __CYGWIN__ && !defined __EMSCRIPTEN__
			if(pixScale!=1) SDL_SetRenderTarget(ren, renProxy);
#endif
		}
	}
	static void setSize(const int& w_, const int& h_) { w = w_; h = h_; if(!getLocked()) { sw = w/pixScale; sh = h/pixScale; } updateSize(); }
	static void setSize(const helpers::vec2<int>& s) { setSize(s.x, s.y); }
	static void setWidth(const int& w_) { w = w_; updateSize(); }
	static void setHeight(const int& h_) { h = h_; updateSize(); }
	static void init(const int& w_, const int& h_)
	{
		sw = w = w_;
		sh = h = h_;
		updateSize();
	}
	//////////////////////

	// For external use //
	static helpers::vec2<int> getSize() { return {sw, sh}; }
	static helpers::vec2<int> getRealSize() { return {w, h}; }

	static SDL_Rect getSizeRect() { return {0, 0, sw, sh}; }
	static SDL_Rect getRealSizeRect() { return {0, 0, w, h}; }

	static int getWidth() { return sw; }
	static int getHeight() { return sh; }
	static int getRealWidth() { return w; }
	static int getRealHeight() { return h; }

	static int getPixScale() { return pixScale; }
	static bool getLocked() { return slocked; }

	static void setPixScale(const int& scale) { if(getLocked()) return; pixScale = scale; sw = w/pixScale; sh = h/pixScale; updateSize(); }
	static void setScaledSize(const int& sw_, const int& sh_) { if(getLocked()) return; sw = sw_; sh = sh_; pixScale = (float)w/(float)sw; updateSize(); }
	static void setLocked(const bool& lock) { slocked = lock; }
	//////////////////////
};
struct Input
{
private:
	inline static std::map<SDL_Keycode, bool> keys;
	inline static std::map<uint8_t, bool> mbs;
	inline static helpers::vec2<int> m_pos;
public:
	static void init()
	{
		m_pos = {};
		keys = {};
		mbs = {};
	}

	// For internal use //
	static void setKey(const SDL_Keycode& key, const bool& state)
	{
		keys[key] = state;
		if(state)	onKeyDown(key);
		else		onKeyUp(key);
	}
	static void setMB(const uint8_t& but, const bool& state)
	{
		mbs[but] = state;
		if(state)	onMBDown(but);
		else		onMBUp(but);
	}
	static void setMP(const int& x, const int& y) { m_pos.x = x; m_pos.y = y; onMouseMoved(m_pos); }
	//////////////////////

	// For external use //
	static bool getKey(const SDL_Keycode& key) { return keys[key]; }
	static bool getMB(const uint8_t& but) { return mbs[but]; }
	static helpers::vec2<int> getMP() { return m_pos; } 
	static helpers::vec2<int> getSSMP() { return (m_pos.to<float>()/WindowProps::getRealSize()*WindowProps::getSize()).to<int>(); } 
	//////////////////////
};

inline void init(const char* name, int w, int h, bool resizable = 1, int winFlags = 0, int renFlags = 0)
{
	SDL_Init(SDL_INIT_VIDEO);
	win = SDL_CreateWindow(name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, (resizable ? SDL_WINDOW_RESIZABLE : 0) | winFlags);
	ren = SDL_CreateRenderer(win, -1, renFlags | SDL_RENDERER_TARGETTEXTURE);

	Input::init();
	WindowProps::init(w, h);
}

// For internal use only //
#ifdef __EMSCRIPTEN__
inline SDL_Event ev;
inline Uint32 last = SDL_GetTicks();
inline double delta, elapsed = 0;

inline void loopCycle()
#else
inline void loopCycle(SDL_Event& ev, Uint32& last, double& delta, double& elapsed)
#endif
{
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

	SDL_SetRenderDrawColor(ren, bg_col.r, bg_col.g, bg_col.b, bg_col.a);
	SDL_RenderClear(ren);
	
	delta 	= 	(double)(SDL_GetTicks() - last)	/ 1000.;
	elapsed	= 	(double)(SDL_GetTicks()) 		/ 1000.;
	last = SDL_GetTicks();

	onRender(delta, elapsed);

	if(WindowProps::getPixScale()!=1 || WindowProps::getLocked())
	{
		SDL_Rect r = WindowProps::getSizeRect();
		SDL_Rect d;

#if defined _WIN32 || defined __CYGWIN__ || defined __EMSCRIPTEN__
		static int pc = WindowProps::getWidth()*WindowProps::getHeight();
		static uint32_t* pixels = new uint32_t[pc];
		if(pc!=WindowProps::getWidth()*WindowProps::getHeight())
		{
			pc = WindowProps::getWidth()*WindowProps::getHeight();
			delete[] pixels;
			pixels = new uint32_t[pc];
		}

		int pitch = r.w*4;

		SDL_RenderReadPixels(ren, &r, WindowProps::format, pixels, pitch);
		SDL_UpdateTexture(WindowProps::renProxy, 0, pixels, pitch);
#endif

		if(WindowProps::getLocked())
		{
			switch(WindowProps::lock_type)
			{
				case WindowProps::BARS:
					{
						SDL_SetRenderDrawColor(ren, bars_col.r, bars_col.g, bars_col.b, bars_col.a);
						SDL_SetRenderTarget(ren, 0);
						SDL_RenderFillRect(ren, 0);

						float cR = float(WindowProps::getRealWidth())/WindowProps::getRealHeight(), oR = float(WindowProps::getWidth())/WindowProps::getHeight(); 
						d = WindowProps::getRealSizeRect();
						if(cR>oR)
						{
							d.w = oR*d.h;
							d.x = (WindowProps::getRealWidth()-d.w)/2;
						}
						else if(oR>cR)
						{
							d.h = d.w/oR;
							d.y = (WindowProps::getRealHeight()-d.h)/2;
						}
					} break;
				case WindowProps::CUTOFF:
					{
						float cR = float(WindowProps::getRealWidth())/WindowProps::getRealHeight(), oR = float(WindowProps::getWidth())/WindowProps::getHeight(); 
						d = WindowProps::getRealSizeRect();
						if(cR>oR) d.h = d.w/oR;
						else if(oR>cR) d.w = oR*d.h;
					} break;
				case WindowProps::STRETCH:
				default: d = WindowProps::getRealSizeRect(); break;
			}
		}
		else d = WindowProps::getRealSizeRect();

#if !defined _WIN32 && !defined __CYGWIN__ && !defined __EMSCRIPTEN__
		SDL_SetRenderTarget(ren, 0);
#endif
		SDL_RenderCopy(ren, WindowProps::renProxy, &r, &d);
		SDL_RenderPresent(shitrndr::ren);
#if !defined _WIN32 && !defined __CYGWIN__ && !defined __EMSCRIPTEN__
		SDL_SetRenderTarget(ren, WindowProps::renProxy);
#endif
	}
	else SDL_RenderPresent(shitrndr::ren);
}
///////////////////////////

// Main loop
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

// Replaces the default event functions with empty ones,
// useful if you don't enjoy an abysmally cluttered cout
inline void silentDefs()
{
	onRender = [](double d, double t){};
	onMBUp = onMBDown = [](const uint8_t& but){}; 
	onMouseMoved = [](const helpers::vec2<int>& mp){};
	onKeyDown = onKeyHeld = onKeyUp = [](const SDL_Keycode& key) {};
	onEvent = [](SDL_Event* event) {};
}


// DRAWING FUNCTIONS //
inline void SetRenderColour(SDL_Renderer* rend, const SDL_Colour& col) { SDL_SetRenderDrawColor(rend, col.r, col.g, col.b, col.a); }
inline void SetColour(const SDL_Colour& col) { SetRenderColour(ren, col); }

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

inline void RenderFillRect(SDL_Renderer* rend, const SDL_Rect& r) { SDL_RenderFillRect(ren, &r); }
inline void FillRect(const SDL_Rect& r) { RenderFillRect(ren, r); }

inline void RenderDrawRect(SDL_Renderer* rend, const SDL_Rect& r) { SDL_RenderDrawRect(rend, &r); }
inline void DrawRect(const SDL_Rect& r) { RenderDrawRect(ren, r); }

inline void RenderCopy(SDL_Renderer* rend, SDL_Texture* tex, const SDL_Rect& src, const SDL_Rect& dst) { SDL_RenderCopy(rend, tex, &src, &dst); }
inline void RenderCopy(SDL_Renderer* rend, SDL_Texture* tex, const SDL_Rect& dst) { SDL_RenderCopy(rend, tex, 0, &dst); }
inline void Copy(SDL_Texture* tex, const SDL_Rect& src, const SDL_Rect& dst) { RenderCopy(ren, tex, src, dst); }
inline void Copy(SDL_Texture* tex, const SDL_Rect& dst) { RenderCopy(ren, tex, dst); }

inline void RenderCopyEx(SDL_Renderer* rend, SDL_Texture* tex, const SDL_Rect& src, const SDL_Rect& dst, const double& angle, const SDL_Point& centre, const SDL_RendererFlip& flip) { SDL_RenderCopyEx(rend, tex, &src, &dst, angle, &centre, flip); }
inline void CopyEx(SDL_Texture* tex, const SDL_Rect& src, const SDL_Rect& dst, const double& angle, const SDL_Point& centre, const SDL_RendererFlip& flip) { SDL_RenderCopyEx(ren, tex, &src, &dst, angle, &centre, flip); }
///////////////////////
}
