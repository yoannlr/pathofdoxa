#include <iostream>
#include "SDL2/SDL.h"

using namespace std;

const int WIDTH = 1280;
const int HEIGHT = 720;

SDL_Texture* loadImage(const char* path, SDL_Renderer *renderer) {
	SDL_Surface *surface = SDL_LoadBMP(path);
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
	return texture;
}

void renderImage(SDL_Texture *texture, SDL_Renderer *renderer, int x, int y, int w, int h) {
	SDL_Rect rect;
	rect.x = x;
	rect.y = y;
	rect.w = w;
	rect.h = h;
	SDL_RenderCopy(renderer, texture, NULL, &rect);
}

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Event event;
bool running = true;

SDL_Texture *img;

int main(int argc, char const *argv[])
{
	if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		cout << SDL_GetError() << endl;
		return 1;
	}

	window = SDL_CreateWindow("Path of doxa", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);

	img = loadImage("img.bmp", renderer);

	while(running) {
		SDL_WaitEvent(&event);
		if(SDL_PollEvent(&event)) {
			switch(event.type) {
			case SDL_QUIT: running = false; break;
			}
		}

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		renderImage(img, renderer, 10, 20, 100, 100);

		SDL_RenderPresent(renderer);
	}

	SDL_Quit();

	return 0;
}