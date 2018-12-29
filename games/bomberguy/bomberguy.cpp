
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
static const bool kDebug = false; 
static const int kCols = 13;
static const int kRows = 9;

using BomberState = enum {STAND, DOWN, UP, LEFT, RIGHT};
using VectorOfFloat = std::vector<float>;
using VectorOfInt = std::vector<short>;
using SpriteSheetMap = std::map<BomberState, VectorOfFloat>;
using Map = std::vector<VectorOfInt>;
using CollisionPoints = std::vector<Vector2>;

VectorOfFloat makeSpriteSheet(short first, short size, float tileSize)
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
	{STAND, makeSpriteSheet(3, 1, kTileSize)},
	{DOWN, makeSpriteSheet(0, 8, kTileSize)},
	{UP, makeSpriteSheet(24, 8, kTileSize)},
	{LEFT, makeSpriteSheet(16, 8, kTileSize)},
	{RIGHT, makeSpriteSheet(8, 8, kTileSize)} 
};

static const Map level1 = 
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

void drawTiles(Texture2D &tiles, const Map &level)
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

bool collides(Rectangle &r1, Rectangle &r2)
{
    bool a = r1.x < r2.x + r2.width;
    bool b = r1.x + r1.width > r2.x;
    bool c = r1.y < r2.y + r2.height;
    bool d = r1.y + r1.height > r2.y;
    return a && b && c && d;
}

bool MapCollides(Vector2 &p, const Map &level)
{
    int ix = int(p.y) / kTileSize;
    int iy = int(p.x) / kTileSize;
    std::set<short> solid = {1, 2};
    return solid.find(level[ix][iy]) != solid.end();
}

using Frame = struct {Rectangle rect; int counter; int speed; int current; int stand;}; 
using Player = struct 
{
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
    void onKeyDown()
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
    void draw(Color c)
    {
		DrawTextureRec(texture, frame.rect, position, c);
    }
};

int main()
{
	InitWindow(kWindowWidth, kWindowHeight, "The Bombeguy");
	SetTargetFPS(kFrameRate); Player player; Texture2D tilesTexture = LoadTexture("resources/images/arena_wall.png");
	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(GREEN);
        drawTiles(tilesTexture, level1);
        player.direction = {0.f, 0.f};
		player.state = BomberState::STAND;
        player.onKeyDown();
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
        player.draw(WHITE);
	    EndDrawing();
	}
	CloseWindow();
}

