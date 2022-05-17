def_EHelper(jal) {
    rtl_addi(s, ddest, &s->pc, 4);
    rtl_addi(s, &s->snpc, &s->pc, id_src1->imm);
}