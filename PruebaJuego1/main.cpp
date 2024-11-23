#include <iostream>
#include "Game.h"

int main()
{
    //obj juego
    Game game;
    //init srand
    std::srand(static_cast<unsigned>(time(NULL)));

    //loop del juego
    while (game.getRunning())
    {
        //Update
        game.update();

        //Render
        game.render();
    }

	return 0;
}