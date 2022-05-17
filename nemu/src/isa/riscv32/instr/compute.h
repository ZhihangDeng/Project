def_EHelper(lui) {
  rtl_li(s, ddest, id_src1->imm);
}

def_EHelper(auipc) {
  rtl_slli(s, ddest, id_src1->preg, id_src2->imm);
  rtl_add(s, ddest, ddest, &s->pc);
}