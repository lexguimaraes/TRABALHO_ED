#include "../headers/ARVBP.h"
#include "../headers/extras.h"
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




TARVBP* le_no(char* nome, int t) {
    if (!nome || strlen(nome) == 0) return NULL;
    
    char caminho[100];
    sprintf(caminho, "./dados/%s.bin", nome);
    
    FILE* f = fopen(caminho, "rb");
    if (!f) return NULL;
    
    TARVBP* no = TARVBP_cria(nome, t);
    if (!no) {
        fclose(f);
        return NULL;
    }
    
    // Lê os campos básicos
    if (fread(&no->folha, sizeof(int), 1, f) != 1 ||
        fread(&no->n_chaves, sizeof(int), 1, f) != 1) {
        TARVBP_libera(no, t);
        fclose(f);
        return NULL;
    }
    
    // Se não é folha, lê as chaves
    if (!no->folha) {
        for (int i = 0; i < no->n_chaves; i++) {
            if (fread(&no->chaves[i], sizeof(unsigned long int), 1, f) != 1) {
                TARVBP_libera(no, t);
                fclose(f);
                return NULL;
            }
        }
    }
    // Se é folha, lê os registros
    else {
        for (int i = 0; i < no->n_chaves; i++) {
            no->reg[i] = malloc(sizeof(TIM));
            if (!no->reg[i] || fread(no->reg[i], sizeof(TIM), 1, f) != 1) {
                TARVBP_libera(no, t);
                fclose(f);
                return NULL;
            }
        }
    }
    
    // Se não é folha, lê os filhos
    if (!no->folha) {
        for (int i = 0; i <= no->n_chaves; i++) {
            char filho[50];
            int tam;
            ler_filho(f, filho, &tam);
            if (tam > 0) {
                strcpy(no->filhos[i], filho);
            }
        }
    }
    
    // Lê o ponteiro próximo se houver
    char prox[50];
    int tam;
    ler_filho(f, prox, &tam);
    if (tam > 0) {
        strcpy(no->prox, prox);
    }
    
    fclose(f);
    return no;
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
    if(x && x!= a){
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
TARVBP* prim_folha(TARVBP* a, int t) {
    if(!a) return NULL;
    
    TARVBP* aux = a;
    TARVBP* esq = NULL;
    
    while(!aux->folha) {
        if (aux->n_chaves == 0) {
            if (aux != a) TARVBP_libera(aux, t);
            return NULL;
        }
        
        esq = le_no(aux->filhos[0], t);
        if (!esq) {
            if (aux != a) TARVBP_libera(aux, t);
            return NULL;
        }
        
        if (a != aux) {
            TARVBP_libera(aux, t);
        }
        aux = esq;
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
    TARVBP_imprime_aux(a, 0, t);
    printf("\n");
    reg_imp(a, t);
}

TIM* buscaImovel_id(TARVBP* a, unsigned long int id, int t) {
    if (!a) return NULL;
    
    TARVBP* aux = prim_folha(a, t);
    if (!aux) return NULL;
    
    TIM* resultado = NULL;
    TARVBP* atual = aux;
    
    while (atual && !resultado) {
        for (int i = 0; i < atual->n_chaves; i++) {
            if (atual->reg[i] && atual->reg[i]->id == id) {
                resultado = copia_imovel(atual->reg[i]);
                break;
            }
        }
        
        if (!resultado && atual->prox[0] != '\0') {
            TARVBP* proximo = le_no(atual->prox, t);
            if (atual != a) TARVBP_libera(atual, t);
            atual = proximo;
        } else {
            if (atual != a) TARVBP_libera(atual, t);
            atual = NULL;
        }
    }
    
    return resultado;
}

// Função auxiliar para limpar registros
void limpa_registros(TARVBP* no, int pos_inicio, int pos_fim) {
    for(int i = pos_inicio; i < pos_fim; i++) {
        if(no->reg[i]) {
            free(no->reg[i]);
            no->reg[i] = NULL;
        }
    }
}

// Função auxiliar para mover registros
void move_registros(TARVBP* destino, TARVBP* origem, int pos_destino, int pos_origem, int quantidade) {
    for(int i = 0; i < quantidade; i++) {
        destino->reg[pos_destino + i] = origem->reg[pos_origem + i];
        destino->chaves[pos_destino + i] = origem->chaves[pos_origem + i];
        origem->reg[pos_origem + i] = NULL;
    }
}

TARVBP* TARVBP_retira(TARVBP* a, unsigned long int id, int t) {
    if (!a) return NULL;
    
    // Encontra o nó folha que contém o id
    TARVBP* no = TARVBP_busca(a, id, t);
    if (!no) return a;  // ID não encontrado
    
    // Remove o registro
    int pos_reg = -1;
    for (int i = 0; i < no->n_chaves; i++) {
        if (no->reg[i] && no->reg[i]->id == id) {
            pos_reg = i;
            break;
        }
    }
    
    if (pos_reg == -1) {
        if (no != a) TARVBP_libera(no, t);
        return a;
    }
    
    // Remove o registro e atualiza o nó
    free(no->reg[pos_reg]);
    for (int i = pos_reg; i < no->n_chaves-1; i++) {
        no->reg[i] = no->reg[i+1];
        no->chaves[i] = no->chaves[i+1];
    }
    no->reg[no->n_chaves-1] = NULL;
    no->n_chaves--;
    
    // Escreve o nó atualizado
    esc_no(no->arq, no);
    
    // Se é a raiz ou tem chaves suficientes, termina
    if (no->n_chaves >= t-1 || strcmp(no->arq, "0") == 0) {
        if (no != a) TARVBP_libera(no, t);
        return a;
    }
    
    // Busca o pai para rebalanceamento
    TARVBP* pai = busca_pai(a, no->arq, t);
    if (!pai) {
        if (no != a) TARVBP_libera(no, t);
        return a;
    }
    
    // Encontra a posição do nó no pai
    int pos_no = 0;
    while (pos_no <= pai->n_chaves && strcmp(pai->filhos[pos_no], no->arq) != 0) {
        pos_no++;
    }
    
    // Rebalanceia a árvore
    TARVBP* resultado = rebalancear(a, no, pai, pos_no, t);
    
    // Atualiza as chaves do pai
    if (pai != a) {
        resultado = atualiza_chaves_pai(resultado, pai, t);
    }
    
    return resultado;
}

TARVBP* merge_nos(TARVBP* esq, TARVBP* dir, TARVBP* pai, int pos_pai, int t) {
    printf("[DEBUG-MN] Iniciando merge de %s e %s\n", esq->arq, dir->arq);
    if (!esq || !dir || !pai) return pai;
    
    if (esq->folha && dir->folha) {
        printf("[DEBUG-MN] Merge de nós folha\n");
        
        // Guarda o ponteiro próximo do nó direito
        char prox_temp[50];
        strcpy(prox_temp, dir->prox);
        
        // Move registros do nó direito para o esquerdo
        for (int i = 0; i < dir->n_chaves; i++) {
            esq->reg[esq->n_chaves + i] = dir->reg[i];
            esq->chaves[esq->n_chaves + i] = dir->chaves[i];
            dir->reg[i] = NULL;  // Evita double free
        }
        esq->n_chaves += dir->n_chaves;
        
        // Atualiza o ponteiro próximo do nó esquerdo
        strcpy(esq->prox, prox_temp);
        
        printf("[DEBUG-MN] Removendo entrada do pai na posição %d\n", pos_pai);
        // Remove a entrada do pai
        for (int i = pos_pai; i < pai->n_chaves - 1; i++) {
            pai->chaves[i] = pai->chaves[i + 1];
            strcpy(pai->filhos[i + 1], pai->filhos[i + 2]);
        }
        pai->n_chaves--;
        
        // Se o pai ficou vazio e é a raiz, ajusta a árvore
        if (pai->n_chaves == 0 && strcmp(pai->arq, "0") == 0) {
            printf("[DEBUG-MN] Pai é raiz e ficou vazio, ajustando árvore\n");
            strcpy(esq->arq, "0");
            char dir_arq[50];
            strcpy(dir_arq, dir->arq);
            TARVBP_libera(dir, t);
            TARVBP_libera(pai, t);
            esc_no(esq->arq, esq);
            remove(dir_arq);
            return esq;
        }
        
        printf("[DEBUG-MN] Atualizando arquivos\n");
        // Atualiza os arquivos
        esc_no(esq->arq, esq);
        esc_no(pai->arq, pai);
        char dir_arq[50];
        strcpy(dir_arq, dir->arq);
        TARVBP_libera(dir, t);
        remove(dir_arq);  // Remove o arquivo do nó direito
        
        printf("[DEBUG-MN] Atualizando chaves do pai\n");
        // Atualiza as chaves do pai recursivamente
        TARVBP* novo_pai = atualiza_chaves_pai(pai, esq, t);
        if (novo_pai != pai) {
            TARVBP_libera(pai, t);
            return novo_pai;
        }
        return pai;
    }
    
    printf("[DEBUG-MN] Nós não são folhas, retornando pai\n");
    return pai;
}

TARVBP* redistribui_nos(TARVBP* no_destino, TARVBP* no_origem, TARVBP* pai, int pos_pai, int t) {
    printf("[DEBUG-RN] Iniciando redistribuição\n");
    
    if (!no_destino || !no_origem || !pai) {
        printf("[DEBUG-RN] ERRO: Nó nulo na redistribuição\n");
        return NULL;
    }
    
    printf("[DEBUG-RN] Redistribuindo de %s para %s\n", no_origem->arq, no_destino->arq);
    
    // Se o nó destino está à direita do nó origem
    if (strcmp(no_destino->arq, pai->filhos[pos_pai + 1]) == 0) {
        printf("[DEBUG-RN] Redistribuindo da esquerda para direita\n");
        
        // Move um registro do nó origem para o destino
        no_destino->reg[no_destino->n_chaves] = no_origem->reg[no_origem->n_chaves - 1];
        no_destino->chaves[no_destino->n_chaves] = no_origem->chaves[no_origem->n_chaves - 1];
        no_origem->reg[no_origem->n_chaves - 1] = NULL;
        no_origem->n_chaves--;
        no_destino->n_chaves++;
        
        // Atualiza a chave no pai
        pai->chaves[pos_pai] = no_destino->reg[0]->id;
    }
    // Se o nó destino está à esquerda do nó origem
    else {
        printf("[DEBUG-RN] Redistribuindo da direita para esquerda\n");
        
        // Abre espaço no início do nó destino
        for (int i = no_destino->n_chaves; i > 0; i--) {
            no_destino->reg[i] = no_destino->reg[i - 1];
            no_destino->chaves[i] = no_destino->chaves[i - 1];
        }
        
        // Move um registro do nó origem para o destino
        no_destino->reg[0] = no_origem->reg[0];
        no_destino->chaves[0] = no_origem->chaves[0];
        no_destino->n_chaves++;
        
        // Remove o registro do nó origem
        for (int i = 0; i < no_origem->n_chaves - 1; i++) {
            no_origem->reg[i] = no_origem->reg[i + 1];
            no_origem->chaves[i] = no_origem->chaves[i + 1];
        }
        no_origem->reg[no_origem->n_chaves - 1] = NULL;
        no_origem->n_chaves--;
        
        // Atualiza a chave no pai
        pai->chaves[pos_pai] = no_origem->reg[0]->id;
    }
    
    printf("[DEBUG-RN] Salvando alterações\n");
    // Salva as alterações nos arquivos
    esc_no(no_destino->arq, no_destino);
    esc_no(no_origem->arq, no_origem);
    esc_no(pai->arq, pai);
    
    printf("[DEBUG-RN] Redistribuição concluída\n");
    return pai;
}

TARVBP* busca_pai(TARVBP* raiz, char* arq_filho, int t) {
    printf("[DEBUG-BP] Buscando pai de %s\n", arq_filho);
    if (!raiz || raiz->folha) {
        printf("[DEBUG-BP] Raiz nula ou é folha\n");
        return NULL;
    }
    
    // Primeiro verifica se é filho direto
    for (int i = 0; i <= raiz->n_chaves; i++) {
        if (strcmp(raiz->filhos[i], arq_filho) == 0) {
            printf("[DEBUG-BP] Pai encontrado diretamente: %s\n", raiz->arq);
            return raiz;
        }
    }
    
    printf("[DEBUG-BP] Procurando em subárvores de %s\n", raiz->arq);
    TARVBP* resultado = NULL;
    
    // Depois procura recursivamente
    for (int i = 0; i <= raiz->n_chaves && !resultado; i++) {
        printf("[DEBUG-BP] Verificando filho %s\n", raiz->filhos[i]);
        TARVBP* filho = le_no(raiz->filhos[i], t);
        if (!filho) {
            printf("[DEBUG-BP] ERRO: Não foi possível ler o filho %s\n", raiz->filhos[i]);
            continue;
        }
        
        if (!filho->folha) {
            resultado = busca_pai(filho, arq_filho, t);
        }
        
        if (filho != raiz) {
            printf("[DEBUG-BP] Liberando filho %s\n", filho->arq);
            TARVBP_libera(filho, t);
        }
    }
    
    if (resultado) {
        printf("[DEBUG-BP] Pai encontrado em subárvore: %s\n", resultado->arq);
    } else {
        printf("[DEBUG-BP] Pai não encontrado em %s\n", raiz->arq);
    }
    
    return resultado;
}

TARVBP* atualiza_chaves_pai(TARVBP* raiz, TARVBP* no, int t) {
    printf("[DEBUG-ACP] Iniciando atualização de chaves do pai\n");
    if (!raiz || !no) {
        printf("[DEBUG-ACP] raiz ou nó nulo\n");
        return raiz;
    }
    
    // Se é a raiz, não precisa atualizar
    if (strcmp(no->arq, "0") == 0) {
        printf("[DEBUG-ACP] Nó é a raiz, retornando\n");
        return raiz;
    }
    
    printf("[DEBUG-ACP] Buscando pai do nó %s\n", no->arq);
    TARVBP* pai = busca_pai(raiz, no->arq, t);
    if (!pai) {
        printf("[DEBUG-ACP] Pai não encontrado\n");
        return raiz;
    }
    printf("[DEBUG-ACP] Pai encontrado: %s\n", pai->arq);
    
    // Encontra a posição do filho no pai
    int pos_filho = 0;
    while (pos_filho <= pai->n_chaves && 
           strcmp(pai->filhos[pos_filho], no->arq) != 0) {
        pos_filho++;
    }
    printf("[DEBUG-ACP] Posição do filho no pai: %d\n", pos_filho);
    
    // Atualiza a chave se necessário
    if (pos_filho > 0) {
        printf("[DEBUG-ACP] Atualizando chave na posição %d\n", pos_filho-1);
        TARVBP* filho = le_no(pai->filhos[pos_filho], t);
        if (filho) {
            pai->chaves[pos_filho-1] = filho->chaves[0];
            TARVBP_libera(filho, t);
            printf("[DEBUG-ACP] Chave atualizada para %lu\n", pai->chaves[pos_filho-1]);
        } else {
            printf("[DEBUG-ACP] ERRO: Não foi possível ler o filho\n");
        }
    }
    
    // Se o pai ficou com underflow
    if (pai->n_chaves < t-1 && strcmp(pai->arq, "0") != 0) {
        printf("[DEBUG-ACP] Pai com underflow (n_chaves=%d, t-1=%d)\n", 
               pai->n_chaves, t-1);
        printf("[DEBUG-ACP] Buscando avô\n");
        TARVBP* avo = busca_pai(raiz, pai->arq, t);
        if (avo) {
            printf("[DEBUG-ACP] Avô encontrado, recursão\n");
            return atualiza_chaves_pai(raiz, avo, t);
        } else {
            printf("[DEBUG-ACP] Avô não encontrado\n");
        }
    } else {
        printf("[DEBUG-ACP] Pai não precisa de rebalanceamento\n");
    }
    
    printf("[DEBUG-ACP] Escrevendo pai atualizado no arquivo\n");
    esc_no(pai->arq, pai);
    printf("[DEBUG-ACP] Retornando raiz\n");
    return raiz;
}

TARVBP* rebalancear(TARVBP* a, TARVBP* no, TARVBP* pai, int pos_no, int t) {
    printf("[DEBUG-RB] Iniciando rebalanceamento do nó %s\n", no->arq);
    if (!pai) {
        printf("[DEBUG-RB] Sem pai, retornando raiz\n");
        return a;  // Se não tem pai, é a raiz
    }
    
    // Encontra a posição do nó no pai
    int pos_no_pai = 0;
    while (pos_no_pai <= pai->n_chaves && strcmp(pai->filhos[pos_no_pai], no->arq) != 0) {
        pos_no_pai++;
    }
    printf("[DEBUG-RB] Posição do nó no pai: %d\n", pos_no_pai);
    
    TARVBP* irmao = NULL;
    TARVBP* resultado = NULL;
    
    // Tenta redistribuir com irmão esquerdo
    if (pos_no_pai > 0) {
        printf("[DEBUG-RB] Tentando redistribuir com irmão esquerdo\n");
        irmao = le_no(pai->filhos[pos_no_pai - 1], t);
        if (irmao && irmao->n_chaves > t-1) {
            resultado = redistribui_nos(no, irmao, pai, pos_no_pai - 1, t);
            if (resultado) {
                printf("[DEBUG-RB] Redistribuição com irmão esquerdo bem sucedida\n");
                if (irmao != a) TARVBP_libera(irmao, t);
                if (no != a) TARVBP_libera(no, t);
                return resultado;
            }
        }
        if (irmao && irmao != a) {
            TARVBP_libera(irmao, t);
            irmao = NULL;
        }
    }
    
    // Tenta redistribuir com irmão direito
    if (pos_no_pai < pai->n_chaves) {
        printf("[DEBUG-RB] Tentando redistribuir com irmão direito\n");
        irmao = le_no(pai->filhos[pos_no_pai + 1], t);
        if (irmao && irmao->n_chaves > t-1) {
            resultado = redistribui_nos(no, irmao, pai, pos_no_pai, t);
            if (resultado) {
                printf("[DEBUG-RB] Redistribuição com irmão direito bem sucedida\n");
                if (irmao != a) TARVBP_libera(irmao, t);
                if (no != a) TARVBP_libera(no, t);
                return resultado;
            }
        }
        if (irmao && irmao != a) {
            TARVBP_libera(irmao, t);
            irmao = NULL;
        }
    }
    
    // Se não conseguiu redistribuir, tenta merge
    printf("[DEBUG-RB] Redistribuição falhou, tentando merge\n");
    
    // Tenta merge com irmão esquerdo
    if (pos_no_pai > 0) {
        printf("[DEBUG-RB] Tentando merge com irmão esquerdo\n");
        irmao = le_no(pai->filhos[pos_no_pai - 1], t);
        if (irmao) {
            resultado = merge_nos(irmao, no, pai, pos_no_pai - 1, t);
            if (resultado) {
                printf("[DEBUG-RB] Merge com irmão esquerdo bem sucedido\n");
                return resultado;
            }
        }
    }
    
    // Tenta merge com irmão direito
    if (pos_no_pai < pai->n_chaves) {
        printf("[DEBUG-RB] Tentando merge com irmão direito\n");
        irmao = le_no(pai->filhos[pos_no_pai + 1], t);
        if (irmao) {
            resultado = merge_nos(no, irmao, pai, pos_no_pai, t);
            if (resultado) {
                printf("[DEBUG-RB] Merge com irmão direito bem sucedido\n");
                return resultado;
            }
        }
    }
    
    // Se chegou aqui, não conseguiu rebalancear
    printf("[DEBUG-RB] Não foi possível rebalancear\n");
    if (no != a) TARVBP_libera(no, t);
    return a;
}

