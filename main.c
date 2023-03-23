#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

uint32_t tick = 0;

uint32_t snake_head_x = 20;
uint32_t snake_head_y = 1;

uint32_t snake_color = 3;
void init_snake() {

}

void clear_snake() {
  move(snake_head_y, snake_head_x);
  attron(262176 | (0 << 8));
  printw(" ");
  attroff(262176 | (0 << 8));
}

void draw_snake() {
  move(snake_head_y, snake_head_x);
  attron(262176 | (snake_color << 8));
  printw(" ");
  attroff(262176 | (snake_color << 8));
}

void do_tick() {
  if (++tick > 100) {
    tick = 0;
    clear_snake();
    snake_head_y++;
    draw_snake();
    
    char buf[128];
    move(30, 10);
    attron(262176 | (1 << 8));
    sprintf(buf, "snake_head_y=%d", snake_head_y);
    printw(buf);
    attroff(262176 | (1 << 8));

    if (snake_head_y >= 40) {
      snake_head_y = 1;
    }
  }
}

void run_loop() {
  while (1) {
    do_tick();
    usleep(10000);
    char c = getch();
    if (c == 'q') {
      return;
    }
    refresh();
  }
}

int main() {
  initscr();
  start_color();
  // colours indexed by their position in the block
  for (int i = 1; i < 8; i++) {
    init_pair(i, i, 0);
  }
  raw();
  noecho();
  timeout(0);
  curs_set(0);
  attron(262176 | 7 << 8);
  box(stdscr, '#', '#');
  attroff(262176 | 7 << 8);
  char *c = "welcome to snake game!";
  mvprintw(LINES / 2, (COLS - strlen(c)) / 2, c);

  char buf[128];
  move(100, 50);
  attron(262176 | (1 << 8));
  sprintf(buf, "LINES=%d, COLS=%d", LINES, COLS);
  printw(buf);
  printw(" ");
  attroff(262176 | (1 << 8));
  for (int i = 1; i < 8; i++) {
    move(i, 10);
    attron(262176 | (i % 8) << 8);
    printw(" ");
    attroff(262176 | (i % 8) << 8);
  }
  move(20, 10);
  attron(262176 | (4 << 8));
  printw(" ");
  attroff(262176 | (4 << 8));
  refresh();
  run_loop();
  getch();
  endwin();

  return 0;
}

