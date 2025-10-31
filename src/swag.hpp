#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <raylib.h>
#include <nlohmann/json.hpp>
#include <hexrgb.h>
using json = nlohmann::json;

///////////// read file /////////////
inline std::string ReadFile(const std::string &path)
{
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open())
        return {};
    std::ostringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

///////////// spprite /////////////
struct SpriteData
{
    std::string id;
    std::string name;
    std::string base64Src;
    Texture2D texture{};
};

///////////// obkect /////////////
struct ObjectData
{
    int gridX;
    int gridY;
    std::string spriteId;
    Vector2 offset{0, 0};
};

///////////// swag loader. so. friggin. swag. /////////////
class SwagLoader
{
public:
    static bool Load(const std::string &path,
                     int &mapWidth, int &mapHeight,
                     int &tileWidth, int &tileHeight, int &wallHeight,
                     Color &groundColor1, Color &groundColor2,
                     Color &wallColor, Color &wallShade,
                     std::vector<SpriteData> &sprites,
                     std::vector<ObjectData> &objects)
    {

        /////////// read ///////////
        std::string content = ReadFile(path);
        if (content.empty())
            return false;

        try
        {
            /////////// parse all the json ///////////
            auto j = json::parse(content);
            auto cfg = j["config"];

            /////////// config for the map ///////////
            mapWidth = cfg.value("mapWidth", 10);
            mapHeight = cfg.value("mapHeight", 10);
            tileWidth = cfg.value("tileWidth", 64);
            tileHeight = cfg.value("tileHeight", 32);
            wallHeight = cfg.value("wallHeight", 80);

            /////////// colors ///////////
            groundColor1 = fixColors(cfg.value("groundColor1", "#d2b48c"));
            groundColor2 = fixColors(cfg.value("groundColor2", "#bfa078"));
            wallColor = fixColors(cfg.value("wallColor", "#d2b48c"));
            wallShade = fixColors(cfg.value("wallShade", "#977b56"));

            /////////// load all the sprites ///////////
            sprites.reserve(j["sprites"].size());
            for (auto &s : j["sprites"])
            {
                SpriteData sp;
                sp.id = s.value("id", "");
                sp.name = s.value("name", "");
                sp.base64Src = s.value("src", "");
                if (!sp.base64Src.empty())
                {
                    std::vector<unsigned char> data;
                    DecodeBase64Image(sp.base64Src, data); // decode the image,.,
                    Image img = LoadImageFromMemory(".png", data.data(), data.size());
                    sp.texture = LoadTextureFromImage(img);
                    UnloadImage(img);
                }
                sprites.push_back(std::move(sp));
            }

            /////////// load the obkects ///////////
            objects.reserve(j["objects"].size());
            for (auto &o : j["objects"])
            {
                ObjectData obj;
                obj.gridX = o.value("gridX", 0);
                obj.gridY = o.value("gridY", 0);
                obj.spriteId = o.value("spriteId", "");
                if (o.contains("offset"))
                {
                    obj.offset.x = o["offset"].value("x", 0);
                    obj.offset.y = o["offset"].value("y", 0);
                }
                objects.push_back(obj);
            }

            return true;
        }
        catch (std::exception &e)
        {
            std::cerr << "error parsing swag: " << e.what() << "\n";
            return false;
        }
    }

private:
    /////////// decode base64 img ///////////
    static void DecodeBase64Image(const std::string &base64str, std::vector<unsigned char> &out)
    {
        std::string str = base64str;
        if (str.rfind("data:image/png;base64,", 0) == 0)
            str = str.substr(22);
        std::string decoded = DecodeBase64(str);
        out.assign(decoded.begin(), decoded.end());
    }

    /////////// decode ///////////
    static std::string DecodeBase64(const std::string &in)
    {
        static const std::string chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
        std::string out;
        out.reserve(in.size() * 3 / 4);
        std::vector<int> T(256, -1);
        for (int i = 0; i < 64; i++)
            T[chars[i]] = i;
        int val = 0, valb = -8;
        for (unsigned char c : in)
        {
            if (T[c] == -1)
                break;
            val = (val << 6) + T[c];
            valb += 6;
            if (valb >= 0)
            {
                out.push_back(char((val >> valb) & 0xFF));
                valb -= 8;
            }
        }
        return out;
    }
};
