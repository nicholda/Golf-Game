#include <windows.h>
#include "Game.h"

Game* game = nullptr;

int main(int argc, char* argv[]) {
	HWND hWin = GetForegroundWindow();
	ShowWindow(hWin, SW_SHOW); // variable can be changed to hide the output

	const int FPS = 60;
	const int frameDelay = 1000 / FPS;

	Uint32 frameStart;
	int frameTime;

	game = new Game(); // allocate the game memory from the heap
	game->init("Golf Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 640, false);

	while (game->running()) {
		frameStart = SDL_GetTicks();

		// game loop
		game->handleEvents();
		game->update();
		game->render();

		frameTime = SDL_GetTicks() - frameStart;

		if (frameDelay > frameTime) {
			SDL_Delay(frameDelay - frameTime); // limit the game to the 'FPS' variable
		}
	}

	game->clean();
	return 0;
}