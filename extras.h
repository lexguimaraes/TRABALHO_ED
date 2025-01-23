#include "TLSEIM.h"

TARVBP* busca_arq(TARVBP* a, char* arq, int t); //FEITA

int maior_desc(TARVBP*a, int t); //FEITA

TLSETIM* imoveis_maior_preco(TARVBP*a, int t, int venda); //FEITA

TLSETIM* imoveis_menor_preco(TARVBP*a, int t, int venda);//FEITA

TLSETIM* imoveis_string(TARVBP*a, int t,int (*comp)(TIM*,char*),char* str); //FEITA

TLSETIM* imoveis_faixa_de_preco(TARVBP* a, int t, float lim_inf, float lim_sup); //FEITA

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

TARVBP* remove_imoveis_faixa_de_preco(TARVBP* a, int t, float lim_inf, float lim_sup); //FEITA

TARVBP* edita_latitude(TARVBP* a, int t,unsigned long int id, char* str);//FEITA

TARVBP* edita_longitude(TARVBP* a, int t,unsigned long int id, char* str); //FEITA

TARVBP* edita_num(TARVBP* a, int t,unsigned long int id, int num); //FEITA

TARVBP* edita_rua(TARVBP* a, int t,unsigned long int id, char* str); //FEITA;

TARVBP* edita_tipo(TARVBP* a, int t,unsigned long int id, char* str); //FEITA;

TARVBP* edita_bairro(TARVBP* a, int t,unsigned long int id, char* str); //FEITA;

TARVBP* edita_cep(TARVBP* a, int t,unsigned long int id, char* str); //FEITA;





TARVBP* edita_preco_porM2(TARVBP* a, int t, unsigned long int id, float novo_preco_porM2); //FEITA

TARVBP* edita_preco_total(TARVBP* a, int t, unsigned long int id, float novo_preco_total); //FEITA

TARVBP* edita_descricao(TARVBP* a, int t,unsigned long int id, char* str); //FEITA

double prox_de_aux(double distanciaMax, double lat1, double long1, double lat2, double long2);

double prox_de(double distanciaMax, TIM* imovel, double latitude, double longitude);

TLSETIM* imoveis_prox_de(TARVBP* a, int t, double distanciaMax, double latitude, double longitude);

char* substituir_virgula_por_ponto(char *str);

void abrir_google_maps(TARVBP* a, int t, unsigned long int id);
