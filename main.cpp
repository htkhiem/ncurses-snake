// Snake game, ncurses version
#include "snake.hpp"
#include "utils.h"
#define WIDTH 80
#define HEIGHT 24
#define ITR 250

int main(int argc, char **argv) {
  initscr();       // Init ncurses window
  noecho();        // Don't display keypresses
  curs_set(false); // Don't display cursor
  cbreak();

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
    while (input != 'q' && input != 'Q') {
      // Check if snake's eaten the food piece
      if (s.front->x == food_x && s.front->y == food_y) {
        s.grow();
        score++;
        food_x = rand() % (WIDTH - 3) + 1;
        food_y = rand() % (HEIGHT - 3) + 1;
      } else {
        // Get input and move snake
        input = getch();
        switch (input) {
        case 'w':
        case 'W':
          if (s.current_dir != Direction::down) {
            s.move(Direction::up);
            usleep(250 * ITR);
          } else {
            s.move(Direction::down);
            usleep(1000 * ITR);
          }
          break;
        case 'a':
        case 'A':
          if (s.current_dir != Direction::right) {
            s.move(Direction::left);
            usleep(250 * ITR);
          } else {
            s.move(Direction::right);
            usleep(1000 * ITR);
          }
          break;
        case 's':
        case 'S':
          if (s.current_dir != Direction::up) {
            s.move(Direction::down);
            usleep(250 * ITR);
          } else {
            s.move(Direction::up);
            usleep(1000 * ITR);
          }
          break;
        case 'd':
        case 'D':
          if (s.current_dir != Direction::left) {
            s.move(Direction::right);
            usleep(250 * ITR);
          } else {
            s.move(Direction::left);
            usleep(1000 * ITR);
          }
          break;
        default:
          s.move(s.current_dir);
	  usleep(1000 * ITR);
          break;
        }

        SnakeSegment *cursor = s.back;
	clear();
	clear();
        attron(COLOR_PAIR(SNAKE_COLOUR)); // Snake is red-black
        // Draw snake body
        while (cursor->next) {
          switch (cursor->dir) {
          case Direction::up:
          case Direction::down:
            mvaddch(cursor->y + 2, cursor->x, ACS_VLINE);
            break;
          case Direction::left:
          case Direction::right:
            mvaddch(cursor->y + 2, cursor->x, ACS_HLINE);
            break;
          case Direction::upleft:
          case Direction::rightdown:
            mvaddch(cursor->y + 2, cursor->x, ACS_URCORNER);
            break;
          case Direction::upright:
          case Direction::leftdown:
            mvaddch(cursor->y + 2, cursor->x, ACS_ULCORNER);
            break;
          case Direction::downleft:
          case Direction::rightup:
            mvaddch(cursor->y + 2, cursor->x, ACS_LRCORNER);
            break;
          case Direction::downright:
          case Direction::leftup:
            mvaddch(cursor->y + 2, cursor->x, ACS_LLCORNER);
            break;
          }
          cursor = cursor->next;
        }
        // Head drawn last
        switch (cursor->dir) {
        case Direction::left:
          mvaddch(cursor->y + 2, cursor->x, ACS_LARROW);
          break;
        case Direction::up:
          mvaddch(cursor->y + 2, cursor->x, ACS_UARROW);
          break;
        case Direction::right:
          mvaddch(cursor->y + 2, cursor->x, ACS_RARROW);
          break;
        case Direction::down:
          mvaddch(cursor->y + 2, cursor->x, ACS_DARROW);
          break;
        default:
          mvaddch(cursor->y + 2, cursor->x, ACS_DARROW);
        }

        attroff(COLOR_PAIR(SNAKE_COLOUR));

        // Draw food
        mvaddch(food_y + 2, food_x, ACS_DIAMOND);

        // Draw UI
        DrawBorder(WIDTH, HEIGHT, score);

        // Refresh
        refresh();
      }
    }
  } catch (std::string e) {
    clear();
    string game_over = "GAME OVER", instr = "Press any key to exit...";
    DrawBorder(WIDTH, HEIGHT, score);
    mvprintw(floor(HEIGHT / 2) - 2, floor((WIDTH - game_over.length()) / 2),
             game_over.c_str());
    mvprintw(floor(HEIGHT / 2), floor((WIDTH - e.length()) / 2), e.c_str());
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
