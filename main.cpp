#include <iostream>
#include <map>
#include <SDL2/SDL.h>

using namespace std;

const int WIDTH = 1280;
const int HEIGHT = 720;
const int FPS = 60;
const int REFRESHPERIOD = 1000 / FPS;
const int TICKRATE = 20;
const int TICKPERIOD = 1000 / TICKRATE;
const char TITLE[] = "Path of doxa";

static SDL_Window *window;
static SDL_Renderer *renderer;

bool running = true;

std::map<int, bool> keys;

SDL_Event event;

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

class State {
public:
	virtual void init() = 0;
	virtual void tick(int dt) = 0;
	virtual void render(SDL_Renderer *renderer) = 0;
	virtual void keyPressed(int key) = 0;
	virtual void keyReleased(int key) = 0;
};

class TestState : public State {
public:
	void init() {}
	void tick(int dt) {}
	void render(SDL_Renderer *renderer) {}
	void keyPressed(int key) {}
	void keyReleased(int key) {}
};

static bool isKeyDown(char key) {
	return keys[int(key)];
}


int main(int argc, char const *argv[])
{
	int lastTickTime = 0, lastDispTime = 0, currentTime;

	TestState state;
	State *currentState = &state;

	if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		cout << SDL_GetError() << endl;
		return 1;
	}

	window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);

	while(running) {
		while(SDL_PollEvent(&event)) {
			switch(event.type) {
			case SDL_QUIT:
				running = false;
				break;
			case SDL_KEYDOWN:
			case SDL_KEYUP:
				SDL_KeyboardEvent *key = &event.key;
				if(key->type == SDL_KEYUP) {
					keys[key->keysym.sym] = false;
					currentState->keyReleased(key->keysym.sym);
				} else {
					keys[key->keysym.sym] = true;
					currentState->keyPressed(key->keysym.sym);
				}
				break;
			}
		}

		currentTime = SDL_GetTicks();

		if(currentTime > lastTickTime + TICKPERIOD) {
			lastTickTime = currentTime;
			currentState->tick(currentTime - lastTickTime);
		}

		if(currentTime > lastDispTime + REFRESHPERIOD) {
			lastDispTime = currentTime;

			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			SDL_RenderClear(renderer);

			currentState->render(renderer);

			SDL_RenderPresent(renderer);
		}
	}

	SDL_Quit();

	return 0;
}