#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "DESC.h"

// Tabela de conversão de números por extenso
static const struct {
    const char *palavra;
    int valor;
} NUMEROS[] = {
    {"hum", 1}, {"um", 1}, {"uma", 1},
    {"dois", 2}, {"duas", 2},
    {"tres", 3}, {"três", 3},
    {"quatro", 4}, {"cinco", 5},
    {"seis", 6}, {"sete", 7}, {"oito", 8},
    {"nove", 9}, {"dez", 10}, {"onze", 11},
    {"doze", 12}, {"treze", 13}, {"catorze", 14},
    {"quinze", 15}, {"vinte", 20}, {"trinta", 30}
};

// Função para normalizar strings
void normalizar_string(char *str) {
    const char *com_acento = "ÁÀÃÂáàãâÉÊéêÍíÓÔÕóôõÚÜúüÇç";
    const char *sem_acento = "AAAAaaaaEEeeIiOOOoooUUuUcC";
    
    for(int i = 0; str[i]; i++) {
        // Converter para minúsculo
        str[i] = tolower(str[i]);
        
        // Remover acentos
        for(int j = 0; com_acento[j]; j++) {
            if(str[i] == com_acento[j]) {
                str[i] = sem_acento[j];
                break;
            }
        }
    }
}

// Função para converter palavras em números
int palavra_para_numero(const char *palavra) {
    for(size_t i = 0; i < sizeof(NUMEROS)/sizeof(NUMEROS[0]); i++) {
        if(strcmp(palavra, NUMEROS[i].palavra) == 0) {
            return NUMEROS[i].valor;
        }
    }
    return -1;
}

DadosImovel extrair_dados(const char *descricao) {
    DadosImovel dados = {0};
    char buffer[2500];
    char *token, *next, *resto;
    long num;

    strncpy(buffer, descricao, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';
    normalizar_string(buffer);

    // Verificação de padrões (aluguel, financiamento, etc.)
    dados.venda = strstr(buffer, "venda") || (strstr(buffer,"vende") || (strstr(buffer,"vendo")));
    dados.aluguel = strstr(buffer, "aluguel") || strstr(buffer, "locacao");
    dados.financiamento = strstr(buffer, "financiamento");
    dados.reforma = strstr(buffer, "reforma") || strstr(buffer, "modernizacao");
    dados.comercial = strstr(buffer, "comercial") || strstr(buffer, "clinica");
    dados.academia = strstr(buffer, "academia") || strstr(buffer, "musculaçao");
    dados.piscina = strstr(buffer, "piscina");
    dados.elevador = strstr(buffer, "elevador");
    dados.quadra = strstr(buffer, "quadra");
    dados.churrasqueira = strstr(buffer, "churrasqueira");
    dados.portaria = strstr(buffer, "portaria") || (strstr(buffer, "porteiro"));
    dados.portaria24h = strstr(buffer, "24h") || strstr(buffer,"24 horas");
    dados.metro_perto = strstr(buffer, "metro") || strstr(buffer, "metros");
    dados.arborizadas = strstr(buffer, "arborizada") || strstr(buffer, "arborizado") ||  strstr(buffer, "arborizadas") || strstr(buffer, "arborizados");
    dados.shopping = strstr(buffer, "shoppings") || strstr(buffer, "shopping");
    dados.pao_de_acucar = strstr(buffer, "pao de acucar");
    dados.lagoa = strstr(buffer, "lagoa");
    dados.escola = strstr(buffer, "escola") || strstr(buffer, "colegio");
    dados.praia_b = strstr(buffer, "praia de botafogo") || strstr(buffer,"praia botafogo");
    dados.praia_i = strstr(buffer, "praia de ipanema") || strstr(buffer,"praia ipanema");
    dados.praia_c = strstr(buffer, "praia de copacabana") || strstr(buffer,"praia copacabana");
    dados.cristo = strstr(buffer, "cristo") || strstr(buffer, "corcovado");

    // Tokenização com delimitadores estendidos
    token = strtok_r(buffer, " ,.-;()\t\n", &next);
    while (token != NULL) {
        // Tentar converter para número (palavra ou numeral)
        int valor = palavra_para_numero(token);
        if (valor == -1) {
            num = strtol(token, &resto, 10);
            if (resto != token) { // Conversão bem-sucedida para número
                valor = (int)num;
            }
        }


        // Verificar contexto no próprio token
        if (valor > 0) {
            if (strstr(token, "quarto") || strstr(token, "quartos")) {
                dados.quartos = valor;
            } else if (strstr(token, "banheiro") || strstr(token, "banheiros")) {
                dados.banheiros = valor;
            } else if (strstr(token, "vaga") || strstr(token, "vagas")) {
                dados.vagas = valor;
            } else if (strstr(token, "m2")) {
                dados.area_util = valor;
            } else if(strstr(token,"dormitorios") || strstr(token,"dormitorio")) {
                dados.dormitorios = valor;
            } else if(strstr(token,"suite") || strstr(token,"suites")){
                dados.suites = valor;
            }
            else {
                    // Verificar no próximo token
                    char *proximo = strtok_r(NULL, " ,.-;()\t\n", &next);
                    if (proximo) {
                        if (strstr(proximo, "quarto") || strstr(proximo, "quartos")) {
                            dados.quartos = valor;
                        } else if (strstr(proximo, "banheiro") || strstr(proximo, "banheiros")) {
                            dados.banheiros = valor;
                        } else if (strstr(proximo, "vaga") || strstr(proximo, "vagas")) {
                            dados.vagas = valor;
                        } else if (strstr(proximo, "m2")) {
                            dados.area_util = valor;
                        }else if(strstr(proximo,"dormitorios") || strstr(proximo,"dormitorio")) {
                            dados.dormitorios = valor;
                        } else if(strstr(proximo,"suite") || strstr(proximo,"suites")){
                            dados.suites = valor;
                        }

                    }
                }

        } else {
            // Verificar formato como 190m2
            char *ptr = token;
            num = strtol(ptr, &resto, 10);
            if (resto != ptr && strstr(resto, "m2")) {
                dados.area_util = (int)num;
            }
        }

        token = strtok_r(NULL, " ,.-;()\t\n", &next);
    }

    return dados;
}


void imprimir_info_imovel(TARVBP *arvore, unsigned long int id, int t) {
    if (arvore == NULL || t < 2) {
        printf("Parâmetros inválidos.\n");
        return;
    }

    // Primeiro vamos buscar o imóvel diretamente usando buscaImovel_id
    TIM* imovel = buscaImovel_id(arvore, id, t);
    if (imovel == NULL) {
        printf("Imóvel com ID %lu não encontrado.\n", id);
        return;
    }

    DadosImovel dados = extrair_dados(imovel->descricao);
    printf("Descrição do imóvel %lu: %s\n", id, imovel->descricao);
    printf("\nInformações adicionais do imóvel %lu:\n", id);
    if (dados.venda) printf(" - Disponível para venda\n");
    if (dados.aluguel) printf(" - Disponível para aluguel\n");
    if (dados.financiamento) printf(" - Aceita financiamento\n");
    if (dados.reforma) printf(" - Precisa de reforma/modernização\n");
    if (dados.comercial) printf(" - Permite uso comercial/clínica\n");
    if (dados.elevador) printf (" - Possui elevador\n");
    if (dados.portaria && !dados.portaria24h) printf(" - Possui portaria\n");
    if (dados.portaria24h) printf( " - Possui portaria 24h\n");
    if (dados.escola) printf(" - Perto de escolas");
    if (dados.lagoa) printf(" - Perto da Lagoa Rodrigo de Freitas");
    if (dados.praia_b) printf(" - Perto da Praia de Botafogo");
    if (dados.praia_i) printf(" - Perto da Praia de Ipanema");
    if (dados.praia_c) printf(" - Perto da Praia de Copacabana");
    if (dados.pao_de_acucar) printf (" - Possui linda vista para o Pão de Açucar\n");
    if (dados.cristo) printf(" - Possui bela vista para o Cristo Redentor");
    if (dados.arborizadas) printf( " - Ruas arborizadas");
    if (dados.shopping) printf(" - Perto do shopping");
    if (dados.metro_perto) printf(" - Fica perto do(s) metro(s) de %s\n",imovel->bairro);
    if (dados.piscina) printf(" - Possui piscina\n");
    if (dados.quadra) printf( " - Possui quadra poliesportiva\n");
    if (dados.academia) printf(" - Possui academia\n");
    if (dados.churrasqueira) printf(" - Possui churrasqueira\n");
    if (dados.suites > 0) printf (" - Suites: %d\n", dados.suites);
    if (dados.dormitorios > 0) printf(" - Dormitorios: %d\n", dados.dormitorios);
    if (dados.quartos > 0) printf(" - Quartos: %d\n", dados.quartos);
    if (dados.banheiros > 0) printf(" - Banheiros: %d\n", dados.banheiros);
    if (dados.vagas > 0) printf(" - Vagas de estacionamento: %d\n", dados.vagas);
    if (dados.area_util > 0) printf(" - Área útil: %d m²\n", dados.area_util);

    // Verifica se nenhum dado foi encontrado
    if (!dados.aluguel && !dados.financiamento && !dados.reforma && !dados.comercial &&
        dados.quartos <= 0 && dados.banheiros <= 0 && dados.vagas <= 0 && dados.area_util <= 0) {
        printf(" - Nenhuma informação adicional identificada na descrição.\n");
    }
    printf("\n");

    free(imovel); // Não esquecer de liberar o imóvel alocado
}