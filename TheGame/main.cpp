#include "Game.h"
#include "ToMenuTransition.h"

int main()
{
	Game game;

	game.pushState(new ToMenuTransition(&game));
	game.gameLoop();

	return 0;
}