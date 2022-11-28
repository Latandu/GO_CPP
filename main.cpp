#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include"conio2.h"
/* Comment: in the final program declare appropriate constants, e.g.,
   to eliminate from your program numerical values by replacing them
   with well defined identifiers */
void CreateBoard(int xSize, int ySize, int xCord, int yCord){
    for(int i = xCord;  i < xSize+xCord+2; i++){
        for(int j = yCord; j < ySize+yCord+2; j++) {
            gotoxy(i, j);
            if (j == yCord || j == ySize + yCord + 1) cputs("*");
            else if (i == xCord || i == xSize + xCord + 1) cputs("|");
        }
        cputs("\n");
    }
}
int main() {
	int zn = 0, x = 12, y = 7, attr = 7, back = 0, zero = 0, xCord = 10, yCord = 5, boardXSize = 13, boardYSize = 13;
	char txt[32];
	// if the program is compiled in pure C, then we need to initialize
	// the library ourselves; __cplusplus is defined only if a C++ compiler
	// is used
#ifndef __cplusplus
	Conio2_Init();
#endif
	// settitle sets the window title
	settitle("Jakub Andrunik, 193166");
	// hide the blinking cursor
	_setcursortype(_NOCURSOR);
	do {
		// we set black to be the background color
		// check conio2.h for available colors
		textbackground(BLACK);
		// clear the screen: we fill it out with spaces with
		// the specified background color
		clrscr();
		// we set the text color (7 == LIGHTGRAY)
		textcolor(7);
		// we move the coursor to column 48 and row 1
		// rows and column are numbered starting with 1

        gotoxy(48, 1);
		// we print out a text at a given cursor position
		// the cursor will move by the length of the text
		cputs("q       = exit");
		gotoxy(48, 2);
		cputs("cursors = moving");
		gotoxy(48, 3);
		cputs("space   = change color");
		gotoxy(48, 4);
		cputs("enter   = change background color");
		// print out the code of the last key pressed
        gotoxy(48, 5);
		if(zero) sprintf(txt, "key code: 0x00 0x%02x", zn);
		else sprintf(txt, "key code: 0x%02x", zn);
        cputs(txt);
        gotoxy(48, 6);
        sprintf(txt, "x coordinates: %02d", x);
        cputs(txt);
        gotoxy(48, 7);
        sprintf(txt, "y coordinates: %02d", y);
        cputs(txt);
		// we draw a star
		gotoxy(x, y);
		textcolor(attr);
		textbackground(back);
		// putch prints one character and moves the cursor to the right
		putch('*');

        gotoxy(xCord, yCord);
        CreateBoard(boardXSize, boardYSize, xCord, yCord);
        if(x <= xCord){
            x++;
            continue;
        } else if (x > xCord+ boardXSize){
            x--;
            continue;
        } else if (y <= yCord){
            y++;
            continue;
        } else if(y > yCord + boardYSize){
            y--;
            continue;
        }
		zero = 0;
		zn = getch();
		// we do not want the key 'H' to play role of "up arrow"
		// so we check if the first code is zero
		if(zn == 0) {
            zero = 1;        // if this is the case then we read
            zn = getch();
            // the next code knowing that thi;
               // enter key is 0x0d or '\r'
           switch(zn){
               case 0x48: y--; break;
               case 0x50: y++; break;
               case 0x4b: x--; break;
               case 0x4d: x++; break;
               default: break;
           }
        }
        switch(zn){
            case 0x61:
                boardXSize = 9; boardYSize = 9;
                break;
            case 0x62:
                boardXSize = 13; boardYSize = 13;
                break;
            case 0x63:
                boardXSize = 19; boardYSize = 19;
                break;
            case ' ': attr = (attr + 1) % 16; break;
            case 0x0d: back = (back + 1) % 16; break;
            default: break;
        }
	} while (zn != 'q');

	_setcursortype(_NORMALCURSOR);	// show the cursor so it will be
					// visible after the program ends
	return 0;
	}
