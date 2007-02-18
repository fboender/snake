/*                                                                   */
/* Snake                                                             */
/*                                                                   */
/* Description: Simple snake game to exercise my ncurses skills      */
/* Author     : F.Boender <fboender@gelrevision.nl>                  */
/* Date       : March 20, 2000                                       */
/* Compile    : gcc -Wall -lncurses -o snake snake.c                 */
/* Version    : 0.3                                                  */
/* Remarks    : This code could use a clean-up, and I'm not just     */
/*              talking about the syntax.                            */
/*                                                                   */
/* Copyright (C) 2000  Ferry Boender <fboender@gelrevision.nl>       */
/*                                                                   */
/* This program is free software; you can redistribute it and/or     */
/* modify it under the terms of the GNU General Public License       */
/* as published by the Free Software Foundation; either version 2    */
/* of the License, or (at your option) any later version.            */
/*                                                                   */
/* This program is distributed in the hope that it will be useful,   */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of    */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the     */
/* GNU General Public License for more details.                      */
/*                                                                   */
/* You should have received a copy of the GNU General Public License */
/* along with this program; if not, write to the Free Software       */
/* Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.         */
/*                                                                   */
/* For more information, see the COPYING file supplied with this     */
/* program.                                                          */
/*                                                                   */

#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <unistd.h>
#include <time.h>

#define _TAIL 'o'
#define _HEAD '@'
#define _HEADDEAD '*'
#define _DOLLAR '$'

//#define _DELAY_X 100
//#define _DELAY_Y 120
//#define _GROW 5

int _DELAY_X = 100;
int _DELAY_Y = 150;
int _GROW = 5;
int _NROFDOLLARS = 100;

WINDOW *stdscr;
int head_x,head_y, head_d;
int tail_l = 10;
int tail_x[4001];
int tail_y[4001];
int points;

//
// random: Returns a random number between 0 to r
//
int getrandom(float r)
{
  return 0+(int)(r*rand()/(RAND_MAX+1.0));
}

//
// random_dollar: Puts a dollar in a random place on the screen
//
void random_dollar()
{
  int dollar_y, dollar_x;
  bool placed = FALSE;

  do
    {
      dollar_y = 2+getrandom(LINES-2);
      dollar_x = 1+getrandom(COLS-2);
      
      if (mvinch(dollar_y, dollar_x)==32)
        { placed = TRUE; }
    } while (!placed);
  
    mvprintw (dollar_y, dollar_x,"$");
}

//
// explosion: Shows an ascii-art explosion at given coordinates
//
void explosion(int y, int x)
{
  init_pair (0, COLOR_WHITE, COLOR_BLACK);
  init_pair (1, COLOR_BLACK, COLOR_WHITE);

  // Frame 1
  mvaddch (y,x,ACS_CKBOARD);
  refresh(); napms(50);
  
  // Frame 2
  mvaddch (y-1,x,ACS_CKBOARD); mvaddch (y+1,x,ACS_CKBOARD);
  mvaddch (y,x-1,ACS_CKBOARD); mvaddch (y,x+1,ACS_CKBOARD);
  attrset (COLOR_PAIR(0)); mvaddch (y,x,' '); attrset (COLOR_PAIR(1));
  refresh(); napms(50);
  
  // Frame 3
  attrset (COLOR_PAIR(1));
  mvaddch (y-1,x,' '); mvaddch (y+1,x,' ');
  mvaddch (y,x-1,' '); mvaddch (y,x+1,' ');
  mvaddch (y,x,' ');
  attrset (COLOR_PAIR(0));
  mvaddch (y-1,x-1,ACS_CKBOARD); mvaddch (y+1,x-1,ACS_CKBOARD);
  mvaddch (y-1,x+1,ACS_CKBOARD); mvaddch (y+1,x+1,ACS_CKBOARD);
  refresh(); napms(50);
  
  // Frame 4
  mvaddch (y-1,x-1,' '); mvaddch (y+1,x-1,' ');
  mvaddch (y-1,x+1,' '); mvaddch (y+1,x+1,' ');
  refresh(); napms(50);

  // Frame 5
  mvaddch (y-1,x,' '); mvaddch (y+1,x,' ');
  mvaddch (y,x-1,' '); mvaddch (y,x+1,' ');
  refresh(); napms(50);

  mvaddch (y,x,_HEADDEAD);
}

//
// initcurses: Initialize ncurses interface and draw screen
//
void initcurses()
{
  /* Initialize ncurses interface */
  stdscr = initscr();
  start_color();
  nodelay (stdscr, TRUE);
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  nonl();
  curs_set(0);

  border (0,0,0,0,0,0,0,0);
  mvaddch (2,0, ACS_LTEE);
  mvaddch (2,COLS-1, ACS_RTEE);
  mvhline (2,1,ACS_HLINE,COLS-2);
  refresh();
}

//
// initsnake: Initialize beginning positions of snakes head and tail
//
void initsnake()
{
  int i;
  
  head_x = (int)COLS/2;
  head_y = (int)LINES/2;
  head_d = 6;

  points = 0;
  tail_l = 10;
  
  for (i=0; i!=4001; i++)
    {
      tail_x[i] = (int)COLS/2;
      tail_y[i] = (int)LINES/2;
    }
}

//
// initdollars: Place some random dollars in the screen
//
void initdollars()
{
  int i;
  
  srand (time(0));
  for (i=0; i!=_NROFDOLLARS; i++)
    {
      random_dollar();
    }
}

//
// endsnake: The snake is dead, end the game.
//
void endsnake()
{
  int key;
  int center_y;
  
  refresh();
  
  /* Clear keyboard buffer */
  napms (1000);
  while (!getch()==ERR){ }
  
  /* You're dead */
  center_y = (int)LINES/2;
  
  mvprintw (center_y-1,COLS/2-19, "You've hit your head against something!");
  mvprintw (center_y  ,COLS/2-17, "Please press a key to continue dying");
  refresh();
  getchar();
  mvprintw (center_y+2,COLS/2-7, "You have died.");
  refresh();
  
  /* Clean up and exit or start all over again */
  getchar();
  clear();
  mvprintw (center_y,COLS/2-17, "Would you like to play again? (Y/N)");
  refresh();
  
  key = getchar();
  if (key=='N'||key=='n')
    {
      endwin();
      exit(0);
    }
  else
    {
      clear();
      endwin();
      initcurses();
      initsnake();
      initdollars();
    }
}

void initparams(int argc, char **argv)
{
  int c;
  
  
  while ((c = getopt (argc, argv, "hd:g:n:")) != -1)
    switch (c)
      {
      case 'h':
        printf ("\nSnake v0.3\t\t(c) F.Boender\n\n");
        printf ("usage: snake [-h|-d|-g|-n]\n\n");
        printf ("\t-h\tShow help\n");
        printf ("\t-d #\tSet delay in ms (Default 100)\n");
        printf ("\t-g #\tSet tail growth per dollar (Default 5)\n");
        printf ("\t-n #\tSet number of dollars in screen (Default 5)\n\n");
        printf ("Snake is distributed under GPL (See COPYING file)\n\n");
        exit (0);
      case 'g':
        _GROW = atoi(optarg);
        break;
      case 'd' :
        _DELAY_X = atoi(optarg);
        _DELAY_Y = _DELAY_X + (_DELAY_X / 2);
        break;
      case 'n' :
        _NROFDOLLARS =atoi(optarg);
        break;
      case '?': printf ("help: snake -h\n"); exit (1);
      default: abort();
      }
  
}

int main (int argc, char *argv[])
{
    int key;
    int i;
    chtype collision;    

    initparams(argc, argv);  // Initialize cmdline params
    initcurses();  // ncurses interface
    initsnake();   // snakes head and tail
    initdollars(); // Put some dollars at random
    
    while (1)
    {
      /* Process keypresses */
      key = getch();
      if (key!=ERR)
        {
          switch (key)
            {
            case KEY_UP:    head_d = 8; break;
            case KEY_DOWN:  head_d = 2; break;
            case KEY_LEFT:  head_d = 4; break;
            case KEY_RIGHT: head_d = 6; break;
            }
        }

      /* Tail processing */
      for (i=tail_l; i!=0; i--)
        {
          tail_y[i]=(int)tail_y[i-1];
          tail_x[i]=(int)tail_x[i-1];
          mvaddch (tail_y[i], tail_x[i], (char)_TAIL);
        }

      mvaddch (tail_y[tail_l], tail_x[tail_l], ' ');
      mvaddch (head_y, head_x, 'o');
      mvaddch (0,0,ACS_ULCORNER);
      
      tail_x[0] = head_x;
      tail_y[0] = head_y;

      /* Make snake's head go the right way */
      switch (head_d)
        {
        case 2 : head_y++; break;
        case 4 : head_x--; break;
        case 6 : head_x++; break;
        case 8 : head_y--; break;
        }

      /* Check head collision */
      collision = mvinch (head_y, head_x);
      switch (collision&A_CHARTEXT)
        {
        case 36: points++; random_dollar(); collision=32; tail_l=tail_l+_GROW; break;
        }
      if ((collision&A_CHARTEXT) != 32)
        {
          mvaddch (head_y, head_x, (int) _HEADDEAD);
          explosion (head_y, head_x);
          endsnake();
        }
      
      /* Draw snake's head */
      mvaddch (head_y, head_x, (int) _HEAD);

      /* Draw scoreboard */
      mvprintw (1,2,"Score: %i \t\t Tail length: %i",points, tail_l);

      /* Finish up round */
      refresh();
      if ((head_d==8)||(head_d==2)) { napms(_DELAY_Y); }
      if ((head_d==6)||(head_d==4)) { napms(_DELAY_X); }
    }

    return 0;
}
