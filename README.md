# Cracolandia

Project done in the scope of the course Analysis and Synthesis of Algorithms, IST 2022/2023.

# Index

1. [Relatório](#Relatório)
    1. [Descrição do Problema e da Solução](#Descrição-do-Problema-e-da-Solução)
    1. [Análise Teórica](#Análise-Teórica)
    1. [Avaliação Experimental dos Resultados](#Avaliação-Experimental-dos-Resultados)

# Relatório

## Descrição do Problema e da Solução
A solução que encontrámos passa por utilizar uma variação do algoritmo de
Kruskal. Em vez de querermos a Minimum Spanning Tree, no contexto deste
problema pretendemos encontrar a Maximum Spanning Tree, i.e., a spanning tree
cuja soma dos pesos é máxima. Para isto basta ordenarmos os arcos por ordem
decrescente, em vez de crescente, e aplicar o algoritmo normalmente.

## Análise Teórica
A complexidade total do algoritmo é O(ElogV), sendo E o número de arcos e V o
número de vértices do grafo. Abaixo encontra-se uma análise mais
pormenorizada de cada função.

- **Leitura de dados de entrada** (read_input): Simples leitura do input, com um
ciclo que depende do número de arcos do grafo, Θ(E).
- **Implementação do algoritmo de kruskal** (get_maximum_cost_spanning_tree):
Para ordenar os arcos do grafo foi utilizada a função sort, da Standard Template
Library do c++, que tem complexidade O(ElogE) [(1)](#Notas). A estrutura de dados utilizada
para representar os conjuntos disjuntos foi uma árvore e fazemos uso das
heurísticas “compressão de caminhos” e “união por categoria”. Atendendo a isto,
sabemos que é possível assegurar que a complexidade desta implementação é
O(ElogV) [(2)](#Notas).
- **Operações sobre conjuntos disjuntos** (make_set, find_set, link, node_union):
Tanto a função make_set como a função link são O(1). Quanto ás funções
find_set e node_union, a utilização das heurísticas “compressão de caminhos” e
“união por categoria” garante-nos que ambas são O(V) [(3)](#Notas).

#### Notas:

1. https://en.cppreference.com/w/cpp/algorithm/sort
1. CLRS, pág. 633
1. CLRS, pág. 572

## Avaliação Experimental dos Resultados

Após gerados testes com número de vértices crescente, computou-se o seguinte
gráfico, onde o eixo das abcissas representa o número vértices e o eixo das
ordenadas representa o tempo, em segundos, que demorou a ser obtido o
resultado com o algoritmo final.

![](https://github.com/Rua-Gouveia-Alliance/Cracolandia/blob/main/imgs/graph1.png)

Este resultado indica-nos, na prática, que a complexidade do algoritmo é linear
para problemas relacionados com o objetivo do projeto, ou seja, que respeitam a
triangulação de Delaunay. No entanto, a análise teórica acima realizada diz
respeito à complexidade real do algoritmo e pressupõe grafos aleatórios. Após a
realização de mais testes, desta vez com grafos aleatórios, e da elaboração de
um novo gráfico, verifica-se a validez do resultado obtido na análise teórica.

![](https://github.com/Rua-Gouveia-Alliance/Cracolandia/blob/main/imgs/graph2.png)
