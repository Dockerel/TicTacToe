
#include "play.h"

#include <ctype.h>
#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

typedef struct {
  int x;
  int y;
} Player;

typedef struct {
  int x;
  int y;
  char mark;
  int selected;
} Loc;

Player player;
int oTurn = 1, xTurn = 0;
int stop = 0;
int ch;
char whoWins = ' ';

char board[3][3];

int main() {
  initscr();
  keypad(stdscr, TRUE);
  noecho();
  curs_set(FALSE);

  timeout(0);
  init();
  drawBoard();

  do {
    ch = getch();
    moveCursor(ch);
    drawAll();
    checkWin();
    refresh();
  } while (!stop);

  clear();
  if (whoWins == 'd')
    mvprintw(3, 3, "Draw");
  else
    mvprintw(3, 3, "%c wins!", toupper(whoWins));
  refresh();
  sleep(2);

  endwin();
}

void init() {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      board[i][j] = ' ';
    }
  }

  player.x = player.y = 1;
}

void drawBoard() {
  for (int i = 0; i < WIDTH; i++) {
    mvprintw(0, i, "-");
  }
  for (int i = 1; i < HEIGHT; i++) {
    mvprintw(i, 0, "|");
    mvprintw(i, 4, "|");
    mvprintw(i, 8, "|");
    mvprintw(i, WIDTH - 1, "|");
    if (i == 4 || i == 8) {
      for (int j = 0; j < WIDTH; j++) {
        if (j != 0 || j != 4 || j != 8 || j != WIDTH - 1) mvprintw(i, j, "-");
      }
    }
  }
  for (int i = 0; i < WIDTH; i++) {
    mvprintw(HEIGHT - 1, i, "-");
  }
}

void selectLoc() {
  if (board[player.y][player.x] == ' ') {
    if (oTurn)
      board[player.y][player.x] = 'o';
    else if (xTurn)
      board[player.y][player.x] = 'x';
    player.x = player.y = 1;
    changeTurn();
  }
}

void moveCursor(int ch) {
  switch (ch) {
    case KEY_UP:
      if (player.y > 0) {
        player.y -= 1;
      }
      break;
    case KEY_DOWN:
      if (player.y < 2) {
        player.y += 1;
      }
      break;
    case KEY_RIGHT:
      if (player.x < 2) {
        player.x += 1;
      }
      break;
    case KEY_LEFT:
      if (player.x > 0) {
        player.x -= 1;
      }
      break;
    case ' ':
      selectLoc();
      break;
  }
}

void drawAll() {
  // 1. 현재 커서 위치
  // 2. x와 o
  int realX, realY;
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      realY = 2 + 4 * i;
      realX = 2 + 4 * j;
      if (player.y == i && player.x == j) {
        standout();
      }
      if (board[i][j] == ' ') {
        for (int l = -1; l <= 1; l++) mvprintw(realY + l, realX - 1, "   ");
      } else if (board[i][j] == 'o') {
        mvprintw(realY - 1, realX - 1, "/-\\");
        mvprintw(realY, realX - 1, "| |");
        mvprintw(realY + 1, realX - 1, "\\-/");
      } else if (board[i][j] == 'x') {
        mvprintw(realY - 1, realX - 1, "\\ /");
        mvprintw(realY, realX - 1, " X ");
        mvprintw(realY + 1, realX - 1, "/ \\");
      }
      standend();
    }
  }
}

void checkWin() {
  for (int i = 0; i < 3; i++) {
    // 가로 확인
    if (board[i][0] == board[i][1] && board[i][1] == board[i][2] &&
        board[i][0] != ' ') {
      whoWins = board[i][0];
      goto Fin;
    }
    // 세로 확인
    if (board[0][i] == board[1][i] && board[1][i] == board[2][i] &&
        board[0][i] != ' ') {
      whoWins = board[0][i];
      goto Fin;
    }
  }
  // 대각선 확인
  if (board[0][0] == board[1][1] && board[1][1] == board[2][2] &&
      board[0][0] != ' ') {
    whoWins = board[0][0];
    goto Fin;
  } else if (board[0][2] == board[1][1] && board[1][1] == board[2][0] &&
             board[0][2] != ' ') {
    whoWins = board[0][2];
    goto Fin;
  }
  // 무승부 확인
  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++) {
      if (board[i][j] == ' ') goto Fin;
    }
  whoWins = 'd';

Fin:
  if (whoWins != ' ') {
    stop = 1;
  }
}

void changeTurn() {
  int tmp;
  tmp = oTurn;
  oTurn = xTurn;
  xTurn = tmp;
}

void error_handling(char *msg) {
  perror(msg);
  exit(1);
}
