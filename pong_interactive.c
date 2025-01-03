#include <stdio.h>

#include "ncurses.h"
#include "unistd.h"
void field();
void show_scores();
void game();
int main() {
    game();
    return 0;
}
void field(int x1, int y1, int z1, int x2, int y2, int z2, int length, int height) {
    for (int i = 0; i < 25; i++) {
        for (int j = 0; j < 80; j++) {
            if (j == 0 || j == 79) {
                printw("|");
            } else if (i == 0 || i == 24) {
                printw("-");
            } else {
                if (j == 1) {
                    if (x1 == i || y1 == i || z1 == i) {
                        printw("]");
                    } else {
                        printw(" ");
                    }
                } else if (j == 78) {
                    if (x2 == i || y2 == i || z2 == i) {
                        printw("[");
                    } else {
                        printw(" ");
                    }
                } else if (i == height && j == length) {
                    printw("o");
                } else {
                    printw(" ");
                }
            }
        }
        printw("\n");
    }
}

void show_scores(int score1, int score2) {
    for (int i = 0; i < 39; i++) {
        printw(" ");
    }
    printw("%d / %d", score1, score2);
    printw("\n For play: left player(a/z), right player(m/k)\n ");
    printw("For quit: q\n ");
}
void game() {
    int x1, y1, z1, x2, y2, z2;  //координаты ракеток, стартовое положение
    int length, height;          //координаты шарика
    int score1, score2;          //счетчик очков
    score1 = score2 = 0;
    x1 = x2 = 5;
    y1 = y2 = 6;
    z1 = z2 = 7;
    length = 40;
    height = 13;
    int polet = 1;
    int key = 1;
    field(x1, y1, z1, x2, y2, z2, length, height);  //отрисовка старта
    show_scores(score1, score2);
    initscr();
    noecho();
    while (key != 0) {
        nodelay(stdscr, TRUE);
        curs_set(0);
        clear();
        usleep(100000);
        printw("\n");
        char perv;
        perv = getch();  //считывание двух символов слитно, одновременное управление
        switch (perv) {  //управление ракетками
            case 'a':
                if (x1 != 1) {
                    x1 -= 1;
                    y1 -= 1;
                    z1 -= 1;
                }
                break;
            case 'z':
                if (z1 != 23) {
                    x1 += 1;
                    y1 += 1;
                    z1 += 1;
                }
                break;
            case 'k':
                if (x2 != 1) {
                    x2 -= 1;
                    y2 -= 1;
                    z2 -= 1;
                }
                break;
            case 'm':
                if (z2 != 23) {
                    x2 += 1;
                    y2 += 1;
                    z2 += 1;
                }
                break;
        }
        if (length == 3 && (height == x1 || height == y1 || height == z1 || height == x1 - 1 ||
                            height == z1 + 1)) {  //отскок от ракетки первого игрока
            if (polet == 1) {
                polet = 2;
            } else {
                polet = 3;
            }
        }
        if (length == 76 && (height == x2 || height == y2 || height == z2 || height == x2 - 1 ||
                             height == z2 + 1)) {  //отскок от ракетки второго игрока
            if (polet == 2) {
                polet = 1;
            } else {
                polet = 4;
            }
        }
        if (height == 1) {  //отскоки от краев
            if (polet == 1)
                polet = 4;
            else
                polet = 3;
        }
        if (height == 23) {
            if (polet == 3)
                polet = 2;
            else
                polet = 1;
        }
        if (length == 2) {  //отскоки от краев с подсчетом результата
            score2 += 1;
            if (polet == 1)
                polet = 2;
            else
                polet = 3;
        }
        if (length == 77) {
            score1 += 1;
            if (polet == 2)
                polet = 1;
            else
                polet = 4;
        }
        if (polet == 1) {  //варианты полета
            length -= 1;
            height -= 1;
        }
        if (polet == 2) {
            length += 1;
            height -= 1;
        }
        if (polet == 3) {
            length += 1;
            height += 1;
        }
        if (polet == 4) {
            length -= 1;
            height += 1;
        }
        field(x1, y1, z1, x2, y2, z2, length, height);
        show_scores(score1, score2);
        refresh();
        flushinp();
        if (score1 == 21 || score2 == 21||perv=='q') {  //проверка условий победы
            key = 0;
            endwin();
            printw("\033[0d\033[2J");
            if (score1 == 21) {
                printf("\n Congratulations for PLAYER 1\n");
            } else if (score2==21){
                printf("\n Congratulations for PLAYER 2\n");
            }
            else{
                printf("Game was stopped");
            }
        }
    }
}
