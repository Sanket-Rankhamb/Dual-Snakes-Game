#include "GameBoard.h"
#include <iostream>
#include "ConsoleFunctions.h"
#include <windows.h>
#include "Macros.h"
#include <stdlib.h>
#include <time.h>
#include <future>

using namespace std;

GameBoard::GameBoard(int minX, int minY, int maxX, int maxY ){	
	snake[0].initialize(minX+1, minY+1, RIGHT);
	snake[1].initialize(maxX-1, maxY-1, LEFT );
	this->minX = minX;
	this->minY = minY;
	this->maxX = maxX;
	this->maxY = maxY;
}

GameBoard::~GameBoard(){
	
}

void GameBoard::drawBorder() {
	
    // All 4 corners
    
    setCursorPosition(minX - 1, minY - 1);
    cout << (char) TOP_LEFT_CORNER;
    
    setCursorPosition(maxX + 1, minY - 1);
    cout << (char) TOP_RIGHT_CORNER;
    
    setCursorPosition(minX - 1, maxY + 1);
    cout << (char) BOTTOM_LEFT_CORNER;
    
    setCursorPosition(maxX + 1, maxY + 1);
    cout << (char) BOTTOM_RIGHT_CORNER;
    
    // Top and Bottom border
    for (int x = minX; x <= maxX; ++x) {
    	
    	// Top Border
        setCursorPosition(x, minY - 1);
        cout << (char) HORIZONTAL;
        //Bottom Border
        setCursorPosition(x, maxY + 1);
        cout << (char) HORIZONTAL;
    }
    
    // Left and right border
    for (int y = minY; y <= maxY; ++y) {
    	
        // Left border 
        setCursorPosition(minX - 1, y);
        cout << (char) VERTICLE;
        
        // Right border
        setCursorPosition(maxX + 1, y);
        cout << (char) VERTICLE;
    }
    
    // Reset cursor to the bottom right corner or an empty area
    setCursorPosition(minX, maxY+1);
}

void GameBoard::drawSnake(Snake& snake, int color){
    
    // Erase the tail position of snake
    int x = snake.getOldTailX();
    int y = snake.getOldTailY();
    setCursorPosition(x, y);
    cout << " ";
    
    // draw the snake body ( Except Head )
    int length = snake.getLength();
	for(int i=1; i<length; i++ ){
		int x = snake.getBodySegment(i).getX();
		int y = snake.getBodySegment(i).getY();
		setCursorPosition(x, y);
        cout << (char) BODY_SEGMENT;
	}
	
    // draw the head
    x = snake.getHeadX();
	y = snake.getHeadY();
	setCursorPosition(x, y);
    cout << (char) HEAD;    
}

Snake& GameBoard::getSnake(int index){
	return this->snake[index];
}

Food& GameBoard::getFood(int index){
	return this->food[index];
}

bool GameBoard::isBorderCollision(Snake& snake){
	int x = snake.getHeadX();
	int y = snake.getHeadY();
	if(x > maxX || y > maxY) return 1;
	else if( x < minX || y < minY ) return 1;
	return 0;
}

bool GameBoard::isSnakeToSnakeCollision(Snake& snake, Snake& otherSnake){

	if( otherSnake.isPositionOccupied(snake.getHeadX(), snake.getHeadY() ) ) return 1;
	else return 0;
}

bool GameBoard::isSnakeColide(Snake& snake, Snake& otherSnake){
	return isBorderCollision(snake) || isSnakeToSnakeCollision(snake, otherSnake) || snake.isSelfCollision();
}

int GameBoard::generateFood(Food& food) {
    int x, y;
    // Loop until a valid position is found
    while (true) {
        // Generate random positions within bounds
        x = rand() % (maxX - minX) + minX; // x position
        y = rand() % (maxY - minY) + minY; // y position

        // Check if the generated position is occupied by the snake or existing food
        bool occupied = false;
        for (int i = 0; i < 2; i++) { // Replace '2' with a constant if applicable
            if (this->snake[i].isPositionOccupied(x, y) || 
                (this->food[i].x == x && this->food[i].y == y)) {
                occupied = true;
                break;
            }
        }

        // If the position is not occupied, break out of the loop
        if (!occupied) {
            food.x = x;
            food.y = y;
            break; // Exit the loop
        }
    }
    
    return 0;
}


void GameBoard::drawFood(Food& food){
	setCursorPosition(food.x, food.y);
    cout << (char) FOOD;
}

bool GameBoard::isSnakeEatingFood(Snake& snake, Food& food){
	if( snake.getHeadX() == food.x && snake.getHeadY() == food.y ) return 1;
	else return 0;
}

bool GameBoard::isSnakeEatingWrongFood(Snake& snake, Food& food){
	return isSnakeEatingFood(snake, food);
}


bool GameBoard::isFreePosition(int x, int y){
	if( x < minX || x > maxX) return 0;
	else if( y < minY || y > maxY) return 0;
	return ! ( snake[0].isPositionOccupied(x, y) || snake[1].isPositionOccupied(x, y) );
}

int GameBoard::getRandomFreeDirectionForSnake(Snake& snake){
	
	int hx = snake.getHeadX();
	int hy = snake.getHeadY();
	int *possibles = getPossibleDirections( snake.getDirection() );
	
	int freeDirs[3];
	int count = 0;
	BodySegment tmp;
	tmp.setPosition(hx, hy);
	
	for( int i = 0; i < 3; i++ ){
		tmp.move( possibles[i] );
		if( isFreePosition( tmp.getX(), tmp.getY() ) ){
			freeDirs[count++] = possibles[i];
		}
		tmp.setPosition(hx, hy);
	}
	
	if( count == 0 ) return snake.getDirection();
	else{
		return freeDirs[ rand() % count ];
	}
}

int GameBoard::getAutoDirection(Snake& snake, Food& food, bool isFoodNew, int aiLevel){
	
	int d = snake.getDirection(), dir; // new direciton
	int hx = snake.getHeadX();
	int hy = snake.getHeadY();
	int fx = food.x;
	int fy = food.y;
	
	if( isFoodNew )
	{
		if( aiLevel == EASY ){
			if( snake.getLength() > 5 ) return getRandomDirection(d); 
			else return getRandomFreeDirectionForSnake(snake);
		}
		else if ( aiLevel == MEDIUM ){
			return getRandomFreeDirectionForSnake(snake);
		}
		else if( aiLevel == HARD )
		{
			BodySegment tmp; 
			tmp.setPosition(hx, hy);
			tmp.move(d);
			if( isFreePosition( tmp.getX(), tmp.getY() ) ) return d;
			else return getRandomFreeDirectionForSnake(snake);
		}
	}
	
	if( hy != fy && hx != fx ){
		if( aiLevel == EASY ){
			if ( snake.getLength() > 5 && rand() % 10 < 2) return getRandomFreeDirectionForSnake(snake);
		}
		else if( aiLevel == MEDIUM ){
			if( snake.getLength() > 10 && rand() % 10 < 2) return getRandomFreeDirectionForSnake(snake);
		}
	}
	
	
	if( d == RIGHT ){
		
		if(hy == fy){ // s and f at same line
			if(hx < fx) dir = RIGHT; // f is infront of s
			else{
			 	dir = ( (hy == minY || hy != maxY) ? DOWN : UP ); //f is backword of s
			}
		} 
		
		else if(hx == fx) dir = ( (hy < fy ) ? DOWN : UP );// s and f are in same column
		
		else if(hx < fx) dir = RIGHT;
		
		else if( hy < fy) dir = DOWN; // s is upside of f
		else dir = UP; // s is downside of f
		
	}
	else if( d == LEFT ){
		
		if(hy == fy ){
			if(hx > fx) dir = LEFT;
			else dir = ( (hy == minY || hy != maxY) ? DOWN : UP );
		} 
		
		else if(hx == fx) dir = ( (hy < fy ) ? DOWN : UP );
		
		else if(hx > fx) dir = LEFT;
		
		else if( hy < fy) dir = DOWN;
		else dir = UP;	
			
	}
	else if( d == UP ){
		
		if(hx == fx){ // s and f are in same column
			if(hy > fy) dir = UP;  // f is infront of s
			else dir = ( (hx == minX || hx != maxX) ? RIGHT : LEFT ); // f is backword of s
		} 
		
		else if(hy == fy) dir = ( (hx < fx ) ? RIGHT : LEFT ); // s and f are in same line
		
		else if(hy > fy) dir = UP;
		
		else if( hx < fx ) dir = RIGHT;
		else dir = LEFT;
		
	}
	else if( d == DOWN ){
		
		if(hx == fx){ // s and f are in same column
			if(hy < fy) dir = DOWN;  // f is infront of s
			else dir = ( (hx == minX || hx != maxX) ? RIGHT : LEFT ); // f is backword of s
		} 
		
		else if(hy == fy) dir = ( (hx < fx ) ? RIGHT : LEFT ); // s and f are in same line
		
		else if(hy < fy) dir = DOWN;
		
		else if( hx < fx ) dir = RIGHT;
		else dir = LEFT;
		
	}
		
	BodySegment tmp;
	tmp.setPosition(hx, hy);
	tmp.move(dir);
	if( !isFreePosition( tmp.getX(), tmp.getY() ) ){
		
		if( aiLevel == EASY  ) return getRandomDirection(d);
		else if( aiLevel == MEDIUM ) return getRandomFreeDirectionForSnake(snake);
		else {
			tmp.setPosition(hx, hy);
			tmp.move(d);
			if( isFreePosition( tmp.getX(), tmp.getY() ) ) return d;
			else return getRandomFreeDirectionForSnake(snake);
		}
	}
	
	return dir;
}

int GameBoard::getRandomDirection(int curDir) {
    int *possibles = getPossibleDirections(curDir);
    int d = possibles[rand() % 3];
    delete[] possibles;
    return d;
}

int* GameBoard::getPossibleDirections( int curDir ){
	
	int *possibles = new int[3];
	
	switch (curDir) {
        case RIGHT:
            possibles[0] = UP;
            possibles[1] = DOWN;
            possibles[2] = RIGHT;
            break;
        case LEFT:
            possibles[0] = UP;
            possibles[1] = DOWN;
            possibles[2] = LEFT;
            break;
        case UP:
            possibles[0] = RIGHT;
            possibles[1] = LEFT;
            possibles[2] = UP;
            break;
        case DOWN:
            possibles[0] = RIGHT;
            possibles[1] = LEFT;
            possibles[2] = DOWN;
            break;
        default:
            // Handle invalid direction case, should not happen
            possibles[0] = RIGHT;
            possibles[1] = LEFT;
            possibles[2] = UP;
            break;
	}
	
    return possibles;
}




