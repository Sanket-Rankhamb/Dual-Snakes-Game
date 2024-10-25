#include "snake.h"
#include "Macros.h"
#include <iostream>
#include <cstring>
using namespace std;

Snake::Snake(){
	capacity = 64;
	length = 2;
	body = new BodySegment[capacity];
}

Snake::Snake(int hx, int hy, int direction){
	Snake();
	initialize(hx, hy, direction);
}

void Snake::initialize(int hx, int hy, int direction){
	
	length = 2;
	this->direction = direction;
	body[0].setPosition(hx, hy);
	
	int x; int y;
	if( direction == RIGHT ){
		x = hx - 1;
		y = hy;
	}
	else if( direction == LEFT ){
		x = hx + 1;
		y = hy;
	}
	else if( direction == UP ){
		x = hx;
		y = hy + 1;
	}
	else if( direction == DOWN ){
		x = hx;
		y = hy - 1;
	}
	body[1].setPosition(x, y);
	
	oldTail = body[1];
}

Snake::~Snake(){
	delete body;
}

int Snake::getLength(){
	return length;
}

int Snake::getHeadX(){
	return body[0].getX();
}

int Snake::getHeadY(){
	return body[0].getY();
}

int Snake::getDirection(){
	return direction;
}

bool Snake::isHeadAt( int x, int y){
	if( getHeadX() == x && getHeadY() == y ) return 1;
	else return 0;
}

BodySegment& Snake::getBodySegment(int index){
	return body[index];
}

int Snake::getOldTailX(){
	return oldTail.getX();
}

int Snake::getOldTailY(){
	return oldTail.getY();	
}

void Snake::followHead() {
	
	this->oldTail = body[length-1]; // saving old tail position
	
    for(int i = length - 1; i > 0; --i) {
        body[i] = body[i - 1];
    }
}


void Snake::moveRight(){
	followHead();
	int x = body[0].getX();
	int y = body[0].getY();
	body[0].setPosition( x+1, y);
	direction = RIGHT;
}

void Snake::moveLeft(){
	followHead();
	int x = body[0].getX();
	int y = body[0].getY();
	body[0].setPosition( x-1, y);
	direction = LEFT;
}

void Snake::moveUp(){
	followHead();
	int x = body[0].getX();
	int y = body[0].getY();
	body[0].setPosition( x, y-1);
	direction = UP;
}

void Snake::moveDown(){
	followHead();
	int x = body[0].getX();
	int y = body[0].getY();
	body[0].setPosition( x, y+1);
	direction = DOWN;
}

void Snake::grow(){
	length++;
	if(length > capacity){
		BodySegment* oldBody = body;
		body =  new BodySegment[++capacity];
		memcpy(body, oldBody, sizeof(BodySegment) * (capacity -1) );
		delete oldBody;
	}
	body[length-1] = oldTail;
}

bool Snake::isSelfCollision(){
	int hx = getHeadX();
	int hy = getHeadY();
	for(int i = length-1; i > 0; --i){
		if( hx == body[i].getX() && hy == body[i].getY() ) return 1;
	}
	return 0;
}

bool Snake::isPositionOccupied( int x, int y){
	for( int i = 0; i < length; ++i ){
		if( body[i].getX() == x && body[i].getY() == y ) return 1;
	}
	return 0;
}

BodySegment::BodySegment(){
	x = 1; 
	y = 1;
}

void BodySegment::setPosition(int x, int y){
	this->x = x;
	this->y = y;
}

int BodySegment::getX(){
	return x;
}

int BodySegment::getY(){
	return y;
}
