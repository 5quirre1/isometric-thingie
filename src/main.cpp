#include <raylib.h>
#include "game.hpp"

int main(void)
{
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