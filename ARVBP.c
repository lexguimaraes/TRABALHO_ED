#include "ARVBP.h"
#include "extras.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int nos_atual = 0; //saber o numero do arquivo

void del_dados(void){
    for(int i = 0;i<170;i++){
        char buff[4];
        sprintf(buff,"%d",i);
        char novo_nome[50];
        strcpy(novo_nome, "./dados/");
        strcat(novo_nome,buff);
        strcat(novo_nome, ".bin");
        remove(novo_nome);
    }
}

TARVBP* cat2a(int t){
    TARVBP* a = TARVBP_cria("0",t);
    TIM* IM;
    FILE* f = fopen("catalogo.csv","r");
    char linha[10000];
    fgets(linha, sizeof(linha), f);
    for(int i = 0;i<199;i++){
        fgets(linha, sizeof(linha), f);
        IM = ler_imovel(linha);
        a = TARVBP_insere(a, IM, t);
    }
    fclose(f);
    return a;
}


TARVBP *TARVBP_cria(char *arq, int t){
    TARVBP *a = (TARVBP *) malloc(sizeof(TARVBP));
    a->folha = 1;
    a->n_chaves = 0;
    a->chaves = (unsigned long int *)malloc(sizeof(unsigned long int)*(2*t-1));
    a->reg = (TIM **)malloc(sizeof(TIM *)*(2*t-1));
    for(int i=0; i<2*t-1; i++) a->reg[i] = NULL;
    a->filhos = (char **)malloc(sizeof(char *)*2*t);
    for(int i=0; i < 2*t; i++) {
        a->filhos[i] = (char *)malloc(sizeof(char)*50);
        a->filhos[i][0] = '\0';
    }
    a->arq = (char *)malloc(sizeof(char)*50);
    strcpy(a->arq, arq);
    a->prox = (char *)malloc(sizeof(char)*50);
    a->prox[0] = '\0';

    return a;
}

void ler_filho(FILE *f, char* str, int* tam){
    char c;
    int i = 0,a;
    while(i<50){
        a = fread(&c, 1,1,f);
        if(c == '\n' || a != 1){
            break;
        }
        str[i] = c;
        i++;
    }
    *tam = i;
    str[i] = '\0';
}




TARVBP* le_no(char* arq, int t){
    int a = strlen(arq);
    char novo_nome[a + 20];
    strcpy(novo_nome, "./dados/");
    strcat(novo_nome,arq);
    strcat(novo_nome, ".bin");
    FILE* f = fopen(novo_nome, "rb");
    if(!f)return NULL;
    TARVBP* novo_no = TARVBP_cria(arq, t);
    strcpy(novo_no->arq, arq);
    fread(&novo_no->folha, sizeof(int),1,f);
    fread(&novo_no->n_chaves,sizeof(int),1,f);
    if(!novo_no->folha){
        for(int i = 0;i<novo_no->n_chaves;i++){
            fread(&novo_no->chaves[i],sizeof(unsigned long int),1,f);
        }
    }
    else{
        for(int i = 0;i < novo_no->n_chaves;i++){
            novo_no->reg[i] = malloc(sizeof(TIM));
            fread(novo_no->reg[i],sizeof(TIM),1,f);
        }
    }
    int tam = 0;
    char c[50];
    if(!novo_no->folha){
        for(int i = 0;i<novo_no->n_chaves+1;i++){
            ler_filho(f,c,&t);
            strcpy(novo_no->filhos[i],c);
        }
    }
    ler_filho(f,c,&tam);
    if(tam>0){
        //printf("prox lido: %s  tam: %d\n",c, tam);
        strcpy(novo_no->prox, c);
    }
    fclose(f);
    return novo_no;
}


void esc_no(char* novo_arq, TARVBP* no){
    char nome_final[50];
    strcpy(nome_final, "./dados/");
    strcat(nome_final,novo_arq);
    strcat(nome_final,".bin");
    FILE* f = fopen(nome_final, "wb");
    if(!f)return;
    fwrite(&no->folha, sizeof(int),1,f);
    fwrite(&no->n_chaves,sizeof(int),1,f);
    for(int i = 0;i<no->n_chaves;i++){
        if(!no->folha){  // ESCREVER CHAVES S QUANDO NO FOLHA E REGISTROS QUANDO FOLHA FACILITA A LEITURA
            fwrite(&no->chaves[i],sizeof(unsigned long int),1,f);
        }
        else
            fwrite(no->reg[i],sizeof(TIM),1,f);
    }
    /*if(no->folha){
        for(int i = 0;i<no->n_chaves;i++){
            fwrite(no->reg[i],sizeof(TIM),1,f);
        }
    }*/
    if(!no->folha){
        for(int i = 0;i<no->n_chaves+1;i++){
            fwrite(no->filhos[i], sizeof(char), strlen(no->filhos[i])+1, f);
            fwrite("\n", sizeof(char), 1, f);
        }
    }
    if(no->prox[0]){ //SEM \0 POIS J VAI SER O FIM DO ARQUIVO
        //("PROX ESCRITO: %s\n",no->prox);
        fwrite(no->prox,sizeof(char),strlen(no->prox)+1,f);
    }
    fclose(f);
}

TARVBP* divisao(TARVBP* a, TARVBP* p, int t, int i){
    nos_atual++;
    char buffer[10];
    sprintf(buffer, "%d",nos_atual);
    TARVBP* z = TARVBP_cria(buffer, t);
    z->folha = a->folha;
    if(!a->folha){
        z->n_chaves = t-1;
        for(int j = 0;j<t-1;j++){
            z->chaves[j] = a->chaves[j+t];
        }
        for(int k = 0;k<t;k++){
            strcpy(z->filhos[k],a->filhos[t+k]);
            strcpy(a->filhos[t+k],"");
        }
    }
    else{
        z->n_chaves = t;
        for(int j = 0;j<t;j++){
            z->reg[j] = a->reg[j+t-1];
        }
        strcpy(z->prox, a->prox);
        strcpy(a->prox, z->arq);
    }
    for(int j = p->n_chaves;j>i-1;j--){
        strcpy(p->filhos[j+1], p->filhos[j]);
    }
    strcpy(p->filhos[i],z->arq);
    for(int k = p->n_chaves;k>i-1;k--){
        p->chaves[k] = p->chaves[k-1];
    }
    if(!a->folha){          //se for folha da erro, porque s escreve registro quando folha
        p->chaves[i-1] = a->chaves[t-1];
    }
    else{
        p->chaves[i-1] = a->reg[t-1]->id;
    }
    a->n_chaves=t-1;
    p->n_chaves++;
    for(int jk = t-1;jk< (2*t) -1;jk++){
        a->reg[jk] = NULL;
    }
    esc_no(a->arq,a);
    esc_no(z->arq,z);
    esc_no(p->arq,p);
    TARVBP_libera(z, t);
    return p;
}

TARVBP* insere_n_completo(TARVBP* a, TIM* IM, int t){
    int i;
    if(a->folha){ //finalmente insere de fato
        i = a->n_chaves-1;
        while((i>-1)&& (IM->id < a->reg[i]->id )){
            a->reg[i+1] = a->reg[i];
            i--;
        }
        a->reg[i+1] = IM;
        a->n_chaves++;
        esc_no(a->arq, a);
        return a;
    }
    i = a->n_chaves-1;
    while((i>-1) && (IM->id < a->chaves[i])){
        i--;
    }
    TARVBP *f = le_no(a->filhos[i+1], t);
    i++;
    if(f->n_chaves == ((2*t)-1)){ // no cheio
        a = divisao(f,a,t,i+1);
        TARVBP_libera(f, t);
        f = NULL;
        if((a->folha) && (a->reg[i]->id < IM->id)     ||    (!a->folha) && (a->chaves[i] < IM ->id))i++;
    }
    if(!f){    //divisao foi feita
        f = le_no(a->filhos[i],t);
    }
    f = insere_n_completo(f, IM, t);
    esc_no(f->arq, f);
    TARVBP_libera(f,t);
    return a;
}

TARVBP* TARVBP_insere(TARVBP* a, TIM* IM, int t){
    if(TARVBP_tem_id(a, IM->id, t) == 1)return a;
    if(!a){
        printf("????????????????????????\n\n\n");
        exit(1);
    }
    if(a->n_chaves != (2*t)-1){
        a = insere_n_completo(a, IM, t);
        return a;
    }
    nos_atual++;
    char buf[11];
    sprintf(buf,"%d",nos_atual);
    TARVBP* p = TARVBP_cria(buf, t);
    strcpy(p->filhos[0],a->arq);
    p->folha = 0;
    p = divisao(a,p,t,1);
    TARVBP_libera(a,t);
    p = insere_n_completo(p, IM, t);
    return p;
}

TARVBP* aux_busca(TARVBP* a, unsigned long int id, int t){
    if(!a)return NULL;
    if(a->folha){
        for(int i = 0;i<a->n_chaves;i++){
            if(a->reg[i]->id == id)return a;
        }
        TARVBP_libera(a,t);
        return NULL;
    }
    else{
        int i = 0;
        while((i < a->n_chaves) && (a->chaves[i] <= id))i++;
        TARVBP* y = le_no(a->filhos[i],t);
        TARVBP* aux = aux_busca(y, id, t);
        TARVBP_libera(a,t);
        return aux;
    }

}

TARVBP* TARVBP_busca(TARVBP* a, unsigned long int id, int t){
    if(a->folha) { // CASO RAIZ SEJA FOLHA, T>100
        for (int i = 0; i < a->n_chaves; i++) {
            if (a->reg[i]->id == id)return a;
        }
    }
    int i = 0;
    while((i < a->n_chaves) && (a->chaves[i] <= id)) i++;
    TARVBP* x = le_no(a->filhos[i], t);
    TARVBP* aux = aux_busca(x, id, t);
    return aux;
}

int TARVBP_tem_id(TARVBP *a, unsigned long int id, int t){
    TARVBP* x = TARVBP_busca(a, id, t);
    if(x){
        TARVBP_libera(x,t);
        return 1;
    }
    return 0;
}

void TARVBP_libera(TARVBP* a, int t){
    free(a->chaves);
    if(a->folha){
        for(int i=0; i < 2*t-1; i++){
            if(a->reg[i]) free(a->reg[i]);
        }
    }
    free(a->reg);
    for(int i=0; i < 2*t; i++) free(a->filhos[i]);
    free(a->arq);
    free(a->filhos);
    free(a->prox);
    free(a);
}

TARVBP* prim_folha(TARVBP* a, int t){
    if(!a)return NULL;
    TARVBP* aux = a;
    TARVBP* esq = le_no(aux->filhos[0],t);
    while(esq){
        if(a!= aux) TARVBP_libera(aux,t);
        aux = esq;
        esq = le_no(aux->filhos[0],t);
    }
    return aux;
}



void no_imp(TARVBP* no){
    if(!no)return;
    printf("\nnome arq: %s\n",no->arq);
    printf("folha: %d\n",no->folha);
    printf("n_chaves %d\n", no->n_chaves);
    if(no->folha){
        printf("n_filhos %d\n",0);
        for(int i = 0; i<no->n_chaves;i++){
            printf("\n\nIMOVEL %d:\n\n",i+1);
            imprime_imovel(no->reg[i]);
        }
        printf("prox -> %s",no->prox);
        return;
    }
    printf("n_filhos %d\n",no->n_chaves+1);
    for(int i = 0;i<no->n_chaves+1;i++){
        printf("filho %d: %s \n",i,no->filhos[i]);
    }
    printf("////////////////////////////////////////\n");
}

void reg_imp(TARVBP* a, int t){
    if(!a)return;
    TARVBP* aux = prim_folha(a,t);
    int cont = 0;
    while(aux){

        for(int i = 0; i < aux->n_chaves;i++){
            printf("%d.%lu  ",i+1, aux->reg[i]->id);
            cont++;
        }
        TARVBP* temp = aux;
        //printf("ARQ: %s AUX->PROX: %s\n",aux->arq, aux->prox);
        //no_imp(aux);
        aux = le_no(aux->prox,t);
        if(a!=temp)
            TARVBP_libera(temp,t);
    }
    puts("");
    printf("NUMERO DE REGISTROS: %d",cont);
}

void TARVBP_imprime_aux(TARVBP *a, int andar,int t){
    if(a){
        int i,j;
        TARVBP *f = le_no(a->filhos[a->n_chaves], t);
        TARVBP_imprime_aux(f, andar+1, t);
        if(f) {
            TARVBP_libera(f, t);
        }
        for(i=a->n_chaves-1;i>=0; i--){
            for(j=0; j<=andar; j++) {
                printf("\t");
            }
            if (a->folha != 0) {
                printf("%d.%lu\n",i+1, a->reg[i]->id);
            } else {
                printf("%d.%lu\n",i+1, a->chaves[i]);
            }
            f = le_no(a->filhos[i],t);
            TARVBP_imprime_aux(f,andar+1,t);
            if(f)TARVBP_libera(f,t);
        }
    }
}

void TARVBP_imprime(TARVBP *a, int t){
    printf("\n");
    TARVBP_imprime_aux(a, 0, t);
    printf("\n");
    reg_imp(a, t);
}

TIM* buscaImovel_id(TARVBP* a, unsigned long int id, int t){
    TARVBP* aux = prim_folha(a, t);
    while(aux){
        for(int i = 0; i < aux->n_chaves; i++){
            if(aux->reg[i]->id == id){
                TIM* res = copia_imovel(aux->reg[i]);
                if(aux != a) {
                    TARVBP_libera(aux,t);
                }
                return res;
            }
        }
        TARVBP* te = aux;
        aux = le_no(aux->prox, t);
        if(a != te)
            TARVBP_libera(te,t);
    }
    return NULL;
}

void TARVBP_imprime_imovel_primeira_folha(TARVBP* a, int t){
    if(!a) return;
    TARVBP* aux = prim_folha(a, t);
    no_imp(aux);
    TARVBP_libera(aux, t);
}

TARVBP* remover(TARVBP* aux, unsigned long int id, int t){
    if(!aux) return aux;
    int i;
    if(!aux->folha) for(i = 0; i < aux->n_chaves && aux->chaves[i] < id; i++); //ENCONTRA O NÓ ONDE ESTÁ O ID
    else for(i = 0; i < aux->n_chaves && aux->reg[i]->id < id; i++);

    //SÓ VAI ENTRAR NESSE IF QUANDO CHEGAR NA FOLHA
    if((i < aux->n_chaves) && (aux->folha) && (id == aux->reg[i]->id)){ //VERIFICA O CASO 1 NÓ FOLHA
        int j;
        free(aux->reg[i]);
        for(j = i; j < aux->n_chaves-1; j++) aux->reg[j] = aux->reg[j+1]; //atualiza o nó que foi retirado deslocando os outros nós
        aux->reg[aux->n_chaves-1] = NULL;
        aux->n_chaves--; //decrementa a quantidade de chaves daquele nó
        esc_no(aux->arq,aux);
        if(!aux->n_chaves){ //se a árvore ficou vazia depois da remoção, libera a árvore
            char buff[50];
            strcpy(buff,"./dados/");
            strcat(buff, aux->arq);
            strcat(buff, ".bin");
            remove(buff);
            TARVBP_libera(aux, t);
            aux = NULL;
        }
        return aux;      
    }

    if((i < aux->n_chaves) && (id == aux->chaves[i])) i++;

    TARVBP* filho = le_no(aux->filhos[i], t);

    TARVBP* y = filho, *z = NULL;


    if (y->n_chaves == t-1){ //CASO 3A 
        if(aux->n_chaves > i){
            z = le_no(aux->filhos[i+1], t);
        }
        if(aux->n_chaves > i && z && z->n_chaves>=t){
            if (y->folha) { //nó folha, mexe nos ids e nos registros
                aux->chaves[i] = z->reg[0]->id + 1; //passa o id do primeiro nó de z para aux->chaves[i]
                y->reg[t - 1] = z->reg[0]; //passa o imóvel do primeiro nó de z para y
            } else { //nó interno, não mexe nos registros, só nos ids
                y->chaves[t - 1] = aux->chaves[i];   //dar a y a chave i de aux
                aux->chaves[i] = z->chaves[0];     //dar a aux uma chave de z
            }
            y->n_chaves++;

            int j;
            if(!z->folha){
                for(j = 0; j < z->n_chaves-1; j++) z->chaves[j] = z->chaves[j+1];  //ajustar chaves de z
                strcpy(y->filhos[y->n_chaves],z->filhos[0]); //troca o filho
                for(j = 0;j < z->n_chaves;j++) strcpy(z->filhos[j],z->filhos[j+1]);//ajusta os filhos
            }
            else{
                for(j = 0; j < z->n_chaves-1; j++) z->reg[j] = z->reg[j+1];  //ajustar chaves de z
                z->reg[z->n_chaves-1] = NULL;
            }
            z->n_chaves--;
            esc_no(z->arq, z);
            y = remover(y, id, t); //remoção a partir da subarvore
            esc_no(y->arq,y);
            TARVBP_libera(y,t);
            TARVBP_libera(z,t);
            y = NULL;
            z = NULL;
            return aux;
        }
        if(z){
            TARVBP_libera(z,t);
            z = NULL;
        }
        if(i){
            z = le_no(aux->filhos[i-1],t);
        }
        if(i > 0 && z && z->n_chaves>=t){
            int j;
            if(y->folha){
                for(j = y->n_chaves;j > 0; j--)y->reg[j] = y->reg[j-1];
                aux->chaves[i-1] = z->reg[z->n_chaves-1]->id;
                y->reg[0] = z->reg[z->n_chaves-1];
                z->reg[z->n_chaves-1]=NULL;
            }
            else{
                for(j = y->n_chaves;j > 0;j--)y->chaves[j] = y->chaves[j-1];
                y->chaves[0] = aux->chaves[i-1];
                aux->chaves[i-1] = z->chaves[z->n_chaves-1];
                for(j = y->n_chaves+1; j > 0; j--)strcpy(y->filhos[j],y->filhos[j-1]);
            }
            y->n_chaves++;
            if(!y->folha)strcpy(y->filhos[0],z->filhos[z->n_chaves]);
            z->n_chaves--;
            esc_no(z->arq,z);
            TARVBP_libera(z,t);
            z = NULL;
            y = remover(y,id,t);
            esc_no(y->arq,y);
            TARVBP_libera(y,t);
            y = NULL;
            return aux;
        }
        if(z){
            TARVBP_libera(z,t); //clion tava me assustando, mas acho que não deveria precisar não
            z = NULL;
        }
        if(aux->n_chaves>i){
            z = le_no(aux->filhos[i+1],t);
        }
        if(aux->n_chaves>i && z && z->n_chaves == t-1){
            if(!y->folha){
                y->chaves[t-1] = aux->chaves[i];
                y->n_chaves++;
            }
            int j = 0;
            while(j < t-1){
                if(y->folha){
                    y->reg[t+j-1] = z->reg[j];
                    z->reg[j] = NULL;
                }
                else{
                    y->chaves[t+j] = z->chaves[j];
                }
                y->n_chaves++;
                j++;
            }
            strcpy(y->prox,z->prox);
            if(!y->folha){
                int k;
                for(k = 0;k<t;k++){
                    strcpy(y->filhos[t+k],z->filhos[k]);
                }
            }
            char buff[50];
            strcpy(buff,"./dados");
            strcat(buff,z->arq);
            strcat(buff,".bin");
            remove(buff);
            TARVBP_libera(z,t); //z cortado
            z = NULL;
            for(j = i; j <aux->n_chaves-1;j++){
                aux->chaves[j] = aux->chaves[j+1];
                strcpy(aux->filhos[j+1],aux->filhos[j+2]);
            }
            strcpy(aux->filhos[aux->n_chaves],"");
            aux->n_chaves--;
            if(aux->n_chaves == 0){
                TARVBP* temp = aux;
                strcpy(temp->filhos[0],"");
                char buff2[50];
                strcpy(buff2, "./dados");
                strcat(buff2,aux->arq);
                strcat(buff2, ".bin");
                remove(buff2);
                aux = y;
                TARVBP_libera(temp,t);
                temp = NULL;
                aux = remover(aux,id,t);
                esc_no(aux->arq,aux);
                return aux;
            }
            esc_no(y->arq,y);
            TARVBP_libera(y,t);
            y = NULL;
            aux = remover(aux,id,t);
            return aux;
        }
        if(z){
            TARVBP_libera(z,t);
            z = NULL;
        }
        if(i > 0) {
            z = le_no(aux->filhos[i-1],t);
        }
        if(i > 0 && z && z->n_chaves == t-1){
            if(!y->folha){
                if(i == aux->n_chaves)z->chaves[t-1] = aux->chaves[i-1];
                else z->chaves[t-1] = aux->chaves[i];
                z->n_chaves++;
            }
            int j = 0;
            while(j<t-1){
                if (!y->folha){
                    z->chaves[t+j] = y->chaves[j];
                }
                else{
                    z->reg[t+j-1] = y->reg[j];
                    y->reg[j] = NULL;
                }
                z->n_chaves++;
                j++;

            }
            strcpy(z->prox,y->prox);
            if(!z->folha){
                for(j = 0; j < t; j++){
                    strcpy(z->filhos[t+j],y->filhos[j]);
                    strcpy(y->filhos[j],"");
                }
                char buff[50];
                strcpy(buff, "./dados");
                strcat(buff, y->arq);
                strcat(buff,".bin");
                remove(buff);
            }
            TARVBP_libera(y,t);
            y = NULL;
            esc_no(z->arq,z);
            strcpy(aux->filhos[aux->n_chaves],"");
            aux->n_chaves--;
            if(aux->n_chaves == 0){
                TARVBP* temp = aux;
                aux = le_no(aux->filhos[0],t);
                strcpy(temp->filhos[0],"");
                char buff3[50];
                strcpy(buff3,"./dados");
                strcat(buff3, temp->arq);
                strcat(buff3,".bin");
                remove(buff3);
                TARVBP_libera(temp,t);
                temp = NULL;
                aux = remover(aux,id,t);
                TARVBP_libera(z,t);
                z = NULL;
                return aux;
            }
            else{
                z = remover(z,id,t);
                esc_no(z->arq,z);
                TARVBP_libera(z,t);
                z = NULL;
            }
            if(z){
                TARVBP_libera(z,t);
                z = NULL;
            }
            return aux;
        }
        if(z){
            TARVBP_libera(z,t);
            z = NULL;
        }
    }
    y = remover(y,id,t);
    esc_no(y->arq,y);
    TARVBP_libera(y,t);
    y = NULL;
    return aux;
}


TARVBP* TARVBP_retira(TARVBP* a, unsigned long int id, int t) {
    if(!a || !TARVBP_tem_id(a, id, t)){
        printf("\nArvore vazia ou id nao encontrado!");
        return a;
    }
    return remover(a, id, t);
}
