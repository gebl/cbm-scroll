#ifndef _MAIN_H
#define _MAIN_H

/**
 * The first important thing is that the VICII can only adress 16k ram at once.
 * This means that the 64k memory is divided into four 16k VIC banks. $DD00's
 * lowmost 2 bits controls that which bank is seen by the VIC:
 *
 * $DD00 = %xxxxxx11 -> bank0: $0000-$3fff
 * $DD00 = %xxxxxx10 -> bank1: $4000-$7fff
 * $DD00 = %xxxxxx01 -> bank2: $8000-$bfff
 * $DD00 = %xxxxxx00 -> bank3: $c000-$ffff
 *
 * If speed is not critical its generally a good idea to change vic bank like
 * this:
 *
 * lda $DD00
 * and #%11111100
 * ora #%000000xx ;<- your desired VIC bank value, see above
 * sta $DD00
 *
 * These are controlled by $D018.
 * The Screen dimensions are 40×25 = 1000 bytes of data. When telling the C64
 * where you want to put your screen data, this is rounded up to 1024 = $400, so
 * you can put screen data on each multiple of $400 bytes in memory. $0400,
 * $0800, $0c00, $1000, $1400, and so on.
 *
 * Char mem is made up of 256x8 byte chars: 256*8= 2048 -> $0800
 * Bitmap is (40x8)x25 = 8000, rounded up to 8192 -> $2000
 *
 * $D018/53272/VIC+24:   Memory Control Register
 * +----------+---------------------------------------------------+
 * | Bits 7-4 |   Video Matrix Base Address (inside VIC)          |
 * | Bit  3   |   Bitmap-Mode: Select Base Address (inside VIC)   |
 * | Bits 3-1 |   Character Dot-Data Base Address (inside VIC)    |
 * | Bit  0   |   Unused                                          |
 * +----------+---------------------------------------------------+
 *
 *
 * Bitmap
 * $D018 = %xxxx0xxx -> bitmap is at $0000
 * $D018 = %xxxx1xxx -> bitmap is at $2000
 *
 *
 * Character memory
 * $D018 = %xxxx000x -> charmem is at $0000
 * $D018 = %xxxx001x -> charmem is at $0800
 * $D018 = %xxxx010x -> charmem is at $1000
 * $D018 = %xxxx011x -> charmem is at $1800
 * $D018 = %xxxx100x -> charmem is at $2000
 * $D018 = %xxxx101x -> charmem is at $2800
 * $D018 = %xxxx110x -> charmem is at $3000
 * $D018 = %xxxx111x -> charmem is at $3800
 *
 *
 * Screen memory
 * $D018 = %0000xxxx -> screenmem is at $0000
 * $D018 = %0001xxxx -> screenmem is at $0400
 * $D018 = %0010xxxx -> screenmem is at $0800
 * $D018 = %0011xxxx -> screenmem is at $0c00
 * $D018 = %0100xxxx -> screenmem is at $1000
 * $D018 = %0101xxxx -> screenmem is at $1400
 * $D018 = %0110xxxx -> screenmem is at $1800
 * $D018 = %0111xxxx -> screenmem is at $1c00
 * $D018 = %1000xxxx -> screenmem is at $2000
 * $D018 = %1001xxxx -> screenmem is at $2400
 * $D018 = %1010xxxx -> screenmem is at $2800
 * $D018 = %1011xxxx -> screenmem is at $2c00
 * $D018 = %1100xxxx -> screenmem is at $3000
 * $D018 = %1101xxxx -> screenmem is at $3400
 * $D018 = %1110xxxx -> screenmem is at $3800
 * $D018 = %1111xxxx -> screenmem is at $3c00
 * 
 * 
 * Sprites
 * Sprites read their data based on the value of their corresponding Sprite Pointer. Sprite Pointers are located always at the _given_ screen memory's last 8 bytes. ie: screen_memory+$03f8 = sprite pointer0. As everything sprite adresses are also _relative_ to the start of the VIC bank.
 * 
 * for dummies:
 * screen_memory_start+$03f8 = sprite pointer0
 * screen_memory_start+$03f9 = sprite pointer1
 * screen_memory_start+$03fa = sprite pointer2
 * screen_memory_start+$03fb = sprite pointer3
 * screen_memory_start+$03fc = sprite pointer4
 * screen_memory_start+$03fd = sprite pointer5
 * screen_memory_start+$03fe = sprite pointer6
 * screen_memory_start+$03ff = sprite pointer7
 * 
 *   https://codebase64.org/doku.php?id=base:vicii_memory_organizing
 * 
 * 
 */
#define BANK 0xc000
#define BUFFER (unsigned char *)0x0400
#define SCREEN1 (unsigned char *)0xE000
#define SCREEN2 (unsigned char *)0xE400
#define CHARSET (unsigned char *)0xE800

#define SPRITE0_PTR 0x00
#define SPRITE1_PTR 0x01
#define SPRITE2_PTR 0x02
#define SPRITE3_PTR 0x03
#define SPRITE4_PTR 0x04
#define SPRITE5_PTR 0x05
#define SPRITE6_PTR 0x06
#define SPRITE7_PTR 0x07



#define SPRITE0 0x03F8
#define SPRITE1 0x03F9
#define SPRITE2 0x03FA
#define SPRITE3 0x03FB
#define SPRITE4 0x03FC
#define SPRITE5 0x03FD
#define SPRITE6 0x03FE
#define SPRITE7 0x03FF

#define XSCROLL (unsigned char *)0xD016
#define outb(addr, val) (*(addr) = (val))
#define inb(addr) (*(addr))

/** Values for the VIC address register to switch between the two pages
 * Shift address right 6 bits and mask with 0xF0 to get the high bits from the
 * address into the high 4 bits of the address register. This is used to allow
 * the defines above of what pages of video ram are being used to be dynamically
 * calculated.
 *
 * This is then ORed with the high bits of the charset address shifted right 10,
 * the three bits select where the charset is in memory. This is masked with
 * 0x0e to remove the least significant bit which is undefined.
 *
 * TODO: For perfomance reasons, this should prebably be statically set instead.
 *
 *
 */
#define PAGE1                               \
    (((unsigned int)SCREEN1 >> 6) & 0xF0) | \
        (((unsigned int)CHARSET >> 10) & 0x0E)
#define PAGE2                               \
    (((unsigned int)SCREEN2 >> 6) & 0xF0) | \
        (((unsigned int)CHARSET >> 10) & 0x0E)

#define WRAP ((MAPCOLS - 32) * 8)

static unsigned char *curbuffer;
static unsigned int curpage;
static unsigned char *backbuffer;
static unsigned int backpage;
#endif