#include "TLSEIM.h"

TARVBP* busca_arq(TARVBP* a, char* arq, int t); //FEITA

int maior_desc(TARVBP*a, int t); //FEITA

TLSETIM* imoveis_maior_preco(TARVBP*a, int t); //FEITA

TLSETIM* imoveis_menor_preco(TARVBP*a, int t);//FEITA

TLSETIM* imoveis_string(TARVBP*a, int t,int (*comp)(TIM*,char*),char* str); //FEITA

TARVBP* retira_imoveis(TARVBP* a, int t, TLSETIM* IMs); // FEITA

TLSETIM* imoveis_metragem(TARVBP* a, int t,int m);//FEITA

int comp_bairro(TIM* IM, char* bairro); //FEITA

int comp_tipo(TIM* IM, char* tipo); //FEITA

int comp_rua(TIM* IM, char* rua); //FEITA

int comp_latitude(TIM* IM, char* latitude); //FEITA

int comp_longitude(TIM* IM, char* longitude); //FEITA

int comp_cep(TIM* IM, char* cep); //FEITA

int comp_desc(TIM* IM, char* desc); //FEITA

TLSETIM* imoveis_metragem(TARVBP* a, int t,int m);//FEITA

TARVBP* remove_imoveis_metragem(TARVBP* a, int t, int m); //FEITA

TARVBP* edita(TARVBP* a, int t,unsigned long int id, char* str); //FEITA

int prox_de_aux(double distanciaMax, double lat1, double long1, double lat2, double long2);

int prox_de(double distanciaMax, TIM* imovel, double latitude, double longitude);