#ifndef GAME_H
#define GAME_H

#include "Snake.h"
#include <windows.h>
#include "GameBoard.h"

void menu();
int game();
int match();
void header();
void status( int yourScore, int opponentScore, int gameLoopStatus, bool autoPilot, int colisionStatus = false);
int gamemach();

void handleUserInput();
void moveSnake(Snake& snake, int direction);
void drawSnake(GameBoard& gameBoard, Snake& snake, int color = DEFAULT_COLOR);
void drawFood(GameBoard& gameBoard, Food& food, int color);

#endif
