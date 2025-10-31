#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "raylib.h"
#include <vector>
#include <string>
#include <algorithm>
#include <functional>
#include <thread>

// convert gri to isometric stuff
inline Vector2 GridToIso(float x, float y, int tileWidth, int tileHeight)
{
    Vector2 pos;
    pos.x = (x - y) * tileWidth / 2.0f;
    pos.y = (x + y) * tileHeight / 2.0f;
    return pos;
}

// isometric obiect
class IsometricObject
{
public:
    float gridX;
    float gridY;
    Texture2D texture;
    Vector2 offset;
    bool loaded;
    int spriteLayer;
    float heightTiles;

    IsometricObject() : gridX(0), gridY(0), offset({0, 0}), loaded(false), spriteLayer(0), heightTiles(0) {}

    IsometricObject(float x, float y, const char *texturePath, Vector2 customOffset = {0, 0})
        : gridX(x), gridY(y), offset(customOffset), loaded(false), spriteLayer(0), heightTiles(0)
    {
        texture = LoadTexture(texturePath);
        if (texture.id > 0)
            loaded = true;
    }
    // pos
    void SetPosition(float x, float y)
    {
        gridX = x;
        gridY = y;
    }
    // offset
    void SetOffset(Vector2 newOffset) { offset = newOffset; }
    // layer
    void SetLayer(int layer) { spriteLayer = layer; }
    // height
    void SetHeightTiles(float height) { heightTiles = height; }
    // draw
    void Draw(int screenCenterX, int screenCenterY, int tileWidth, int tileHeight)
    {
        if (!loaded)
            return;
        Vector2 pos = GridToIso(gridX, gridY, tileWidth, tileHeight);
        pos.x += screenCenterX;
        pos.y += screenCenterY;
        float drawX = pos.x - texture.width / 2.0f + offset.x;
        float drawY = pos.y - texture.height + tileHeight + offset.y;
        DrawTexture(texture, drawX, drawY, WHITE);
    }
    // unload
    void Unload()
    {
        if (loaded)
        {
            UnloadTexture(texture);
            loaded = false;
        }
    }

    ~IsometricObject() { /* nothing cause auto unload */ }
};

// obect manager
class ObjectManager
{
private:
    std::vector<IsometricObject> objects;
    Texture2D skybox;
    bool skyboxLoaded = false;

public:
    // add objects tuff
    IsometricObject &AddObject(float gridX, float gridY, const char *texturePath, Vector2 offset = {0, 0})
    {
        objects.emplace_back(gridX, gridY, texturePath, offset);
        return objects.back();
    }

    IsometricObject &AddObject(float gridX, float gridY, Texture2D texture, Vector2 offset = {0, 0})
    {
        objects.emplace_back();
        IsometricObject &obj = objects.back();
        obj.gridX = gridX;
        obj.gridY = gridY;
        obj.offset = offset;
        obj.texture = texture;
        obj.loaded = true;
        obj.spriteLayer = 0;
        obj.heightTiles = 0;
        return obj;
    }

    // remove
    void RemoveObject(IsometricObject &obj)
    {
        auto it = std::find_if(objects.begin(), objects.end(), [&](const IsometricObject &o)
                               { return &o == &obj; });
        if (it != objects.end())
        {
            it->Unload();
            objects.erase(it);
        }
    }

    // skybox
    void SetSkybox(const char *texturePath)
    {
        if (skyboxLoaded)
            UnloadTexture(skybox);
        skybox = LoadTexture(texturePath);
        skyboxLoaded = (skybox.id > 0);
    }

    // actually draw
    void DrawSkybox(int screenWidth, int screenHeight)
    {
        if (!skyboxLoaded)
            return;

        float scale = 2.5f;
        float w = screenWidth * scale;
        float h = screenHeight * scale;
        float offsetX = (screenWidth - w) / 2.0f;
        float offsetY = (screenHeight - h) / 2.0f - screenHeight * 0.15f;

        DrawTexturePro(
            skybox,
            {0.0f, 0.0f, (float)skybox.width, (float)skybox.height},
            {offsetX, offsetY, w, h},
            {0.0f, 0.0f},
            0.0f, WHITE);
    }

    // draw all
    void DrawAll(int screenCenterX, int screenCenterY, int tileWidth, int tileHeight)
    {
        std::vector<std::reference_wrapper<IsometricObject>> sorted(objects.begin(), objects.end());
        std::sort(sorted.begin(), sorted.end(), [](const IsometricObject &a, const IsometricObject &b)
                  {
                      if (a.spriteLayer != b.spriteLayer)
                          return a.spriteLayer < b.spriteLayer;
                      float depthA = a.gridX + a.gridY + a.heightTiles;
                      float depthB = b.gridX + b.gridY + b.heightTiles;
                      return depthA < depthB; });

        for (auto &obj : sorted)
            obj.get().Draw(screenCenterX, screenCenterY, tileWidth, tileHeight);
    }

    // clear
    void Clear()
    {
        std::vector<std::thread> threads;
        for (auto &obj : objects)
        {
            threads.emplace_back([&obj]()
                                 { obj.Unload(); });
        }
        for (auto &t : threads)
            if (t.joinable())
                t.join();

        objects.clear();

        if (skyboxLoaded)
        {
            UnloadTexture(skybox);
            skyboxLoaded = false;
        }
    }

    ~ObjectManager() { Clear(); }
};

#endif
