#include "../shitrndr.h"

// sky demo

using namespace shitrndr;

constexpr uint SCREEN_H = 360, SCREEN_W = 720;
constexpr float CLOUD_R = 20;
constexpr size_t CLOUD_C = 50;

struct cloud
{
	size_t			circlec;	
	helpers::vec2	circles[5];
	float			phases[5];
	float			speed;

	helpers::vec2 pos;

	cloud()
	{
		pos = { -CLOUD_R, (float)(std::rand() % SCREEN_H) };
		circlec = std::rand() % 3 + 2;
		speed = 1 + std::rand() % 4;
		for(uint i = 0; i < circlec; i++)
		{
			circles[i] = { (float)(std::rand() % circlec * CLOUD_R), (float)(std::rand() % (int)CLOUD_R) };
			phases[i] += (float)std::rand();
		}
	}

	void update(double delta)
	{
		pos.x += delta * CLOUD_R * speed * (1 - pos.y/SCREEN_H);
		if(pos.x > SCREEN_W + CLOUD_R * circlec)
			pos = { -CLOUD_R * circlec, (float)(std::rand() % SCREEN_H) };
	}
	void render(double t)
	{
		for(uint i = 0; i < circlec; i++)
			FillCircle((int)(pos.x + circles[i].x), (int)(pos.y + circles[i].y), CLOUD_R * (1.5 + std::sin(t / 10 + phases[i])));
	}
};
cloud clouds[CLOUD_C];

int main()
{
	for(uint i = 0; i < CLOUD_C; i++)
	{
		clouds[i] = {};
		clouds[i].pos.x = std::rand() % SCREEN_W;
	}

	init("sky demo", SCREEN_W, SCREEN_H, 0);
	SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND); // allows transparency

	bg_col = { 150, 200, 255, 255 };
	onRender = [](double d, double t)
	{
			SDL_SetRenderDrawColor(ren, 255, 255, 255, 200);
			for(uint i = 0; i < CLOUD_C; i++)
			{
				clouds[i].update(d);
				clouds[i].render(t);
			}

	};

	loop();	

	return 0;
}
