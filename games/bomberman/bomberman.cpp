// bomberman.cpp : Este arquivo contém a função 'main'. A execução do programa começa e termina ali.
//

#include <iostream>
#include <map>
#include <vector>
#include "raylib.h"

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

static const int kWidth = 800;
static const int kHeight = 576;
static const float kFrameWidth = 64.f;
static const float kFrameHeight = 96.f;

using BomberState = enum { STAND, DOWN, UP, LEFT, RIGHT };
using MapSprites = std::map<BomberState, std::vector<float>>;
using Map = std::vector<std::vector<int>>;

static const MapSprites mapSprites = {
	{STAND, {3 * kFrameWidth}},
	{DOWN, {0, 1 * kFrameWidth, 2 * kFrameWidth, 3 * kFrameWidth, 4 * kFrameWidth, 5 * kFrameWidth, 6 * kFrameWidth, 7 * kFrameWidth}},
	{UP, {24 * kFrameWidth, 25 * kFrameWidth, 26 * kFrameWidth, 27 * kFrameWidth, 28 * kFrameWidth, 29 * kFrameWidth, 30 * kFrameWidth, 31 * kFrameWidth}},
	{LEFT, {16 * kFrameWidth, 17 * kFrameWidth, 18 * kFrameWidth, 19 * kFrameWidth, 20 * kFrameWidth, 21 * kFrameWidth, 22 * kFrameWidth, 23 * kFrameWidth}},
	{RIGHT, {8 * kFrameWidth, 9 * kFrameWidth, 10 * kFrameWidth, 11 * kFrameWidth, 12 * kFrameWidth, 13 * kFrameWidth, 14 * kFrameWidth, 15 * kFrameWidth}},
};

static const Map level1 = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 0},
    {0, 8, 0, 8, 0, 8, 0, 8, 0, 8, 0, 8, 0, 8, 0, 8, 0, 8, 0, 8, 0, 8, 0, 8, 0},
    {0, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 0},
    {0, 8, 0, 8, 0, 8, 0, 8, 0, 8, 0, 8, 0, 8, 0, 8, 0, 8, 0, 8, 0, 8, 0, 8, 0},
    {0, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 0},
    {0, 8, 0, 8, 0, 8, 0, 8, 0, 8, 0, 8, 0, 8, 0, 8, 0, 8, 0, 8, 0, 8, 0, 8, 0},
    {0, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 0},
    {0, 8, 0, 8, 0, 8, 0, 8, 0, 8, 0, 8, 0, 8, 0, 8, 0, 8, 0, 8, 0, 8, 0, 8, 0},
    {0, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 0},
    {0, 8, 0, 8, 0, 8, 0, 8, 0, 8, 0, 8, 0, 8, 0, 8, 0, 8, 0, 8, 0, 8, 0, 8, 0},
    {0, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 0},
    {0, 8, 0, 8, 0, 8, 0, 8, 0, 8, 0, 8, 0, 8, 0, 8, 0, 8, 0, 8, 0, 8, 0, 8, 0},
    {0, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 0},
    {0, 8, 0, 8, 0, 8, 0, 8, 0, 8, 0, 8, 0, 8, 0, 8, 0, 8, 0, 8, 0, 8, 0, 8, 0},
    {0, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 0},
    {0, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
};

void print(Rectangle &rect)
{
    std::cout << "(" << rect.x
        << ", " << rect.y
        << ", " << rect.width
        << ", " << rect.height << ")\n";
}

using Frame = struct {Rectangle rect; int counter; int speed; int current; int stand;}; 

void onKeyDown(BomberState &state, BomberState &lastState, Vector2 &direction, const float speed)
{
    if (IsKeyDown(KEY_RIGHT)) {
        state = BomberState::RIGHT;
        lastState = BomberState::RIGHT;
        direction.x = speed;
    }
	else if (IsKeyDown(KEY_LEFT)) {
		state = BomberState::LEFT;
		lastState = BomberState::LEFT;
        direction.x = -speed;
	}
	else if (IsKeyDown(KEY_UP)) {
        state = BomberState::UP;
        lastState = BomberState::UP;
        direction.y = -speed;
    }
	else if (IsKeyDown(KEY_DOWN)) {
        state = BomberState::DOWN;
        lastState = BomberState::DOWN;
        direction.y = speed;
    }
}

void updatePosition(Vector2 &position, Vector2 direction)
{
    position.x += direction.x;
    position.y += direction.y;
}

void updateFrame(Frame &bomberFrame, BomberState &state, BomberState &lastState)
{
    float frame = 0.f;
    ++bomberFrame.counter;
    if (state == BomberState::STAND) 
    {
        if (lastState == BomberState::STAND) bomberFrame.stand = 0;
        else bomberFrame.stand = 4;
        bomberFrame.rect.x = mapSprites.at(lastState)[bomberFrame.stand];
    }            
    if (bomberFrame.counter >= (60 / bomberFrame.speed) 
            && state != BomberState::STAND)
    {
        bomberFrame.counter = 0;			
        if (bomberFrame.current < mapSprites.at(state).size())
        {
            frame = mapSprites.at(state)[bomberFrame.current];
            ++bomberFrame.current;
        }
        else
        {	
            frame = mapSprites.at(state)[0];
            bomberFrame.current = 0;
        }
        bomberFrame.rect.x = frame;
    }
}

void drawTiles(Texture2D &wall, const Map &level)
{
    int cols = 25;
    int rows = 18;
    float w = 32.f;
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            int l = level[i][j];
            if (l >= 8) continue;
            float x = l * w + 1.f;
            Rectangle r = {1.f, 1.f, w, w};
            DrawTextureRec(wall, r, {j * w, i * w}, WHITE);
        }
    }
}

int main()
{
	InitWindow(kWidth, kHeight, "Bomberman");
	SetTargetFPS(60);
	Texture2D bomberman = LoadTexture("resources/images/arena_bomber_walk.png");
	Texture2D wall = LoadTexture("resources/images/arena_wall.png");
	BomberState state = BomberState::STAND;
	BomberState lastState = BomberState::STAND;
    Frame bomberFrame = { { mapSprites.at(state)[0], 0.0f, kFrameWidth, kFrameHeight }, 0, 13, 0,
        1
    };
	Vector2 position = { (float)kWidth / 2.f, (float)kHeight / 2.f };
    Vector2 direction = { 0.f, 0.f };
    float speed = 3.f;
	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(GREEN);
        drawTiles(wall, level1);
        direction = {0.f, 0.f};
		state = BomberState::STAND;
        onKeyDown(state, lastState, direction, speed);
        updatePosition(position, direction);
        updateFrame(bomberFrame, state, lastState);
		DrawTextureRec(bomberman, bomberFrame.rect, position, WHITE);
		EndDrawing();
	}
	CloseWindow();
}

