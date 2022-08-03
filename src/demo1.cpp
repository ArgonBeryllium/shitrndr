#include "shitrndr.h"

#if defined _WIN32 || defined __CYGWIN__
int WinMain()
#else
int main()
#endif
{
	using namespace shitrndr;

	init("shitrndr demo 1", 640, 480, 1, 0, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	silentDefs();
	bg_col = {15, 15, 45, 255};
	onRender = [](double d, double t)
	{
		int W = WindowProps::getWidth(), H = WindowProps::getHeight();
		SetColour(abs(sin(t))*255, abs(sin(t*1.2))*255, abs(sin(t*.8))*255);
		float mag = 3;
		for(float i = 0; i != 100; i++)
		{
			DrawLine(((i/100-.5)*2*mag)*W, H, W/2, H*2/3);
		}

		static int s = 2;
		if(Input::getKey(SDLK_SPACE)) s = std::rand();
		std::srand(s);
		for(int i = 0; i != 10; i++)
		{
			SDL_SetRenderDrawColor(ren, std::rand()%255, std::rand()%255, std::rand()%255, 255);
			FillCircle(std::rand()%320, std::rand()%240+std::sin(t*.9+i)*i, std::rand()%100);
		}
		
		SDL_SetRenderDrawColor(ren, 255, 125, 0, 255);
		FillRect({0, 200, 320, 40});
		FillCircle(160, 120+std::sin(t)*50, 24);
	};
	WindowProps::setPixScale(2);
	WindowProps::setLocked(1);
	WindowProps::setLockType(WindowProps::BARS);
	loop();
}
