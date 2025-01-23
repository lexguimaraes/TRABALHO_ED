#include "extras.h"
#include <math.h>
#include <stdlib.h>
#include <limits.h>
#include "DESC.h"

#define M_PI 3.14159265358979323846

TARVBP* busca_arq(TARVBP* a, char* arq, int t){
    TARVBP* aux = prim_folha(a,t);
    while(aux){
        if(strcmp(aux->arq,arq)==0)return aux;
        TARVBP* temp = le_no(aux->prox,t);
        if(a!= temp)
            TARVBP_libera(temp,t);
    }
    return NULL;
}

int maior_desc(TARVBP*a, int t){
    TARVBP* aux = prim_folha(a,t);
    int maior = 0,b;
    while(aux){
        for(int i = 0; i < aux->n_chaves;i++){
            b = strlen(aux->reg[i]->tipo);
            if(b>maior)maior = b;
        }
        TARVBP* temp = aux;
        aux = le_no(aux->prox, t);
        if(a!=temp)
            TARVBP_libera(temp,t);
    }
    return maior;
}

TLSETIM* imoveis_maior_preco(TARVBP*a, int t, int venda){
    TARVBP* aux = prim_folha(a,t);
    TLSETIM* res = NULL;
    float maior = 0,b;
    while(aux){
        for(int i = 0; i < aux->n_chaves;i++){
            b = aux->reg[i]->preco_total;
            DadosImovel dados = extrair_dados(aux->reg[i]->descricao);
            if(venda){
                if(b == maior && dados.venda){
                    res = TLSETIM_insere(res, aux->reg[i]);
                }
                if(b>maior && dados.venda){
                    maior = b;
                    TLSETIM_libera(res);
                    res = NULL;
                    res = TLSETIM_insere(res, aux->reg[i]);
                }
            }
            else{
                if(b == maior && dados.aluguel){
                    res = TLSETIM_insere(res, aux->reg[i]);
                }
                if(b>maior && dados.aluguel){
                    maior = b;
                    TLSETIM_libera(res);
                    res = NULL;
                    res = TLSETIM_insere(res, aux->reg[i]);
                }
            }


        }
        TARVBP* temp = aux;
        aux = le_no(aux->prox, t);
        if(a!=temp)
            TARVBP_libera(temp,t);
    }
    return res;
}

TLSETIM* imoveis_menor_preco(TARVBP*a, int t, int venda){
    TARVBP* aux = prim_folha(a,t);
    TLSETIM* res = NULL;
    float menor = INT_MAX,b;
    while(aux){
        for(int i = 0; i < aux->n_chaves;i++){
            b = aux->reg[i]->preco_total;
            DadosImovel dados = extrair_dados(aux->reg[i]->descricao);
            if (venda){
                if(b == menor && dados.venda){
                    res = TLSETIM_insere(res, aux->reg[i]);
                }
                if(b<menor && dados.venda){
                    menor = b;
                    TLSETIM_libera(res);
                    res = NULL;
                    res = TLSETIM_insere(res, aux->reg[i]);
                }
            }else{
                if(b == menor && dados.aluguel){
                    res = TLSETIM_insere(res, aux->reg[i]);
                }
                if(b<menor && dados.aluguel) {
                    menor = b;
                    TLSETIM_libera(res);
                    res = NULL;
                    res = TLSETIM_insere(res, aux->reg[i]);
                }
            }


        }
        TARVBP* temp = aux;
        aux = le_no(aux->prox, t);
        if(a!=temp)
            TARVBP_libera(temp,t);
    }
    return res;
}

TLSETIM* imoveis_string(TARVBP*a, int t,int (*comp)(TIM*,char*),char* str){
    TARVBP* aux = prim_folha(a,t);
    TLSETIM* res = NULL;

    while(aux){
        for(int i = 0; i < aux->n_chaves;i++) {
            if (!comp(aux->reg[i], str)) {
                res = TLSETIM_insere(res, aux->reg[i]);
            }
        }
        TARVBP* temp = aux;
        aux = le_no(aux->prox, t);
        if(a!=temp)
            TARVBP_libera(temp,t);
    }
    return res;
}

TLSETIM* imoveis_faixa_de_preco(TARVBP* a, int t, float lim_inf, float lim_sup){
    TARVBP* aux = prim_folha(a,t);
    TLSETIM* res = NULL;
    while(aux){
        for(int i = 0; i < aux->n_chaves;i++) {
            if(aux->reg[i]->preco_total >= lim_inf && aux->reg[i]->preco_total <= lim_sup){
                res = TLSETIM_insere(res, aux->reg[i]);
            }
        }
        TARVBP* temp = aux;
        aux = le_no(aux->prox, t);
        if(a!=temp)
            TARVBP_libera(temp,t);
    }
    return res;
}

TARVBP* retira_imoveis(TARVBP* a, int t, TLSETIM* IMs){
    TLSETIM* aux = IMs;
    while(aux){
        a = TARVBP_retira(a, aux->imovel->id, t);
        aux = aux->next;
    }
    TLSETIM_libera(IMs);
    return a;
}

TARVBP* remove_imoveis_faixa_de_preco(TARVBP* a, int t, float lim_inf, float lim_sup){
    TLSETIM* l = imoveis_faixa_de_preco(a,t,lim_inf,lim_sup);
    a = retira_imoveis(a,t,l);
    return a;
}

TLSETIM* imoveis_metragem(TARVBP* a, int t,int m){
    TARVBP* aux = prim_folha(a,t);
    TLSETIM* res = NULL;
    int m2_aprox;
    while(aux){
        for(int i = 0; i < aux->n_chaves;i++) {
            m2_aprox = aux->reg[i]->preco_total/aux->reg[i]->preco_por_m2;
            int leniencia = m2_aprox/100; //LENIENCIA PARA O ARREDONDAMENTO E PARA IMOVEIS MAIORES QUE NAO TEM OS M2 CALCULADOS TAO PRECISAMENTE
            if(leniencia < 2)leniencia = 2;
            if(abs(m2_aprox - m)<leniencia) {
                //printf("\n%d\n",m2_aprox);
                res = TLSETIM_insere(res, aux->reg[i]);
            }
        }
        TARVBP* temp = aux;
        aux = le_no(aux->prox, t);
        if(a!=temp)
            TARVBP_libera(temp,t);
    }
    return res;
}

TARVBP* remove_imoveis_metragem(TARVBP* a, int t, int m){
    TLSETIM* aux = imoveis_metragem(a,t,m);
    a = retira_imoveis(a,t,aux);
    return a;
}

TARVBP* edita_latitude(TARVBP* a, int t,unsigned long int id, char* str){
    TARVBP* aux = TARVBP_busca(a,id,t);
    for(int i = 0;i<aux->n_chaves;i++){
        if (aux->reg[i]->id == id){
            strcpy(aux->reg[i]->latitude,str);
            esc_no(aux->arq,aux);
            if(aux!=a)
                TARVBP_libera(aux,t);
            return a;
        }
    }
    return a;
}



TARVBP* edita_longitude(TARVBP* a, int t,unsigned long int id, char* str){
    TARVBP* aux = TARVBP_busca(a,id,t);
    for(int i = 0;i<aux->n_chaves;i++){
        if (aux->reg[i]->id == id){
            strcpy(aux->reg[i]->longitude,str);
            esc_no(aux->arq,aux);
            if(aux!=a)
                TARVBP_libera(aux,t);
            return a;
        }
    }
    return a;
}





TARVBP* edita_num(TARVBP* a, int t,unsigned long int id, int num){
    TARVBP* aux = TARVBP_busca(a,id,t);
    for(int i = 0;i<aux->n_chaves;i++){
        if (aux->reg[i]->id == id){
            aux->reg[i]->num = num;
            esc_no(aux->arq,aux);
            if(aux!=a)
                TARVBP_libera(aux,t);
            return a;
        }
    }
    return a;
}




TARVBP* edita_rua(TARVBP* a, int t,unsigned long int id, char* str){
    TARVBP* aux = TARVBP_busca(a,id,t);
    for(int i = 0;i<aux->n_chaves;i++){
        if (aux->reg[i]->id == id){
            strcpy(aux->reg[i]->rua,str);
            esc_no(aux->arq,aux);
            if(aux!=a)
                TARVBP_libera(aux,t);
            return a;
        }
    }
    return a;
}





TARVBP* edita_tipo(TARVBP* a, int t,unsigned long int id, char* str){
    TARVBP* aux = TARVBP_busca(a,id,t);
    for(int i = 0;i<aux->n_chaves;i++){
        if (aux->reg[i]->id == id){
            strcpy(aux->reg[i]->tipo,str);
            esc_no(aux->arq,aux);
            if(aux!=a)
                TARVBP_libera(aux,t);
            return a;
        }
    }
    return a;
}




TARVBP* edita_bairro(TARVBP* a, int t,unsigned long int id, char* str){
    TARVBP* aux = TARVBP_busca(a,id,t);
    for(int i = 0;i<aux->n_chaves;i++){
        if (aux->reg[i]->id == id){
            strcpy(aux->reg[i]->bairro,str);
            esc_no(aux->arq,aux);
            if(aux!=a)
                TARVBP_libera(aux,t);
            return a;
        }
    }
    return a;
}


TARVBP* edita_cep(TARVBP* a, int t,unsigned long int id, char* str){
    TARVBP* aux = TARVBP_busca(a,id,t);
    for(int i = 0;i<aux->n_chaves;i++){
        if (aux->reg[i]->id == id){
            strcpy(aux->reg[i]->CEP,str);
            esc_no(aux->arq,aux);
            if(aux!=a)
                TARVBP_libera(aux,t);
            return a;
        }
    }
    return a;
}

TARVBP* edita_descricao(TARVBP* a, int t,unsigned long int id, char* str){
    TARVBP* aux = TARVBP_busca(a,id,t);
    for(int i = 0;i<aux->n_chaves;i++){
        if (aux->reg[i]->id == id){
            strcpy(aux->reg[i]->descricao,str);
            esc_no(aux->arq,aux);
            if(aux!=a)
                TARVBP_libera(aux,t);
            return a;
        }
    }
    return a;
}

TARVBP* edita_preco_total(TARVBP* a, int t, unsigned long int id, float novo_preco_total){
    TARVBP* aux = TARVBP_busca(a,id,t);
    for(int i = 0;i<aux->n_chaves;i++){
        if (aux->reg[i]->id == id){
            aux->reg[i]->preco_total = novo_preco_total;
            esc_no(aux->arq,aux);
            if(aux!=a)
                TARVBP_libera(aux,t);
            return a;
        }
    }
    return a;
}

TARVBP* edita_preco_porM2(TARVBP* a, int t, unsigned long int id, float novo_preco_porM2){
    TARVBP* aux = TARVBP_busca(a,id,t);
    for(int i = 0;i<aux->n_chaves;i++){
        if (aux->reg[i]->id == id){
            aux->reg[i]->preco_por_m2 = novo_preco_porM2;
            esc_no(aux->arq,aux);
            if(aux!=a)
                TARVBP_libera(aux,t);
            return a;
        }
    }
    return a;
}

// FUNÇÕES DE COMPARAÇÃO
int comp_bairro(TIM* IM, char* bairro){
    return strcmp(IM->bairro,bairro);
}

int comp_tipo(TIM* IM, char* tipo){
    return strcmp(IM->tipo,tipo);
}

int comp_rua(TIM* IM, char* rua){
    return strcmp(IM->rua,rua);
}

int comp_latitude(TIM* IM, char* latitude){
    return strcmp(IM->latitude,latitude);
}

int comp_longitude(TIM* IM, char* longitude){
    return strcmp(IM->longitude,longitude);
}

int comp_cep(TIM* IM, char* cep){
    return strcmp(IM->CEP,cep);
}

int comp_desc(TIM* IM, char* desc){
    return strcmp(IM->descricao,desc);
}

double prox_de_aux(double distanciaMax, double lat1, double long1, double lat2, double long2){
    //raio da Terra
    const double R = 6371.0;

    double lat1_rad = lat1 * M_PI / 180.0;
    double lon1_rad = long1 * M_PI / 180.0;
    double lat2_rad = lat2 * M_PI / 180.0;
    double lon2_rad = long2 * M_PI / 180.0;

    double dlat = lat2_rad - lat1_rad;
    double dlon = lon2_rad - lon1_rad;

    double a = pow(sin(dlat/2), 2) + cos(lat1_rad) * cos(lat2_rad) * pow(sin(dlon/2), 2);
    double c = 2 * atan2(sqrt(a), sqrt(1-a));

    double d = R * c;
    if(d> distanciaMax)return 0;
    return d;
}

double prox_de(double distanciaMax, TIM* imovel, double latitude, double longitude){
    double latImovel = strParaFloat(imovel->latitude);
    double longImovel = strParaFloat(imovel->longitude);
    return prox_de_aux(distanciaMax, latImovel, longImovel, latitude, longitude);
}

TLSETIM* imoveis_prox_de(TARVBP* a, int t, double distanciaMax, double latitude, double longitude){
    TARVBP* aux = prim_folha(a,t);
    TLSETIM* res = NULL;
    while(aux){
        for(int i = 0; i < aux->n_chaves;i++) {
            double d = prox_de(distanciaMax,aux->reg[i],latitude,longitude);
            if(d){
                printf("\nImovel a %.3f kilometros de distancia:\n",d);
                imprime_imovel(aux->reg[i]);
                res = TLSETIM_insere(res, aux->reg[i]);

            }
        }
        TARVBP* temp = aux;
        aux = le_no(aux->prox, t);
        if(a!=temp)
            TARVBP_libera(temp,t);
    }
    return res;
}

char* substituir_virgula_por_ponto(char *str) {
    int i;
    for (i = 0; str[i] != '\0'; i++) {
        if (str[i] == ',') {
            str[i] = '.';
        }
    }
    return str;
}


void abrir_google_maps(TARVBP* a, int t, unsigned long int id){
    TARVBP* aux = TARVBP_busca(a,id,t);
    char* latitude = substituir_virgula_por_ponto(aux->reg[0]->latitude);
    char* longitude = substituir_virgula_por_ponto(aux->reg[0]->longitude);

    printf("https://www.google.com/maps/@%s,%s,15z", latitude, longitude);
}