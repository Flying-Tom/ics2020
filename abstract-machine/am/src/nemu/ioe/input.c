#include <am.h>
#include <nemu.h>

#define KEYDOWN_MASK 0x8000

void __am_input_keybrd(AM_INPUT_KEYBRD_T *kbd)
{
    //bd->keydown = 0;
    if (inb(0x64) & 0x1)
    {
        kbd->keydown = 1;
        kbd->keycode = inl(KBD_ADDR);
    }
    else
    {
        kbd->keydown = 0;
        kbd->keycode = AM_KEY_NONE;
    }
}
