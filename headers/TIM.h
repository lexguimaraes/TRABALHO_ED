#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//struct do im�vel
typedef struct Imovel {
    unsigned long int id;
    char bairro[50];
    char tipo[50];
    char rua[50];
    int num;
    float preco_total;
    float preco_por_m2;
    char descricao[2500];  // A MAIOR � 2495, FUN��O PARA CHECAR EM TARVBP.h
    char CEP[20];
    char latitude[20];
    char longitude[20];
} TIM;

float strParaFloat(char* str);

char* strsep(char **stringp, const char *delim);

TIM* ler_imovel(char* linha);

void imprime_imovel(TIM* imovel);

void ler_binario(char* arq);

void salva(TIM* imovel, int indice);

TIM* copia_imovel(TIM* imovel);