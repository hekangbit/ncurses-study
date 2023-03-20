#include <string.h>
#include <ncurses.h>


int main() {
  initscr();
  start_color();
  // colours indexed by their position in the block
  for (int i = 1; i < 8; i++) {
    init_pair(i, i, 0);
  }
  raw();
  noecho();
  curs_set(0);
  char *c = "hello world!";
  mvprintw(LINES / 2, (COLS - strlen(c)) / 2, c);
  move(20, 10);
  attron(262176 | 1 << 8);
  printw("  ");

  for (int i = 0; i < 10; i++) {
    move (1 + i, 50);
    attron(262176 | ((i % 8) + 1) << 8);
    printw("qqq");
  }
  // box(stdscr, 's', 'k');
  refresh();
  int cnt = 5;
  while (cnt--) {
    beep();
    sleep(1);
  }
  getch();
  endwin();

  return 0;
}

