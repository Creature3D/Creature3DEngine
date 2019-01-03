#ifndef _CONIOPORT_H_
#define _CONIOPORT_H_

#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

void conio_init (int* enterKey, int* backspaceKey);
void conio_exit ();
int conio_kbhit ();
int conio_getch ();
void conio_gotoxy (int x, int y);
void conio_getxy(int* x, int* y);

//clears the console window.  The cursor position after this operation is
//unchanged.
void conio_clear();

//conio_getsize returns x == y == 0 if it cannot get the size
void conio_getsize(int* x, int* y);

//conio_settitle sets a title for the console window, whereever possible,
//else does nothing.
void conio_settitle(const char* title);

void conio_putchar(int ch);

int conio_vprintf(const char* format, va_list ap);

int conio_vscanf(const char* format, va_list ap);

#ifdef __cplusplus
}
#endif

#endif
