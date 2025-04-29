#include <stdio.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_init.h>
#include <stdbool.h>
#define SDL_MAIN_HANDLED
//run sudo apt install libsdl3-dev

int running = false;
SDL_Window* window = NULL;
SDL_Renderer* renderer;

int initialize_window() {
	/*SDL_INIT_GAMEPAD;
	SDL_INIT_JOYSTICK;
	SDL_INIT_EVENTS;
	SDL_INIT_VIDEO;
	SDL_INIT_AUDIO;
	*/

	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS | SDL_INIT_JOYSTICK | SDL_INIT_GAMEPAD | SDL_INIT_HAPTIC);

	window = SDL_CreateWindow(
		"Game Screen",
		1280,
		720,
		SDL_WINDOW_RESIZABLE
	);

	if (!window) {
		fprintf(stderr, "SDL window fail");
		return 0;
	}
	renderer = SDL_CreateRenderer(window, NULL);
	if (!renderer) {
		fprintf(stderr, "renderer issue");
		return 0;
	}

	return 1;
}

void process_input() {
	SDL_Event event;
	SDL_PollEvent(&event);

	switch (event.type) {
	case SDL_EVENT_QUIT:
		running = false;
		break;
	case SDL_EVENT_KEY_DOWN:
		if (event.key.key == SDLK_ESCAPE)
			running = false;
		break;
	}
}

void setup() {
	//TODO:
}

void update() {
	//TODO:
}

void render() {
	//TODO: 
}

destroy_window() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

int main(int argc, char* argv[]) {
	running = initialize_window();

	setup();

	while (running) {
		process_input();
		update();
		render();
	}

	destroy_window();

	return 0;
}