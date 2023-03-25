#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

uint32_t score = 0;
uint32_t tick = 0;

uint32_t height = 30;
uint32_t width = 40;
uint32_t snake_head_x = 20;
uint32_t snake_head_y = 1;

uint32_t food_x = 1;
uint32_t food_y = 1;

uint32_t direction = 1;

uint32_t snake_color = 3;
void init_snake() {}

void clear_snake() {
  move(snake_head_y, snake_head_x);
  printw("  ");
}

void draw_snake() {
  move(snake_head_y, snake_head_x);
  attron(262176 | (snake_color << 8));
  printw("  ");
  attroff(262176 | (snake_color << 8));
}

void draw_score() {
  attron(262176 | (7 << 8));
  move(height - 1, 5);
  printw("Score: %d", score);
  attroff(262176 | (7 << 8));
}

void do_tick() {
  if (++tick > 10) {
    tick = 0;
    clear_snake();
    if (++snake_head_y >= height - 1) {
      snake_head_y = 2;
    }
    draw_snake();
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
  // resizeterm(22, 22);
  attron(262176 | 7 << 8);
  box(stdscr, 0, 0);
  attroff(262176 | 7 << 8);
  height = LINES;
  width = COLS;
  draw_score();
  refresh();
  run_loop();
  endwin();

  return 0;
}

// void backup() {
//   move(0, 5);
//   printw("AAA");
//   move(1, 5);
//   printw("BBB");
//   mvprintw(height - 1, 10, "Score: %d", score);
//   char *c = "welcome to snake game!";
//   mvprintw(LINES / 2, (COLS - strlen(c)) / 2, c);
//   refresh();
//   test show color
//   for (int i = 1; i < 8; i++) {
//     move(i, 10);
//     attron(262176 | (i % 8) << 8);
//     printw(" ");
//     attroff(262176 | (i % 8) << 8);
//   }
// }