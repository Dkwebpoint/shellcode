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

