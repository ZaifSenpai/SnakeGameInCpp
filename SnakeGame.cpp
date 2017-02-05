#include <iostream>
#include <conio.h> // kbhit(), getch()
#include <stdlib.h> //srand(), rand()
#include <map> // map DS
#include <time.h> // time()
#include <windows.h> // Sleep()
#include "Snake.cpp" // Snake queue class

using namespace std;

/****************************
*                           *
*     GLOBAL VARIABLES      *
*                           *
*****************************/

// 2D map
map<int, map<int, char> > frame;

// Speed of snake or Frames per second, recommended: 3
float speed = 3.0;

// Loop control variables
int i, j;

// map dimensions
int width = 50, height = 20;

// Snake queue object
Snake snake;

// Snake head dimension
int headxpos, headypos;

// Snake body part dimension
int bodyxpos, bodyypos;

// Fruit dimension
int fruitx, fruity;

// Snake body and fruit
char head = '\351';
char body_part = 'o';
char fruit = '\242';


// Score
int score;

// boolian variable to tell if game is over or not
bool game_over;

// Directions
bool direction; // 0 means vertical, 1 means horizontal
bool whichward; // In vertical: 0 means right 1 means left.
				// In horizontal: 0 means up and 1 means down

/****************************
*                           *
*   FUNCTION DECLARATIONS   *
*                           *
*****************************/

void initialize();
void add_body();
void update();
void generate_fruit();
void change_direction(char dir);
void move_head();
void print_map();

/****************************
*                           *
*      main Function        *
*                           *
*****************************/

int main() {
	
	initialize();
	generate_fruit();
	
	// Game loop
	do {
		update();
		if (_kbhit()) {
			change_direction(_getch());
		}
	} while(game_over == false);

	cout << "Game Over.\n\n";

	system("pause");
	return 0;
}


/****************************
*                           *
*    FUNCTION DEFINITION    *
*                           *
*****************************/

void initialize() {

	score = 0;
	
	// outerbox
	for(i = 0; i < width; i++)
		frame[0][i] = '\262';
	for(i = 1; i < height-1; i++) {
		frame[i][0] = '\262';
		for(j = 1; j < width-1; j++)
			frame[i][j] = ' ';
		frame[i][j] = '\262';
	}
	for(i = 0; i < width; i++)
		frame[height-1][i] = '\262';
	
	// innerbox
	frame[1][1] = '\311';
	for(i = 2; i < width-2; i++)
		frame[1][i] = frame[height-2][i] = '\315';
	frame[1][i] = '\273';
	for(i = 2; i < height-2; i++)
		frame[i][1] = frame[i][width-2] = '\272';
	frame[i][1] = '\310';
	frame[i][width-2] = '\274';

	
	// Placing head at random position in start
	srand(time(NULL));
	
	headxpos = (rand() % (width - 4)) + 4;
	headypos = (rand() % (height - 5)) + 2;
	
	frame[headypos][headxpos] = head;
	
	snake.enqueue(headxpos-2, headypos);
	frame[headypos][headxpos-2] = body_part;
	snake.enqueue(headxpos-1, headypos);
	frame[headypos][headxpos-1] = body_part;

	print_map();

	// initializing snake direction to vertical and right
	direction = 0;
	whichward = 0;

	game_over = false;

	Sleep(1000/speed);
}

void add_body() { // Add a new body part (this is called when a fruit is eaten)
	move_head();
	
	print_map();

	Sleep(1000/speed);
}

void update() { // Show next frame
	move_head();

	snake.dequeue(bodyxpos, bodyypos);
	frame[bodyypos][bodyxpos] = ' ';
	
	print_map();
	
	Sleep(1000/speed);
}

void generate_fruit() {
	fruitx = (rand() % (width - 7)) + 3;
	fruity = (rand() % (height - 6)) + 3;
	if (frame[fruity][fruitx] == ' ')  // fruit will not be generated
		frame[fruity][fruitx] = fruit; // where snake body is
	else generate_fruit();
}

void change_direction(char dir) {
	if ((dir == 'w' || dir == 'W' || dir == 's' || dir == 'S') && direction == 0) { // Vertical
		if (dir == 'w' || dir == 'W')
				whichward = 0;
		else
			whichward = 1;

		direction = 1;
	}
	else if ((dir == 'a' || dir == 'A' || dir == 'd' || dir == 'D') && direction == 1) { // Horizontal
		if (dir == 'a' || dir == 'A')
			whichward = 1;
		else
			whichward = 0;

		direction = 0;
	}
}

void move_head() {   // Move head (according to its current direction) to its next node
				    //  also add score if head reaches fruit or game over when it reaches wall
	snake.enqueue(headxpos, headypos);
	frame[headypos][headxpos] = body_part;
	
	if (direction == 0 && whichward == 0)
		headxpos++;
	else if (direction == 0 && whichward == 1)
		headxpos--;
	else if (direction == 1 && whichward == 0)
		headypos--;
	else if (direction == 1 && whichward == 1)
		headypos++;

	// if head reaches the fruit
	if (headxpos == fruitx && headypos == fruity) {
		score++;
		add_body();
		generate_fruit();
	}

	// gameover if head touches the wall or body
	if (frame[headypos][headxpos] == '\315'
		|| frame[headypos][headxpos] == '\272'
		|| frame[headypos][headxpos] == body_part)
		game_over = true;
	
	frame[headypos][headxpos] = head;
}

void print_map() { // print the frame
	system("cls");
	for(i = 0; i < height; i++) {
		for(j = 0; j < width; j++) {
				cout << frame[i][j];
			}
		if (i == 3)
			cout << "    Use WASD keys";
		if (i == 4)
			cout << "         W";
 		if (i == 5)
			cout << "       A + D";
		if (i == 6)
			cout << "         S";
		if (i == 10)
			cout << "    Score: " << score;
		cout << endl;
	}
}
