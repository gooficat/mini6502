# a 6502 assembler, written from scratch, in pure, compliant c

this is a 6502 assembler I made to test some programming patterns for a larger project
it supports the full core 6502 instruction set, taken procedurally from the opcode table on <https://www.masswerk.at/6502/6502_instruction_set.html>

In this repo, you will find the tools (except an online html tag stripper on w3docs.org) I used to create this

you may build the project with cmake, and, as I did, test it with <https://www.masswerk.at/6502/>

program usage: `/path/to/mini6502 <input file> <output file>`

to use the emulator i linked you will need to hex dump the output. use `format-hex` on powershell

compilation only tested on msvc but should be 100% compliant

the syntax is as follows

 - `%`, followed by a number, is a register. this is a convention i am testing. `%0` is A, `%1` is X, `%2` is Y
 - `#` precedes an immediate
 - `$` precedes a memory address
 - `:labelname` for labels
 - there are several directives:
 -- .byte puts a byte
 -- .dbyte puts a double-byte (2 bytes)
 -- .org for the organization
 -- .align to align with a padding byte (.align 0 512 aligns to 512 with a padding of 0x00)
 -- .defnum makes a pseudo-label with the value specified (.defnum foo 0x67)
 --