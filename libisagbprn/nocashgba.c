#include <stdarg.h>
#include <stdio.h>
#include <gba/gba.h>

// originally for auto no$gba support, the string "no$gba" should be at this address,
// the user needs to read this string out as the memory viewer won't show it.
#define NOCASHGBAIDADDR 0x4FFFA00
#define NOCASHGBAPRINTADDR1 0x4FFFA10 // automatically adds a newline after the string has finished
#define NOCASHGBAPRINTADDR2 0x4FFFA14 // does not automatically add the newline. by default, NOCASHGBAPRINTADDR2 is used. this is used to keep strings consistent between no$gba and VBA-RR, but a user can choose to forgo this.

void NoCashGBAPrint(const char *pBuf)
{
    *(volatile u32*)NOCASHGBAPRINTADDR2 = (u32)pBuf;
}

void NoCashGBAPrintf(const char *pBuf, ...)
{
    char bufPrint[0x100];
    va_list vArgv;
    va_start(vArgv, pBuf);
    vsprintf(bufPrint, pBuf, vArgv);
    va_end(vArgv);
    NoCashGBAPrint(bufPrint);
}
