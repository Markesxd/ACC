# Compilador de C
Este projeto se constitui em criar um simples compilador de C.

## Compilação
para compilar o programa basta utilizar o script do makefile:
```
make all
```
## Palavras Reservadas
As palavras resevadas são as seguintes
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
O compilador é composto por 4 partes: analisador léxico, sintático, semântico e gerador de código.
A principio o programa lerá um arquivo com o código fonte e irá passa-lo para a memória principal onde o analisador lexico vai transformá-lo em uma lista de tokens...

## Analisador Léxico
O analisador léxico tem a função de vasculhar o código fonte em busca de tokens (palavras válidas na linguagem) e classificá-los. Para isso este percore a string do código buscando caracteres que existem no final de tokens (' ', '\n', '(', etc) e então ele agrupa os tokens em uma das seguintes categorias:
- Palara Reservada
- Identificador
- Inteiro
- Separador
- Operador
- Número de Ponto Flutuante
- Delimitador
- Cadeia de Caracteres
- caractere
## Analisador Sintático
O analisador sintático tem como propósito verificar se os tokens estão organizados de maneira adequada para se formar comandos. Os comandos aceitos pelo compilador são os descritos pela seguinte gramática:  
  
code &rarr; command code  
code &rarr; EOF  
  
command &rarr; declaration  
command &rarr; atribution  
command &rarr; condition  
command &rarr; WhileLoop  
command &rarr; forLoop  
  
declaration &rarr; &lt;type&gt; &lt;a&gt; A;  
A &rarr; = expression A'  
A &rarr; &lt; &gt;  
A' &rarr; , &lt;a&gt; A  
  
atribution &rarr; ++&lt;a&gt;;    
atribution &rarr; --&lt;a&gt;;    
atribution &rarr; &lt;a&gt;++;    
atribution &rarr; &lt;a&gt;--;    
atribution &rarr; &lt;a&gt; = expression;    
  
condition &rarr; if(expression) block    
whileLoop &rarr; while(expression) block    
forLoop &rarr; for(declaration; expression; attibution) block    
  
block &rarr; command  
block &rarr; {command B'  
B' &rarr; command B'  
B' &rarr; }
  
expression &rarr; E X'  
X' &rarr; > X'' E X'    
X' &rarr; < X'' E X'    
X' &rarr; != E X'    
X' &rarr; == E X'    
X' &rarr; && E X'    
X' &rarr; || E X'    
X'' &rarr; =    
E &rarr; T E'    
E' &rarr; + T E'  
E' &rarr; - T E'  
T &rarr; F T'  
T &rarr; -F T'  
T &rarr; !F T'  
T' &rarr; * F T'  
T' &rarr; / F T'  
T' &rarr; % F T'  
F &rarr; (expression)  
F &rarr; &lt;a&gt;  
F &rarr; &lt;n&gt;  

## Analisador Semântico
Em Desenvolvimento...

## Gerador de código
Em Desenvolvimento...