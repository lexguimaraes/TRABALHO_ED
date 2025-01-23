#include "ARVBP.h"

typedef struct lista_set_imovel {
    TIM* imovel;
    struct lista_set_imovel* next;
} TLSETIM;

TLSETIM* TLSETIM_inicializa();

TLSETIM* TLSETIM_insere(TLSETIM* l, TIM* IM);

void TLSETIM_libera(TLSETIM* l);

void TLSETIM_imprime(TLSETIM* l);

