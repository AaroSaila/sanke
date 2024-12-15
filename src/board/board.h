#ifndef BOARD_H_
#define BOARD_H_

#include "../globals.h"

extern boardInfo brdInfo;

void setBoardBorders(char board[][brdInfo.x]);
void printBoard(char board[][brdInfo.x]);

#endif
