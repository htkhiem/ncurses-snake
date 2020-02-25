// Snake game, ncurses version
#include "snake.hpp"
#include "utils.h"
#include <cstdlib> // simple rand() will do
#include <cstring>
#include <ctime>
#include <ncurses.h>
#include <string>
#include <unistd.h>

#define WIDTH 80
#define HEIGHT 24
#define ITR 500
using namespace std; // test

int main(int argc, char **argv) {
  initscr();       // Init ncurses window
  noecho();        // Don't display keypresses
  curs_set(FALSE); // Don't display cursor

  // Window height = playing field height + 2 to accomodate title bar
  WINDOW *main_window =
      newwin(HEIGHT + 2, WIDTH, (LINES - HEIGHT) / 2, (COLS - WIDTH) / 2);
  nodelay(main_window, true); // Snake never stops

  char input = 0;
  Snake s(WIDTH, HEIGHT);
  int score = 0; // Starts at snake length = 2

  srand(time(nullptr));
  int food_x = rand() % WIDTH, food_y = rand() % HEIGHT;

  try {
    while (input != 'q' && input != 'Q') {
      // Check if snake's eaten the food piece
      if (s.front->x == food_x && s.front->y == food_y) {
        s.grow();
        score++;
        food_x = rand() % WIDTH;
        food_y = rand() % HEIGHT;
      } else {
        wclear(main_window);
        SnakeSegment *cursor = s.back;

        // Draw snake
        while (cursor->next) {
          mvwaddch(main_window, cursor->y + 2, cursor->x, '*');
          cursor = cursor->next;
        }
        // Snake head
        switch (s.current_dir) {
        case (Direction::up):
          mvwaddch(main_window, cursor->y + 2, cursor->x, '^');
          break;
        case (Direction::left):
          mvwaddch(main_window, cursor->y + 2, cursor->x, '<');
          break;
        case (Direction::down):
          mvwaddch(main_window, cursor->y + 2, cursor->x, 'V');
          break;
        case (Direction::right):
          mvwaddch(main_window, cursor->y + 2, cursor->x, '>');
          break;
        }

        // Draw food
        mvwaddch(main_window, food_y + 2, food_x, 'o');

        // Draw border and update window
        wborder(main_window, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE,
                ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);

        // Draw title bar
        mvwprintw(main_window, 1, 1, "Score: ");
        wprintw(main_window, "%d", score);
        wmove(main_window, 2, 0);
	waddch(main_window, ACS_LTEE);
        whline(main_window, ACS_HLINE, WIDTH - 1);
	wmove(main_window, 2, WIDTH - 1);
	waddch(main_window, ACS_RTEE);
        wrefresh(main_window);

        usleep(1000 * ITR);

        // Get input and move snake
        input = wgetch(main_window);
        switch (input) {
        case 'x':
        case 'X':
          s.grow();
          score++;
          break;
        case 'w':
        case 'W':
          s.move(Direction::up);
          break;
        case 'a':
        case 'A':
          s.move(Direction::left);
          break;
        case 's':
        case 'S':
          s.move(Direction::down);
          break;
        case 'd':
        case 'D':
          s.move(Direction::right);
          break;
        default:
          s.move(s.current_dir);
          break;
        }
      }
    }
  } catch (exception &e) {
    wclear(main_window);
    string game_over = "GAME OVER", why = e.what(),
           instr = "Press any key to exit...";
    wborder(main_window, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE,
            ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
    mvwprintw(main_window, floor(HEIGHT / 2) - 2,
              floor((WIDTH - game_over.length()) / 2), game_over.c_str());
    mvwprintw(main_window, floor(HEIGHT / 2),
              floor((WIDTH - strlen(e.what())) / 2), e.what());
    mvwprintw(main_window, floor(HEIGHT / 2) + 2,
              floor((WIDTH - instr.length()) / 2), instr.c_str());
    wrefresh(main_window);
    char wait;
    cin >> wait;
    endwin();
    return 0;
  }
  wgetch(main_window);
  endwin();
  return 0;
}
