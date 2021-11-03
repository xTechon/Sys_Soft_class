#include <stdio.h>

int main() {
  short scott = 0;
  short limit = 10;
  for (scott = 0; scott < limit; scott++) {
    printf("Hello %d\n", scott);
  }
}
/*--------------------------------------- Written in SIC Asm
** DEMO     START     1207
** SCOTT    WORD      0
** LIMIT    WORD      10
** ONE      WORD      1
**          STA       SCOTT     # 1 cpu cycle      1  cycle
** LOOP     COMP      LIMIT     # 5-6 cycles       55 cycles COMP compares A
*with mem add
**          JEQ       DONE      # 2 cpu cycles     20 cycles
**          ADD       ONE       # 1 cpu cycles     10 cycles instr don't take
*litterals
**          J         LOOP      # 1 cpu cycles     10 cycles
** DONE     END                 #                  ---------
**                                                 96 cycles
**-------------------------------------------------Optimized
** # Optimized by loop unwrapping
** DEMO     START     1207
** SCOTT    WORD      0
** LIMIT    WORD      10
**          STA       SCOTT     # 1 cpu cycle
**          CALL      printf
**          ADD       ONE       # 1 cpu cycle
**          CALL      printf
**          ADD       ONE       # 1 cpu cycle
**          CALL      printf
**          ADD       ONE       # 1 cpu cycle
**          CALL      printf
**          ADD       ONE       # 1 cpu cycle
**          CALL      printf
**          ADD       ONE       # 1 cpu cycle
**          CALL      printf
**          ADD       ONE       # 1 cpu cycle
**          CALL      printf
**          ADD       ONE       # 1 cpu cycle
**          CALL      printf
**          ADD       ONE       # 1 cpu cycle
**          CALL      printf
**          ADD       ONE       # 1 cpu cycle
** #                             ------------
**
*/
