#include <iostream>
#include <map>
#include "SDL2/SDL.h"

using namespace std;

const int WIDTH = 1280;
const int HEIGHT = 720;

std::map<int, bool> keys;

static SDL_Window *window;
static SDL_Renderer *renderer;
SDL_Event event;
bool running = true;

class Texture {
public:
	SDL_Texture *texture;
	SDL_Rect crop;
	SDL_Rect dest;
	Texture(const char* path, SDL_Renderer *renderer) {
		SDL_Surface *surface = SDL_LoadBMP(path);
		texture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);
	}

	~Texture() {

	}

	void render(SDL_Renderer *renderer, int x, int y, int w, int h, int cx, int cy, int cw, int ch) {
		dest.x = x;
		dest.y = y;
		dest.w = w;
		dest.h = h;

		crop.x = cx;
		crop.y = cy;
		crop.w = cw;
		crop.h = ch;

		SDL_RenderCopyEx(renderer, texture, &crop, &dest, 0, NULL, SDL_FLIP_NONE);
	}
};

static bool isKeyDown(char key) {
	return keys[int(key)];
}

void handleKeyEvent(SDL_KeyboardEvent *key) {
	if(key->type == SDL_KEYUP) {
		keys[key->keysym.sym] = false;
	} else {
		keys[key->keysym.sym] = true;
	}
}

int x = 0;

int main(int argc, char const *argv[])
{
	if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		cout << SDL_GetError() << endl;
		return 1;
	}

	window = SDL_CreateWindow("Path of doxa", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);

	
	Texture img("img.bmp", renderer);

	while(running) {
		while(SDL_PollEvent(&event)) {
			switch(event.type) {
			case SDL_QUIT:
				running = false;
				break;
			case SDL_KEYDOWN:
			case SDL_KEYUP:
				handleKeyEvent(&event.key);
				break;
		}
	}

		if(isKeyDown('q')) x--;
		if(isKeyDown('d')) x++;

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		img.render(renderer, x, 10, 100, 100, 0, 0, 20, 20);

		SDL_RenderPresent(renderer);
	}

	SDL_Quit();

	return 0;
}