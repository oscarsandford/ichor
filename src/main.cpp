#include "SDL.h"
#include <iostream>
#include <list>

#define WHITE {255, 255, 255, 255}
#define OFF_WHITE {242, 242, 242, 255}
#define BLACK {0, 0, 0, 255}
#define RED {255, 0, 0, 255}
#define GREEN {0, 255, 0, 255}
#define BLUE {0, 0, 255, 255}

struct Color {
	uint8_t r,g,b,a;
};
struct Line {
	int x1,y1,x2,y2;
	Color color;
};


class IchWin {
public:
	IchWin(int width, int height) 
	{
		SDL_Init(SDL_INIT_VIDEO);
		window = SDL_CreateWindow(
				"ichora",
				SDL_WINDOWPOS_UNDEFINED, 
				SDL_WINDOWPOS_UNDEFINED,
				width,
				height,
				0
		);
		renderer = SDL_CreateRenderer(window, -1, 0);
	}

	~IchWin()
	{
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
	}
	
	void draw(int x, int y, int x2, int y2)
	{
		SDL_SetRenderDrawColor(renderer, pen_color.r, pen_color.g, pen_color.b, pen_color.a);
		SDL_RenderDrawLine(renderer, x, y, x2, y2);
	}

	void pen_toggle()
	{
		pen_color = BLUE;
		std::cout << "Changed colour" << std::endl;
	}

	void clear()
	{
		SDL_SetRenderDrawColor(renderer, bg_color.r, bg_color.g, bg_color.b, bg_color.a);
	        SDL_RenderClear(renderer);
	}

	void update(std::list<Line> &li)
	{
		for (auto const &l : li) {
			SDL_SetRenderDrawColor(renderer, l.color.r, l.color.g, l.color.b, l.color.a);
			SDL_RenderDrawLine(renderer, l.x1, l.y1, l.x2, l.y2);
		}
	}

	void render()
	{
		SDL_RenderPresent(renderer);
	}
	Color pen_color = RED;

private:
	int height, width;
	Color bg_color = OFF_WHITE;
	SDL_Window *window;
	SDL_Renderer *renderer;
};

int main(int argc, char *argv[]) 
{
	IchWin ich(640, 480);
	
	std::list<Line> drawn_lines;

	SDL_Event event;
	bool mouse_pressed = false;
	int x1,x2,y1,y2;
	
	while (1) {
		SDL_Delay(10);

		while (SDL_PollEvent(&event)) {
			// Exit application.
			if (event.type == SDL_QUIT) {
				std::cout << "Shutting down." << std::endl;
				exit(0);
			}
			// Click event.
			else if (event.type == SDL_MOUSEBUTTONDOWN) {
				x1 = event.motion.x;
				y1 = event.motion.y;
				x2 = event.motion.x;
				y2 = event.motion.y;
				mouse_pressed = true;
			}
			// Unclick event.
			else if (event.type == SDL_MOUSEBUTTONUP) {
				Line l = {x1,y1,x2,y2,ich.pen_color};
				drawn_lines.push_back(l);
				mouse_pressed = false;
			}
			// Draw event (drag with mouse down).
			else if (event.type == SDL_MOUSEMOTION && mouse_pressed) {
				x2 = event.motion.x;
				y2 = event.motion.y;
			}
			// Switch colour.
			else if (event.type == SDL_KEYDOWN) {
				ich.pen_toggle();
			}
		}

		ich.clear();
		if (mouse_pressed) {
			 ich.draw(x1, y1, x2, y2);
		}
		ich.update(drawn_lines);
		ich.render();
	}		
	return 0;
}
