#include "extras.h"
#include "DESC.h"
#include <locale.h>

int main(void) {  //CRISTO REDENTOR, COPACABANA PALACE, MARACANA, FLORESTA DA TIJUCA, TEATRO MUNICIPAL, RESERVA DO GRAJAU, PARQUE LAGE, MURETA DA URCA, PRAIA DE BOTAFOGO, PRAIA DE IPANEMA, LAGOA, CALCADAO, UFF
    double pt_latitude[] = {-22.951804,-22.967311,-22.912109,-22.941400,-23.545235,-22.922138,-22.958154,-22.949800,-22.945325,-22.987064, -22.965704,-22.967142,-22.905802};
    double pt_longitude[] = {-43.210760,-43.178954,-43.230156,-43.292794,-46.638615,-43.272439,-43.211643,-22.949800,-43.180774,-43.204797, -43.204401,-43.177116,-43.133421};
    del_dados();
    setlocale(LC_ALL, "pt_BR.UTF-8");
    int t;
    printf("Digite o grau t da arvore b+\n");
    scanf("%d",&t);
    if(t<2) t = 2;
    TARVBP* a = cat2a(t);
    TARVBP_imprime(a, t);
    int comando;
    do{
        printf("\n-----------------------------------\n");
        printf("Digite qual comando voce quer executar: \n");
        printf("1 - Retorno do imovel a venda mais barato (e caro)\n"); //FEITO
        printf("2 - Retorno do imovel alugado mais barato (e caro)\n"); //FEITO
        printf("3 - Buscar imoveis por (bairro, rua, CEP, latitude, longitude, tipo)\n"); //FEITO
        printf("4 - Retirar imoveis por (bairro, rua, CEP, latitude, longitude, tipo)\n"); //FEITO
        printf("5 - Dada a descricao, obter informacoes a mais do imovel\n"); //FEITO
        printf("6 - Retorno ou retirada de imoveis de uma dada metragem\n"); //FEITO
        printf("7 - Inserir imovel\n"); //FEITO
        printf("8 - Alterar imovel \n"); //FEITO
        printf("9 - Retirada ou Retorno de todos os imoveis de uma determinada faixa de preco\n"); //FEITO
        printf("10 - Buscar imovel por ID\n"); //FEITO
        printf("11 - Remover imovel por ID\n"); //FEITO
        printf("12 - Imprimir arvore\n"); //FEITO
        printf("13 - Imprimir a primeira folha da arvore\n"); //FEITO
        printf("14 (EXTRA) - Buscar imoveis proximos de um ponto turístico\n"); //FEITO
        printf("15 (EXTRA) - Abrir imóvel no Google Maps\n"); //FEITO
        printf("0 - Sair\n"); //FEITO
        printf("-----------------------------------\n");
        scanf("%d", &comando);
        
        if(comando == 1){
            int b;
            printf("1 - Imovel mais caro\n");
            printf("2 - Imovel mais barato\n");
            scanf("%d,",&b);
            TLSETIM* imovel;
            switch(b){
                case 1:
                    imovel = imoveis_maior_preco(a,t,1);
                    TLSETIM_imprime(imovel);
                    TLSETIM_libera(imovel);
                    break;
                case 2:
                    imovel = imoveis_menor_preco(a,t,1);
                    TLSETIM_imprime(imovel);
                    TLSETIM_libera(imovel);
                    break;
                default:
                    break;
            }

        }

        else if(comando == 2){
            int b;
            printf("1 - Imovel mais caro\n");
            printf("2 - Imovel mais barato\n");
            scanf("%d,",&b);
            TLSETIM* imovel;
            switch(b){
                case 1:
                    imovel = imoveis_maior_preco(a,t,0);
                    TLSETIM_imprime(imovel);
                    TLSETIM_libera(imovel);
                    break;
                case 2:
                    imovel = imoveis_menor_preco(a,t,0);
                    TLSETIM_imprime(imovel);
                    TLSETIM_libera(imovel);
                    break;
                default:
                    break;
            }
        }

        else if(comando == 3){
            int tipo;
            printf("-----------------------------------\n");
            printf("1 - Bairro\n");
            printf("2 - Rua\n");
            printf("3 - CEP\n");
            printf("4 - Latitude\n");
            printf("5 - Longitude\n");
            printf("6 - Tipo\n");
            printf("-----------------------------------\n");
            printf("Digite o tipo de busca que voce quer fazer: ");
            scanf("%d", &tipo);
            getchar();
            if(tipo == 1){
                char bairro[50];
                printf("Digite o bairro que voce quer buscar: ");
                scanf("%49[^\n]", bairro);
                TLSETIM *imoveis = imoveis_string(a,t, comp_bairro, bairro);
                if (!imoveis) printf("Nenhum imovel encontrado no bairro %s\n", bairro);
                TLSETIM_imprime(imoveis);
                TLSETIM_libera(imoveis);
                
            }

            else if(tipo == 2){
                char rua[50];
                printf("Digite a rua que voce quer buscar: ");
                scanf("%49[^\n]", rua);
                TLSETIM *imoveis = imoveis_string(a,t, comp_rua, rua);
                if (!imoveis) printf("Nenhum imovel encontrado na rua %s\n", rua);
                TLSETIM_imprime(imoveis);
                TLSETIM_libera(imoveis);
            }

            else if(tipo == 3){
                char cep[20];
                printf("Digite o CEP que voce quer buscar: ");
                scanf("%s", cep);
                TLSETIM *imoveis = imoveis_string(a,t, comp_cep, cep);
                if (!imoveis) printf("Nenhum imovel encontrado no cep %s\n", cep);
                TLSETIM_imprime(imoveis);
                TLSETIM_libera(imoveis);
            }

            else if(tipo == 4){
                char latitude[20];
                printf("Digite a latitude que voce quer buscar: ");
                scanf("%s", latitude);
                TLSETIM *imoveis = imoveis_string(a,t, comp_latitude, latitude);
                if (!imoveis) printf("Nenhum imovel encontrado na latitude %s\n", latitude);
                TLSETIM_imprime(imoveis);
                TLSETIM_libera(imoveis);
            }

            else if(tipo == 5){
                char longitude[20];
                printf("Digite a longitude que voce quer buscar: ");
                scanf("%s", longitude);
                TLSETIM *imoveis = imoveis_string(a,t, comp_longitude, longitude);
                if (!imoveis) printf("Nenhum imovel encontrado na longitude %s\n", longitude);
                TLSETIM_imprime(imoveis);
                TLSETIM_libera(imoveis);
            }

            else if(tipo == 6){
                char tipo[20];
                printf("Digite o tipo que voce quer buscar: ");
                scanf("%s", tipo);
                TLSETIM *imoveis = imoveis_string(a,t, comp_tipo, tipo);
                if (!imoveis) printf("Nenhum imovel encontrado do tipo %s\n", tipo);
                TLSETIM_imprime(imoveis);
                TLSETIM_libera(imoveis);
            }
        }

        else if(comando == 4){
            int tipo;
            printf("1 - Bairro\n");
            printf("2 - Rua\n");
            printf("3 - CEP\n");
            printf("4 - Latitude\n");
            printf("5 - Longitude\n");
            printf("6 - Tipo\n");
            printf("Digite o tipo do imovel que voce quer remover: ");
            scanf("%d", &tipo);
            getchar();
            if(tipo == 1){
                char bairro[50];
                printf("Digite o bairro que você quer remover: ");
                scanf(" %[^\n]", bairro);
                TLSETIM* imoveis = imoveis_string(a, t, comp_bairro, bairro);
                if (!imoveis) {
                    printf("\nNenhum imovel encontrado no bairro %s\n", bairro);
                }
                else{
                    a = retira_imoveis(a, t, imoveis);
                    printf("\nImoveis removidos com sucesso!");
                }
            }

            if(tipo == 2){
                char rua[50];
                printf("Digite a rua que voce quer remover: ");
                scanf(" %[^\n]", rua);
                TLSETIM* imoveis = imoveis_string(a, t, comp_rua, rua);
                if (!imoveis) {
                    printf("\nNenhum imovel encontrado na %s\n", rua);
                }
                else{
                    a = retira_imoveis(a, t, imoveis);
                    printf("\nImoveis removidos com sucesso!");
                }
            }

            if(tipo == 3){
                char cep[20];
                printf("Digite o cep que voce quer remover: ");
                scanf("%s", cep);
                TLSETIM* imoveis = imoveis_string(a, t, comp_cep, cep);
                if (!imoveis) {
                    printf("\nNenhum imovel encontrado no cep %s\n", cep);
                }
                else{
                    a = retira_imoveis(a, t, imoveis);
                    printf("\nImoveis removidos com sucesso!");
                }
            }

            if(tipo == 4){
                char latitude[20];
                printf("Digite a latitude que voce quer remover: ");
                scanf("%s", latitude);
                TLSETIM* imoveis = imoveis_string(a, t, comp_latitude, latitude);
                if (!imoveis) {
                    printf("\nNenhum imovel encontrado no latitude %s\n", latitude);
                }
                else{
                    a = retira_imoveis(a, t, imoveis);
                    printf("\nImoveis removidos com sucesso!");
                }
            }

            if(tipo == 5){
                char longitude[20];
                printf("Digite a longitude que voce quer remover: ");
                scanf("%s", longitude);
                TLSETIM* imoveis = imoveis_string(a, t, comp_longitude, longitude);
                if (!imoveis) {
                    printf("\nNenhum imovel encontrado na longitude %s\n", longitude);
                }
                else{
                    a = retira_imoveis(a, t, imoveis);
                    printf("\nImoveis removidos com sucesso!");
                }
            }

            if(tipo == 6){
                char tipo[50];
                printf("Digite o tipo que voce quer remover: ");
                scanf("%s", tipo);
                TLSETIM* imoveis = imoveis_string(a, t, comp_tipo, tipo);
                if (!imoveis) {
                    printf("\nNenhum imovel encontrado no bairro %s\n", tipo);
                }
                else{
                    a = retira_imoveis(a, t, imoveis);
                    printf("\nImoveis removidos com sucesso!");
                }
            }

        }

        else if(comando == 5){
            unsigned long int id;
            printf("Digite o ID do imovel que voce quer buscar: ");
            scanf("%lu", &id);
            imprimir_info_imovel(a, id, t);
        }

        else if(comando == 6){
            int op, m;
            printf("-----------------------------------\n");
            printf("1 - Retorno de imoveis dada uma certa metragem\n");
            printf("2 - Retirada de imoveis dada uma certa metragem\n");
            scanf("%d", &op);
            getchar();
            if(op == 1){
                printf("Insira a metragem desejada: ");
                scanf("%d", &m);
                TLSETIM* imoveis = imoveis_metragem(a, t, m);
                if(!imoveis) printf("\nNao foram encontrados imoveis com a metragem dada!");
                TLSETIM_imprime(imoveis);
                TLSETIM_libera(imoveis);
            }

            else if(op == 2){
                printf("Insira a metragem desejada: ");
                scanf("%d", &m);
                TLSETIM* imoveis = imoveis_metragem(a, t, m);
                if(!imoveis) printf("\nNao foram encontrados imoveis com a metragem dada!");
                else {
                    a = retira_imoveis(a, t, imoveis);
                    printf("\nImoveis removidos com sucesso!");
                }
            }

            else{
                printf("Opcao Invalida!");
            } 
        }

        else if(comando == 7){
            unsigned long int id;
            char bairro[50];
            char tipo[50];
            char rua[50];
            int num;
            float preco_total;
            float preco_por_m2;
            char descricao[2500];
            char CEP[20];
            char latitude[20];
            char longitude[20];
            TIM* imovel = (TIM*)malloc(sizeof(TIM));

            printf("\nInsira o id do imovel:");
            scanf("%lu", &id);
            imovel->id = id;
            getchar();

            printf("\nInsira o bairro do imovel:");
            scanf("%49[^\n]", bairro);
            strcpy(imovel->bairro, bairro);
            getchar();

            printf("\nInsira o tipo do imovel:");
            scanf("%49[^\n]", tipo);
            strcpy(imovel->tipo, tipo);
            getchar();

            printf("\nInsira o rua do imovel:");
            scanf("%49[^\n]", rua);
            strcpy(imovel->rua, rua);
            getchar();

            printf("\nInsira o numero do imovel:");
            scanf("%d", &num);
            imovel->num = num;
            getchar();

            printf("\nInsira o preco total:");
            scanf("%f", &preco_total);
            imovel->preco_total = preco_total;
            getchar();

            printf("\nInsira o preco por m2 do imovel:");
            scanf("%f", &preco_por_m2);
            imovel->preco_por_m2 = preco_por_m2;
            getchar();

            printf("\nInsira a descricao do imovel:");
            scanf("%2499[^\n]", descricao);
            strcpy(imovel->descricao, descricao);
            getchar();

            printf("\nInsira a CEP do imovel:");
            scanf("%19[^\n]", CEP);
            strcpy(imovel->CEP, CEP);
            getchar();

            printf("\nInsira a latitude do imovel:");
            scanf("%19[^\n]", latitude);
            strcpy(imovel->latitude, latitude);
            getchar();

            printf("\nInsira a longitude do imovel:");
            scanf("%19[^\n]", longitude);
            strcpy(imovel->longitude, longitude);

            a = TARVBP_insere(a, imovel, t);
            imprime_imovel(imovel);
            printf("\nImovel adicionado com sucesso!");
        }

        else if(comando == 8){
            int opcao;
            unsigned long int id;
            printf("-----------------------------------\n");
            printf("Informe o id do imovel:\n");
            scanf("%lu", &id);
            TIM* imovel = buscaImovel_id(a, id, t);

            if(!imovel) printf("Imovel nao encontrado.\n");


            else{
                printf("-----------------------------------\n");
                printf("1 - Preco total\n");
                printf("2 - Preco por metro quadrado\n");
                printf("3 - Descricao\n");
                printf("4 - Latitude\n");
                printf("5 - Longitude\n");
                printf("6 - Numero\n");
                printf("7 - Rua\n");
                printf("8 - Tipo\n");
                printf("9 - Bairro\n");
                printf("10 - CEP\n");

                printf("-----------------------------------\n");
                printf("Digite o tipo de alteracao que voce quer fazer: ");
                scanf("%d", &opcao);
                getchar();
                
                if(opcao == 1){
                    float preco_total;
                    printf("Digite o novo preco total: ");
                    scanf("%f", &preco_total);
                    a = edita_preco_total(a, t, id, preco_total);
                    printf("\nImovel alterado com sucesso!");
                }
                else if(opcao == 2){
                    float preco_por_m2;
                    printf("Digite o novo preco por metro quadrado: ");
                    scanf("%f", &preco_por_m2);
                    a = edita_preco_porM2(a, t, id, preco_por_m2);
                    printf("\nImovel alterado com sucesso!");
                }

                else if(opcao == 3){
                    char str[2500];
                    printf("Digite a nova descricao: ");
                    scanf(" %[^\n]", str);
                    a = edita_descricao(a, t, id, str);
                    printf("\nImovel alterado com sucesso!");
                }
                else if(opcao == 4){
                    char str[10];
                    printf("Digite a nova longitude: ");
                    scanf(" %[^\n]", str);
                    a = edita_longitude(a, t, id, str);
                    printf("\nImovel alterado com sucesso!");
                }
                else if(opcao == 5){
                    char str[10];
                    printf("Digite a nova latitude: ");
                    scanf(" %[^\n]", str);
                    a = edita_latitude(a, t, id, str);
                    printf("\nImovel alterado com sucesso!");
                }
                else if(opcao == 6){
                    int str;
                    printf("Digite o novo numero: ");
                    scanf(" %d", &str);
                    a = edita_num(a, t, id, str);
                    printf("\nImovel alterado com sucesso!");
                }
                else if(opcao == 7){
                    char str[300];
                    printf("Digite a nova rua: ");
                    scanf(" %[^\n]", str);
                    a = edita_rua(a, t, id, str);
                    printf("\nImovel alterado com sucesso!");
                }
                else if(opcao == 8){
                    char str[10];
                    printf("Digite o novo tipo: ");
                    scanf(" %[^\n]", str);
                    a = edita_tipo(a, t, id, str);
                    printf("\nImovel alterado com sucesso!");
                }
                else if(opcao == 9){
                    char str[200];
                    printf("Digite o novo bairro: ");
                    scanf(" %[^\n]", str);
                    a = edita_bairro(a, t, id, str);
                    printf("\nImovel alterado com sucesso!");
                }
                else if(opcao == 10){
                    char str[10];
                    printf("Digite o novo CEP: ");
                    scanf(" %[^\n]", str);
                    a = edita_cep(a, t, id, str);
                    printf("\nImovel alterado com sucesso!");
                }
                else printf("Opcao Invalida!");
            }    
            free(imovel);
        }

        else if(comando == 9){
            int op;
            float lim_inf, lim_sup;
            printf("-----------------------------------\n");
            printf("1 - Retornar todos os imoveis de uma determinada faixa de preco\n");
            printf("2 - Remover todos os imoveis de uma determinada faixa de preco\n");
            scanf("%d", &op);
            getchar();
            if(op == 1){
                printf("-----------------------------------\n");
                printf("\nInforme o limite inferior: ");
                scanf("%f", &lim_inf);
                printf("\nInforme o limite superior: ");
                scanf("%f", &lim_sup);
                TLSETIM* imoveis = imoveis_faixa_de_preco(a, t, lim_inf, lim_sup);
                if(!imoveis) printf("\nNao foram encontrados imoveis com a faixa de preco dada!");
                TLSETIM_imprime(imoveis);
                TLSETIM_libera(imoveis);
            }

            else if(op == 2){
                printf("-----------------------------------\n");
                printf("\nInforme o limite inferior: ");
                scanf("%f", &lim_inf);
                printf("\nInforme o limite superior: ");
                scanf("%f", &lim_sup);
                TLSETIM* imoveis = imoveis_faixa_de_preco(a, t, lim_inf, lim_sup);
                if(!imoveis) printf("\nNao foram encontrados imoveis com a faixa de preco dada!");
                else {
                    a = retira_imoveis(a,t,imoveis);
                    printf("\nImoveis removidos com sucesso!");
                }
            }

            else printf("\nOpcao Invalida!");
            
        }           

        if(comando == 10){
            long unsigned int id;
            printf("Digite o ID do imovel que voce quer buscar: ");
            scanf("%lu", &id);
            TIM* imovel = buscaImovel_id(a, id, t);
            if(imovel != NULL) {
                imprime_imovel(imovel);
                free(imovel);
            } else {
                printf("Imovel não encontrado.\n");
            }
        }


        else if(comando == 11){ //TESTANDO PARA NÓ FOLHA!!!
            unsigned long int id;
            printf("Digite o ID do imovel que voce quer remover: ");
            scanf("%lu", &id);
            a = TARVBP_retira(a, id, t);
            printf("\nNo removido com sucesso!");
        }

        else if(comando == 12){
            TARVBP_imprime(a, t);
        }

        else if(comando == 13){
            TARVBP_imprime_imovel_primeira_folha(a, t);
        }
        else if(comando == 14){
            double distanciaMax;
            int buff;
            printf("Mostra todos os imóveis a uma distância máxima de um ponto turístico escolhido.\n");
            //CRISTO REDENTOR, COPACABANA PALACE, MARACANA, FLORESTA DA TIJUCA, TEATRO MUNICIPAL,
            // RESERVA DO GRAJAU, PARQUE LAGE, MURETA DA URCA, PRAIA DE BOTAFOGO, PRAIA DE IPANEMA, LAGOA
            printf(" 1 - Cristo Redentor\n 2 - Copacabana Palace\n 3 - Maracana\n 4 - Floresta da Tijuca\n 5 - Teatro Municipal\n 6 - Reserva do Grajau\n 7 - Parque Lage"
                   "\n 8 - Mureta da Urca\n 9 - Praia de Botafogo\n 10 - Praia de Ipanema\n 11 - Lagoa\n 12 - Calçadão de Copacabana\n 13 - Universidade Federal Fluminense\n");
            scanf("%d", &buff);
            buff--;
            if(buff>12)continue;
            //Locale forçava que a gente tinha que aceitar vírgula pra float tipo 3,14, então mudei pra ponto pra parecido com o mundo todo.
            setlocale(LC_NUMERIC, "C"); // Temporariamente muda para aceitar ponto
            printf("Diga a  distância máxima possível do ponto turístico, em kilometros\n");
            scanf("%lf", &distanciaMax);
            setlocale(LC_NUMERIC, "pt_BR.UTF-8"); // Volta para o locale brasileiro, preciso dos acentos.
            
            TLSETIM* imoveis = imoveis_prox_de(a, t, distanciaMax, pt_latitude[buff], pt_longitude[buff]);
            TLSETIM_libera(imoveis);
        }
        else if(comando == 15){
            unsigned long int id;
            printf("Digite o ID do imovel que voce quer abrir no Google Maps: ");
            scanf("%lu", &id);
            TIM* te = buscaImovel_id(a,id,t);
            if(te){
                abrir_google_maps(a, t, id);
            }
            else{
                printf("Imovel nao encontrado.");
            }
            free(te);
        }


    }while(comando != 0);
    if(a){
        TARVBP_libera(a,t);
    }
    return 0;
}
