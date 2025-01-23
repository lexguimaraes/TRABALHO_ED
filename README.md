# Sistema de Gerenciamento de Imóveis - Rio de Janeiro

Um sistema de gerenciamento de imóveis utilizando Árvore B/B+ em memória secundária.

## 📋 Descrição

O sistema gerencia um catálogo de imóveis do Rio de Janeiro usando uma estrutura de Árvore B ou B+ (e opcionalmente tabelas hash) em memória secundária. Cada nó folha da árvore é representado em um arquivo separado, enquanto os nós internos podem ser mantidos em um único arquivo.

## 📊 Formato do Catálogo

Cada registro contém os seguintes campos separados por ponto e vírgula (;):
- ID único
- Bairro do imóvel
- Tipo de imóvel
- Rua (opcional)
- Número do imóvel (opcional)
- Preço total
- Preço por metro quadrado
- Descrição
- CEP
- Latitude
- Longitude

## 🚀 Parâmetros de Entrada

- Grau mínimo da árvore B (t)
- Catálogo inicial no formato especificado

## ✅ Operações a serem implementadas

### Consultas por Preço
- [x] Retorno do imóvel a venda mais barato (e caro)
- [x] Retorno do imóvel alugado mais barato (e caro)
- [x] Retirada de todos os imóveis de uma determinada faixa de preço

### Consultas por Localização
- [x] Retorno de todos os imóveis, dado um CEP (e uma latitude, e uma longitude)
- [x] Retorno de todos os imóveis de uma mesma rua
- [x] Retorno de todos os imóveis de um mesmo bairro

### Consultas por Tipo e Filtros Compostos
- [x] Retorno de todos os imóveis dado um tipo
- [x] Retirada de todos os imóveis dado um tipo (e um CEP, e um nome de rua, e uma latitude e uma longitude, e um bairro)
- [x] Retorno ou retirada de imóveis de uma dada metragem

### Consultas por Descrição
- [x] Dada a descrição, obter informações a mais do imóvel
- [x] Retorno de informações adicionais dada a descrição (vaga, salão de festas, valor do condomínio, entre outras informações que seu grupo considerar relevante)

### Operações de Modificação
- [x] Inserção de imóveis
- [x] Alteração de ALGUMAS informações adicionais, a saber: preço total, preço por metro quadrado e descrição

## 🔍 Restrições de Implementação

- Os nós folha da árvore B devem estar em arquivos separados
- Apenas as informações necessárias para a operação atual devem ser mantidas em memória principal
- Durante operações como inserção, apenas os arquivos no caminho da raiz até a folha podem estar em memória

## 📅 Informações do Projeto

### Prazos
- **Entrega:** 22/01 às 23:59h
- **Apresentações:** 22, 23 e 24 de janeiro
- **Email para envio:** rosseti@ic.uff.br
