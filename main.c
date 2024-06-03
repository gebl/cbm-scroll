#include "main.h"

#include <c64.h>
#include <cbm.h>
#include <peekpoke.h>
#include <stdint.h>
#include <stdio.h>

#include "blt.h"
#include "game.h"
#include "rnd.h"

#define BSTART 239
#define LIVESSTART 0
#define LIVESWIDTH 26
#define LIVESY 50

#define GROUND 8 * 13 + 50 + 4

#define DINOX 130
#define JUMPCLEAR GROUND - 20

#define LIVES1HI 0x40
#define LIVES2HI 0x20
#define LIVES3HI 0x10

#define BARRELHISTART 0x5a

#define BARREL1HI 0x2
#define BARREL1HI_MASK 0xFD
#define BARREL2HI 0x4
#define BARREL2HI_MASK 0xFB
#define BARREL3HI 0x8
#define BARREL3HI_MASK 0xF7
typedef struct viz_struct {
    unsigned x0 : 1;
    unsigned x1 : 1;
    unsigned x2 : 1;
} viz_struct;

void bordercolor(unsigned char color) { POKE(53280, color); }

void bgcolor(unsigned char color) { POKE(53281, color); }

void initPat() {
    unsigned int i;

    for (i = 0; i < PATLEN; i++) {
        outb((unsigned char *)CHARSET + i, pat[i]);
    }
}

void fillSit(unsigned int frame, unsigned int row) {
    dst = (unsigned char *)(backbuffer + ((row) * 40));
    src = (unsigned char *)(map[row]+frame);
    blt();
    
}

void initColorTable() {
    unsigned int i, j;

    for (i = 0; i < 16; i++) {
        for (j = 0; j < 40; j++) {
            outb((unsigned char *)COLOR_RAM + i * 40 + j, COLOR_BLUE);
        }
    }

    for (i = 16; i < 24; i++) {
        for (j = 0; j < 40; j++) {
            outb((unsigned char *)COLOR_RAM + i * 40 + j, COLOR_BROWN);
        }
    }
}

unsigned int game() {
    unsigned int frame;

    unsigned char *temp;
    unsigned int temppage;

    unsigned int score, score2;
    unsigned char block;
    unsigned char v;
    unsigned int i, j;
    int yv = 0;
    unsigned char key, lastjump, lives;
    viz_struct viz;
    char b1, b2;

    score = 0;
    score2 = 0;
    lives = 3;
    lastjump = 0;
    v = inb(&VIC.addr);
    key = 0;

    b1 = rnd();
    b2 = rnd();

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

    initColorTable();
    // outb(&CIA2.pra, (block & 0xFC) | (((unsigned int)curbuffer >> 14) ^
    // 0x03)); // video ram at $c000-$ffff

    /**
     * $DD00 = %xxxxxx11 -> bank0: $0000-$3fff
     * $DD00 = %xxxxxx10 -> bank1: $4000-$7fff
     * $DD00 = %xxxxxx01 -> bank2: $8000-$bfff
     * $DD00 = %xxxxxx00 -> bank3: $c000-$ffff
     *
     * https://codebase64.org/doku.php?id=base:vicii_memory_organizing
     */
    block = inb(&CIA2.pra);
    outb(&CIA2.pra,
         block & 0xfffc);  // video ram at $c000-$ffff - 3	xxxxxx00 in DD00
    outb(&VIC.addr, curpage);

    // dinosaur
    outb((unsigned char *)SCREEN1 + SPRITE0, SPRITE0_PTR);
    outb((unsigned char *)SCREEN2 + SPRITE0, SPRITE1_PTR);

    // barrel1
    outb((unsigned char *)SCREEN1 + SPRITE1, SPRITE2_PTR);
    outb((unsigned char *)SCREEN2 + SPRITE1, SPRITE3_PTR);
    // barrel2

    outb((unsigned char *)SCREEN1 + SPRITE2, SPRITE3_PTR);
    outb((unsigned char *)SCREEN2 + SPRITE2, SPRITE2_PTR);
    // barrel3
    outb((unsigned char *)SCREEN1 + SPRITE3, SPRITE2_PTR);
    outb((unsigned char *)SCREEN2 + SPRITE3, SPRITE3_PTR);

    // Live 1
    outb((unsigned char *)SCREEN1 + SPRITE4, SPRITE0_PTR);
    outb((unsigned char *)SCREEN2 + SPRITE4, SPRITE1_PTR);

    // Live 2
    outb((unsigned char *)SCREEN1 + SPRITE5, SPRITE0_PTR);
    outb((unsigned char *)SCREEN2 + SPRITE5, SPRITE1_PTR);
    // Live 3
    outb((unsigned char *)SCREEN1 + SPRITE6, SPRITE0_PTR);
    outb((unsigned char *)SCREEN2 + SPRITE6, SPRITE1_PTR);

    for (i = 0; i < 64; i++) {
        outb((unsigned char *)BANK + (SPRITE0_PTR * 64) + i, dino1[i]);
    }
    for (i = 0; i < 64; i++) {
        outb((unsigned char *)BANK + (SPRITE1_PTR * 64) + i, dino2[i]);
    }
    for (i = 0; i < 64; i++) {
        outb((unsigned char *)BANK + (SPRITE2_PTR * 64) + i, barrel1[i]);
    }
    for (i = 0; i < 64; i++) {
        outb((unsigned char *)BANK + (SPRITE3_PTR * 64) + i, barrel2[i]);
    }

    initPat();
    outb(XSCROLL, inb(XSCROLL) & 0xF8 & 0xf7);

    VIC.spr_ena = 0x7f;
    VIC.spr0_x = DINOX;
    VIC.spr0_y = GROUND;
    VIC.spr0_color = COLOR_WHITE;

    VIC.spr1_x = BSTART;
    VIC.spr1_y = GROUND;

    VIC.spr2_x = BSTART;
    VIC.spr2_y = GROUND;

    VIC.spr3_x = BSTART;
    VIC.spr3_y = GROUND;

    VIC.spr4_x = LIVESSTART + 2 * LIVESWIDTH;
    VIC.spr4_y = LIVESY;

    VIC.spr5_x = LIVESSTART + LIVESWIDTH;
    VIC.spr5_y = LIVESY;

    VIC.spr6_x = LIVESSTART;
    VIC.spr6_y = LIVESY;

    VIC.spr_hi_x =
        LIVES1HI | LIVES2HI | LIVES3HI | BARREL1HI | BARREL2HI | BARREL3HI;
    VIC.spr0_color = COLOR_WHITE;
    VIC.spr1_color = COLOR_BROWN;
    VIC.spr2_color = COLOR_BROWN;
    VIC.spr3_color = COLOR_BROWN;
    VIC.spr4_color = COLOR_WHITE;
    VIC.spr5_color = COLOR_WHITE;
    VIC.spr6_color = COLOR_WHITE;

    viz.x0 = 1;
    viz.x1 = 0;
    viz.x2 = 0;
    lives = 3;

    while (lives > 0) {
        score++;


        if (score % 8 == 0) {
            frame++;
            frame %= (MAPCOLS - 40);
            temp = curbuffer;
            curbuffer = backbuffer;
            backbuffer = temp;
            temppage = curpage;
            curpage = backpage;
            backpage = temppage;
            waitvsync();
            outb(&VIC.addr, curpage);
        }
        outb(XSCROLL, (inb(XSCROLL) & 0xF8) + (7 - score % 8));

        fillSit(frame, (7 - score % 8) * 3);
        fillSit(frame, (7 - score % 8) * 3 + 1);
        fillSit(frame, (7 - score % 8) * 3 + 2);

        if (VIC.spr0_y < GROUND) {
            yv = yv + 1;
        } else {
            VIC.spr0_y = GROUND;
            yv = 0;
            lastjump = 0;
        }

        key = cbm_k_getin();
        if (key == ' ') {
            if (yv == 0 && lastjump == 0) {
                yv = -35;
            } else if (lastjump < 2) {
                lastjump++;
                yv = -30;
            }
        }

        VIC.spr0_y += (yv / 8);

        if (VIC.spr0_y < 50) {
            VIC.spr0_y = 50;
        }

        if (viz.x0) {
            VIC.spr1_x--;
            if (VIC.spr1_x == 0) {
                if (VIC.spr_hi_x & BARREL1HI) {
                    VIC.spr_hi_x = VIC.spr_hi_x & BARREL1HI_MASK;
                    VIC.spr1_x = 255;
                } else {
                    VIC.spr1_x = BARRELHISTART;
                    VIC.spr_hi_x = VIC.spr_hi_x | BARREL1HI;
                }
            }
            if (!(VIC.spr_hi_x & BARREL1HI)) {
                if (VIC.spr1_x > DINOX - 10 && VIC.spr1_x < DINOX + 10 &&
                    VIC.spr0_y > JUMPCLEAR) {
                    lives--;
                    VIC.spr_ena = VIC.spr_ena >> 1;
                    VIC.spr1_x = BARRELHISTART;
                    VIC.spr_hi_x = VIC.spr_hi_x | BARREL1HI;
                }
                if (VIC.spr1_x < 5) {
                    VIC.spr1_x = BARRELHISTART;
                    VIC.spr_hi_x = VIC.spr_hi_x | BARREL1HI;
                    score2 = score2 + 2;
                    b1 = rnd();
                    b2 = rnd();
                }
            }
        }

        if (viz.x1) {
            VIC.spr2_x--;
            if (VIC.spr2_x == 0) {
                if (VIC.spr_hi_x & BARREL2HI) {
                    VIC.spr_hi_x = VIC.spr_hi_x & BARREL2HI_MASK;
                    VIC.spr2_x = 255;
                } else {
                    VIC.spr2_x = BARRELHISTART;
                    VIC.spr_hi_x = VIC.spr_hi_x | BARREL2HI;
                }
            }

            if (!(VIC.spr_hi_x & BARREL2HI)) {
                if (VIC.spr2_x > DINOX - 10 && VIC.spr2_x < DINOX + 10 &&
                    VIC.spr0_y > JUMPCLEAR) {
                    lives--;
                    VIC.spr_ena = VIC.spr_ena >> 1;
                    VIC.spr2_x = BARRELHISTART;
                    VIC.spr_hi_x = VIC.spr_hi_x | BARREL2HI;
                }
                if (VIC.spr2_x < 5) {
                    VIC.spr2_x = BARRELHISTART;
                    VIC.spr_hi_x = VIC.spr_hi_x | BARREL2HI;
                    score2 = score2 + 2;
                    viz.x1 = 0;
                }
            }
        } else {
            if (score % (b1 / 2) == 0) {
                viz.x1 = 1;
                b1 = rnd();
            }
        }
        if (viz.x2) {
            VIC.spr3_x--;
            if (VIC.spr3_x == 0) {
                if (VIC.spr_hi_x & BARREL3HI) {
                    VIC.spr_hi_x = VIC.spr_hi_x & BARREL3HI_MASK;
                    VIC.spr3_x = 255;
                } else {
                    VIC.spr3_x = BARRELHISTART;
                    VIC.spr_hi_x = VIC.spr_hi_x | BARREL3HI;
                }
            }

            if (!(VIC.spr_hi_x & BARREL3HI)) {
                if (VIC.spr3_x > DINOX - 10 && VIC.spr3_x < DINOX + 10 &&
                    VIC.spr0_y > JUMPCLEAR) {
                    lives--;
                    VIC.spr_ena = VIC.spr_ena >> 1;
                    VIC.spr3_x = BARRELHISTART;
                    VIC.spr_hi_x = VIC.spr_hi_x | BARREL3HI;
                }
                if (VIC.spr3_x < 5) {
                    VIC.spr3_x = BARRELHISTART;
                    VIC.spr_hi_x = VIC.spr_hi_x | BARREL3HI;
                    score2 = score2 + 2;
                    viz.x2 = 0;
                }
            }
        } else {
            if (score % (b2 / 5) == 0) {
                viz.x2 = 1;
                b2 = rnd();
            }
        }
    }

    outb(&VIC.addr, v);
    outb(&CIA2.pra, block);

    return score;
}

int main() {
    unsigned int score;
    bgcolor(COLOR_LIGHTGREEN);
    bordercolor(COLOR_BLACK);
    rnd_init();
    printf("Dino Run\n");
    printf("Press space to jump\n");
    printf("Avoid the barrels\n");
    printf("Score 2 points for each barrel you pass\n");
    printf("Press RUN/STOP to exit\n");
    printf("Press any key to start\n");

    cbm_k_getin();
    while (cbm_k_getin() != 0);

    while (1) {
        score = game();
        printf("Score: %d\n", score);
    }

    return 0;
}
