#ifndef ARVBP_H
#define ARVBP_H

#include "TIM.h"

typedef struct arvbpimovel{
    char* arq;          //nome do arquivo do nó
    int folha;              //verifica se é folha ou não
    int n_chaves;         //numero de chaves do nó
    unsigned long int* chaves;   //id (unsigned long int) porque o id excede o INT_MAX
    TIM** reg;              //lista de ponteiros de TIMovel para os imóveis das folhas (Acesso direto aos dados do imóvel)
    char** filhos;          //nomes dos arquivos dos filhos [lista de strings]
    char* prox;             //caso seja folha, tem o ponteiro para o nome do próximo arquivo
}TARVBP;

TARVBP* TARVBP_cria(char* arq, int t);

TARVBP* TARVBP_insere(TARVBP* a, TIM* IM, int t);

TARVBP* TARVBP_busca(TARVBP* a, unsigned long int id, int t);

TARVBP* TARVBP_retira(TARVBP* a, unsigned long int id, int t);

void TARVBP_libera(TARVBP* a, int t);

void reg_imp(TARVBP* a, int t);

void no_imp(TARVBP* a);

void TARVBP_imprime(TARVBP *a, int t);

TARVBP* le_no(char* arq, int t);

void esc_no(char* novo_arq, TARVBP* no);

int TARVBP_tem_id(TARVBP *a, unsigned long int id, int t);

TIM* buscaImovel_id(TARVBP* a, unsigned long int id, int t);

TARVBP* prim_folha(TARVBP* a, int t);

void ler_filho(FILE *f, char* str, int* tam);

void del_dados(void);

TARVBP* cat2a(int t);

void TARVBP_imprime_imovel_primeira_folha(TARVBP* a, int t);

TARVBP* remover(TARVBP* a, unsigned long int id, int t);

#endif // ARVBP_H
