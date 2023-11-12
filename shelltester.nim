GNU nano 7.2                           shelltester.nim
import std/[posix, strformat]
import std/strutils

var
  payload = array[28, byte] [
  byte 0x01,0x30,0x8f,0xe2,0x13,0xff,0x2f,0xe1,0x02,0xa0,0x49,0x40,0x52,0x40,0xc2,0x71,0x0b,0x27,0x01,0xdf,0x2f,0x62,0x69,0x6e,0x2f,0x73,0x68,0x58
  ]

  (prot, flag) = (PROT_WRITE or PROT_EXEC, MAP_ANONYMOUS or MAP_PRIVATE)
  tstptr = mmap(nil, sizeof payload, prot, flag, -1, 0)
  tstptr2: pointer
  memad: int

if tstptr != nil:
  echo "Mémoire allouée avec succès a :",tstptr.repr
else:                                                                                     echo "Erreur lors de l'allocation de mémoire"
copyMem(tstptr, payload[0].addr, sizeof payload)
cast[proc(){.cdecl.}](tstptr)()                                                         echo payload.repr
echo tstptr.repr # prints hello, world!
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
