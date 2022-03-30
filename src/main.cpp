#include "SDL.h"
#include <iostream>

class IchWin {
public:
	IchWin(int width, int height) 
	{
		SDL_Init(SDL_INIT_VIDEO);
		SDL_CreateWindowAndRenderer(640, 480, 0, &window, &renderer);
		SDL_SetRenderDrawColor(renderer, 0,0,0,0);
		SDL_RenderClear(renderer);
		SDL_RenderPresent(renderer);
	}

	~IchWin()
	{
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
	}
private:
	int height, width;
	SDL_Window *window = NULL;
	SDL_Renderer *renderer = NULL;
};

int main(int argc, char *argv[]) 
{
	IchWin ich(640, 480);
	
	SDL_Event event;
	
	while (event.type != SDL_QUIT) {
		SDL_Delay(10);
		SDL_PollEvent(&event);
	}
	std::cout << "All done!" << std::endl;

	return 0;
}
