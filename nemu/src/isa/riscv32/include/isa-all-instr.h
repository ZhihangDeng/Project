#include <cpu/decode.h>
#include "../local-include/rtl.h"

#define INSTR_LIST(f) f(lui) f(lw) f(sw) f(inv) f(nemu_trap) f(auipc) f(jal) f(addi) f(jalr) f(add) f(sub) f(sltiu) f(beq) f(bne) f(mul) f(div) f(bge) f(andi) f(rem) f(lh) f(lhu)

def_all_EXEC_ID();
