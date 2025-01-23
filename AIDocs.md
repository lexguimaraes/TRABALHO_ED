# Documentação do Sistema de Gerenciamento de Imóveis

## Estrutura do Projeto

O projeto está organizado em bibliotecas que implementam diferentes funcionalidades:

### 1. TIM (Tipo Imóvel)
Arquivo: `bibliotecas/TIM.c` e `headers/TIM.h`

Responsável pelo gerenciamento da estrutura básica de um imóvel, contendo:
- Funções de leitura/escrita de imóveis
- Manipulação de dados do imóvel
- Conversão de tipos

#### Funções Principais:
- `TIM* ler_imovel(char* linha)`: Lê um imóvel a partir de uma linha do arquivo CSV
- `void imprime_imovel(TIM* imovel)`: Imprime todos os dados de um imóvel
- `TIM* copia_imovel(TIM* imovel)`: Cria uma cópia profunda de um imóvel
- `void salva(TIM* imovel, int indice)`: Salva um imóvel em arquivo binário

### 2. ARVBP (Árvore B+ para Imóveis)
Arquivo: `bibliotecas/ARVBP.c` e `headers/ARVBP.h`

Implementa a estrutura de Árvore B+ para indexação dos imóveis, com:
- Operações básicas da árvore
- Gerenciamento de arquivos dos nós
- Busca e manipulação de registros

#### Funções Principais:
- `TARVBP* TARVBP_cria(char* arq, int t)`: Cria um novo nó da árvore
- `TARVBP* TARVBP_insere(TARVBP* a, TIM* IM, int t)`: Insere um imóvel na árvore
- `TARVBP* TARVBP_busca(TARVBP* a, unsigned long int id, int t)`: Busca um imóvel por ID
- `void TARVBP_imprime(TARVBP *a, int t)`: Imprime a estrutura da árvore
- `TIM* buscaImovel_id(TARVBP* a, unsigned long int id, int t)`: Retorna o imóvel com o ID especificado

### 3. TLSEIM (Lista Encadeada de Imóveis)
Arquivo: `bibliotecas/TLSEIM.c` e `headers/TLSEIM.h`

Gerencia listas encadeadas de imóveis, utilizada para:
- Armazenar resultados de consultas
- Manipular conjuntos de imóveis

#### Funções Principais:
- `TLSETIM* TLSETIM_inicializa()`: Inicializa uma lista vazia
- `TLSETIM* TLSETIM_insere(TLSETIM* l, TIM* IM)`: Insere um imóvel na lista
- `void TLSETIM_libera(TLSETIM* l)`: Libera a memória da lista
- `void TLSETIM_imprime(TLSETIM* l)`: Imprime todos os imóveis da lista

### 4. Extras (Funções Auxiliares)
Arquivo: `bibliotecas/extras.c` e `headers/extras.h`

#### Funções de Busca
- `TARVBP* busca_arq(TARVBP* a, char* arq, int t)`: Busca um nó específico pelo nome do arquivo
- `int maior_desc(TARVBP*a, int t)`: Retorna o tamanho da maior descrição na árvore

#### Funções de Consulta por Preço
- `TLSETIM* imoveis_maior_preco(TARVBP*a, int t)`: Lista os imóveis mais caros
- `TLSETIM* imoveis_menor_preco(TARVBP*a, int t)`: Lista os imóveis mais baratos

#### Funções de Consulta por String
- `TLSETIM* imoveis_string(TARVBP*a, int t, int (*comp)(TIM*,char*), char* str)`: 
  - Busca imóveis que correspondam a uma string em um campo específico
  - Utiliza função de comparação passada como parâmetro

#### Funções de Manipulação
- `TARVBP* retira_imoveis(TARVBP* a, int t, TLSETIM* IMs)`: Remove um conjunto de imóveis
- `TARVBP* remove_imoveis_metragem(TARVBP* a, int t, int m)`: Remove imóveis por metragem
- `TARVBP* edita(TARVBP* a, int t, unsigned long int id, char* str)`: Edita descrição de imóvel

#### Funções de Comparação
Todas retornam um inteiro para comparação de strings:
- `comp_bairro(TIM* IM, char* bairro)`: Compara bairros
- `comp_tipo(TIM* IM, char* tipo)`: Compara tipos de imóvel
- `comp_rua(TIM* IM, char* rua)`: Compara ruas
- `comp_latitude(TIM* IM, char* latitude)`: Compara latitudes
- `comp_longitude(TIM* IM, char* longitude)`: Compara longitudes
- `comp_cep(TIM* IM, char* cep)`: Compara CEPs
- `comp_desc(TIM* IM, char* desc)`: Compara descrições

### Remoção em Árvore B+
A função `TARVBP_retira` implementa a remoção de registros seguindo as propriedades da árvore B+:

#### Lógica Principal
1. **Localização do Registro**
   - Desce a árvore até encontrar o nó folha que contém o registro
   - Usa as chaves dos nós internos para navegação

2. **Remoção do Registro**
   - Remove o registro do nó folha
   - Reorganiza os registros restantes para manter continuidade
   - Atualiza o número de chaves do nó

3. **Tratamento de Underflow**
   - Verifica se o nó ficou com menos que t-1 chaves (underflow)
   - Tenta resolver o underflow através de:
     1. Redistribuição com irmão (se possível)
     2. Merge com irmão (se redistribuição não for possível)

#### Detalhamento da Implementação
```c
TARVBP* TARVBP_retira(TARVBP* a, unsigned long int id, int t)
```

1. **Caso Base - Nó Folha**
   - Encontra a posição do registro a ser removido
   - Libera o registro e reorganiza o nó
   - Se não houver underflow, apenas atualiza o arquivo

2. **Tratamento de Underflow**
   - Busca o pai do nó atual usando `busca_pai`
   - Identifica os irmãos (esquerdo e direito)
   - Tenta redistribuição:
     - Verifica se algum irmão tem chaves extras (> t-1)
     - Se sim, move chaves do irmão para o nó atual
   - Se redistribuição falhar, faz merge:
     - Une o nó com um de seus irmãos
     - Remove a chave separadora do pai
     - Atualiza ponteiros entre folhas

3. **Caso Recursivo - Nó Interno**
   - Localiza o filho que contém o registro
   - Chama recursivamente a remoção nesse filho
   - Atualiza chaves do nó interno se necessário

4. **Casos Especiais**
   - Tratamento da raiz quando fica vazia
   - Manutenção dos ponteiros entre folhas após merge
   - Atualização dos arquivos em disco

#### Funções Auxiliares
1. `merge_nos`: Une dois nós quando há underflow
2. `redistribui_nos`: Move chaves entre nós irmãos
3. `busca_pai`: Localiza o nó pai na árvore

#### Pontos Essenciais da Árvore B+
- Todas as folhas ficam no mesmo nível
- Nós internos guardam apenas chaves para navegação
- Folhas mantêm ponteiros para próxima folha
- Nós (exceto raiz) devem ter pelo menos t-1 chaves
- Remoção pode causar underflow e reorganização da árvore

### Underflow em Árvore B+

O underflow é uma condição que ocorre quando um nó da árvore B+ fica com menos chaves que o mínimo permitido após uma operação de remoção.

#### Regras de Ocupação
Em uma árvore B+ de ordem t:
- Nós não-raiz devem ter no mínimo t-1 chaves
- A raiz deve ter pelo menos 1 chave
- Qualquer nó pode ter no máximo 2t-1 chaves

#### Exemplo
Para uma árvore B+ com t=3:
- Mínimo de chaves por nó: 2 (t-1)
- Máximo de chaves por nó: 5 (2t-1)
- Se um nó não-raiz ficar com apenas 1 chave, ocorre underflow

#### Tratamento do Underflow
1. **Tentativa de Redistribuição**
   - Verifica se um irmão adjacente tem chaves extras (> t-1)
   - Se sim, move chaves do irmão para o nó com underflow
   - Atualiza as chaves do pai para refletir a nova distribuição
   - Preserva a ordem e os ponteiros entre folhas

2. **Merge (Quando Redistribuição Falha)**
   - Une o nó com underflow com um de seus irmãos
   - Remove a chave separadora do pai
   - Atualiza ponteiros entre folhas (se forem nós folha)
   - Pode causar underflow no pai, propagando a reorganização para cima

#### Casos Especiais
1. **Underflow na Raiz**
   - A raiz pode ter menos que t-1 chaves
   - Se ficar vazia após remoção, é substituída por seu único filho

2. **Propagação do Underflow**
   - O merge pode causar underflow no pai
   - O processo se repete recursivamente até:
     - Encontrar um nó com chaves suficientes
     - Chegar à raiz
     - Conseguir fazer redistribuição

#### Implementação
```c
// Verifica se há underflow
if (a->n_chaves < t-1 && strcmp(a->arq, "0") != 0) {
    // Tenta redistribuição primeiro
    if (irmao->n_chaves > t-1) {
        redistribui_nos(no, irmao, pai, pos_pai, t);
    }
    // Se não for possível, faz merge
    else {
        merge_nos(no, irmao, pai, pos_pai, t);
    }
}
```

#### Tratamento Especial da Raiz
- A raiz é armazenada no arquivo "0"
- Identificada por `strcmp(a->arq, "0") == 0`
- Única exceção à regra de mínimo de chaves (t-1)
- Só é modificada quando:
  - Fica completamente vazia (n_chaves = 0)
  - É substituída por seu único filho

## Como Rodar

### Compilação
Para compilar o programa, execute o seguinte comando no terminal:
```bash
gcc main.c bibliotecas/ARVBP.c bibliotecas/TIM.c bibliotecas/extras.c bibliotecas/TLSEIM.c -o programa
```

### Execução
Após compilar, execute o programa com:
```bash
./programa
```

