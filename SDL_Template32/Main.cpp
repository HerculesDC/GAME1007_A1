#include "Engine.h"
#define WIDTH 1024
#define HEIGHT 768

int main(int argc, char* args[])
{
	Game::Instance()->Run("Assignment 1 Start Project", SDL_WINDOWPOS_CENTERED, 
		SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
	return 0;
}