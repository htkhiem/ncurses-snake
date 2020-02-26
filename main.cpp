// Snake game, ncurses version
#include "snake.hpp"
#include "utils.h"
#include <cstdio>
#include <cstdlib> // simple rand() will do
#include <cstring>
#include <ctime>
#include <ncurses.h>
#include <string>
#include <unistd.h>

#define WIDTH 80
#define HEIGHT 24
#define ITR 250

#define BORDER_COLOUR 1
#define SNAKE_COLOUR 2
using namespace std; // test

void DrawBorder(size_t width, size_t height, int score) {
  // Draw border
  move(0, 0);
  attron(COLOR_PAIR(BORDER_COLOUR));
  hline(ACS_BLOCK, WIDTH);
  vline(ACS_BLOCK, HEIGHT + 4);
  move(0, WIDTH - 1);
  vline(ACS_BLOCK, HEIGHT + 4);
  move(HEIGHT + 4, 0);
  hline(ACS_BLOCK, WIDTH);
  attroff(COLOR_PAIR(BORDER_COLOUR));
  // Draw title bar
  mvprintw(1, 1, "Score: %d", score);

  // Draw horizontal separator
  move(2,1);
  hline(ACS_HLINE, width - 2);
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
  nodelay(stdscr, true); // Snake never stops

  start_color();
  init_pair(BORDER_COLOUR, COLOR_WHITE, COLOR_WHITE);
  init_pair(SNAKE_COLOUR, COLOR_BLACK, COLOR_RED);

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
        clear();

        // Get input and move snake
        input = getch();
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

        attron(COLOR_PAIR(SNAKE_COLOUR)); // Snake is red-black
        // Draw snake body
        while (cursor->next) {
          mvaddch(cursor->y + 2, cursor->x, '*');
          cursor = cursor->next;
        }
        // Head drawn last
        mvaddch(cursor->y + 2, cursor->x, head_shape);
        attroff(COLOR_PAIR(SNAKE_COLOUR));

        // Draw food
        mvaddch(food_y + 2, food_x, ACS_DIAMOND);

        // Draw UI
        DrawBorder(WIDTH, HEIGHT, score);

        // Refresh
        refresh();
        usleep(1000 * ITR);
      }
    }
  } catch (exception &e) {
    clear();
    string game_over = "GAME OVER", why = e.what(),
           instr = "Press any key to exit...";
    DrawBorder(WIDTH, HEIGHT, score);
    mvprintw(floor(HEIGHT / 2) - 2, floor((WIDTH - game_over.length()) / 2),
             game_over.c_str());
    mvprintw(floor(HEIGHT / 2), floor((WIDTH - strlen(e.what())) / 2),
             e.what());
    mvprintw(floor(HEIGHT / 2) + 2, floor((WIDTH - instr.length()) / 2),
             instr.c_str());
    refresh();
    char wait;
    cin >> wait;
    endwin();
    return 0;
  }
  getch();
  endwin();
  return 0;
}
