def_EHelper(jal) {
    rtl_addi(s, ddest, &s->pc, 4);
    Log("%d\n", s->snpc);
    rtl_addi(s, &s->snpc, &s->pc, id_src1->imm);
    Log("%d\n", s->snpc);
}