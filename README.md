# Compilador de C
Este projeto se constitui em criar um simples compilador de C.

## Compilação
para compilar o programa basta utilizar o script do makefile:
```
make all
```
## Palavras Reservadas
O compilador recebe uma lista de palavras reservadas do arquivo "reservedWords.txt", ele pode ser alterado caso se deseje modificar quais palavras reservadas o programa deve reconhecer. O arquivo deve manter o padrão de uma palavra por linha. <br>
Por padrão as palavras reseva   das são as seguintes
- if
- else
- for
- while
- int
- char
- float
- void
- return
- break
- continue

## Funcionamento Básico
O compilador é composto por 4 partes: analizador léxico, sintático, semântico e gerador de código.
A principio o programa lerá um arquivo com o código fonte e irá passa-lo para a memória principal onde o analizador lexico vai transformá-lo em uma lista de tokens...

## Analizador Léxico
O analizador léxico tem a função de vasculhar o código fonte em busca de tokens (palavras válidas na linguagem) e classificá-los. Para isso este percore a string do código buscando caracteres que existem no final de tokens (' ', '\n', '(', etc) e então ele agrupa os tokens em uma das seguintes categorias:
- Palara Reservada
- Identificador
- Inteiro
- Separador
- Operador
- Número de Ponto Flutuante
- Delimitador
- Cadeia de Caracteres
## Analizador Sintático
Em Desenvolvimento...

## Analizador Semântico
Em Desenvolvimento...

## Gerador de código
Em Desenvolvimento...