#include "start.h"

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int selected = 0;
int playAgainState = 1;

int main() {
  int ch;
  initscr();
  keypad(stdscr, TRUE);
  noecho();
  curs_set(FALSE);

  timeout(0);

  do {
    drawStart();
    ch = getch();
    playGame(ch);
  } while (!selected);
  clear();
  refresh();

  pid_t pid = fork();
  if (pid < 0)
    error_handling("fork");
  else if (pid == 0) {
    char *args[] = {"./play.o", NULL};
    execv(args[0], args);
    error_handling("exec");
  } else {
    wait(NULL);
    refresh();
  }

  clear();
  mvprintw(6, 6, "bye bye");
  refresh();
  sleep(2);
  clear();
  refresh();
  endwin();
}

void drawStart() {
  mvprintw(2, 2, "___ o ___");
  mvprintw(3, 2, " |  | |  ");
  mvprintw(4, 2, " |  | |__");
  mvprintw(5, 2, "___  ^  ___");
  mvprintw(6, 2, " |  /-\\ |  ");
  mvprintw(7, 2, " |  | |  |__");
  mvprintw(8, 2, "___ --- ___");
  mvprintw(9, 2, " |  | | |--");
  mvprintw(10, 2, " |  --- |__");

  standout();
  mvprintw(12, 4, "Press space to start >>>");
  standend();

  refresh();
}

void playGame(int ch) {
  switch (ch) {
    case ' ':
      selected = 1;
      break;
  }
}

void error_handling(char *msg) {
  perror(msg);
  exit(1);
}
