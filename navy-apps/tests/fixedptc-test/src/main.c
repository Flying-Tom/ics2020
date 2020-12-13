#include <fixedptc.h>
#include <stdio.h>
int main()
{
    fixedpt a = fixedpt_rconst(1.2);
    fixedpt b = fixedpt_fromint(10);
    int c = 0;
    if (b > fixedpt_rconst(7.9))
    {
        c = fixedpt_toint(fixedpt_div(fixedpt_mul(a + FIXEDPT_ONE, b), fixedpt_rconst(2.3)));
    }
    printf("%d\n", fixedpt_ceil(fixedpt_rconst(3.1)));
    printf("%d\n", fixedpt_ceil(fixedpt_rconst(4.5)));
    printf("%d\n", fixedpt_ceil(fixedpt_rconst(5.7)));
    printf("%d\n", fixedpt_ceil(fixedpt_rconst(2.0)));
    printf("%d\n", fixedpt_ceil(fixedpt_rconst(2.58)));
    printf("%d\n", fixedpt_fromint(2));
    return 0;
}