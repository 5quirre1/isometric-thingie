#include "game.hpp"
#include <raylib.h>
#include <hexrgb.h>
#include "object.hpp"

void run_game()
{
    //////////// variables ////////////
    const int screenWidth = 800;
    const int mapWidth = 10;
    const int mapHeight = 10;
    const int tileWidth = 64;
    const int tileHeight = 32;
    ///////////////////////////////////

    // colors for ground
    Color gColor1 = fixColors("#d2b48cff");
    Color gColor2 = fixColors("#bfa078ff");


    ObjectManager objectManager; //  object


    // all the obkects
    objectManager.AddObject(0, 0, "resources/bed.png");
    objectManager.AddObject(1, 0, "resources/desk.png", {20, 15});
    objectManager.AddObject(1, 0, "resources/chair.png", {0, 15});

    /////////// main loop ///////////
    while (!WindowShouldClose())
    {
        BeginDrawing();
            ClearBackground(fixColors("#c4ccd1ff"));

            // ground tiles
            for (int y = 0; y < mapHeight; y++)
            {
                for (int x = 0; x < mapWidth; x++)
                {
                    Vector2 pos = GridToIso(x, y, tileWidth, tileHeight); // convert the grid to iso
                    pos.x += screenWidth / 2;
                    pos.y += 50;

                    // tile corner
                    Vector2 points[4] = {
                        {pos.x, pos.y},
                        {pos.x + tileWidth / 2, pos.y + tileHeight / 2},
                        {pos.x, pos.y + tileHeight},
                        {pos.x - tileWidth / 2, pos.y + tileHeight / 2}};

                    Color tileColor = ((x + y) % 2 == 0) ? gColor1 : gColor2; //color
                    
                    // draw 2 trigianls per square
                    DrawTriangle(points[0], points[3], points[1], tileColor);
                    DrawTriangle(points[1], points[3], points[2], tileColor);
                }
            }

            objectManager.DrawAll(screenWidth / 2, 47, tileWidth, tileHeight); // draw all objects

        EndDrawing();
    }
    /////////////////////////////////

    objectManager.Clear();
}
