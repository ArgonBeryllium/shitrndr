#pragma once
#include <iostream>
#include <map>

#if defined _WIN32 || defined __CYGWIN__
#include <cmath>
#define uint unsigned int
#endif

#include <SDL2/SDL.h>

namespace shitrndr::helpers
{
struct vec2
{
    vec2() : x(0), y(0) {}
    vec2(const float x_, const float y_) : x(x_), y(y_) {}
    float x, y;
    float getLength();
    float getLengthSquare();
    vec2 normalised();
    void operator+=(const vec2& v) { x += v.x; y += v.y; }
    void operator-=(const vec2& v) { x -= v.x; y -= v.y; }
    void operator*=(const vec2& v) { x *= v.x; y *= v.y; }
    void operator/=(const vec2& v) { x /= v.x; y /= v.y; }
    void operator*=(const float s) { x *= s; y *= s; }
    void operator/=(const float s) { x /= s; y /= s; }
    static float dot(const vec2& a, const vec2& b);
};

inline vec2 operator+(const vec2& a, const vec2& b) { return vec2{a.x + b.x, a.y + b.y}; }
inline vec2 operator-(const vec2& a, const vec2& b) { return vec2{a.x - b.x, a.y - b.y}; }
inline vec2 operator*(const vec2& a, const vec2& b) { return vec2{a.x * b.x, a.y * b.y}; }
inline vec2 operator/(const vec2& a, const vec2& b) { return vec2{a.x / b.x, a.y / b.y}; }
inline vec2 operator*(const vec2& v, float s) { return vec2{v.x*s, v.y*s}; } 
inline vec2 operator/(const vec2& v, float s) { return vec2{v.x/s, v.y/s}; } 

inline bool operator==(const vec2& a, const vec2& b) { return a.x==b.x && a.y==b.y; }

inline float vec2::getLength() { return sqrt(x*x + y*y); }
inline float vec2::getLengthSquare() { return x*x + y*y; }
inline vec2 vec2::normalised() { float l = getLength(); return l==0 ? vec2{} : *this / l; }
inline float vec2::dot(const vec2& a, const vec2& b) { return a.x * b.x + a.y * b.y; }

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
inline void (*onKeyUp)(const SDL_Keycode& key) = &defOnKey;


inline void defOnMB(const uint& but)
{
	std::cout << "mouse button action: " << but << '\n';
}
inline void (*onMBDown)(const uint& but) = &defOnMB;
inline void (*onMBUp)(const uint& but) = &defOnMB;

inline void defOnMM(const helpers::vec2& mp)
{
	std::cout << "mouse moved: { " << mp.x << ", " << mp.y << " }\n"; 
}
inline void (*onMouseMoved)(const helpers::vec2& mp) = &defOnMM;

struct Input
{
private:
	inline static std::map<SDL_Keycode, bool> keys;
	inline static std::map<uint, bool> mbs;
	inline static helpers::vec2 m_pos;
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

	static void setMB(const uint& but, const bool& state)
	{
		mbs[but] = state;
		if(state)	onMBDown(but);
		else		onMBUp(but);
	}
	static bool getMB(const int& but) { return mbs[but]; }

	static void setMP(const int& x, const int& y) { m_pos.x = x; m_pos.y = y; onMouseMoved(m_pos); }
	static helpers::vec2 getMP() { return m_pos; } 
};

inline void init(const char* name, int w, int h, bool resizable)
{
	SDL_Init(SDL_INIT_VIDEO);
	win = SDL_CreateWindow(name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, resizable ? SDL_WINDOW_RESIZABLE : 0);
	ren = SDL_CreateRenderer(win, -1, 0);

	Input::init();
}

// default onRender void
inline void defOnRender(double d, double t)
{
	std::cout << "delta: " << d << '\n';
	std::cout << "elapsed: " << t << '\n';
}
inline void (*onRender)(double delta, double time) = &defOnRender;

// main loop
inline void loop()
{
	SDL_Event ev;

	Uint32 last;
	double delta, elapsed = 0;

	while(ev.window.event != SDL_WINDOWEVENT_CLOSE)
	{
		// handle events
		while(SDL_PollEvent(&ev) && ev.window.event != SDL_WINDOWEVENT_CLOSE)
			switch (ev.type)
			{
			case SDL_MOUSEMOTION:
				int mx, my;
				SDL_GetMouseState(&mx, &my);
				Input::setMP(mx, my);
				break;
			case SDL_KEYDOWN:	Input::setKey(ev.key.keysym.sym, 1); break;
			case SDL_KEYUP:		Input::setKey(ev.key.keysym.sym, 0); break;
			case SDL_MOUSEBUTTONDOWN:	 Input::setMB(ev.button.button, 1); break;
			case SDL_MOUSEBUTTONUP:		 Input::setMB(ev.button.button, 0); break;
			}

		// clear render buffer
		SDL_SetRenderDrawColor(shitrndr::ren, bg_col.r, bg_col.g, bg_col.b, bg_col.a);
		SDL_RenderClear(ren);
		
		// calculate timing
		delta 	= 	(double)(SDL_GetTicks() - last)	/ 1000;
		elapsed	= 	(double)(SDL_GetTicks()) 		/ 1000;
		last = SDL_GetTicks();

		onRender(delta, elapsed);

		SDL_RenderPresent(shitrndr::ren); // copy render buffer to window
		SDL_Delay(1);
	}
}

// drawing functions

constexpr float PI = 3.14159265358979;

inline void RenderDrawCircle(SDL_Renderer* rend, const int& x, const int& y, const float& r)
{
	float l = 2*PI*r;
	for(float a = 0; a < l; a++)
		SDL_RenderDrawPoint(rend, x + std::cos(a/l * 2*PI) * r, y + std::sin(a/l * 2*PI) * r);
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

}
