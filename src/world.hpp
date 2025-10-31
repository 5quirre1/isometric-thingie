#pragma once
#include <raylib.h>
#include <string>
#include <vector>
#include <unordered_map>
#include "object.hpp"
#include "swag.hpp"

// world
class World
{
public:
    ///////////// world stuff /////////////
    int mapWidth = 10;
    int mapHeight = 10;
    int tileWidth = 64;
    int tileHeight = 32;
    int wallHeight = 80;

    //  color
    Color groundColor1 = fixColors("#d2b48c");
    Color groundColor2 = fixColors("#bfa078");
    Color wallColor = fixColors("#d2b48c");
    Color wallShade = fixColors("#977b56");

    // sprite and object stuff
    std::vector<SpriteData> sprites;
    std::vector<ObjectData> objects;

    ObjectManager objectManager;

    // swag
    bool LoadFromSwag(const std::string &path)
    {
        Texture2D skyboxTexture{};
        bool success = SwagLoader::Load(path,
                                mapWidth, mapHeight,
                                tileWidth, tileHeight, wallHeight,
                                groundColor1, groundColor2,
                                wallColor, wallShade,
                                sprites, objects, skyboxTexture);
        
        // skybox zet
        if (success && skyboxTexture.id > 0)
        {
            objectManager.SetSkybox(skyboxTexture);
        }
        
        return success;
    }

    // draw the world
    void DrawWorld(int screenWidth, float yOffset)
    {
        for (int y = 0; y < mapHeight; y++)
        {
            for (int x = 0; x < mapWidth; x++)
            {
                Vector2 pos = GridToIso(x, y, tileWidth, tileHeight);
                pos.x += screenWidth / 2;
                pos.y += yOffset;

                Vector2 points[4] = {
                    {pos.x, pos.y},
                    {pos.x + tileWidth / 2, pos.y + tileHeight / 2},
                    {pos.x, pos.y + tileHeight},
                    {pos.x - tileWidth / 2, pos.y + tileHeight / 2}};

                Color tileColor = ((x + y) % 2 == 0) ? groundColor1 : groundColor2;
                DrawTriangle(points[0], points[3], points[1], tileColor);
                DrawTriangle(points[1], points[3], points[2], tileColor);

                if (x == 0)
                    DrawWall(pos, "left");
                if (y == 0)
                    DrawWall(pos, "back");
            }
        }

        std::unordered_map<std::string, Texture2D> spriteMap;
        for (auto &s : sprites)
            spriteMap[s.id] = s.texture;

        for (auto &obj : objects)
        {
            auto it = spriteMap.find(obj.spriteId);
            if (it != spriteMap.end())
            {
                IsometricObject &addedObj = objectManager.AddObject(obj.gridX, obj.gridY, it->second, obj.offset);
                addedObj.SetLayer(obj.spriteLayer);
            }
        }

        objectManager.DrawAll(screenWidth / 2, yOffset - 3, tileWidth, tileHeight);
    }

    // clear
    void Clear()
    {
        objectManager.Clear();
        for (auto &s : sprites)
        {
            if (s.texture.id > 0)
                UnloadTexture(s.texture);
        }
        sprites.clear();
        objects.clear();
    }
    // remove wold
    ~World()
    {
        Clear();
    }

private:
    // draw walls
    void DrawWall(Vector2 basePos, const std::string &side)
    {
        Vector2 pos = basePos;
        if (side == "left")
        {
            Vector2 points[4] = {
                {pos.x - tileWidth / 2, pos.y + tileHeight / 2},
                {pos.x - tileWidth / 2, pos.y + tileHeight / 2 - wallHeight},
                {pos.x, pos.y - wallHeight},
                {pos.x, pos.y}};
            DrawTriangle(points[0], points[2], points[1], wallShade);
            DrawTriangle(points[0], points[3], points[2], wallShade);
        }
        else if (side == "back")
        {
            Vector2 points[4] = {
                {pos.x, pos.y},
                {pos.x, pos.y - wallHeight},
                {pos.x + tileWidth / 2, pos.y + tileHeight / 2 - wallHeight},
                {pos.x + tileWidth / 2, pos.y + tileHeight / 2}};
            DrawTriangle(points[0], points[2], points[1], wallColor);
            DrawTriangle(points[0], points[3], points[2], wallColor);
        }
    }
};
