#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include "snake.h"

struct Food {
	int x, y;
};

class GameBoard{
    int minX, minY, maxX, maxY;
    Snake snake[2];
    Food food;
	public:
    GameBoard(int minX, int minY, int maxX, int maxY);
    ~GameBoard();
    void drawBorder();
    void drawSnake(Snake& snake);
    Snake& getSnake(int index);
    bool isBorderCollision(Snake& snake);
    int generateFood();
    void drawFood();
	bool isSnakeEatingFood(Snake& snake);
	bool isFreePosition(int x, int y);
	int getFreeDirectionForSnake(Snake& snake); 
	int getAutoDirection(Snake& snake);  
	bool isSnakeToSnakeCollision(Snake& snake, Snake& otherSnake);
};

#endif
