#include "utils.h"

void DrawBorder(size_t width, size_t height, int score) {
  // Draw border
  move(0, 0);
  attron(COLOR_PAIR(BORDER_COLOUR));
  hline(ACS_BLOCK, width);
  vline(ACS_BLOCK, height + 4);
  move(0, width - 1);
  vline(ACS_BLOCK, height + 4);
  move(height + 4, 0);
  hline(ACS_BLOCK, width);
  attroff(COLOR_PAIR(BORDER_COLOUR));
  // Draw title bar
  mvprintw(1, 1, "Score: %d", score);

  // Draw horizontal separator
  move(2, 1);
  hline(ACS_HLINE, width - 2);
}
