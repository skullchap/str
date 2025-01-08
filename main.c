#include <stdio.h>
#include <string.h>
#include "str.h"

int
main(void)
{
        str* s[64] = {0};
        str **p;

        s[0] = STR("Ass");
        s[1] = STR(" Dick");

        printf("%s (%d)\n", s[0], strn(s[0]));
        printf("%s (%d)\n", s[1], strn(s[1]));

        s[2] = sstrcat(s[0],s[1]);
        printf("%s (%d)\n", s[2], strn(s[2]));

        s[2] = strize(s[2], 2);
        printf("%s (%d)\n", s[2], strn(s[2]));

        s[3] = STR("こんにちは world 😁");
        printf("%s (%d), runen (%d)\n", s[3], strn(s[3]), runen(s[3]));

        s[4] = STR("QWERTY"), s[5] = STR("QWERTY");
        printf("comparing '%s' and '%s' using strcmp, (%d)\n", s[4], s[5], strcmp(s[4], s[5]));

        p=s;
        while(*p){
                freestr(*p);
                ++p;
        }
        return 0;
}
