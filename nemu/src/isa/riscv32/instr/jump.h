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
    rtl_jrelop(s, RELOP_EQ, dsrc1, dsrc2, s->pc+id_dest->simm);
}

def_EHelper(bne) {
    rtl_jrelop(s, RELOP_NE, dsrc1, dsrc2, s->pc+id_dest->simm);
}

def_EHelper(bge) {
    rtl_jrelop(s, RELOP_GE, dsrc1, dsrc2, s->pc+id_dest->simm);
}

def_EHelper(blt) {
    rtl_jrelop(s, RELOP_LT, dsrc1, dsrc2, s->pc+id_dest->simm);
}

def_EHelper(bltu) {
    rtl_jrelop(s, RELOP_LTU, dsrc1, dsrc2, s->pc+id_dest->simm);
}

def_EHelper(bgeu) {
    rtl_jrelop(s, RELOP_GEU, dsrc1, dsrc2, s->pc+id_dest->simm);
}