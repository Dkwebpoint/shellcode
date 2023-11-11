GNU nano 7.2                                                 ShellTester.c
/**********************************************************************
*
* Program: shelltester.c
*
* Date: 08/11/2023
*
* Author: Travis Phillips
* 
* Purpose: This code is used to provide a C template to paste shellcode
*          into and be able to run it live from within an ELF binary's
*          char buffer. This allows you to create a buffer with the
*          shellcode globally and this program will mark it as RWX using
*          mprotect() and then finally jump into.
*
* gcc -m32 -z execstack ShellTester.c -o shellesster
*
***********************************************************************/
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>

/////////////////////////////////////////////////////
//  source file: shell.s
/////////////////////////////////////////////////////
char payload[] = "\x01\x30\x8f\xe2"
                 "\x13\xff\x2f\xe1"
                 "\x02\xa0"
                 "\x49\x40"
                 "\x52\x40"
                 "\xc2\x71"
                 "\x0b\x27"
                 "\x01\xdf"
                 "\x2f\x62\x69\x6e"
                 "\x2f\x73\x68\x58";

int main() {
    // Print the banner.
    puts("\n\t\033[33;1m---===[ Shellcode Tester Stub v1.0 ]===---\033[0m\n");

    // Print the size of the shellcode.
    printf(" [\033[34;1m*\033[0m] Shellcode Size:  %d\n", sizeof(payload)-1);

    // Create a function pointer to the shellcode and
    // display it to the user.
    void (*shell)();
        shell=(void (*)())payload;

    printf(" [\033[34;1m*\033[0m] Shellcode Address: %p\n", shell);

    // Calculate the address to the start of the page for the
    // the shellcode.
    void *page_offset = (void *)((int)shell & ~(getpagesize()-1));
    printf(" [\033[34;1m*\033[0m] Shellcode page: %p\n", &page_offset);

    // Use mprotect to mark that page as RWX.
    mprotect(page_offset, 4096, PROT_READ|PROT_WRITE|PROT_EXEC);

    // Finally, use our function pointer to jump into our payload.
    puts("\n\033[33;1m---------[ Begin Shellcode Execution ]---------\033[0m");
    (*shell)();

    // We likely won't get here, but might as well include it just in case.
    puts("\033[33;1m---------[  End Shellcode Execution  ]---------\033[0m");
    return 0;
}
