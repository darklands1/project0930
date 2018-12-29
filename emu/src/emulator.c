/* The core of the emulator. */
#include "emulator.h"

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

/*
    Initialize components for emulation
 */
cpu_t *init_emu(word *rom) {
    if (sizeof(rom) != sizeof(MEMORY_MAX) * sizeof(word)) {
        error(1, "Invalid ROM", true);
    }

    cpu_t *cpu = malloc(sizeof(cpu_t));
    memset(cpu, 0, sizeof(cpu));

    for (int i = 0; i < sizeof(rom); i++) {
        cpu->m[i] = rom[i];
    }
    
    return cpu;
}

/* 
   Executes the operation at memory location PC
*/
void exec_op(cpu_t *cpu) {
    word opw = cpu->m[cpu->r[PC]];
    op_t *op = construct_op(opw);
    if (ops[op->c][op->o] == NULL) {
        error(1, cpu->r[PC], "Invalid opcode", true);
    }
    ops[op->c][op->o](cpu, op->x, op->y);
}

/*
   Main emulator loop
 */
void emulate(cpu_t *cpu) {
    cpu->running = true;
    while (cpu->running) {
        exec_op(cpu);

        cpu->r[PC]++;
        if (cpu->r[PC] >= PROGRAM_MAX - 1) {
            cpu->running = false; // Halt processing if PC is out of bounds
        }
    }

    dump(cpu);
}

void dump(cpu_t *cpu) {
    for (int i = 0; i < 16; i++) {
        printf("R%d: 0x%x\n", i, cpu->r[i]);
    }
}

void error(int code, int location, const char *desc, bool fatal) {
    printf("An exception occured at 0x%x.\n", location);
    printf("Code: 0x%x\n", code);
    printf("Description: %s\n", desc);
    if (fatal) {
        printf("Program cannot continue.\n");
        exit(code);
    }
}