#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

uint32_t score = 0;
int32_t tick = 0;

int32_t screen_height = 30;
int32_t screen_width = 40;
int32_t min_y = 0;
int32_t max_y = 0;
int32_t min_x = 0;
int32_t max_x = 0;

int32_t food_x = 1;
int32_t food_y = 1;

#define SNAKE_MAX_LENGTH 256
int32_t snake_body_length = 1;

typedef struct {
  int32_t y;
  int32_t x;
} pos;

uint32_t direction = 0;
// right, down, left, up
const pos step_vals[4] = {{0, 2}, {1, 0}, {0, -2}, {-1, 0}};
pos snake_body_buf[SNAKE_MAX_LENGTH];
int32_t snake_color = 4;
void init_snake() {}

void clear_snake() {
  for (uint32_t i = 0; i < snake_body_length; i++) {
    move(snake_body_buf[i].y, snake_body_buf[i].x);
    printw("  ");
  }
}

void draw_snake(int32_t old_head_y, int32_t old_head_x, int32_t new_head_y,
                int32_t new_head_x) {
  int32_t old_y = old_head_y;
  int32_t old_x = old_head_x;
  int32_t tmp_y;
  int32_t tmp_x;
  // set color, start draw
  attron(262176 | (snake_color << 8));
  snake_body_buf[0].y = new_head_y;
  snake_body_buf[0].x = new_head_x;
  move(snake_body_buf[0].y, snake_body_buf[0].x);
  printw("  ");
  for (uint32_t i = 1; i < snake_body_length; i++) {
    tmp_y = snake_body_buf[i].y;
    tmp_x = snake_body_buf[i].x;
    snake_body_buf[i].y = old_y;
    snake_body_buf[i].x = old_x;
    old_y = tmp_y;
    old_x = tmp_x;
    move(snake_body_buf[i].y, snake_body_buf[i].x);
    printw("%02d", i);
  }

  // stop draw
  attroff(262176 | (snake_color << 8));
}

void draw_food() {
  food_y = (rand() % (screen_height - 2)) + 1;
  food_x = (rand() % ((screen_width - 2) / 2)) * 2 + 1;
  attron(262176 | (1 << 8));
  move(food_y, food_x);
  printw("  ");
  attroff(262176 | (1 << 8));
}

void draw_score() {
  attron(262176 | (7 << 8));
  move(screen_height - 1, 5);
  printw("Score: %d", score);
  attroff(262176 | (7 << 8));
}

int32_t do_tick() {
  int32_t old_head_y;
  int32_t old_head_x;
  int32_t new_head_y;
  int32_t new_head_x;
  if (++tick > 10) {
    tick = 0;
    clear_snake();
    old_head_y = snake_body_buf[0].y;
    old_head_x = snake_body_buf[0].x;
    new_head_y = snake_body_buf[0].y + step_vals[direction].y;
    new_head_x = snake_body_buf[0].x + step_vals[direction].x;
    if (new_head_x < min_x || new_head_x > max_x || new_head_y < min_y ||
        new_head_y > max_y) {
      return 0;
    }
    if (new_head_y == food_y && new_head_x == food_x) {
      snake_body_length++;
      draw_food();
      score++;
      draw_score();
    }
    draw_snake(old_head_y, old_head_x, new_head_y, new_head_x);
  }
  return 1;
}

void draw_game_over_text() {
  char *c = "Game Over!";
  mvprintw(screen_height / 2, (screen_width - strlen(c)) / 2, c);
  refresh();
}

void game_over() {
  draw_game_over_text();
  while (1) {
    usleep(10000);
    char c = getch();
    if (c == 'q') {
      return;
    }
  }
}

void run_loop() {
  char c;
  while (do_tick()) {
    usleep(10000);
    c = getch();
    if (c == 'w' && direction != 1) {
      direction = 3;
    } else if (c == 'a' && direction != 0) {
      direction = 2;
    } else if (c == 's' && direction != 3) {
      direction = 1;
    } else if (c == 'd' && direction != 2) {
      direction = 0;
    } else if (c == 'q') {
      return;
    }
    refresh();
  }
  if (c != 'q') {
    game_over();
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
  screen_height = LINES;
  screen_width = COLS & ~0x01;
  resizeterm(screen_height, screen_width);
  attron(262176 | 7 << 8);
  box(stdscr, 0, 0);
  attroff(262176 | 7 << 8);
  min_y = 1;
  max_y = screen_height - 2;
  min_x = 1;
  max_x = screen_width - 3;
  snake_body_buf[0].y = screen_height / 2;
  snake_body_buf[0].x = screen_width / 2 - 1;
  draw_score();
  draw_food();
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
//   mvprintw(screen_height - 1, 10, "Score: %d", score);
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