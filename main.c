#include <stdio.h>
#include <string.h>
#include "str.h"

int
main(void)
{
        str* s[64] = {0};
        str **p;
        int i; long n;
        rune *r;

        s[0] = str("Ass");
        s[1] = str(" Dick");

        printf("%s (%d)\n", s[0], strn(s[0]));
        printf("%s (%d)\n", s[1], strn(s[1]));

        s[2] = scat(s[0],s[1]);
        printf("%s (%d)\n", s[2], strn(s[2]));

        s[2] = sresize(s[2], 2);
        printf("%s (%d)\n", s[2], strn(s[2]));

        s[3] = str("こんにちは world 😁");
        printf("%s (%d), runen (%d)\n", s[3], strn(s[3]), runesn(s[3]));
        printf("repeat cached runen (%d)\n", runesn(s[3]));

        s[4] = str("QWERTY");
        s[5] = str("QWERTY");
        printf("comparing '%s' and '%s' using strcmp, (%d)\n", s[4], s[5], strcmp(s[4], s[5]));

        s[6] = sdup(s[5]);
        printf("%s (%d)\n", s[6], strn(s[6]));

        s[7] = sreadfile("main.c");
        printf("(%d)\n", strn(s[7]));

        s[8] = scatn(s, 4);
        printf("%s (%d)\n", s[8], strn(s[8]));

        r = runes(s[3]);
        n = runesn(s[3]);
        printf("%s\n", s[3]);
        for(i=0; i<n; ++i){
                printf("%d ", runew(r[i]));
        }
        puts("");

        nilrunes(s[3]);
        
        r = runes(s[3]);
        n = runesn(s[3]);
        printf("%s (%d)\n", s[3], n);
        for(i=0; i<n; ++i){
                printf("%d ", runew(r[i]));
        }
        puts("");

        p=s;
        while(*p){
                freestr(*p);
                ++p;
        }
        return 0;
}
