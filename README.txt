A compilação é simples, basta compilar o main: 
    g++ main.cpp -o main.exe


O código com a estrutura em si está no "bst-eda.cpp"

As estruturas também são bem diretas:

    struct node: Representa um nó da BST persistente

    struct mod: Armazena uma modificação em um nó

    struct BST: Mantém a raíz, o vetor de versões, e a versão atual. É usada para gerenciar a árvore

Quanto às funções:

    ler_mods: Lê um nó em uma versão específica e copia seu estado para um nó auxiliar.

    operacao: Funciona quase como uma atribuição. Adiciona uma modificação no vetor de mods de um nó. Chama atualizar_no se o vetor mods estiver cheio.

    atualizar_no: Atualiza o estado efêmero* criando um novo nó quando o vetor de mods estoura, aplicando todas modificações acumuladas no estado passado.
    (o estado efêmero, como dito no artigo do Driscoll, é o estado inicial de um nó, sem ser afetado pelo vetor de mods atual dele.)

    inserir, remover, transplantar: atuam na última versão da árvore, criando uma nova

    busca, minimo, maximo, sucessor, imprimir: Efetuam a leitura da árvore na versão desejada.


E o código que lê o arquivo de texto (linha por linha) e chama as funções da estrutura é o "main.cpp", que usa bibliotecas básicas do cpp para a leitura do arquivo.

e é isso!