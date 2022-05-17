def_EHelper(jal) {
    rtl_addi(s, ddest, &s->pc, 4);
    rtl_addi(s, &s->dnpc, &s->pc, id_src1->imm);
}

def_EHelper(jalr) {
    rtl_addi(s, ddest, &s->pc, 4);
    Log("0x%08x\n", *ddest);
    rtl_addi(s, &s->dnpc, dsrc1, id_src2->imm);
    Log("0x%08x\n", s->dnpc);
    rtl_andi(s, &s->dnpc, &s->dnpc, 0xfffffffe);
    Log("0x%08x\n", s->dnpc);
}