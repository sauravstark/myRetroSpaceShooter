#include <thread>
#include "Game.h"

int main(void)
{
	gameinstance newgame;

	std::thread soundthread(play_sounds);
	std::thread gamethread(run_graphics, std::ref(newgame));
	
	run_game(newgame);

	gamethread.join();
	soundthread.join();
	return 0;
}
