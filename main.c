#include "main.h"

#include <c64.h>
#include <cbm.h>
#include <peekpoke.h>
#include <stdint.h>
#include <stdio.h>

#include "blt.h"
#include "game.h"

void bgcolor(unsigned char color) { POKE(53280, color); }

void bordercolor(unsigned char color) { POKE(53281, color); }

void initPat() {
    unsigned int i;

    for (i = 0; i < PATLEN; i++) {
        outb((unsigned char *)CHARSET + i, pat[i]);
    }
}

void fillSit(unsigned int frame, unsigned int row) {
    dst = backbuffer + ((row) * 40);
    src = (unsigned char *)&map[row] + frame;

    blt();
}

unsigned int game() {
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

    for (i = 0; i < 25; i++) {
        for (j = 0; j < 40; j++) {
            outb(SCREEN1 + (i * 40) + j, 0);
            outb(SCREEN2 + (i * 40) + j, 0);
        }
    }

    for (i=0xD800; i<0xDBFF; i++) {
        outb((unsigned char *)i, COLOR_GREEN);
    }

    block = inb(&CIA2.pra);
    outb(&CIA2.pra, (block & 0xFC) | (((unsigned int)curbuffer >> 14) ^ 0x03));
    outb(&VIC.addr, curpage);

    initPat();
    outb(XSCROLL, inb(XSCROLL) & 0xF8 & 0xf7);
    while (1) {
        // waitvsync();
        score++;

        if (score % 8 == 0) {
            frame++;
            frame %= (MAPCOLS-40);
            temp = curbuffer;
            curbuffer = backbuffer;
            backbuffer = temp;
            temppage = curpage;
            curpage = backpage;
            backpage = temppage;

            block = inb(&CIA2.pra);
            outb(&CIA2.pra,
                 (block & 0xFC) | (((unsigned int)curbuffer >> 14) ^ 0x03));
            outb(&VIC.addr, curpage);
        }
        outb(XSCROLL, (inb(XSCROLL) & 0xF8) + (7 - score % 8));
    
        fillSit(frame, (7 - score % 8)*3);
        fillSit(frame, (7 - score % 8)*3+1);
        fillSit(frame, (7 - score % 8)*3+2);
    }

    outb(&VIC.addr, v);
    outb(&CIA2.pra, block);

    return score;
}

int main() {
    unsigned int score;
    bgcolor(COLOR_BLACK);
    bordercolor(COLOR_BLACK);
    while (1) {
        score = game();
    }

    return 0;
}
