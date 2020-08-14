#include "../shitrndr.h"

// general demo script


#if defined _WIN32 || defined __CYGWIN__
int WinMain()
#else
int main()
#endif
{
	using namespace shitrndr;
	init("test", 540, 360, 1);

	SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND); // allows transparency

	onRender = [](double d, double t)
	{
		bg_col.g = 255*(sin(t / 5)/2 + .5);

		// zoomin' square
		int w = 0;
		SDL_GetWindowSize(win, &w, nullptr);
		const SDL_Rect rect = {(int)(t * 1000) % (w + 50) - 50, 0, 50, 50};
		SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
		SDL_RenderFillRect(ren, &rect);

		// circle grid
		constexpr int RAD = 16;
		constexpr int CNT = 256/RAD;
		for(int y = 0; y < CNT; y++)
			for(int x = 0; x < CNT; x++)
			{
				SDL_SetRenderDrawColor(ren, x * 255/CNT, y * 255/CNT, 0, 255);
				FillCircle(20 + RAD*1.5*x, 20 + RAD*1.5*y, RAD * (1 + sin(t + (x + y)/3) / 3));
			}

		// input demonstration
		if (Input::getKey(SDLK_SPACE))
		{
			SDL_SetRenderDrawColor(ren, 0, 255, 255, 70);
			float r = 100 * (std::sin(t) + 1);
			int x = (int)Input::getMP().x, y = (int)Input::getMP().y;
			FillCircle(x, y, r);
		}

		std::cout << "delta: " << d << '\n';
	};

	loop();

	return 0;
}
