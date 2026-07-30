#include <iostream>
#include <string>
#include "GameSystem.hpp"
#include "CommandHandler.hpp"

using namespace std;

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        return 1;
    }

    GameSystem game_system;
    game_system.load_player(argv[1]);
    game_system.load_admin(argv[2]);

    CommandHandler handler(game_system);
    handler.run();

    return 0;
}