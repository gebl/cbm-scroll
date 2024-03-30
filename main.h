#ifndef _MAIN_H
#define _MAIN_H

#define BUFFER (unsigned char *)0x0400
#define SCREEN1 (unsigned char *)0xE000
#define SCREEN2 (unsigned char *)0xE400
#define CHARSET (unsigned char *)0xE800
#define COLORRAM (unsigned char *)0xD800
#define XSCROLL (unsigned char *)0xD016
#define outb(addr, val) (*(addr) = (val))
#define inb(addr) (*(addr))

/* Values for the VIC address register to switch between the two pages */
#define PAGE1 (((unsigned int)SCREEN1 >> 6) & 0xF0) | (((unsigned int)CHARSET >> 10) & 0x0E)
#define PAGE2 (((unsigned int)SCREEN2 >> 6) & 0xF0) | (((unsigned int)CHARSET >> 10) & 0x0E)

#define WRAP ((MAPCOLS - 32) * 8)

unsigned char *src;
unsigned char *dst;

static unsigned char *curbuffer;
static unsigned int curpage;
static unsigned char *backbuffer;
static unsigned int backpage;
#endif