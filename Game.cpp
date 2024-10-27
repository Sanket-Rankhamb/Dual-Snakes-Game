#include "game.h"
#include <iostream>
#include <conio.h>
#include "Macros.h"
#include "ConsoleFunctions.h"
#include <thread>
using namespace std;

int difficultyLevel = EASY;
int direction;
bool pause;
bool autoPilot;
int score1;
int score2;
int result;
int colisionStatus;
const int SleepTime = 0;

int game(){
	
	while ( true ) {
    	
        int result = match();
        if(result == WIN){
        	setTextColor(PLAYER_COLOR);
        	cout << "CONGRATULATION! YOU WON!!";
		} 
        else if(result == LOST){
        	setTextColor(OPPONENT_COLOR);
        	cout << "SORRY, YOU LOST"; 
		}
		else
		{
			cout << "DRAW";
		}
		setTextColor(DEFAULT_COLOR);
        cout << endl << "Press Enter/Space:Restart, M:Menu, O:Exit";
        
        char c = getch();
        while( !(c == (char) 13 || c == ' ' ) ){ // Enter or SpaceBar key
        	if( c == 'o' || c == 'O' ){
        		exit(0);
			} 
			else if( c == 'm' || c == 'M' ) return 0;
        	else c = getch();
        }
        system("cls");
    }
}

int match() {
	
    header();
    
    GameBoard gameBoard = GameBoard(MIN_X, MIN_Y, MAX_X, MAX_Y);
    Snake &snake1 = gameBoard.getSnake(0);
    Snake &snake2 = gameBoard.getSnake(1);
    Food &food1 = gameBoard.getFood(0);
    Food &food2 = gameBoard.getFood(1);
    direction = snake1.getDirection();
    pause = true;
    autoPilot = false;
    score1 = 0;
    score2 = 0;
    result = false;
    bool isFood1New = false;
    bool isFood2New = false;
    colisionStatus = 0;

	gameBoard.drawBorder();
	drawSnake(gameBoard, snake1, PLAYER_COLOR);
	drawSnake(gameBoard, snake2, OPPONENT_COLOR);
	gameBoard.generateFood(food1);
	gameBoard.generateFood(food2);
	drawFood(gameBoard, food1, PLAYER_FOOD_COLOR);
    drawFood(gameBoard, food2, OPPONENT_FOOD_COLOR);
    status(score1, score2, pause, autoPilot);

    while ( true ) {
    	
        if ( kbhit() ) handleUserInput();
        if (autoPilot) direction = gameBoard.getAutoDirection(snake1, food1, isFood1New, difficultyLevel);
        int opponentDirection = gameBoard.getAutoDirection(snake2, food2, isFood2New, difficultyLevel);
        isFood1New = false;
        isFood2New = false;
        
        if ( !pause ) {
        	
			thread m1 = thread(moveSnake, ref(snake1), direction ); //moveSnake(snake1, direction);
			moveSnake(snake2, opponentDirection);
			m1.join();
			
			drawSnake(gameBoard, snake1, PLAYER_COLOR);
    		drawSnake(gameBoard, snake2, OPPONENT_COLOR);
    		
    		if( snake1.getHeadX() == snake2.getHeadX() &&  snake1.getHeadY() == snake2.getHeadY() ){
    			colisionStatus = DRAW;
    			result = DRAW;
    			break;
			}
			else if( gameBoard.isSnakeColide(snake1, snake2) ){
				colisionStatus = PLAYER_COLIDE;
				if( score1 > score2 + 5 ) result = WIN;
				else result = LOST;
				break;
			} 
			else if( gameBoard.isSnakeColide(snake2, snake1) ){
				colisionStatus = OPPONENT_COLIDE;
				if( score1 >= score2 - 5 ) result = WIN;
				else result = LOST;
				break;
			} 
			
			if( gameBoard.isSnakeEatingWrongFood(snake1, food2) ){
				score1--;
				score2++;
				gameBoard.generateFood(food2);
				drawFood(gameBoard, food2, OPPONENT_FOOD_COLOR);
				isFood2New = true;
			}
			else if( gameBoard.isSnakeEatingWrongFood(snake2, food1) ){
				score2--;
				score1++;
				gameBoard.generateFood(food1);
				drawFood(gameBoard, food1, PLAYER_FOOD_COLOR);
				isFood1New = true;
			}			
			else if( gameBoard.isSnakeEatingFood(snake1, food1) ){
				score1++;
				snake1.grow();
				gameBoard.generateFood(food1);
				drawFood(gameBoard, food1, PLAYER_FOOD_COLOR);
				isFood1New = true;
			}
			else if( gameBoard.isSnakeEatingFood(snake2, food2) ){
				score2++;
				snake2.grow();
				gameBoard.generateFood(food2);
				drawFood(gameBoard, food2, OPPONENT_FOOD_COLOR);
				isFood2New = true;
			}
			
			status( score1, score2, pause, autoPilot, colisionStatus );
            Sleep( SleepTime );
        }
		else if ( pause ) while ( !kbhit() );
    }
    
    status( score1, score2, pause, autoPilot, colisionStatus );
    
    if(colisionStatus > 0){
    	//cout << (char)7;
        Sleep( 500 );
	}
	
	setCursorPosition(0, STATUS_LINE_Y + 1);
    return result;
}

void handleUserInput(){
	
	char c = getch();
    bool wasPaused = pause;
    pause = false;
	
    if ( c == 'e' || c == 'E' ) {
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
    	
    	int tmpDirection = 0;
        switch ( c ) {
            case 'd':
            case 'D': tmpDirection = RIGHT; break;
            case 'a':
            case 'A': tmpDirection = LEFT; break;
            case 'w':
            case 'W': tmpDirection = UP; break;
            case 's':
            case 'S': tmpDirection = DOWN; break; 
            case ARROW_KEY_INDICATOR: c = getch(); // Specific Arrow Key
            	switch( c ){
                	case 'M': tmpDirection = RIGHT; break;
                    case 'K': tmpDirection = LEFT; break;
                    case 'H': tmpDirection = UP; break;
                    case 'P': tmpDirection = DOWN; break;
				} break;
            default: pause = wasPaused;
        }
        
        if( tmpDirection > 0) switch(tmpDirection){
        	case RIGHT: if( direction != LEFT ) direction = RIGHT; else pause = true; break;
        	case LEFT : if( direction != RIGHT ) direction = LEFT; else pause = true; break;
        	case UP : if( direction != DOWN ) direction = UP; else pause = true; break;
        	case DOWN : if( direction != UP ) direction = DOWN; else pause = true; break;
        	default: break;
		}
    }
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
	gameBoard.drawSnake(snake, color);
	setTextColor(DEFAULT_COLOR);
}

void drawFood(GameBoard& gameBoard, Food& food, int color){
	setTextColor(color);
	gameBoard.drawFood(food);
	setTextColor(DEFAULT_COLOR);
}

void status( int yourScore, int opponentScore, int gameLoopStatus, bool autoPilot, int colisionStatus) {
	
    setCursorPosition(STATUS_LINE_X, STATUS_LINE_Y);
    for ( int i = 1; i <= 50; ++i ) cout << " ";
    setCursorPosition(STATUS_LINE_X, STATUS_LINE_Y);
    
    cout << "YOUR SCORE:";
	setTextColor(PLAYER_COLOR);
	cout << yourScore;
    setTextColor(DEFAULT_COLOR);
	cout << " OPPONENT SOCORE:";
	setTextColor(OPPONENT_COLOR);
	cout << opponentScore << " ";
	setTextColor(DEFAULT_COLOR);
	
    if ( gameLoopStatus == 0 ) cout << "GameLoop:Running ";
    else if ( gameLoopStatus == 1 ) cout << "GameLoop:Paused ";
    else cout << "GameLoop:GameOver ";
    
    cout << ( autoPilot ? "Mode:AutoPilot" : "Mode:Manual" ) << " ";
    
    if(colisionStatus == PLAYER_COLIDE) cout << "Player Colide";
    else if(colisionStatus == OPPONENT_COLIDE) cout << "Opponent Colide";
    else if( colisionStatus == DRAW) cout << "DRAW";
}

void header() {
	
    cout << "dual snakes -op computer -fr dual -dl ";
    if( difficultyLevel == EASY ) cout << "easy";
    else if( difficultyLevel == MEDIUM ) cout << "medium";
    else if( difficultyLevel == HARD ) cout << "hard";
    cout << endl;
    
    cout << "RIGHT:D," << (char)RIGHT_ARROW_SYMBOL << "  ";
	cout << "LEFT:A," << (char)LEFT_ARROW_SYMBOL  << "  ";
	cout << "UP:W," << (char)UP_ARROW_SYMBOL << "  ";
	cout << "DOWN:S," << (char)DOWN_ARROW_SYMBOL  << "  ";
	cout << "PAUSE:OppositeDirection  RESUME:NonOppositeDirection  ";
    cout << "AUTOPILOT:E  MANUAL:Q  EXIT:O" << endl << endl;
}

void menu(){
	
    system( "cls" );
    cout << "********************** WELCOME TO DUAL SNAKE GAME ***********************" << endl;
    
    cout << endl << "#Controls {" << endl << endl;
    cout << "       RIGHT: D, " << (char)RIGHT_ARROW_SYMBOL << endl;
	cout << "        LEFT: A, " << (char)LEFT_ARROW_SYMBOL << endl;
	cout << "          UP: W, " << (char)UP_ARROW_SYMBOL  << endl;
	cout << "        DOWN: S, " << (char)DOWN_ARROW_SYMBOL << endl;
	cout << "       PAUSE: Opposite Direction" << endl;
	cout << "      RESUME: Non Opposite Direction" << endl;
    cout << "   AUTOPILOT: E" << endl;
    cout << "      MANUAL: Q" << endl;
    cout << "        EXIT: O" << endl;
    cout << "}" << endl << endl;
    
    cout << "#Object {" << endl << endl;
    
	cout << "     Player Snake: ";
	setTextColor(PLAYER_COLOR);
	cout << (char)BODY_SEGMENT << (char)BODY_SEGMENT << (char)BODY_SEGMENT << (char) HEAD << endl;
	setTextColor(DEFAULT_COLOR);
	
	cout << "   Opponent Snake: ";
	setTextColor(OPPONENT_COLOR);
	cout << (char)BODY_SEGMENT << (char)BODY_SEGMENT << (char)BODY_SEGMENT << (char) HEAD << endl;
	setTextColor(DEFAULT_COLOR);
	
	cout << "      Player Food: ";
	setTextColor(PLAYER_FOOD_COLOR);
	cout << (char)FOOD << endl;
	setTextColor(DEFAULT_COLOR);
	
	cout << "    Opponent Food: ";
	setTextColor(OPPONENT_FOOD_COLOR);
	cout << (char)FOOD << endl << endl << endl;
	setTextColor(DEFAULT_COLOR);
	
	cout << "}";
	
	while(true){
		
		cout << endl << endl << "#AI_Diffuclty_Levels [ " << endl;
		cout << "   1 (Easy)," << endl;
		cout << "   2 (Medium)," << endl;
		cout << "   3 (Hard)" << endl;
		cout << "];" << endl;
		cout << "Choose AI Difficulty Level (ie.1,2,3) :";
		
		string dLevel;
		cin >> dLevel;
		if( dLevel == "1" ) {
			difficultyLevel = EASY;
			break;
		}
		else if( dLevel == "2" ) {
			difficultyLevel = MEDIUM; 
			break;
		}
		else if( dLevel == "3" ){
			difficultyLevel = HARD;
			break;
		}
		else cout << " Wrong Option, Please Choose among [1,2,3]";
	}

    //cout << endl << endl << "Press any key to start game..";
    system( "cls" );
}



