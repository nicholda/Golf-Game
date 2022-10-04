#include "Game.h"

Game* game = nullptr;

int main(int argc, char* argv[]) {
	const int FPS_CAP = 60;
	const int FRAME_DELAY = 1000 / FPS_CAP;

	Uint32 frameStart;
	Uint32 frameEnd;

	game = new Game();
	const int windowFlags = 0;
	const int rendererFlags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
	// resolution must be divisible by 16
	game->init("Golf Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 640, windowFlags, rendererFlags);

	while (game->isRunning()) {
		frameStart = SDL_GetTicks();

		game->handleEvents();
		game->update();
		game->render();

		frameEnd = SDL_GetTicks() - frameStart;

		if (FRAME_DELAY > frameEnd) {
			SDL_Delay(FRAME_DELAY - frameEnd);
		}
	}

	game->clean();

	return 0;
}