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
#include <functional>
#include <vector>
#include <list>

#if defined _WIN32 || defined __CYGWIN__ || defined __EMSCRIPTEN__
#include <cmath>
#endif
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif
#ifndef M_PI
	#define M_PI 3.14159265358979323846
#endif

#include <SDL2/SDL.h>

#pragma clang diagnostic ignored "-Wc++17-extensions"

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
	vec2<T2> to() const { return {(T2)x, (T2)y}; }

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
inline bool operator!=(const vec2<T>& a, const vec2<T>& b) { return !(a==b); }

template <typename T>
inline T vec2<T>::dot(const vec2<T>& a, const vec2<T>& b) { return a.x * b.x + a.y * b.y; }

// required for hashmaps
const inline bool operator<(const shitrndr::helpers::vec2<float>& a, const shitrndr::helpers::vec2<float>& b) { return a.x < b.x; }

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
inline void _defOnKey(const SDL_Keycode& key) { std::cout << "key action: " << SDL_GetKeyName(key) << '\n'; }
inline void _defOnMB(const uint8_t& but) { std::cout << "mouse button action: " << but << '\n'; }
inline void _defOnMM(const helpers::vec2<int>& mp) { std::cout << "mouse moved: { " << mp.x << ", " << mp.y << " }\n"; }
inline void _defOnEvent(SDL_Event* event) { std::cout << "SDL event: " << event->type << '\n'; }
inline void _defOnRender(double d, double t)
{
	std::cout << "delta: " << d << '\n';
	std::cout << "elapsed: " << t << '\n';
}
/////////////////////////

// EVENT FUNCTION POINTERS //
inline std::function<void(const SDL_Keycode& key)> onKeyDown = &_defOnKey;
inline std::function<void(const SDL_Keycode& key)> onKeyHeld = &_defOnKey;
inline std::function<void(const SDL_Keycode& key)> onKeyUp = &_defOnKey;
inline std::function<void(const uint8_t& but)> onMBDown = &_defOnMB;
inline std::function<void(const uint8_t& but)> onMBUp = &_defOnMB;
inline std::function<void(const helpers::vec2<int>& mp)> onMouseMoved = &_defOnMM;
inline std::function<void(SDL_Event* event)> onEvent = &_defOnEvent;
inline std::function<void(double delta, double time)> onRender = &_defOnRender;
/////////////////////////////

struct WindowProps
{
	enum lockType { STRETCH, CUTOFF, BARS };
private:
	inline static int w, h, sw, sh;
	inline static int pixScale = 1;
	inline static bool slocked;
	inline static lockType lock_type = STRETCH;
	inline static std::list<std::function<void(SDL_Texture*, double, double)>> effects;

	static void _applyPixScale(SDL_Texture* proxy, double delta, double elapsed)
	{
		if(getPixScale()==1 && !getLocked()) return;
		SDL_Rect r = getSizeRect();
		SDL_Rect d;

		if(getLocked())
		{
			switch(getLockType())
			{
				case BARS:
					{
						SDL_SetRenderDrawColor(ren, bars_col.r, bars_col.g, bars_col.b, bars_col.a);
						SDL_SetRenderTarget(ren, 0);
						SDL_RenderFillRect(ren, 0);

						float cR = float(getRealWidth())/getRealHeight(), oR = float(getWidth())/getHeight(); 
						d = getRealSizeRect();
						if(cR>oR)
						{
							d.w = oR*d.h;
							d.x = (getRealWidth()-d.w)/2;
						}
						else if(oR>cR)
						{
							d.h = d.w/oR;
							d.y = (getRealHeight()-d.h)/2;
						}
					} break;
				case CUTOFF:
					{
						float cR = float(getRealWidth())/getRealHeight(), oR = float(getWidth())/getHeight(); 
						d = getRealSizeRect();
						if(cR>oR) d.h = d.w/oR;
						else if(oR>cR) d.w = oR*d.h;
					} break;
				case STRETCH:
				default: d = getRealSizeRect(); break;
			}
		}
		else d = getRealSizeRect();

#if !defined _WIN32 && !defined __CYGWIN__ && !defined __EMSCRIPTEN__
		SDL_SetRenderTarget(ren, 0);
#endif
		SDL_RenderCopy(ren, proxy, &r, &d);
#if !defined _WIN32 && !defined __CYGWIN__ && !defined __EMSCRIPTEN__
		SDL_SetRenderTarget(ren, WindowProps::_renProxy);
#endif
	}
public:
	// For internal use // 
	inline static SDL_Texture* _renProxy;
	inline static Uint32 _format;

	static void _updateSize()
	{
		SDL_SetWindowSize(win, w, h);

		if(!slocked)
		{
			SDL_SetRenderTarget(ren, 0);

			_format = SDL_GetWindowPixelFormat(win);
			if(_renProxy) SDL_DestroyTexture(_renProxy);
			_renProxy = SDL_CreateTexture(ren, _format, SDL_TEXTUREACCESS_TARGET, sw, sh);

#if !defined _WIN32 && !defined __CYGWIN__ && !defined __EMSCRIPTEN__
			if(pixScale!=1) SDL_SetRenderTarget(ren, _renProxy);
#endif
		}
	}
	static void _updateRenProxy()
	{
#if defined _WIN32 || defined __CYGWIN__ || defined __EMSCRIPTEN__
		SDL_Rect r = getSizeRect();
		static int pc = WindowProps::getWidth()*WindowProps::getHeight();
		static uint32_t* pixels = new uint32_t[pc];
		if(pc!=WindowProps::getWidth()*WindowProps::getHeight())
		{
			pc = WindowProps::getWidth()*WindowProps::getHeight();
			delete[] pixels;
			pixels = new uint32_t[pc];
		}

		int pitch = r.w*4;

		SDL_RenderReadPixels(ren, &r, _format, pixels, pitch);
		SDL_UpdateTexture(_renProxy, 0, pixels, pitch);
#endif
	}
	static void _applyEffects(double d, double t)
	{
		for(auto e : effects)
		{
			_updateRenProxy();
			e(_renProxy, d, t);
		}
	}
	static void _setSize(const int& w_, const int& h_) { w = w_; h = h_; if(!getLocked()) { sw = w/pixScale; sh = h/pixScale; } _updateSize(); }
	static void _setSize(const helpers::vec2<int>& s) { _setSize(s.x, s.y); }
	static void _setWidth(const int& w_) { w = w_; _updateSize(); }
	static void _setHeight(const int& h_) { h = h_; _updateSize(); }
	static void _init(const int& w_, const int& h_)
	{
		sw = w = w_;
		sh = h = h_;
		_updateSize();

		effects.push_front(&WindowProps::_applyPixScale);
	}
	//////////////////////

	// For external use //
	static helpers::vec2<int> getSize() { return {sw, sh}; }
	static helpers::vec2<int> getRealSize() { return {w, h}; }

	static SDL_Rect getSizeRect() { return {0, 0, sw, sh}; }
	static SDL_Rect getRealSizeRect() { return {0, 0, w, h}; }

	static inline int getWidth() { return sw; }
	static inline int getHeight() { return sh; }
	static inline int getRealWidth() { return w; }
	static inline int getRealHeight() { return h; }

	static inline int getPixScale() { return pixScale; }
	static inline bool getLocked() { return slocked; }
	static inline lockType getLockType() { return lock_type; }

	static inline std::list<std::function<void(SDL_Texture*, double, double)>>* getEffects() { return &effects; }

	static void setScaledSize(const int& sw_, const int& sh_) { if(getLocked()) return; sw = sw_; sh = sh_; pixScale = (float)w/(float)sw; _updateSize(); }
	static void setPixScale(const int& scale) { if(getLocked()) return; pixScale = scale; sw = w/pixScale; sh = h/pixScale; _updateSize(); }
	static void setLocked(const bool& lock) { slocked = lock; }
	static void setLockType(lockType type) { lock_type = type; }

	static inline void pushEffectF(std::function<void(SDL_Texture*, double, double)> e) { effects.push_front(e); }
	static inline void pushEffectB(std::function<void(SDL_Texture*, double, double)> e) { effects.push_back(e); }

	//////////////////////
};
struct Input
{
private:
	inline static std::map<SDL_Keycode, bool> keys;
	inline static std::map<uint8_t, bool> mbs;
	inline static helpers::vec2<int> m_pos;
public:
	static void _init()
	{
		m_pos = {};
		keys = {};
		mbs = {};
	}

	// For internal use //
	static void _setKey(const SDL_Keycode& key, const bool& state)
	{
		keys[key] = state;
		if(state)	onKeyDown(key);
		else		onKeyUp(key);
	}
	static void _setMB(const uint8_t& but, const bool& state)
	{
		mbs[but] = state;
		if(state)	onMBDown(but);
		else		onMBUp(but);
	}
	static void _setMP(const int& x, const int& y) { m_pos.x = x; m_pos.y = y; onMouseMoved(m_pos); }
	//////////////////////

	// For external use //
	static inline bool getKey(const SDL_Keycode& key) { return keys[key]; }
	static inline bool getMB(const uint8_t& but) { return mbs[but]; }
	static inline helpers::vec2<int> getMP() { return (m_pos.to<float>()/WindowProps::getRealSize()*WindowProps::getSize()).to<int>(); } 
	static inline helpers::vec2<int> getRealMP() { return m_pos; } 
	//////////////////////
};

inline void init(const char* name, int w, int h, bool resizable = 1, int winFlags = 0, int renFlags = 0)
{
	if(SDL_Init(SDL_INIT_VIDEO))
	{
	    std::cerr << "Couldn't initialise video: " << SDL_GetError() << '\n';
	    return;
	}
	win = SDL_CreateWindow(name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, (resizable ? SDL_WINDOW_RESIZABLE : 0) | winFlags);
	ren = SDL_CreateRenderer(win, -1, renFlags | SDL_RENDERER_TARGETTEXTURE);

	Input::_init();
	WindowProps::_init(w, h);
}

// For internal use only //
#ifdef __EMSCRIPTEN__
inline SDL_Event ev;
inline Uint32 last = SDL_GetTicks();
inline double delta, elapsed = 0;

inline void _loopCycle()
#else
inline void _loopCycle(SDL_Event& ev, Uint32& last, double& delta, double& elapsed)
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
			Input::_setMP(mx, my);
			break;
		case SDL_KEYDOWN:
			if(Input::getKey(ev.key.keysym.sym)) onKeyHeld(ev.key.keysym.sym);
			else Input::_setKey(ev.key.keysym.sym, 1); break;
		case SDL_KEYUP:		Input::_setKey(ev.key.keysym.sym, 0); break;
		case SDL_MOUSEBUTTONDOWN:	 Input::_setMB(ev.button.button, 1); break;
		case SDL_MOUSEBUTTONUP:		 Input::_setMB(ev.button.button, 0); break;
		}
		if(ev.window.event==SDL_WINDOWEVENT_RESIZED)
			WindowProps::_setSize(ev.window.data1, ev.window.data2); 
	}
#ifdef __EMSCRIPTEN__
	if(ev.window.event==SDL_WINDOWEVENT_CLOSE)
		emscripten_cancel_main_loop();
#endif

	SDL_SetRenderDrawColor(ren, bg_col.r, bg_col.g, bg_col.b, bg_col.a);
	SDL_RenderClear(ren);

	delta   = (double)(SDL_GetTicks() - last) / 1000.;
	elapsed = (double)(SDL_GetTicks())        / 1000.;
	last = SDL_GetTicks();

	onRender(delta, elapsed);
	WindowProps::_applyEffects(delta, elapsed);
	SDL_RenderPresent(shitrndr::ren);
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
		_loopCycle(ev, last, delta, elapsed);

	SDL_Quit();
#else
	emscripten_set_main_loop(_loopCycle, 0, 1);
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
inline void SetRenderColour(SDL_Renderer* rend, Uint8 r = 0, Uint8 g = 0, Uint8 b = 0, Uint8 a = 255) { SDL_SetRenderDrawColor(rend, r, g, b, a); }
inline void SetColour(Uint8 r = 0, Uint8 g = 0, Uint8 b = 0, Uint8 a = 255) { SetRenderColour(ren, r, g, b, a); }
inline void SetRenderColour(SDL_Renderer* rend, const SDL_Colour& col) { SDL_SetRenderDrawColor(rend, col.r, col.g, col.b, col.a); }
inline void SetColour(const SDL_Colour& col) { SetRenderColour(ren, col); }

inline void RenderDrawLine(SDL_Renderer* rend, int x1, int y1, int x2, int y2) { SDL_RenderDrawLine(ren, x1, y1, x2, y2); }
inline void RenderDrawLine(SDL_Renderer* rend, const helpers::vec2<int>& a, const helpers::vec2<int>& b)
{
	SDL_RenderDrawLine(rend, a.x, a.y, b.x, b.y);
}
inline void RenderDrawLine(SDL_Renderer* rend, const helpers::vec2<float> a, const helpers::vec2<float>& b)
{
	SDL_RenderDrawLine(rend, a.x, a.y, b.x, b.y);
}
inline void DrawLine(int x1, int y1, int x2, int y2) { RenderDrawLine(ren, x1, y1, x2, y2); }
inline void DrawLine(const helpers::vec2<int>& a, const helpers::vec2<int>& b) { RenderDrawLine(ren, a, b); }
inline void DrawLine(const helpers::vec2<float>& a, const helpers::vec2<float>& b) { RenderDrawLine(ren, a, b); }

inline void RenderDrawLines(SDL_Renderer* rend, const SDL_Point* points, int count) { SDL_RenderDrawLines(rend, points, count); }
inline void RenderDrawLines(SDL_Renderer* rend, const SDL_FPoint* points, int count) { SDL_RenderDrawLinesF(rend, points, count); }
inline void RenderDrawLines(SDL_Renderer* rend, const std::vector<SDL_Point> points) { RenderDrawLines(rend, points.data(), points.size()); }
inline void DrawLines(const SDL_Point* points, int count) { RenderDrawLines(ren, points, count); }
inline void DrawLines(const SDL_FPoint* points, int count) { RenderDrawLines(ren, points, count); }
inline void DrawLines(const std::vector<SDL_Point> points) { RenderDrawLines(ren, points.data(), points.size()); }
inline void DrawLines(const std::vector<SDL_FPoint> points) { RenderDrawLines(ren, points.data(), points.size()); }

inline void RenderDrawCircle(SDL_Renderer* rend, const int& x, const int& y, const float& r)
{
	float l = 2*M_PI*r;
	for(float a = 0; a < l; a++)
		SDL_RenderDrawPoint(rend, x + std::cos(a/l * 2*M_PI) * r, y + std::sin(a/l * 2*M_PI) * r);
}
inline void RenderDrawCircle(SDL_Renderer* rend, const helpers::vec2<int>& pos, const float& r) { RenderDrawCircle(rend, pos.x, pos.y, r); }
inline void DrawCircle(const int& x, const int& y, const float& r) { RenderDrawCircle(ren, x, y, r); }
inline void DrawCircle(const helpers::vec2<int>& pos, const float& r) { RenderDrawCircle(ren, pos.x, pos.y, r); }

inline void RenderFillCircle(SDL_Renderer* rend, const int& x, const int& y, const float& r)
{
	float l = 2*M_PI*r;
	for(float a = 0; a < l; a++)
	{
		SDL_RenderDrawLine(rend, x, y, x + std::cos(a/l * 2*M_PI) * r, y + std::sin(a/l * 2*M_PI) * r);
		// ugly compensation for single-pixel width SDL lines, results in slight horizontal stretching
		SDL_RenderDrawLine(rend, x+1, y, x+1 + std::cos(a/l * 2*M_PI) * r, y + std::sin(a/l * 2*M_PI) * r);
	}
}
inline void RenderFillCircle(SDL_Renderer* rend, const helpers::vec2<int>& pos, const float& r) { RenderFillCircle(rend, pos.x, pos.y, r); }
inline void FillCircle(const int& x, const int& y, const float& r) { RenderFillCircle(ren, x, y, r); }
inline void FillCircle(const helpers::vec2<int>& pos, const float& r) { RenderFillCircle(ren, pos.x, pos.y, r); }

inline void RenderFillRect(SDL_Renderer* rend, const SDL_Rect& r) { SDL_RenderFillRect(rend, &r); }
inline void RenderFillRect(SDL_Renderer* rend, const helpers::vec2<int>& pos, const helpers::vec2<int>& dim) { RenderFillRect(rend, {pos.x, pos.y, dim.x, dim.y}); }
inline void FillRect(const SDL_Rect& r) { RenderFillRect(ren, r); }
inline void FillRect(const helpers::vec2<int>& pos, const helpers::vec2<int>& dim) { RenderFillRect(ren, {pos.x, pos.y, dim.x, dim.y}); }

inline void RenderDrawRect(SDL_Renderer* rend, const SDL_Rect& r) { SDL_RenderDrawRect(rend, &r); }
inline void RenderDrawRect(SDL_Renderer* rend, const helpers::vec2<int>& pos, const helpers::vec2<int>& dim) { RenderDrawRect(rend, {pos.x, pos.y, dim.x, dim.y}); }
inline void DrawRect(const SDL_Rect& r) { RenderDrawRect(ren, r); }
inline void DrawRect(const helpers::vec2<int>& pos, const helpers::vec2<int>& dim) { RenderDrawRect(ren, {pos.x, pos.y, dim.x, dim.y}); }

inline void RenderCopy(SDL_Renderer* rend, SDL_Texture* tex, const SDL_Rect& src, const SDL_Rect& dst) { SDL_RenderCopy(rend, tex, &src, &dst); }
inline void RenderCopy(SDL_Renderer* rend, SDL_Texture* tex, const SDL_Rect& dst) { SDL_RenderCopy(rend, tex, 0, &dst); }
inline void Copy(SDL_Texture* tex, const SDL_Rect& src, const SDL_Rect& dst) { RenderCopy(ren, tex, src, dst); }
inline void Copy(SDL_Texture* tex, const SDL_Rect& dst) { RenderCopy(ren, tex, dst); }

inline void RenderCopyEx(SDL_Renderer* rend, SDL_Texture* tex, const SDL_Rect& src, const SDL_Rect& dst, const double& angle, const SDL_Point& centre, const SDL_RendererFlip& flip = SDL_FLIP_NONE) { SDL_RenderCopyEx(rend, tex, &src, &dst, angle, &centre, flip); }
inline void RenderCopyEx(SDL_Renderer* rend, SDL_Texture* tex, const SDL_Rect& dst, const double& angle, const SDL_Point& centre, const SDL_RendererFlip& flip = SDL_FLIP_NONE) { SDL_RenderCopyEx(rend, tex, 0, &dst, angle, &centre, flip); }
inline void CopyEx(SDL_Texture* tex, const SDL_Rect& src, const SDL_Rect& dst, const double& angle, const SDL_Point& centre, const SDL_RendererFlip& flip = SDL_FLIP_NONE) { SDL_RenderCopyEx(ren, tex, &src, &dst, angle, &centre, flip); }
inline void CopyEx(SDL_Texture* tex, const SDL_Rect& dst, const double& angle, const SDL_Point& centre, const SDL_RendererFlip& flip = SDL_FLIP_NONE) { SDL_RenderCopyEx(ren, tex, 0, &dst, angle, &centre, flip); }
inline void CopyEx(SDL_Texture* tex, const SDL_Rect& dst, const double& angle, const SDL_RendererFlip& flip = SDL_FLIP_NONE) { SDL_RenderCopyEx(ren, tex, 0, &dst, angle, 0, flip); }
///////////////////////
}
