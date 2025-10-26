//iit's 4:03 am i'm super tired sorry for shitty commenting
#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "raylib.h"
#include <vector>
#include <string>

// convert gri to isometric stuff
inline Vector2 GridToIso(int x, int y, int tileWidth, int tileHeight)
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
    int gridX;
    int gridY;
    Texture2D texture;
    Vector2 offset;
    bool loaded;
    
    IsometricObject() : gridX(0), gridY(0), offset({0, 0}), loaded(false) {}
    
    IsometricObject(int x, int y, const char* texturePath, Vector2 customOffset = {0, 0})
        : gridX(x), gridY(y), offset(customOffset), loaded(false)
    {
        texture = LoadTexture(texturePath);
        if (texture.id > 0)
        {
            loaded = true;
        }
    }
    
    // pos
    void SetPosition(int x, int y)
    {
        gridX = x;
        gridY = y;
    }
    // offset
    void SetOffset(Vector2 newOffset)
    {
        offset = newOffset;
    }
    
    // draw
    void Draw(int screenCenterX, int screenCenterY, int tileWidth, int tileHeight)
    {
        if (!loaded) return;
        
        Vector2 pos = GridToIso(gridX, gridY, tileWidth, tileHeight);
        pos.x += screenCenterX;
        pos.y += screenCenterY;
        float drawX = pos.x - texture.width / 2.0f + offset.x;
        float drawY = pos.y - texture.height + tileHeight + offset.y;
        
        DrawTexture(texture, drawX, drawY, WHITE);
    }
    //unload
    void Unload()
    {
        if (loaded)
        {
            UnloadTexture(texture);
            loaded = false;
        }
    }
    
    ~IsometricObject()
    {
        //nothing cause auto unloa
    }
};

// obect manager
class ObjectManager
{
private:
    std::vector<IsometricObject*> objects;
    
public:
    IsometricObject* AddObject(int gridX, int gridY, const char* texturePath, Vector2 offset = {0, 0})
    {
        IsometricObject* obj = new IsometricObject(gridX, gridY, texturePath, offset);
        objects.push_back(obj);
        return obj;
    }
    // remove
    void RemoveObject(IsometricObject* obj)
    {
        for (size_t i = 0; i < objects.size(); i++)
        {
            if (objects[i] == obj)
            {
                objects[i]->Unload();
                delete objects[i];
                objects.erase(objects.begin() + i);
                break;
            }
        }
    }
    // draw all
    void DrawAll(int screenCenterX, int screenCenterY, int tileWidth, int tileHeight)
    {
        std::vector<IsometricObject*> sorted = objects;
        // blah blah vlah smarty pants sort
        for (size_t i = 0; i < sorted.size(); i++)
        {
            for (size_t j = i + 1; j < sorted.size(); j++)
            {
                int sum1 = sorted[i]->gridX + sorted[i]->gridY;
                int sum2 = sorted[j]->gridX + sorted[j]->gridY;
                if (sum1 > sum2)
                {
                    IsometricObject* temp = sorted[i];
                    sorted[i] = sorted[j];
                    sorted[j] = temp;
                }
            }
        }
        
        // draw
        for (auto obj : sorted)
        {
            obj->Draw(screenCenterX, screenCenterY, tileWidth, tileHeight);
        }
    }
    //clear
    void Clear()
    {
        for (auto obj : objects)
        {
            obj->Unload();
            delete obj;
        }
        objects.clear();
    }
    
    ~ObjectManager()
    {
        Clear();
    }
};

#endif