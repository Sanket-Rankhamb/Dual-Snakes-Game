#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include "snake.h"
#include "ConsoleFunctions.h"
#include "macros.h"
struct Food {
	int x, y;
};

class GameBoard{
	
    int minX, minY, maxX, maxY;
    Snake snake[2];
    Food food[2];
    
	public:
    GameBoard(int minX, int minY, int maxX, int maxY);
    ~GameBoard();
    void drawBorder();
    void drawSnake(Snake& snake, int color = DEFAULT_COLOR);
    Snake& getSnake(int index);
    Food& getFood(int index);
    bool isBorderCollision(Snake& snake);
    bool isSnakeToSnakeCollision(Snake& snake, Snake& otherSnake);
    bool isSnakeColide(Snake& snake, Snake& otherSnake);
    int generateFood(Food& food);
    void drawFood(Food& food);
	bool isSnakeEatingFood(Snake& snake, Food& food);
	bool isSnakeEatingWrongFood(Snake& snake, Food& food);
	bool isFreePosition(int x, int y);
	int getRandomFreeDirectionForSnake(Snake& snake); 
	int getAutoDirection(Snake& snake, Food& food, bool isFoodNew = false, int aiLevel = EASY);
	int getRandomDirection(int curDir);
	int* getPossibleDirections(int curDir);
};

#endif
