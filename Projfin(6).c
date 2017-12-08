/*======================================================================================================================
UTFPr Engenharia Mecânica
Mariana Mayer Grigoleto
RA: 1907751

Projeto final - Planilha de gastos (funções de gasto máximo, pesquisa por gasto, gasto mensal e gravação em arquivo)

=======================================================================================================================*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#define scanfstr(str) scanf("%*c%[^\n]", str)

typedef struct produto
{
    char nome[80];
    float preco;
    int quantidade;
    int mes;
    struct produto *prox;
} Produtos;


Produtos* inserir_ordenado(Produtos *produtos, Produtos* novo);
Produtos* inserir(Produtos* produtos);
void mostrar_produtos(Produtos* produtos);
Produtos* abrir(Produtos* produtos);
void busca(Produtos *produtos, char busca[20]);
float gastos_mensais (Produtos *produtos);
void max(Produtos *produtos);

main()
{
    int opcao;
    int flag = 0;
    char buscado[20];
    float gastos;
    Produtos *produtos = NULL;
    do
    {
        printf("\n\tMENU:\n\t0-Sair\n\t1-Inserir gastos\n\t2-Abrir dados\n\t3-Mostrar Planner\n\t4-Pesquisa por produto\n\t5-Pesquisa custo max\n\t6-Gastos mensais\n\t");
        printf("Entre com opcao->");
        scanf("%d", &opcao);

        switch(opcao)
        {
        case 1:
            if(flag == 0){
                produtos = abrir(produtos);   //flag para evitar duplicação de dados caso o usuário acesse arquivos e depois decida inserir novos itens//
                produtos = inserir(produtos);
                }else{
                produtos = inserir(produtos);
                }
            break;
        case 2:
            produtos = abrir(produtos);
            flag = 1;
            break;
        case 3:
            mostrar_produtos(produtos);
            break;
        case 4:
            printf("\n\tElemento a ser buscado:");
            scanfstr(buscado);
            int i, a = strlen(buscado);
            for(i = 0; i < a; i++)
            {
                buscado[i] = toupper(buscado[i]);  //transforma todas os caracteres para caixa alta//
            }
            busca(produtos,buscado);
            break;
        case 5:
            max(produtos);
            break;
        case 6:
            gastos = gastos_mensais(produtos);
            if(gastos == 0)
            {
                printf("\n\t---NAO HA GASTOS MENSAL---\n\t");
            }
            else
            {
                printf("\n\tGASTO MENSAL-> %4.2f\n", gastos);
            }
            break;
        }

    }
    while(opcao!=0);

}

Produtos * inserir_ordenado (Produtos *produtos, Produtos *novo)
{

    Produtos *anterior;
    Produtos *atual;


    atual = produtos;
    anterior = NULL;

    if(atual == NULL)
    {
        novo->prox = NULL;
        produtos = novo;
    }
    else
    {
        while(atual != NULL && strcmp(atual->nome, novo->nome) < 0)
        {
            anterior = atual;   //caminha na lista até encontrar a posição de ordenamento (salva o atual e o anterior//
            atual = atual->prox;
        }
        novo->prox = atual;  //ao encontrar a posição atribui ao novo prox o fim da lista//
        if(anterior == NULL) //analisa como ordenar o resto da lista//
        {
            produtos = novo; //caso seja no inicio da lista//
        }
        else
        {
            anterior->prox = novo;  //caso seja no meio ou no final da lista//
        }
    }

    return produtos;
}


Produtos* inserir(Produtos *produtos)
{
    char name[80];
    Produtos *novo =  (Produtos *) malloc(sizeof(Produtos));

    printf("\n\tDescricao do gasto:");
    scanfstr(name);
    int i, a = strlen(name);
    for(i = 0; i < a; i++)
    {
        name[i] = toupper(name[i]);
    }
    printf("\n\tPreco:");
    scanf("%f",&novo->preco);
    printf("\n\tQuantidade:");
    scanf("%d",&novo->quantidade);
    printf("\n\tMes:");
    scanf("%d",&novo->mes);
    strcpy(novo->nome, name);

    produtos = inserir_ordenado(produtos, novo);

    return produtos;
}

void mostrar_produtos (Produtos* produtos)
{

    if(produtos != NULL)
    {
        printf("\n\tPRODUTO: %s\n", produtos->nome);
        printf("\n\tPRECO: %4.2f\n", produtos->preco);
        printf("\n\tQNT.: %d\n", produtos->quantidade);
        printf("\n\tDATA: %d\n", produtos->mes);
        mostrar_produtos(produtos->prox);
    }

}

Produtos* abrir(Produtos* produtos)
{
    int a;
    char nome[20];
    Produtos *novo;

    FILE *arq=fopen("PLANNER.txt", "r");

    a=fscanf(arq, "%[^\n]", &nome);

    while (a!= EOF)
    {
        novo =  (Produtos *) malloc(sizeof(Produtos));
        strcpy(novo->nome, nome);
        fscanf(arq,"%f\n", &novo->preco);
        fscanf(arq,"%d\n", &novo->quantidade);
        fscanf(arq,"%d\n", &novo->mes);
        produtos = inserir_ordenado(produtos, novo);
        a=fscanf(arq, "%[^\n]", &nome);
    }
    fclose(arq);
    return produtos;
}


void busca(Produtos *produtos, char busca[20])
{
    int a = 0;


    while(produtos != NULL)
        if(strcmp(produtos->nome, busca) == 0)
        {
            printf("\n\t---Produto encontrado---\n\t");
            printf("\n\t%s", produtos->nome);
            printf("\n\tPRECO->%4.2f", produtos->preco);
            printf("\n\tQNT->%d", produtos->quantidade);
            printf("\n\tDATA->%d\n\t", produtos->mes);
            a = 1;
            break;
        }
        else
        {
            produtos = produtos->prox;
        }
    if(a == 0)
        printf("\n\t---Produto nao encontrado---\n\t");

}

float gastos_mensais (Produtos *produtos)

{
    int data;
    float soma = 0;

    printf("\n\tDeseja conhecer o orcamento de qual mes: ");
    scanf("%d", &data);

    while(produtos != NULL)
    {
        if(produtos->mes == data)
        {
            soma = soma + (produtos->preco * produtos->quantidade);
            produtos = produtos->prox;
        }
        else
        {
            produtos = produtos->prox;
        }
    }

    return soma;
}

void max(Produtos *produtos)
{
    float max = 0;
    float soma = 0;
    Produtos *maximo = NULL;

    while(produtos != NULL)
    {
        soma = (produtos->preco * produtos->quantidade);
        if(soma > max)
        {
            max = soma;
            maximo = produtos;
            produtos = produtos->prox;
        }
        else
        {
            produtos = produtos->prox;
        }
    }

    printf("\n\t---GASTO MAXIMO---\n\t");
    printf("\n\t%s", maximo->nome);
    printf("\n\tGasto maximo->%4.2f\n\t", max);
}
