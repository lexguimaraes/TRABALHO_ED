#include "TIM.h"

float strParaFloat(char* str) {
    for (char* p = str; *p != '\0'; ++p) { //FLOAT TEM QUE TER A VIRGULA NO ATOF
        if (*p == ',') *p = '.';
    }

    return atof(str);
}

//tipo um strtok s� que pega espa�os vazios
char* strsep(char **stringp, const char *delim) {
    char *start = *stringp;
    if (start == NULL) {
        return NULL;
    }

    char *end = strpbrk(start, delim);
    if (end == NULL) {
        *stringp = NULL;
    } else {
        *end = '\0';
        *stringp = end + 1;
    }

    return start;
}

//l� o im�vel a partir da linha do arquivo
TIM* ler_imovel(char* linha) {
    TIM* imovel = (TIM*)malloc(sizeof(TIM));
    if(!linha) return NULL;

    char* token;
    char* strp = linha;
    char* a;
    unsigned long int id;

    //id
    token = strsep(&strp, ";");
    char buf[11];
    strcpy(buf,token);
    id = strtoul(buf,&a,10);
    if (token) imovel->id = id;

    //bairro
    token = strsep(&strp, ";");
    if (token) strcpy(imovel->bairro, token);

    //tipo
    token = strsep(&strp, ";");
    if (token) strcpy(imovel->tipo, token);

    //rua
    token = strsep(&strp, ";");
    if (token) strcpy(imovel->rua, token);

    //n�mero
    token = strsep(&strp, ";");
    if (token) imovel->num = atoi(token);

    //pre�o total
    token = strsep(&strp, ";");
    if (token) imovel->preco_total = strParaFloat(token);

    //pre�o por m2
    token = strsep(&strp, ";");
    if (token) imovel->preco_por_m2 = strParaFloat(token);

    //descri��o
    token = strsep(&strp, ";");
    if (token) strcpy(imovel->descricao, token);

    //cep
    token = strsep(&strp, ";");
    if (token) strcpy(imovel->CEP, token);

    //latitude
    token = strsep(&strp, ";");
    if (token) strcpy(imovel->latitude, token);

    //longitude
    token = strsep(&strp, ";");
    if (token) strcpy(imovel->longitude, token);
    imovel->longitude[strlen(imovel->longitude)-1] = '\0';

    return imovel;
}



//Fun��o para ler os dados de um arquivo bin�rio e preencher a estrutura TImovel
void ler_binario(char* arq) {
    TIM imovel;

    FILE* fp = fopen(arq, "rb");
    if (!fp) {
        printf("Erro ao abrir o arquivo: %s\n", arq);
        exit(1);
    }

    fread(&imovel, sizeof(TIM), 1, fp);

    fclose(fp);
}



//salva o imovel em um arquivo
void salva(TIM* imovel, int indice) {
    char nome_arquivo[50];
    sprintf(nome_arquivo, "imovel_%d.bin", indice);

    FILE* saida = fopen(nome_arquivo, "wb");
    if (!saida) exit(1);

    fwrite(imovel, sizeof(TIM), 1, saida);

    fclose(saida);
}

//imprime o imóvel dado por dado
void imprime_imovel(TIM* imovel){
    if(!imovel) return;
    printf("\n");
    printf("ID: %lu\n", imovel->id);
    printf("Bairro: %s\n", imovel->bairro);
    printf("Tipo: %s\n", imovel->tipo);
    printf("Rua: %s\n", imovel->rua);
    printf("Numero: %d\n", imovel->num);
    printf("Preco Total: %.2f\n", imovel->preco_total);
    printf("Preco por m2: %.2f\n", imovel->preco_por_m2);
    printf("CEP: %s\n", imovel->CEP);
    printf("Latitude: %s\n", imovel->latitude);
    printf("Longitude: %s\n", imovel->longitude);
    printf("\nDescricao: %s\n", imovel->descricao);
}

TIM* copia_imovel(TIM* imovel){
    TIM* new = malloc(sizeof(TIM));
    new->id = imovel->id;
    strcpy(new->bairro,imovel->bairro);
    strcpy(new->tipo,imovel->tipo);
    strcpy(new->rua,imovel->rua);
    new->num = imovel->num;
    new->preco_total = imovel->preco_total;
    new->preco_por_m2 = imovel->preco_por_m2;
    strcpy(new->CEP,imovel->CEP);
    strcpy(new->latitude,imovel->latitude);
    strcpy(new->longitude,imovel->longitude);
    strcpy(new->descricao,imovel->descricao);
    return new;
}
