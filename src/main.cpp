#include "SDL.h"
#include <iostream>
#include <list>
#include <string>

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
};
struct Point {
	int x,y;
};

template <typename T>
struct Obj {
	T feature;
	std::string type;
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
	
	void draw_line(int x, int y, int x2, int y2, Color color)
	{
		SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
		SDL_RenderDrawLine(renderer, x, y, x2, y2);
	}

	void draw_point(int x, int y, Color color)
	{
		SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
		SDL_RenderDrawPoint(renderer, x, y);
	}

	void clear()
	{
		SDL_SetRenderDrawColor(renderer, bg_color.r, bg_color.g, bg_color.b, bg_color.a);
	        SDL_RenderClear(renderer);
	}

	void update(std::list<Obj<Point>> &pli, std::list<Obj<Line>> &lli)
	{
		for (auto const &obj : pli) {
			SDL_SetRenderDrawColor(renderer, obj.color.r, obj.color.g, obj.color.b, obj.color.a);
			SDL_RenderDrawPoint(renderer, obj.feature.x, obj.feature.y);		
		}
		for (auto const &obj : lli) {
			SDL_SetRenderDrawColor(renderer, obj.color.r, obj.color.g, obj.color.b, obj.color.a);
			SDL_RenderDrawLine(renderer, obj.feature.x1, obj.feature.y1, obj.feature.x2, obj.feature.y2);
		}
	}

	void render()
	{
		SDL_RenderPresent(renderer);
	}

private:
	int height, width;
	Color bg_color = OFF_WHITE;
	SDL_Window *window;
	SDL_Renderer *renderer;
};

int main(int argc, char *argv[]) 
{
	IchWin ich(640, 480);
	
	/* 
		Having two lists here is not super aesthetic, and kind of 
		defeats the purpose of what I was going for with a 
		generic drawn object type. It works, but it could be 
		neater and more extensible. Work on this later.
	*/
	std::list<Obj<Point>> drawn_points;
	std::list<Obj<Line>> drawn_lines;

	bool mouse_pressed = false;
	// False: line, True: freehand.	
	bool mode = true;
	Color pen_color = BLACK;
	int x1,x2,y1,y2;
	
	SDL_Event event;

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
				if (mode) { // Freehand
					Point feature = {x1,y1};
					Obj<Point> obj = {feature, "point", pen_color};
					drawn_points.push_back(obj);
				}
				else {				
					Line feature = {x1,y1,x2,y2};
					Obj<Line> obj = {feature, "line", pen_color};
					drawn_lines.push_back(obj);
				}
				mouse_pressed = false;
			}
			// Draw event (drag with mouse down).
			else if (event.type == SDL_MOUSEMOTION && mouse_pressed) {
				x2 = event.motion.x;
				y2 = event.motion.y;
				if (mode) { // Freehand: draw points on drag.
					Point feature = {x2,y2};
					Obj<Point> obj = {feature, "point", pen_color};
					drawn_points.push_back(obj);
				}
			}
			// Switch colour.
			else if (event.type == SDL_KEYDOWN) {
				switch (event.key.keysym.sym) {
					case SDLK_1: // Color swaps.
						pen_color = RED;
						break;
					case SDLK_2:
						pen_color = GREEN;
						break;
					case SDLK_3:
						pen_color = BLUE;
						break;
					case SDLK_4:
						pen_color = BLACK;
						break;
					case SDLK_SPACE: // Switch mode (freehand/line)
						mode = !mode;
						break;
					case SDLK_ESCAPE:
						drawn_points.clear();
						drawn_lines.clear();
						break;
				}				
			}
		}

		ich.clear();
		if (mouse_pressed) {
			if (mode) // Freehand
				ich.draw_point(x1, y1, pen_color);
			else // Line 
				ich.draw_line(x1, y1, x2, y2, pen_color);
		}

		ich.update(drawn_points, drawn_lines);
		ich.render();
	}		
	return 0;
}
