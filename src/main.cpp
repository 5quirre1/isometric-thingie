#include <raylib.h>
#include <string>
#include "game.hpp"
std::string g_swagPath = "map.swag";

int main(int argc, char **argv)
{
    if (argc > 1) g_swagPath = argv[1]; // args
    ///////////// init /////////////
    InitWindow(800, 600, "ddddd");
    InitAudioDevice();
    SetTargetFPS(60);
    ///////////////////////////////

    run_game(); //  run game

    /////////// unload ///////////
    CloseAudioDevice();
    CloseWindow();
    /////////////////////////////
    return 0;
}
