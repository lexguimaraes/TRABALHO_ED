#include <stdbool.h>
#include "ARVBP.h"
typedef struct {
    int quartos;
    int banheiros;
    int vagas;
    int area_util;
    int dormitorios;
    int suites;
    bool cristo;
    bool praia_i;
    bool praia_b;
    bool praia_c;
    bool lagoa;
    bool escola;
    bool shopping;
    bool pao_de_acucar;
    bool arborizadas;
    bool metro_perto;
    bool portaria;
    bool portaria24h;
    bool venda;
    bool piscina;
    bool elevador;
    bool quadra;
    bool academia;
    bool churrasqueira;
    bool aluguel;
    bool financiamento;
    bool reforma;
    bool comercial;
} DadosImovel;

void normalizar_string(char *str);
int palavra_para_numero(const char *palavra);
DadosImovel extrair_dados(const char *descricao);
void imprimir_info_imovel(TARVBP *arvore, unsigned long int id, int t);