def_EHelper(jal) {
    rtl_addi(s, ddest, &s->pc, 4);
    rtl_addi(s, &s->dnpc, &s->pc, id_src1->simm);
}

def_EHelper(jalr) {
    rtl_addi(s, ddest, &s->pc, 4);
    rtl_addi(s, &s->dnpc, dsrc1, id_src2->simm);
    rtl_andi(s, &s->dnpc, &s->dnpc, 0xfffffffe);
}

def_EHelper(beq) {
    Log("0x%x\n", s->pc);
    rtl_jrelop(s, RELOP_EQ, dsrc1, dsrc2, s->pc+id_dest->simm);
    Log("0x%x\n", s->pc+id_dest->simm);
}