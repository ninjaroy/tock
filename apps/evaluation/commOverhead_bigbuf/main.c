#include <firestorm.h>
#include <gpio.h>

void main(void) {
    gpio_enable_output(P3);

    char bigbuf[1000] = "\
1??11111111111111111111111111111111111111111111111\
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaB\
2222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222\
3333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333\
4444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444\
5555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555\
6666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666\
7777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777\
8888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888\
9999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999\
0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000#\n!\
";
    while(1) {
      /*
         base = 0x400E1000
         base+0x54: Set
         base+0x58: Clear
         P3 -> PA12
      */

      // Set pin using direct MMIO
      asm ("\
          movw r3, 0x1054    \n\
          movt r3, 0x400E    \n\
          movs r4, 0x1000    \n\
          str  r4, [r3]      \n\
          "
          :               /* output */
          :               /* input */
          : "r3", "r4"    /* clobbers */
          );

      putnstr(bigbuf, 100);

      // RESULTS:
      // bytes ,   ms
      //    1  ,   0.075
      //    5  ,   0.440
      //   10  ,   1.05
      //   50  ,   5.92
      //  100  ,  12.0 --> pre-call -> start: 99.2 us, start -> int: 11.897 ms, int -> done: 31.47 us
      //  500  ,  60.6
      // 1000  , 121.

      // Clear
      asm ("\
          movw r3, 0x1058    \n\
          movt r3, 0x400E    \n\
          movs r4, 0x1000    \n\
          str  r4, [r3]      \n\
          "
          :               /* output */
          :               /* input */
          : "r3", "r4"    /* clobbers */
          );
      delay_ms(2);
    }
}
