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
static const int kHeight = 600;
static const float kFrameWidth = 43.0833321f;
static const float kFrameHeight = 45.1250000f;

using BomberState = enum { STAND, DOWN, UP, LEFT, RIGHT };
using MapSprites = std::map<BomberState, std::vector<float>>;

static const MapSprites mapSprites = {
	{STAND, {1 * kFrameWidth}},
	{DOWN, {0, 1 * kFrameWidth, 2 * kFrameWidth}},
	{UP, {9 * kFrameWidth, 10 * kFrameWidth, 11 * kFrameWidth}},
	{LEFT, {6 * kFrameWidth, 7 * kFrameWidth, 8 * kFrameWidth}},
	{RIGHT, {3 * kFrameWidth, 4 * kFrameWidth, 5 * kFrameWidth}},
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
        else bomberFrame.stand = 1;
        bomberFrame.rect.x = mapSprites.at(lastState)[bomberFrame.stand];
        //std::cout << "[Standing]\n";
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

int main()
{
	InitWindow(kWidth, kHeight, "Bomberman");
	SetTargetFPS(60);
	Texture2D bomberman = LoadTexture("resources/images/arena_bomber_walk.png");
	BomberState state = BomberState::STAND;
	BomberState lastState = BomberState::STAND;
    Frame bomberFrame = {
	    { 0.0f, 0.0f, mapSprites.at(state)[0], kFrameHeight },
        0,
        13,
        0,
        1
    };
	Vector2 position = { (float)kWidth / 2.f, (float)kHeight / 2.f };
    Vector2 direction = { 0.f, 0.f };
    float speed = 3.f;
	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(MAGENTA);
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

