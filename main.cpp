#include "game.h"
#include <time.h>
using namespace std;

int main() {
	
	maximizeWindow();
	srand(time(0));
	
	while(1){
		menu();
		game();
	}
    
    return 0;
}
