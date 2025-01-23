#include "TLSEIM.h"

TLSETIM* TLSETIM_inicializa(){
    return NULL;
}

TLSETIM* TLSETIM_insere(TLSETIM* l, TIM* IM){
    TLSETIM* new = malloc(sizeof(TLSETIM));
    new->next = l;
    new->imovel = copia_imovel(IM);
    return new;
}

void TLSETIM_libera(TLSETIM* l){
    TLSETIM* p = l;
    while(p){
        TLSETIM* temp = p->next;
        free(p->imovel);
        free(p);
        p = temp;
    }
}

void TLSETIM_imprime(TLSETIM* l){
    while(l){
        imprime_imovel(l->imovel);
        l = l->next;
    }
}