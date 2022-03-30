#include "SDL.h"
#include <iostream>

#define WHITE 0xFFFFFFFF
#define BLACK 0xFF000000
#define RED 0xFFFF0000
#define GREEN 0xFF00FF00
#define BLUE 0xFF0000FF


class IchWin {
public:
	IchWin(int width, int height) 
	{
		SDL_Init(SDL_INIT_VIDEO);
//		SDL_CreateWindowAndRenderer(640, 480, 0, &window, &renderer);
		window = SDL_CreateWindow("ichora", 0, 0, width, height, SDL_WINDOW_SHOWN);
//		SDL_SetRenderDrawColor(renderer, 210,210,210,255);
		
		surface = SDL_CreateRGBSurfaceWithFormat(0, width, height, 32, SDL_PIXELFORMAT_RGBA32);
		SDL_FillRect(surface, NULL, WHITE);
//		SDL_RenderClear(renderer);
//		SDL_RenderPresent(renderer);

	}

	~IchWin()
	{
//		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
	}
	
	void draw(int x, int y)
	{
		std::cout << x << "," << y << std::endl;
	}

	void render() 
	{

	}
	
private:
	int height, width;
	uint32_t colour = BLACK;
	SDL_Window *window = NULL;
	SDL_Surface *surface = NULL;
//	SDL_Renderer *renderer = NULL;
};

int main(int argc, char *argv[]) 
{
	IchWin ich(640, 480);
	
	SDL_Event event;
	bool mouse_pressed = false;
	
	while (1) {
		while (SDL_PollEvent(&event)) {
			// Exit application.
			if (event.type == SDL_QUIT) {
				std::cout << "Shutting down." << std::endl;
				exit(0);
			}
			// Click event.
			else if (event.type == SDL_MOUSEBUTTONDOWN) {
				ich.draw(event.button.x, event.button.y);
				mouse_pressed = true;
			}
			// Unclick event.
			else if (event.type == SDL_MOUSEBUTTONUP) {
				mouse_pressed = false;
			}
			// Draw event (drag with mouse down).
			else if (event.type == SDL_MOUSEMOTION && mouse_pressed) {
				ich.draw(event.button.x, event.button.y);
			}
		}
		if (mouse_pressed) {
			ich.render();
		}
	}	

	return 0;
}
