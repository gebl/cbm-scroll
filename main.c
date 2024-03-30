#include <c64.h>
#include <cbm.h>
#include <peekpoke.h>
#include <stdint.h>
#include <stdio.h>

#include "game.h"
#include "main.h"
#include "blt.h"

void splash()
{
    bgcolor(COLOR_BLACK);
    bordercolor(COLOR_BLACK);
    textcolor(COLOR_GRAY1);
    clrscr();
    cursor(0);
    gotoxy(0, 0);
    cputs("Hello world!");
    cgetc();
}

void initPat()
{
    unsigned int i;

    for (i = 0; i < PATLEN; i++)
    {
        outb((unsigned char *)CHARSET + i, pat0[i]);
    }
}

void fillSit(unsigned int frame, unsigned int row)
{

    dst = backbuffer + ((row) * 40);
    src = (unsigned char *)&map[row] + frame;
    
    outb(dst,*src);

    Blt();
}

unsigned int game()
{
    unsigned int frame;

    unsigned char *temp;
    unsigned int temppage;

    unsigned int score;
    unsigned char block;
    unsigned char v;
    unsigned int i, j;

    score = 0;
    v = inb(&VIC.addr);

    curbuffer = SCREEN1;
    curpage = PAGE1;

    backbuffer = SCREEN2;
    backpage = PAGE2;

    frame = 0;

    for (i = 0; i < 25; i++)
    {
        for (j = 0; j < 40; j++)
        {
            outb(SCREEN1 + (i * 40) + j, 0);
            outb(SCREEN2 + (i * 40) + j, 0);
        }
    }

    block = inb(&CIA2.pra);
    outb(&CIA2.pra, (block & 0xFC) | (((unsigned int)curbuffer >> 14) ^ 0x03));
    outb(&VIC.addr, curpage);

    initPat();
    outb(XSCROLL, inb(XSCROLL) & 0xF8 & 0xf7);
    while (!kbhit())
    {
        // waitvsync();
        score++;

        if (score % 8 == 0)
        {
            frame++;
            frame %= WRAP;
            temp = curbuffer;
            curbuffer = backbuffer;
            backbuffer = temp;
            temppage = curpage;
            curpage = backpage;
            backpage = temppage;

            block = inb(&CIA2.pra);
            outb(&CIA2.pra, (block & 0xFC) | (((unsigned int)curbuffer >> 14) ^ 0x03));
            outb(&VIC.addr, curpage);
        }
        outb(XSCROLL, (inb(XSCROLL) & 0xF8) + (7 - score % 8));
        for (i = 10; i < 24; i++)
        {
            // fillSit(frame, (score % 8) * 3);
            fillSit(frame, i);
        }
    }
    (void)cgetc();
    outb(&VIC.addr, v);
    outb(&CIA2.pra, block);

    return score;
}

void scoresplash(unsigned int score)
{
    bgcolor(COLOR_BLACK);
    textcolor(COLOR_GRAY1);
    clrscr();
    cursor(1);
    gotoxy(0, 0);
    cputs("Your score: ");
    while (!kbhit())
    {
    }
    (void)cgetc();
}

int main()
{
    unsigned int score;
    splash();
    while (1)
    {
        score = game();
        cgetc();
        scoresplash(score);
    }

    return 0;
}
