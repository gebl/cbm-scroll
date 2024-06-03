#ifndef __BLT_H__
#define __BLT_H__
extern void blt();

#pragma bss-name (push, "ZEROPAGE")
extern unsigned char *src;
extern unsigned char *dst;
#pragma bss-name (pop)

#endif