
#include <iostream>
#include <map>
#include <vector>
#include <cmath>
#include <set>
#include "raylib.h"

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

static const int kWindowWidth = 832;
static const int kWindowHeight = 576;
static const float kFrameWidth = 64.f;
static const float kFrameHeight = 96.f;
static const short kStandFrameIndex = 4;
static const short kTiles = 4;
static const int kFrameRate = 40;
static const float kTileSize = 64.f;
static const float kBombSize = 48.f;
static const float kFlameSize = 48.f;
static const bool kDebug = false; 
static const int kCols = 13;
static const int kRows = 9;

using BomberState = enum {STAND, DOWN, UP, LEFT, RIGHT};
using VectorOfFloat = std::vector<float>;
using VectorOfInt = std::vector<short>;
using SpriteSheetMap = std::map<BomberState, VectorOfFloat>;
using Map = std::vector<VectorOfInt>;
using CollisionPoints = std::vector<Vector2>;

VectorOfFloat MakeSpriteSheet(short first, short size, float tileSize)
{
    VectorOfFloat result;
    result.reserve(size);
    for (int i = first; i < first + size; ++i)
    {
        result.emplace_back(i * tileSize);
    }
    return result;
}

static const SpriteSheetMap spriteSheet = {
	{STAND, MakeSpriteSheet(3, 1, kTileSize)},
	{DOWN, MakeSpriteSheet(0, 8, kTileSize)},
	{UP, MakeSpriteSheet(24, 8, kTileSize)},
	{LEFT, MakeSpriteSheet(16, 8, kTileSize)},
	{RIGHT, MakeSpriteSheet(8, 8, kTileSize)} 
};

static Map level1 = 
{
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 2, 2, 0, 2, 0, 2, 0, 0, 1},
    {1, 0, 1, 2, 1, 0, 1, 0, 1, 0, 1, 2, 1},
    {1, 2, 2, 0, 0, 2, 0, 2, 0, 0, 0, 2, 1},
    {1, 2, 1, 0, 1, 2, 1, 0, 1, 0, 1, 0, 1},
    {1, 2, 0, 0, 0, 0, 2, 2, 2, 2, 0, 2, 1},
    {1, 0, 1, 2, 1, 2, 1, 2, 1, 2, 1, 0, 1},
    {1, 0, 2, 0, 0, 2, 2, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};

void DrawTiles(Texture2D &tiles, Map &level)
{
    for (int i = 0; i < kRows; ++i)
    {
        for (int j = 0; j < kCols; ++j)
        {
            short tileIndex = level[i][j];
            if (tileIndex >= kTiles) continue;
            float px = tileIndex * kTileSize;
            Rectangle tileFrame = {px, 0.f, kTileSize, kTileSize};
            Vector2 tilePosition = {j * kTileSize, i * kTileSize};
            DrawTextureRec(tiles, tileFrame, tilePosition, WHITE);
            if (kDebug)
            {
                int ix = int(tilePosition.y) / int(kTileSize);
                int iy = int(tilePosition.x) / int(kTileSize);
                std::string text = "(" + std::to_string(ix) + ", " + std::to_string(iy) + ")";
                DrawText(text.c_str(), tilePosition.x, tilePosition.y, 15, WHITE);
            }
	    }
    }
}

bool Collides(Rectangle &r1, Rectangle &r2)
{
    bool a = r1.x < r2.x + r2.width;
    bool b = r1.x + r1.width > r2.x;
    bool c = r1.y < r2.y + r2.height;
    bool d = r1.y + r1.height > r2.y;
    return a && b && c && d;
}

bool MapCollides(Vector2 &p, Map &level)
{
    int ix = int(p.y) / kTileSize;
    int iy = int(p.x) / kTileSize;
    std::set<short> solid = {1, 2};
    return solid.find(level[ix][iy]) != solid.end();
}

int TileMapValue(Vector2 &p, Map &level)
{
    int ix = int(p.y) / kTileSize;
    int iy = int(p.x) / kTileSize;
    return level[ix][iy];
}

using Frame = struct 
{
    Rectangle rect; 
    int counter; 
    int speed; 
    int current; 
    int stand;
}; 

using Flame = struct
{
    Texture2D texture = LoadTexture("resources/images/flame.png");
    Frame frame = {{0.f, 0.f, kFlameSize, kFlameSize}, 0, 10, 0, 0};
    Vector2 position = {0.f, 0.f};
    bool finished = false;
    int animationCounter = 0;
    void draw(Color c)
    {
		DrawTextureRec(texture, frame.rect, position, c);
    }
    void updateFrame()
    {
        ++frame.counter;
        if (frame.counter >= (kFrameRate / frame.speed))
        {
            ++animationCounter;
            if (animationCounter >= 8) finished = true;
            frame.counter = 0;
            frame.current++;
            if (frame.current > 4) frame.current = 0;
            frame.rect.x = frame.current * kBombSize; 
       }
    }
};

using VectorOfFlames = std::vector<Flame>;

using Bomb = struct
{
    VectorOfFlames flames = {Flame{}, Flame{}, Flame{}, Flame{}, Flame{}};
    Frame frame = {{0.f, 0.f, kBombSize, kBombSize}, 0, 5, 0, 0};
    Texture2D texture = LoadTexture("resources/images/bombs.png");
    Vector2 position = {0.f, 0.f};
    short regressiveCounter = 10;
    int counter = 0;
    bool isActivated = false;
    bool isVisible = false;
    void reset()
    {
        regressiveCounter = 10;
    }
    void draw(Color c, Map &level, Sound &explosion)
    {
        if (isVisible)
        {
		    DrawTextureRec(texture, frame.rect, position, c);
            std::string text = std::to_string(regressiveCounter);
            DrawText(text.c_str(), position.x, position.y, 10, WHITE);
        }
        else
        {
            int counter = 0;
            for (auto &flame: flames)
            {
                if (flame.finished)
                {
                    isActivated = false;
                    isVisible = false;
                    flame.animationCounter = 0;
                    flame.finished = false;
                    regressiveCounter = 10;
                }
                else
                {
                    if (!IsSoundPlaying(explosion)) PlaySound(explosion);
                    flame.position = position;
                    if (counter % 5 == 1)
                    {
                        flame.position.x = position.x - kTileSize;
                    }
                    else if (counter % 5 == 2)
                    {
                        flame.position.x = position.x + kTileSize;
                    }
                    else if (counter % 5 == 3)
                    {
                        flame.position.y = position.y - kTileSize;
                    }
                    else if (counter % 5 == 4)
                    {
                        flame.position.y = position.y + kTileSize;
                    }
                    if (!MapCollides(flame.position, level))
                    {
                        flame.draw(WHITE);
                    }
                    if (TileMapValue(flame.position, level) == 2)
                    {
                        int ix = int(flame.position.y) / kTileSize;
                        int iy = int(flame.position.x) / kTileSize;
                        level[ix][iy] = 0;
                    }
                    flame.updateFrame();
                }
                ++counter;
            }
        }
    }
    void updateFrame()
    {
        ++frame.counter;
        if (frame.counter >= (kFrameRate / frame.speed))
        {
            frame.counter = 0;
            frame.current++;
            if (frame.current > 2) frame.current = 0;
            frame.rect.x = frame.current * kBombSize; 
       }
    }
    void updateRegressiveCounter()
    {
        ++counter;
        if (counter >= (kFrameRate / 4))
        {
            counter = 0;
             --regressiveCounter;
        }
        if (regressiveCounter <= 0)
        {
            isVisible = false;
            regressiveCounter = 10;
            counter = 0;
        }
    }
};

using VectorOfBombs = std::vector<Bomb>;

using Player = struct 
{
    VectorOfBombs bombs = {Bomb{}};
    float offL = 16.f;
    float offR = 32.f;
    float offU = 45.f;
    float offD = 55.f;
	Texture2D texture = LoadTexture("resources/images/arena_bomber_walk.png");
	BomberState state = BomberState::STAND;
	BomberState lastState = BomberState::STAND;
 	Vector2 position = {kTileSize - offL, kTileSize - offU};
    Vector2 direction = { 0.f, 0.f };
    Frame frame = {{spriteSheet.at(state)[0], 0.0f, kFrameWidth, kFrameHeight }, 0, 15, 0, 1};
    float speed = 3.f;
    Rectangle collisionRect = {position.x + offL, position.y + offU, kFrameWidth - offR, kFrameHeight - offD};
    void updateCollisionRect()
    {
        collisionRect = {position.x + offL, position.y + offU, kFrameWidth - offR, kFrameHeight - offD};
    }
    Vector2 calculateNewPosition()
    {
        return {collisionRect.x + direction.x, collisionRect.y + direction.y};
    }
    void onKeyDown(Sound &boing)
    {
        if (IsKeyDown(KEY_RIGHT)) 
        {
            state = BomberState::RIGHT;
            lastState = state;
            direction.x = speed;
        }
	    else if (IsKeyDown(KEY_LEFT)) 
        {
		    state = BomberState::LEFT;
            lastState = state;
            direction.x = -speed;
	    }
	    else if (IsKeyDown(KEY_UP)) 
        {
            state = BomberState::UP;
            lastState = state;
            direction.y = -speed;
        }
	    else if (IsKeyDown(KEY_DOWN)) 
        {
            state = BomberState::DOWN;
            lastState = state;
            direction.y = speed;
        }
        if (IsKeyDown(KEY_SPACE))
        {
            for (auto &bomb: bombs)
            {
                if (!bomb.isActivated)
                {
                    PlaySound(boing);
                    bomb.isActivated = true;
                    bomb.isVisible = true;
                    bomb.position = 
                    {
                        floor(collisionRect.x / kTileSize) * kTileSize + 8, 
                        floor(collisionRect.y / kTileSize) * kTileSize + 8
                    };
                    break;
                }
            }
        }
    }
    void updatePosition()
    {
        position.x += direction.x;
        position.y += direction.y;
    }
    void updateFrame()
    {
        float pxFrame = 0.f;
        ++frame.counter;
        if (state == BomberState::STAND) 
        {
            if (lastState == BomberState::STAND) 
            {
                frame.stand = 0;
            }
            else 
            {
                frame.stand = kStandFrameIndex;
            }
            frame.rect.x = spriteSheet.at(lastState)[frame.stand];
        }            
        if (frame.counter >= (kFrameRate / frame.speed) 
            && state != BomberState::STAND)
        {
            frame.counter = 0;			
            if (frame.current < spriteSheet.at(state).size())
            {
                pxFrame = spriteSheet.at(state)[frame.current];
                ++frame.current;
            }
            else
            {	
                pxFrame = spriteSheet.at(state)[0];
                frame.current = 0;
            }
            frame.rect.x = pxFrame;
        }
    }
    CollisionPoints makeCollisionPoints()
    {
        Vector2 newPosition = calculateNewPosition();
        CollisionPoints result;
        result.reserve(2);
        Vector2 p1 = {newPosition.x, newPosition.y};
        Vector2 p2 = {newPosition.x, newPosition.y + kFrameHeight - offD};
        Vector2 p3 = {newPosition.x + kFrameWidth - offR, newPosition.y};
        Vector2 p4 = {newPosition.x + kFrameWidth - offR, newPosition.y + kFrameHeight - offD};
        if (IsKeyDown(KEY_RIGHT))
        {
            result.emplace_back(p3);
            result.emplace_back(p4);
        }
        else if (IsKeyDown(KEY_LEFT))
        {
            result.emplace_back(p1);
            result.emplace_back(p2);
        }
        else if (IsKeyDown(KEY_UP))
        {
            result.emplace_back(p1);
            result.emplace_back(p3);
        }
        else if (IsKeyDown(KEY_DOWN))
        {
            result.emplace_back(p2);
            result.emplace_back(p4);
        }
        return result;
    }
    void draw(Color c, Map &level, Sound &explosion)
    {
        drawBombs(level, explosion);
		DrawTextureRec(texture, frame.rect, position, c);
    }
    void drawBombs(Map &level, Sound &explosion)
    {
        for (auto &bomb: bombs)
        {
            if (bomb.isActivated)
            {
                bomb.updateFrame();
                bomb.updateRegressiveCounter();
                bomb.draw(WHITE, level, explosion);
            }
        }
    }
};

int main()
{
    InitAudioDevice();
    Sound level = LoadSound("resources/audio/level1.wav");
    Sound boing = LoadSound("resources/audio/boing.wav");
    Sound explosion = LoadSound("resources/audio/explosion.wav");
	InitWindow(kWindowWidth, kWindowHeight, "The Bombeguy");
	SetTargetFPS(kFrameRate); 
    Player player; 
    Texture2D tilesTexture = LoadTexture("resources/images/arena_wall.png");
    while (!WindowShouldClose())
	{
        if (!IsSoundPlaying(level)) PlaySound(level);
		BeginDrawing();
		ClearBackground(GREEN);
        DrawTiles(tilesTexture, level1);
        player.direction = {0.f, 0.f};
		player.state = BomberState::STAND;
        player.onKeyDown(boing);
        Vector2 lastPosition = player.position;
        player.updatePosition();
        player.updateFrame();
        player.updateCollisionRect();
        CollisionPoints collisionPoints = player.makeCollisionPoints();
        for (auto p: collisionPoints)
        {
            if (MapCollides(p, level1))
            {
                player.position = lastPosition;
            }
        }
        player.draw(WHITE, level1, explosion);
	    EndDrawing();
	}
	CloseWindow();
    CloseAudioDevice();
}

