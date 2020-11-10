#include "../shitrndr.h"

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
		constexpr int COUNT = 512/SIZE;
		for(int y = 0; y != COUNT; y++)
			for(int x = 0; x != COUNT; x++)
			{
				SDL_SetRenderDrawColor(ren, x * 255/COUNT, y * 255/COUNT, 0, 255);
				int s = SIZE * (.6 + std::sin(t*4 + (x + y)/2) / 3);
				
				int d = (mx-20-SIZE*x)*(mx-20-SIZE*x)+(my-20-SIZE*y)*(my-20-SIZE*y);
				d /= SIZE;
				if(d < 4*SIZE)
				{
					s /= 5;
					s += d/5;
				}
				FillRect({(int)(20 + SIZE*x), (int)(20 + SIZE*y), s, s});
			}

		SDL_Delay(16); // to prevent CPU overloading
	};

	loop();

	return 0;
}
