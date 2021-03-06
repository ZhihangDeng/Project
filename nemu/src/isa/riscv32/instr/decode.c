#include "../local-include/reg.h"
#include <cpu/ifetch.h>
#include <isa-all-instr.h>

/*
static inline int table_lui (Decode *s) 
{ return EXEC_ID_lui; } 
static inline int table_lw (Decode *s) 
{ return EXEC_ID_lw; } 
static inline int table_sw (Decode *s) 
{ return EXEC_ID_sw; } 
static inline int table_inv (Decode *s) 
{ return EXEC_ID_inv; } 
static inline int table_nemu_trap (Decode *s) 
{ return EXEC_ID_nemu_trap; };
*/
def_all_THelper();

static uint32_t get_instr(Decode *s) {
  return s->isa.instr.val;
}

// decode operand helper
#define def_DopHelper(name) \
  void concat(decode_op_, name) (Decode *s, Operand *op, word_t val, bool flag)

/*  将op->imm修改为目标立即数
static void decode_op_i (Decode *s, Operand *op, word_t val, Bool flag) {
  op->imm = val;
}
*/
static def_DopHelper(i) {
  op->imm = val;
}

/*  将op->preg修改为目的寄存器
static void decode_op_r (Decode *s, Operand *op, word_t val, Bool flag) {
  Bool is_write = flag;
  static word_t zero_null = 0;
  op->preg = (is_write && val == 0) ? &zero_null : &(cpu.gpr[check_reg_idx(val)]._32);
}
*/
static def_DopHelper(r) {
  bool is_write = flag;
  static word_t zero_null = 0;
  op->preg = (is_write && val == 0) ? &zero_null : &gpr(val);
  //op->preg = (is_write && val == 0) ? &zero_null : &(cpu.gpr[check_reg_idx(val)]._32);
}

//译码辅助函数Decode helper
//static void decode_I (Decode *s, int width)
static def_DHelper(I) {
  decode_op_r(s, id_src1, s->isa.instr.i.rs1, false);
  decode_op_i(s, id_src2, s->isa.instr.i.simm11_0, false);
  decode_op_r(s, id_dest, s->isa.instr.i.rd, true);
}

//static void decode_U (Decode *s, int width)
static def_DHelper(U) {
  decode_op_i(s, id_src1, s->isa.instr.u.imm31_12 << 12, true);
  decode_op_r(s, id_dest, s->isa.instr.u.rd, true);
}

//static void decode_S (Decode *s, int width)
static def_DHelper(S) {
  decode_op_r(s, id_src1, s->isa.instr.s.rs1, false);
  sword_t simm = (s->isa.instr.s.simm11_5 << 5) | s->isa.instr.s.imm4_0;
  decode_op_i(s, id_src2, simm, false);
  decode_op_r(s, id_dest, s->isa.instr.s.rs2, false);
}

//static void decode_J (Decode *s, int width)
static def_DHelper(J) {
  decode_op_r(s, id_dest, s->isa.instr.j.rd, true);
  sword_t simm = (s->isa.instr.j.imm20 << 20) | (s->isa.instr.j.imm19_12 << 12) | (s->isa.instr.j.imm11 << 11) | (s->isa.instr.j.imm10_1 << 1);
  simm = (simm << 12) >> 12;
  decode_op_i(s, id_src1, simm, false);
}

//static void decode_R (Decode *s, int width)
static def_DHelper(R) {
  decode_op_r(s, id_dest, s->isa.instr.r.rd, true);
  decode_op_r(s, id_src1, s->isa.instr.r.rs1, false);
  decode_op_r(s, id_src2, s->isa.instr.r.rs2, false);
}

//static void decode_B (Decode *s, int width)
static def_DHelper(B) {
  decode_op_r(s, id_src1, s->isa.instr.b.rs1, false);
  decode_op_r(s, id_src2, s->isa.instr.b.rs2, false);
  sword_t simm = (s->isa.instr.b.imm12 << 12) | (s->isa.instr.b.imm11 << 11) | (s->isa.instr.b.imm10_5 << 5) | (s->isa.instr.b.imm4_1 << 1);
  decode_op_i(s, id_dest, (simm<<19)>>19, false);
}


//static inline int table_load (Decode *s)
def_THelper(load) {
  def_INSTR_TAB("??????? ????? ????? 010 ????? ????? ??", lw);
  return EXEC_ID_inv;
}

//static inline int table_store (Decode *s)
def_THelper(store) {
  def_INSTR_TAB("??????? ????? ????? 010 ????? ????? ??", sw);
  return EXEC_ID_inv;
}

//static inline int table_main (Decode *s)  //匹配RISC-V中的I, S, U三种类型指令
def_THelper(main) {
  def_INSTR_IDTAB("0000001 ????? ????? 111 ????? 01100 11", R     , remu);
  def_INSTR_IDTAB("0000001 ????? ????? 011 ????? 01100 11", R     , mulhu);
  def_INSTR_IDTAB("??????? ????? ????? 110 ????? 00100 11", R     , ori);
  def_INSTR_IDTAB("??????? ????? ????? 000 ????? 00000 11", I     , lb);
  def_INSTR_IDTAB("0000001 ????? ????? 101 ????? 01100 11", R     , divu);
  def_INSTR_IDTAB("??????? ????? ????? 111 ????? 11000 11", B     , bgeu);
  def_INSTR_IDTAB("0000000 ????? ????? 101 ????? 01100 11", R     , srl);
  def_INSTR_IDTAB("0100000 ????? ????? 101 ????? 01100 11", R     , sra);
  def_INSTR_IDTAB("0000001 ????? ????? 001 ????? 01100 11", R     , mulh);
  def_INSTR_IDTAB("0000000 ????? ????? 111 ????? 01100 11", R     , and);
  def_INSTR_IDTAB("0100000 ????? ????? 101 ????? 00100 11", I     , srai);
  def_INSTR_IDTAB("??????? ????? ????? 000 ????? 01000 11", S     , sb);
  def_INSTR_IDTAB("0000000 ????? ????? 100 ????? 01100 11", R     , xor);
  def_INSTR_IDTAB("0000000 ????? ????? 011 ????? 01100 11", R     , sltu);
  def_INSTR_IDTAB("??????? ????? ????? 110 ????? 11000 11", B     , bltu);
  def_INSTR_IDTAB("0000000 ????? ????? 010 ????? 01100 11", R     , slt);
  def_INSTR_IDTAB("??????? ????? ????? 100 ????? 11000 11", B     , blt);
  def_INSTR_IDTAB("??????? ????? ????? 001 ????? 01000 11", S     , sh);
  def_INSTR_IDTAB("0000000 ????? ????? 101 ????? 00100 11", I     , srli);
  def_INSTR_IDTAB("??????? ????? ????? 100 ????? 00100 11", I     , xori);
  def_INSTR_IDTAB("0000000 ????? ????? 001 ????? 01100 11", R     , sll);
  def_INSTR_IDTAB("0000000 ????? ????? 110 ????? 01100 11", R     , or);
  def_INSTR_IDTAB("0000000 ????? ????? 001 ????? 00100 11", I     , slli);
  def_INSTR_IDTAB("??????? ????? ????? 100 ????? 00000 11", I     , lbu);
  def_INSTR_IDTAB("??????? ????? ????? 101 ????? 00000 11", I     , lhu);
  def_INSTR_IDTAB("??????? ????? ????? 001 ????? 00000 11", I     , lh);
  def_INSTR_IDTAB("0000001 ????? ????? 110 ????? 01100 11", R     , rem);
  def_INSTR_IDTAB("??????? ????? ????? 111 ????? 00100 11", I     , andi);
  def_INSTR_IDTAB("??????? ????? ????? 101 ????? 11000 11", B     , bge);
  def_INSTR_IDTAB("0000001 ????? ????? 100 ????? 01100 11", R     , div);
  def_INSTR_IDTAB("0000001 ????? ????? 000 ????? 01100 11", R     , mul);
  def_INSTR_IDTAB("??????? ????? ????? 001 ????? 11000 11", B     , bne);
  def_INSTR_IDTAB("??????? ????? ????? 000 ????? 11000 11", B     , beq);
  def_INSTR_IDTAB("??????? ????? ????? 011 ????? 00100 11", I     , sltiu);
  def_INSTR_IDTAB("0100000 ????? ????? 000 ????? 01100 11", R     , sub);
  def_INSTR_IDTAB("0000000 ????? ????? 000 ????? 01100 11", R     , add);
  def_INSTR_IDTAB("??????? ????? ????? ??? ????? 11001 11", I     , jalr);
  def_INSTR_IDTAB("??????? ????? ????? ??? ????? 11011 11", J     , jal);
  def_INSTR_IDTAB("??????? ????? ????? ??? ????? 00101 11", U     , auipc);
  def_INSTR_IDTAB("??????? ????? ????? 000 ????? 00100 11", I     , addi);
  def_INSTR_IDTAB("??????? ????? ????? ??? ????? 00000 11", I     , load);
  def_INSTR_IDTAB("??????? ????? ????? ??? ????? 01000 11", S     , store);
  def_INSTR_IDTAB("??????? ????? ????? ??? ????? 01101 11", U     , lui);
  def_INSTR_TAB  ("??????? ????? ????? ??? ????? 11010 11",         nemu_trap);
  return table_inv(s);  //都不匹配, 返回指令无效
};

int isa_fetch_decode(Decode *s) {
  s->isa.instr.val = instr_fetch(&s->snpc, 4);
  int idx = table_main(s);  //包含操作码译码和操作数译码，返回EXEC_ID_
  return idx;
}
