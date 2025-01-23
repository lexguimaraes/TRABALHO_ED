#include "../headers/TLSEIM.h"

TLSETIM* TLSETIM_inicializa(){
    return NULL;
}

TLSETIM* TLSETIM_insere(TLSETIM* l, TIM* IM){
    // Verifica se o imóvel já existe na lista pelo ID (que é único)
    TLSETIM* atual = l;
    while (atual) {
        if (atual->imovel->id == IM->id) {
            return l; // Imóvel já existe, retorna a lista sem modificar
        }
        atual = atual->next;
    }
    
    // Se chegou aqui, o imóvel não existe na lista
    TLSETIM* novo = (TLSETIM*)malloc(sizeof(TLSETIM));
    novo->imovel = copia_imovel(IM); // Mantém a cópia profunda para segurança
    novo->next = l;
    return novo;
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