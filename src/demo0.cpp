#include "shitrndr.h"

static constexpr uint16_t CELL_SIZE = 32;
static uint16_t cw = 16, ch = 16;

#if defined _WIN32 || defined __CYGWIN__
int WinMain()
#else
int main()
#endif
{
	using namespace shitrndr;
	init("shitrndr demo", 552, 552, 1);
	silentDefs();

	onRender = [](double d, double t)
	{
		bg_col.g = 255*(sin(t / 5)/2 + .5);
		bg_col.r = Input::getMB(1)?15:215;
		int mx = (int)Input::getMP().x, my = (int)Input::getMP().y;

		constexpr int SIZE = 32; 
		for(int y = 0; y != ch; y++)
			for(int x = 0; x != cw; x++)
			{
				SDL_SetRenderDrawColor(ren, x * 255/cw, y * 255/ch, 0, 255);
				int s = SIZE * (.6 + std::sin(t*4 + (x + y)/2) / 3);
				
				int dst = (mx-20-CELL_SIZE*x)*(mx-20-CELL_SIZE*x)+(my-20-CELL_SIZE*y)*(my-20-CELL_SIZE*y);
				dst /= CELL_SIZE;
				if(dst < 4*CELL_SIZE)
				{
					s /= 5;
					s += d/5;
				}
				FillRect({(int)(20 + CELL_SIZE*x), (int)(20 + CELL_SIZE*y), s, s});
			}

		SDL_Delay(16);
	};
	onEvent = [](SDL_Event* event)
	{
		if(event->window.event==SDL_WINDOWEVENT_RESIZED)
		{
			cw = (event->window.data1-35)/CELL_SIZE;
			ch = (event->window.data2-35)/CELL_SIZE;
		}
	};
	onKeyDown = [](const SDL_Keycode& key) { std::cout << SDL_GetKeyName(key) << " pressed.\n"; };

	loop();

	return 0;
}
