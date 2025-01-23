#include "ARVBP.h"

typedef struct lim {
    TIM* imovel;
    struct lim* next;
} TLSETIM;

TLSETIM* TLSETIM_inicializa();

TLSETIM* TLSETIM_insere(TLSETIM* l, TIM* IM);

void TLSETIM_libera(TLSETIM* l);

void TLSETIM_imprime(TLSETIM* l);

