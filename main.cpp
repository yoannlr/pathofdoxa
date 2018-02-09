#include <iostream>
#include "SDL2/SDL.h"

using namespace std;

class Window {
public:
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Event event;
	bool running;
	int width, height;

	Window(int w, int h) {
		width = w;
		height = h;
		window = SDL_CreateWindow("Path of doxa", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
		running = true;
	}

	~Window() {
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
	}

	void loop() {
		while(running) {
			SDL_WaitEvent(&event);
			if(SDL_PollEvent(&event)) {
				switch(event.type) {
					case SDL_QUIT: running = false; break;
				}
			}
		}

		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderClear(renderer);

		//Affichage ici

		SDL_RenderPresent(renderer);
	}
};

int main(int argc, char const *argv[])
{
	if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		cout << SDL_GetError() << endl;
		return 1;
	}

	Window gameWindow(1280, 720);
	gameWindow.loop();

	SDL_Quit();

	return 0;
}