#include "game.hpp"
#include "world.hpp"
#include <raylib.h>
#include <hexrgb.h>
#include <string>
extern std::string g_swagPath; // get the path for the map

// game
void run_game()
{
    World world;
    world.LoadFromSwag(g_swagPath);

    const int screenWidth = 800;
    const float yOffset = 100.0f;

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(fixColors("#c4ccd1ff"));
        world.DrawWorld(screenWidth, yOffset);
        EndDrawing();
    }

    world.Clear();
}
