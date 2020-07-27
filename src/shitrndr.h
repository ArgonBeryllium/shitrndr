#pragma once
#include <iostream>
#include <map>

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

vec2 operator+(const vec2& a, const vec2& b) { return vec2{a.x + b.x, a.y + b.y}; }
vec2 operator-(const vec2& a, const vec2& b) { return vec2{a.x - b.x, a.y - b.y}; }
vec2 operator*(const vec2& a, const vec2& b) { return vec2{a.x * b.x, a.y * b.y}; }
vec2 operator/(const vec2& a, const vec2& b) { return vec2{a.x / b.x, a.y / b.y}; }
vec2 operator*(const vec2& v, float s) { return vec2{v.x*s, v.y*s}; } 
vec2 operator/(const vec2& v, float s) { return vec2{v.x/s, v.y/s}; } 

bool operator==(const vec2& a, const vec2& b) { return a.x==b.x && a.y==b.y; }

float vec2::getLength() { return sqrt(x*x + y*y); }
float vec2::getLengthSquare() { return x*x + y*y; }
vec2 vec2::normalised() { float l = getLength(); return l==0 ? vec2{} : *this / l; }
float vec2::dot(const vec2& a, const vec2& b) { return a.x * b.x + a.y * b.y; }

}

namespace shitrndr
{
SDL_Window* 	win;
SDL_Renderer* 	ren;

SDL_Colour bg_col = {0xCC, 0xCC, 0xCC};

struct Input
{
private:
	inline static std::map<SDL_Keycode, bool> keys;
	inline static helpers::vec2 m_pos;
public:
	static void init()
	{
		m_pos = {};
		keys = {};
	}

	static void setKey(const SDL_Keycode& key, const bool& state) { keys[key] = state; }
	static bool getKey(const SDL_Keycode& key) { return keys[key]; }

	static void setMP(const int& x, const int& y) { m_pos.x = x; m_pos.y = y; }
	static helpers::vec2 getMP() { return m_pos; } 
};

void init(const char* name, int w, int h, bool resizable)
{
	SDL_Init(SDL_INIT_VIDEO);
	win = SDL_CreateWindow(name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, resizable ? SDL_WINDOW_RESIZABLE : 0);
	ren = SDL_CreateRenderer(win, -1, 0);

	Input::init();
}

// default onRender void
void defOnRender(double d, double t)
{
	std::cout << "delta: " << d << '\n';
	std::cout << "elapsed: " << t << '\n';
}
void (*onRender)(double delta, double time) = &defOnRender;

// main loop
void loop()
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

void RenderDrawCircle(SDL_Renderer* rend, const int& x, const int& y, const float& r)
{
	float l = 2*PI*r;
	for(float a = 0; a < l; a++)
		SDL_RenderDrawPoint(rend, x + std::cos(a/l * 2*PI) * r, y + std::sin(a/l * 2*PI) * r);
}
void DrawCircle(const int& x, const int& y, const float& r) { RenderDrawCircle(ren, x, y, r); }

void RenderFillCircle(SDL_Renderer* rend, const int& x, const int& y, const float& r)
{
	int rs = r*r;
	for(int py = -r; py < r; py++)
		for(int px = -r; px < r; px++)
			if(py*py + px*px < rs)
				SDL_RenderDrawPoint(rend, x + px, y + py);
}
void FillCircle(const int& x, const int& y, const float& r) { RenderFillCircle(ren, x, y, r); }

}
