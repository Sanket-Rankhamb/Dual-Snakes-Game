#ifndef snake_H
#define snake_H

#define RIGHT 1
#define LEFT 2 
#define UP 3
#define DOWN 4

class BodySegment{
	int x, y;
	public:
		BodySegment();
		void setPosition( int x, int y);
		int getX();
		int getY();
		void move(int direction);
};

class Snake{
	
	protected:
		BodySegment *body, oldTail;
		int capacity, length;
		short int direction;
		
	public:
		
		Snake();
		Snake(int hx, int hy, int direction);
		~Snake();
		void initialize( int hx, int hy, int direction );
		int getLength();
		int getDirection();
		int getHeadX();
		int getHeadY();
		BodySegment& getBodySegment(int index);
		int getOldTailX();
		int getOldTailY();
		bool isHeadAt( int x, int y);
		void grow();
		void followHead();
		void moveRight();
		void moveLeft();
		void moveUp();
		void moveDown();
    	bool isSelfCollision();
    	bool isPositionOccupied(int x, int y);
		
};

#endif
