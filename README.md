# cm2-riscv-emulator
> This is the repo containing a RV32IZicsr emulator + other periphereals (full computer) written in C + ncurses + Raylib
>
> Endgoal: To emulate a Roblox CPU inside the game


## Building

1. run `make compile`
2. run './build/cm2-riscv-emulator image.bin'

> If you dont have raylib installed you can compile with `RAYLIB=false`

## Debugger

The emulator has a debugger console that can be accessed by pressing `ctrl+w`.

### Commands

| Command | Description |
|---------|-------------|
| `exit` | exit the emulator |
| `clear` | clear the debugger console |
| `break pop` | remove the last breakpoint |
| `break pop all` | remove all breakpoints |
| `break pop <n>` | pop n times the breakpoint stack |
| `break rm <address>` | remove the breakpoint with that address |
| `break continue` | continue exectution from a breakpoint |
| `break <type (pc or mem)> <address>` | add a breakpoint at the address given with type, accepts hex, binary and decimal. |
| `break ls` | list breakpoints |
| `cpu speed <speed>` | configure the speed of the cpu. |
| `cpu jmp <address>` | make the cpu jump to the address in memory |


| Variable | Description | Usage |
|----------|-------------|-------|
| `pc` | value of program counter | address |
| `ia` | value of last address the cpu loaded/stored to | address |

Example
`break pc pc` 
place a PC type breakpoint that is the value of the program counter. This will cause the cpu to hit a breakpoint

Use enter to submit the command, backspace is supported.
