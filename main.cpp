// Snake game, ncurses version
#include "utils.h"
#include "snake.hpp"
#include <ncurses.h>
#include <unistd.h>
#include <cstdlib> // simple rand() will do
#include <ctime>

#define WIDTH 80
#define HEIGHT 24
#define ITR 500
using namespace std; // test

int main(int argc, char **argv) {
	initscr(); // Init ncurses window
	noecho(); // Don't display keypresses
	curs_set(FALSE); // Don't display cursor
	WINDOW *main_window = newwin(HEIGHT, WIDTH, (LINES-HEIGHT)/2, (COLS-WIDTH)/2);
	nodelay(main_window, true); // Snake never stops
	
	char input = 0;
	Snake s(WIDTH, HEIGHT);
	
	int food_x = rand() % WIDTH, food_y = rand() % HEIGHT;
	srand(time(nullptr));
	
	while (input != 'q' && input != 'Q') {
		// Check if snake's eaten the food piece
		if (s.front->x == food_x && s.front->y == food_y) {
			s.grow();
			food_x = rand() % WIDTH;
			food_y = rand() % HEIGHT;
		}
		
		wclear(main_window);
		SnakeSegment* cursor = s.back;
		
		// Draw snake
		while (cursor) {
			mvwaddch(main_window, cursor->y, cursor->x, '*');
			cursor = cursor->next;
		}
		
		// Draw food
		mvwaddch(main_window, food_y, food_x, 'o');
		
		// Draw border and update window
		wborder(main_window, '|', '|', '-', '-', '+', '+', '+', '+');
		wrefresh(main_window);
		
		usleep(1000*ITR);
		
		// Get input and move snake
		input = wgetch(main_window);
		switch (input) {
			case 'x': case 'X':
				s.grow();		
				break;
			case 'w': case 'W':
				s.move(Direction::up);
				break;
			case 'a': case 'A':
				s.move(Direction::left);
				break;
			case 's': case 'S':
				s.move(Direction::down);
				break;
			case 'd': case 'D':
				s.move(Direction::right);
				break;
			default:
			 	s.move(s.current_dir);
			 	break;
		}
	}
	
	endwin();
	return 0;
}
