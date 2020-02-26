// Snake game, ncurses version
#include "snake.hpp"
#include "utils.h"
#include <cstdlib> // simple rand() will do
#include <cstring>
#include <ctime>
#include <ncurses.h>
#include <string>
#include <unistd.h>
#include <cstdio>

#define WIDTH 80
#define HEIGHT 24
#define ITR 250
using namespace std; // test

void DrawBorder(WINDOW *main_window, size_t width, size_t height, int score) {
  // Draw border
  wborder(main_window, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER,
          ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);

  // Draw title bar
  mvwprintw(main_window, 1, 1, "Score: ");
  wprintw(main_window, "%d", score);

  // Draw horizontal separator
  mvwaddch(main_window, 2, 0, ACS_LTEE);
  whline(main_window, ACS_HLINE, width - 1);
  mvwaddch(main_window, 2, width - 1, ACS_RTEE);
}

int main(int argc, char **argv) {
  initscr();       // Init ncurses window
  noecho();        // Don't display keypresses
  curs_set(false); // Don't display cursor

  if (!has_colors()) {
    endwin();
    printf("Your terminal does not have colour support. Exiting...");
    return 1;
  }
  
  // Window height = playing field height + 2 to accomodate title bar
  WINDOW *main_window =
      newwin(HEIGHT + 2, WIDTH, (LINES - HEIGHT) / 2, (COLS - WIDTH) / 2);
  nodelay(main_window, true); // Snake never stops

  start_color();
  init_pair(1, COLOR_BLACK, COLOR_RED);
  
  char input = 0;
  Snake s(WIDTH, HEIGHT);
  int score = 0; // Starts at snake length = 2

  srand(time(nullptr));
  int food_x = rand() % (WIDTH - 3) + 1, food_y = rand() % (HEIGHT - 3) + 1;

  try {
    char head_shape = ACS_RARROW;
    while (input != 'q' && input != 'Q') {
      // Check if snake's eaten the food piece
      if (s.front->x == food_x && s.front->y == food_y) {
        s.grow();
        score++;
        food_x = rand() % (WIDTH - 3) + 1;
        food_y = rand() % (HEIGHT - 3) + 1;
      } else {
        wclear(main_window);

        // Get input and move snake
        input = wgetch(main_window);
        switch (input) {
        case 'w':
        case 'W':
          if (s.current_dir != Direction::down) {
            s.move(Direction::up);
            head_shape = ACS_UARROW;
          } else
            s.move(Direction::down);
          break;
        case 'a':
        case 'A':
          if (s.current_dir != Direction::right) {
            s.move(Direction::left);
            head_shape = ACS_LARROW;
          } else
            s.move(Direction::right);
          break;
        case 's':
        case 'S':
          if (s.current_dir != Direction::up) {
            s.move(Direction::down);
            head_shape = ACS_DARROW;
          } else
            s.move(Direction::up);
          break;
        case 'd':
        case 'D':
          if (s.current_dir != Direction::left) {
            s.move(Direction::right);
            head_shape = ACS_RARROW;
          } else
            s.move(Direction::left);
          break;
        default:
          s.move(s.current_dir);
          break;
        }

        SnakeSegment *cursor = s.back;

	attron(COLOR_PAIR(1)); // Snake is red-black
        // Draw snake body
        while (cursor->next) {
          mvwaddch(main_window, cursor->y + 2, cursor->x, '*');
          cursor = cursor->next;
        }
        // Head drawn last
        mvwaddch(main_window, cursor->y + 2, cursor->x, head_shape);
	attroff(COLOR_PAIR(1));
	
        // Draw food
        mvwaddch(main_window, food_y + 2, food_x, ACS_DIAMOND);

        // Draw UI
        DrawBorder(main_window, WIDTH, HEIGHT, score);

        // Refresh
        wrefresh(main_window);
        usleep(1000 * ITR);
      }
    }
  } catch (exception &e) {
    wclear(main_window);
    string game_over = "GAME OVER", why = e.what(),
           instr = "Press any key to exit...";
    DrawBorder(main_window, WIDTH, HEIGHT, score);
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
