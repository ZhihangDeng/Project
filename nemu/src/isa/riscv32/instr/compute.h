def_EHelper(lui) {
  rtl_li(s, ddest, id_src1->imm);
}

def_EHelper(addi) {
  rtl_addi(s, ddest, dsrc1, id_src2->imm);
}

def_EHelper(auipc) {
  rtl_addi(s, ddest, &s->pc, id_src1->imm);
}

def_EHelper(add) {
  rtl_add(s, ddest, dsrc1, dsrc2);
}

def_EHelper(sub) {
  rtl_sub(s, ddest, dsrc1, dsrc2);
}

def_EHelper(sltiu) {
  //rtl_setrelopi(RELOP_LTU, ddest, dsrc1, id_src2->imm);
  interpret_relop(RELOP_LTU, *dsrc1, id_src2->imm);
  Log("%d %d\n", *dsrc1, id_src2->imm);
}