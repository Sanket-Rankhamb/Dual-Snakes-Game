#include <iostream>
#include <conio.h>
#include "GameBoard.h"
#include "Macros.h"
#include "SystemFunctions.h"
using namespace std;

void welcome();
void header();
void status( int yourScore, int opponentScore, int gameLoopStatus, bool autoPilot, int colisionStatus = false);
bool gameLoop();

void moveSnake(Snake& snake, int direction);
void drawSnake(GameBoard& gameBoard, Snake& snake, int color = DEFAULT_COLOR);
void drawFood(GameBoard& gameBoard, int color = FOOD_COLOR);
bool isSnakeColide(GameBoard& gameBoard, Snake& snake, Snake& otherSnake);
bool isSnakeEatingFood(GameBoard& gameBoard, Snake& snake, Snake& otherSnake);

const int SleepTime = 130;

int main() {
	
    welcome();
    
    while ( true ) {
    	
        bool win = gameLoop();
        if(win) cout << "CONGRATULATION! YOU WON!!";
        else cout << "SORRY, YOU LOST"; 
        cout << endl << "Press Enter Or Spacebar Key to Restart, or 'O' to Exit";
        
        char c = getch();
        while( !(c == (char) 13 || c == ' ' ) ){ // Enter or SpaceBar key
        	if ( c == 'o' || c == 'O' ){
        		system("cls");
        		exit(0);
			} 
        	c = getch();
        }
        
        system("cls");
    }
    return 0;
}

bool gameLoop() {
	
    header();
    
    GameBoard gameBoard( MIN_X, MIN_Y, MAX_X, MAX_Y );
    gameBoard.drawBorder();
    Snake& snake1 = gameBoard.getSnake(0);
    Snake& snake2 = gameBoard.getSnake(1);
    
    drawSnake(gameBoard, snake1, PLAYER_SNAKE_COLOR);
    drawSnake(gameBoard, snake2, OPPONENT_SNAKE_COLOR);
    gameBoard.generateFood();
    drawFood(gameBoard);

    int direction = snake1.getDirection();
    bool pause = true;
    bool autoPilot = false;
    int score1 = 0;
    int score2 = 0;
    bool win = false;
    int colisionStatus = 0;
    
    status(score1, score2, pause, autoPilot);

    while ( true ) {
    	
        if ( kbhit() ) {
            char c = getch();
            bool wasPaused = pause;
            pause = false;
			
            if ( c == 'e' || c == 'E' ) {
                pause = true;
                autoPilot = true;
            }
            else if ( c == 'q' || c == 'Q' ) {
                autoPilot = false;
                pause = true;
            }
            else if( c == 'o' || c == 'O' ){
            	system("cls"); 
            	exit(0);
			}
            else {
                switch ( c ) {
                    case 'd':
                    case 'D':
                        if ( direction != LEFT ) direction = RIGHT;
                        else pause = true;
                        break;
                    case 'a':
                    case 'A':
                        if ( direction != RIGHT ) direction = LEFT;
                        else pause = true;
                        break;
                    case 'w':
                    case 'W':
                        if ( direction != DOWN ) direction = UP;
                        else pause = true;
                        break;
                    case 's':
                    case 'S':
                        if ( direction != UP ) direction = DOWN;
                        else pause = true;
                        break;
                    default:
                        pause = wasPaused;
                }
            }
        }

        if ( autoPilot ) {
            direction = gameBoard.getAutoDirection(snake1);
        }
        
        int opponentDirection = gameBoard.getAutoDirection(snake2);

        if ( !pause ) {
        	
			moveSnake(snake1, direction);
			moveSnake(snake2, opponentDirection);
			
			drawSnake(gameBoard, snake1, PLAYER_SNAKE_COLOR);
    		drawSnake(gameBoard, snake2, OPPONENT_SNAKE_COLOR);
            
			if( isSnakeColide(gameBoard, snake1, snake2) ){
				colisionStatus = PLAYER_COLIDE;
				win = score1 > score2;	
				break;
			} 
			else if( isSnakeColide(gameBoard, snake2, snake1) ){
				colisionStatus = OPPONENT_COLIDE;
				win = score1 >= score2;
				break;
			} 
						
			if( gameBoard.isSnakeEatingFood(snake1) ){
				score1++;
				snake1.grow();
				gameBoard.generateFood();
				drawFood(gameBoard);
			}
			else if( gameBoard.isSnakeEatingFood(snake2) ){
				score2++;
				snake2.grow();
				gameBoard.generateFood();
				drawFood(gameBoard);
			}
			
            sleep( SleepTime );
        }

        status( score1, score2, pause, autoPilot, colisionStatus );

        if ( pause ) while ( !kbhit() );
    }
    
    status( score1, score2, pause, autoPilot, colisionStatus );
    
    if(colisionStatus > 0){
    	//cout << (char)7;
        sleep( 500 );
	}
	
	setCursorPosition(0, MAX_Y + 2);
    return win;
}

void moveSnake(Snake& snake, int direction){
	
	switch( direction ) {
        case RIGHT: snake.moveRight(); break;
        case LEFT: snake.moveLeft(); break;
        case UP: snake.moveUp(); break;
        case DOWN: snake.moveDown(); break;
    }
}

void drawSnake(GameBoard& gameBoard, Snake& snake, int color){
	setTextColor(color);
	gameBoard.drawSnake(snake);
	setTextColor(DEFAULT_COLOR);
}

void drawFood(GameBoard& gameBoard, int color){
	setTextColor(color);
	gameBoard.drawFood();
	setTextColor(DEFAULT_COLOR);
}

bool isSnakeColide(GameBoard& gameBoard, Snake& snake, Snake& otherSnake){
	
	if( gameBoard.isBorderCollision(snake) || gameBoard.isSnakeToSnakeCollision(snake, otherSnake) ) { 
        return 1;
    }
    else return 0;
}

void status( int yourScore, int opponentScore, int gameLoopStatus, bool autoPilot, int colisionStatus) {
	
    setCursorPosition(STATUS_LINE_X, STATUS_LINE_Y);
    for ( int i = 1; i <= 50; ++i ) cout << " ";
    setCursorPosition(STATUS_LINE_X, STATUS_LINE_Y);
    
    cout << "YOUR SCORE:" << yourScore << " OPPONENT SOCORE:" << opponentScore << " ";
    
    if ( gameLoopStatus == 0 ) cout << "GameLoop:Running ";
    else if ( gameLoopStatus == 1 ) cout << "GameLoop:Paused ";
    else cout << "GameLoop:GameOver ";
    
    cout << ( autoPilot ? "Mode:AutoPilot" : "Mode:Manual" ) << " ";
    
    if(colisionStatus == PLAYER_COLIDE) cout << "Player Colide";
    else if(colisionStatus == OPPONENT_COLIDE) cout << "Opponent Colide";
}

void header() {
    cout << "                         SNAKE GAME " << endl << endl;
    cout << "RIGHT:D  LEFT:A  UP:W  DOWN:S  PAUSE:OppositeDirection  RESUME:NonOppositeDirection  ";
    cout << "AUTOPILOT:E  MANUAL:Q  EXIT:O" << endl << endl;
}

void welcome() {
    system( "cls" );
    cout << "********************** WELCOME TO SNAKE GAME ***********************" << endl;
    cout << endl << "#Controls {" << endl << endl;
    cout << "       RIGHT:D" << endl;
	cout << "        LEFT:A" << endl;
	cout << "          UP:W"  << endl;
	cout << "        DOWN:S" << endl;
	cout << "       PAUSE:Opposite Direction" << endl;
	cout << "      RESUME:Non Opposite Direction" << endl;
    cout << "   AUTOPILOT:E" << endl;
    cout << "      MANUAL:Q" << endl;
    cout << "        EXIT:O" << endl;
    cout << "}" << endl;
    cout << endl << "Press any key to start game..";
    getch();
    system( "cls" );
}


