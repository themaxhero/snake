#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "raylib.h"
#include "raymath.h"

#define GRID_BLOCK_WIDTH 16
#define GRID_BLOCK_HEIGHT 16
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define BLOCKS_PER_ROW (int)(SCREEN_WIDTH / GRID_BLOCK_WIDTH)
#define BLOCKS_PER_COL (int)(SCREEN_HEIGHT / GRID_BLOCK_HEIGHT)
#define BLOCK_COUNT BLOCKS_PER_ROW * BLOCKS_PER_COL
#define PLAYER_STARTING_X ceil(BLOCKS_PER_ROW / 2)
#define PLAYER_STARTING_Y ceil(BLOCKS_PER_COL / 2)
#define TICK_TIMEOUT 0.3


typedef struct Player {
  Vector2 position;
  Vector2 direction;
  int size;
} PLAYER;

Vector2 RandomPosition(){
  int x = rand() % BLOCKS_PER_ROW;
  int y = rand() % BLOCKS_PER_COL;
  return (Vector2){x, y};
}

bool ThereIsFoodInPosition(Vector2 position, Vector2* food, int food_count){
  for(int i = 0; i < food_count; i++){
    if (position.x == food[i].x && position.y == food[i].y){
      return true;
    }
  }
  return false;
}

Vector2 NewFoodLocation(Vector2* food, int food_count){
  Vector2 position = RandomPosition();
  if (!ThereIsFoodInPosition(position, food, food_count))
    return position;
  return NewFoodLocation(food, food_count);
}

void Update(PLAYER* player, Vector2* food, int food_count){
  player->position = Vector2Add(player->position, player->direction);

  for(int i = 0; i < food_count; i++) {
    if (player->position.x == food[i].x && player->position.y == food[i].y){
      food[i] = NewFoodLocation(food, food_count);
      player->size += 1;
      printf("Player Size: %d\n", player->size);
    }
  }
}


void DrawPlayer(PLAYER* player){
  DrawRectangle(
      GRID_BLOCK_WIDTH * player->position.x,
      GRID_BLOCK_HEIGHT * player->position.y,
      GRID_BLOCK_WIDTH,
      GRID_BLOCK_HEIGHT,
      GREEN
  );
}

void DrawFood(Vector2* food, int amount){
  for(int i = 0; i < amount; i++)
    DrawRectangle(
        GRID_BLOCK_WIDTH * food[i].x,
        GRID_BLOCK_HEIGHT * food[i].y,
        GRID_BLOCK_WIDTH,
        GRID_BLOCK_HEIGHT,
        RED
    );
}

void DrawGameGrid(){
  for(int i = 0; i <= BLOCK_COUNT; i++){
    DrawRectangleLines(
        GRID_BLOCK_WIDTH * (i % BLOCKS_PER_ROW),
        GRID_BLOCK_HEIGHT * ((int)(i / BLOCKS_PER_ROW)),
        GRID_BLOCK_WIDTH,
        GRID_BLOCK_HEIGHT,
        (Color){0xC3,0xC3,0xC3,0xFF}
    );
  };
}

int main(int argc, char *argv[]){
  srand(time(NULL));
  float untilTick = TICK_TIMEOUT;
  PLAYER* player = malloc(sizeof(PLAYER));
  player->position = (Vector2){PLAYER_STARTING_X, PLAYER_STARTING_Y};
  player->direction = (Vector2){1,0};
  Vector2* food = malloc(8 * sizeof(Vector2));
  for(int i = 0; i < 8; i++)
    food[i] = NewFoodLocation(food, 8);

  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "SNAKE");

  while (!WindowShouldClose()) {
    if (untilTick < 0) {
      Update(player, food, 8);
      untilTick = TICK_TIMEOUT;
    }
    untilTick -= GetFrameTime();

    // Handle Player Direction changes
    if (IsKeyDown(KEY_A)){
      player->direction = (Vector2){-1,0};
    } else if (IsKeyDown(KEY_D)){
      player->direction = (Vector2){1,0};
    }

    if (IsKeyDown(KEY_W)){
      player->direction = (Vector2){0,-1};
    } else if (IsKeyDown(KEY_S)) {
      player->direction = (Vector2){0,1};
    }

    BeginDrawing();
      ClearBackground(BLACK);
      DrawPlayer(player);
      DrawFood(food, 8);
      DrawGameGrid();
    EndDrawing();
  }

  CloseWindow();

  return 0;
}
