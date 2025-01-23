#include "../headers/extras.h"
#include <math.h>
#include <stdlib.h>

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

TLSETIM* imoveis_maior_preco(TARVBP*a, int t){
    TARVBP* aux = prim_folha(a,t);
    TLSETIM* res = NULL;
    float maior = 0,b;
    while(aux){
        for(int i = 0; i < aux->n_chaves;i++){
            b = aux->reg[i]->preco_total;
            if(b == maior){
                res = TLSETIM_insere(res, aux->reg[i]);
            }
            if(b>maior){
                maior = b;
                TLSETIM_libera(res);
                res = NULL;
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

TLSETIM* imoveis_menor_preco(TARVBP*a, int t){
    TARVBP* aux = prim_folha(a,t);
    TLSETIM* res = NULL;
    float menor = INT_MAX,b;
    while(aux){
        for(int i = 0; i < aux->n_chaves;i++){
            b = aux->reg[i]->preco_total;
            if(b == menor){
                res = TLSETIM_insere(res, aux->reg[i]);
            }
            if(b<menor){
                menor = b;
                TLSETIM_libera(res);
                res = NULL;
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

// Função para verificar se dois imóveis são idênticos
int imoveis_identicos(TIM* im1, TIM* im2) {
    if (!im1 || !im2) return 0;
    
    // Primeiro compara o ID usando unsigned long int para evitar overflow
    if (im1->id != im2->id) return 0;
    
    // Se os IDs são iguais, compara o resto dos campos
    return (strcmp(im1->bairro, im2->bairro) == 0 &&
            strcmp(im1->tipo, im2->tipo) == 0 &&
            strcmp(im1->rua, im2->rua) == 0 &&
            im1->num == im2->num &&
            im1->preco_total == im2->preco_total &&
            im1->preco_por_m2 == im2->preco_por_m2 &&
            strcmp(im1->CEP, im2->CEP) == 0 &&
            strcmp(im1->latitude, im2->latitude) == 0 &&
            strcmp(im1->longitude, im2->longitude) == 0);
}

TLSETIM* imoveis_string(TARVBP* a, int t, int (*comp)(TIM*, char*), char* str) {
    printf("[DEBUG] Iniciando busca por %s\n", str);
    
    if (!a) return NULL;
    
    TARVBP* aux = prim_folha(a, t);
    TLSETIM* res = NULL;
    int encontrados = 0;
    
    if (!aux) {
        return NULL;
    }
    
    while(aux) {
        printf("[DEBUG] Analisando folha %s com %d chaves\n", aux->arq, aux->n_chaves);
        for(int i = 0; i < aux->n_chaves; i++) {
            if (!aux->reg[i]) {
                printf("[DEBUG] Registro %d é nulo\n", i);
                continue;
            }
            
            printf("[DEBUG] Comparando %s com %s\n", aux->reg[i]->bairro, str);
            if (!comp(aux->reg[i], str)) {
                printf("[DEBUG] Match encontrado!\n");
                
                // Verifica se este imóvel já existe na lista de resultados
                int imovel_existe = 0;
                TLSETIM* temp = res;
                while (temp) {
                    if (imoveis_identicos(temp->imovel, aux->reg[i])) {
                        imovel_existe = 1;
                        break;
                    }
                    temp = temp->next;
                }
                
                if (!imovel_existe) {
                    encontrados++;
                    res = TLSETIM_insere(res, aux->reg[i]);
                }
            }
        }
        
        if (aux->prox[0] == '\0') {
            printf("[DEBUG] Fim das folhas\n");
            break;
        }
        
        printf("[DEBUG] Indo para próxima folha: %s\n", aux->prox);
        TARVBP* temp = aux;
        aux = le_no(aux->prox, t);
        
        if (!aux || aux->n_chaves == 0) {
            printf("[DEBUG] Próxima folha inválida ou vazia\n");
            if (a != temp) TARVBP_libera(temp, t);
            break;
        }
        
        if (a != temp) TARVBP_libera(temp, t);
    }
    
    printf("[DEBUG] Busca concluída. Encontrados %d imóveis únicos.\n", encontrados);
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

TARVBP* edita(TARVBP* a, int t,unsigned long int id, char* str){
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

// FUN��ES DE COMPARA��O
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

int prox_de_aux(double distanciaMax, double lat1, double long1, double lat2, double long2){
    // Raio da Terra em km
    const double R = 6371.0;

    // Converte as coordenadas para radiano
    double lat1_rad = lat1 * M_PI / 180.0;
    double lon1_rad = long1 * M_PI / 180.0;
    double lat2_rad = lat2 * M_PI / 180.0;
    double lon2_rad = long2 * M_PI / 180.0;

    // deltaLat e deltaLong
    double dlat = lat2_rad - lat1_rad;
    double dlon = lon2_rad - lon1_rad;

    // Calcula a fórmula de Haversine.
    double a = pow(sin(dlat/2), 2) + cos(lat1_rad) * cos(lat2_rad) * pow(sin(dlon/2), 2);
    double c = 2 * atan2(sqrt(a), sqrt(1-a));

    double d = R * c;

    if(d <= distanciaMax) return 1;
    return 0;
}

int prox_de(double distanciaMax, TIM* imovel, double latitude, double longitude){
    double latImovel = atof(imovel->latitude);
    double longImovel = atof(imovel->longitude);
    return prox_de_aux(distanciaMax, latImovel, longImovel, latitude, longitude);
}