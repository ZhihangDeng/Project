def_EHelper(jal) {
    rtl_addi(s, ddest, &s->pc, 4);
    Log("0x%08x\n", s->snpc);
    rtl_addi(s, &s->snpc, &s->pc, id_src1->imm);
    Log("0x%08x\n", s->snpc);
}