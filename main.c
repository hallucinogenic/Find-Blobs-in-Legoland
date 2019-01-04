/*
    PESQUISA DE BLOBS EM LEGOLANDS

    Trabalho Realizado por:
        - Luís Rei, Nº 35445, Engenharia Informática da Universidade da Beira Interior;
    
    Unidade Curricular: Algoritmos e Estruturas de Dados;
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Custom Includes
#include "include/globals.c"
#include "include/struct.c"
#include "include/custom_functions.c"

int main(int argc, char *argv[])
{
    int colors_aux[3] = {0, 0, 0}; // Variável auxiliar para guardar as cores que pretendemos procurar;
    int tolerancia = 0; // Variável auxiliar para guardar a tolerância das cores RGB;

    /*
        Variável para guardar os valores das cores, com a respetiva tolerância;
            - colors[0][0] -> Cor Mínima do valor Vermelho RGB;
            - colors[0][1] -> Cor Máxima do valor Vermelho RGB;
            - colors[1][0] -> Cor Mínima do valor Verde RGB;
            - colors[1][1] -> Cor Máxima do valor Verde RGB;
            - colors[2][0] -> Cor Mínima do valor Azul RGB;
            - colors[2][1] -> Cor Máxima do valor Azul RGB;
    */
    int **colors = NULL;

    char nome_ficheiro[9] = "imgs.txt"; // Nome do ficheiro default, caso não se utilize um nos argumentos;

    // Caso ao executar o programa, possua o "-debug", será ativado o debugging do programa;
    if((argc > 2) && (strcmp(argv[2], "-debug") == 0))
        DEBUG = 1;
    
    FILE *f = NULL; // Ficheiro que pretendemos pesquisar os píxeis;

    // Caso o nome do ficheiro não esteja inserido no 2º argumento do programa, será usado o default;

    f = fopen((argc > 1)?argv[1]:nome_ficheiro, "r");
    
    // Caso o ficheiro não exista (ou por engano, escrever "-debug" no 2º argumento, o programa termina, dizendo que houve um erro);
    if(f == NULL)
    {
        if(DEBUG)
            printf("Erro de leitura no ficheiro!\n");
        
        return 0;
    }


    /*
        Visto que não houve problemas com o ficheiro, vamos prosseguir;
            Vai ser pedido ao utilizador 3 inteiros que correspondem ao RGB que pretendemos procurar.
                - Se alguma das cores não pertencer ao intervalo [0, 255], será pedido ao utilizador para escrever outra vez;
            Após as cores, será pedido um intervalo de tolerância ao utilizador, para definir os intervalos;
                - Caso seja igual a 0, vai ser procurado o píxel exato;
                - Caso seja menor que 0, será pedido ao utilizador para escrever outra vez;
    */

    printf("Insere o tom Vermelho RGB que pretendes procurar: ");
    scanf("%d", &colors_aux[0]);

    while(colors_aux[0] < 0 || colors_aux[0] > 255)
    {
        printf("O valor %d é %s! Insere novamente: ", colors_aux[0], (colors_aux[0] < 0)?"menor que 0":"maior que 255");
        scanf("%d", &colors_aux[0]);
    }

    printf("Insere o tom Verde RGB que pretendes procurar: ");
    scanf("%d", &colors_aux[1]);

    while(colors_aux[1] < 0 || colors_aux[1] > 255)
    {
        printf("O valor %d é %s! Insere novamente: ", colors_aux[1], (colors_aux[1] < 0)?"menor que 0":"maior que 255");
        scanf("%d", &colors_aux[1]);
    }
    
    printf("Insere o tom Azul RGB que pretendes procurar: ");
    scanf("%d", &colors_aux[2]);

    while(colors_aux[2] < 0 || colors_aux[2] > 255)
    {
        printf("O valor %d é %s! Insere novamente: ", colors_aux[2], (colors_aux[2] < 0)?"menor que 0":"maior que 255");
        scanf("%d", &colors_aux[2]);
    }

    printf("Insere a tolerância que pretendes nas cores: ");
    scanf("%d", &tolerancia);

    while(tolerancia < 0)
    {
        printf("O valor %d é menor que 0! Insere novamente: ", tolerancia);
        scanf("%d", &tolerancia);
    }

    // Após sabermos as cores e a tolerância, vamos calcular o valor mínimo e máximo de cada cor, de acordo com a tolerância;
    colors = IntervaloTolerancia(colors_aux, tolerancia);

    if(DEBUG)
        printf("Cores:\nVermelho: [%d, %d]\nVerde: [%d, %d]\nAzul: [%d, %d]\n", colors[0][0], colors[0][1], colors[1][0], colors[1][1], colors[2][0], colors[2][1]);



    char nome_imagem[256] = {0}; // Nome da Imagem que será pesquisada;
    char buffer_dimensoes[256] = {0}; // String auxiliar para armazenar as dimensões da imagem;

    char *token = NULL; // Variável auxiliar para armazenar o que foi dividido na string com as dimensões;

    int largura = 0; // Variável auxiliar para armazenar a largura da imagem;
    int comprimento = 0; // Variável auxiliar para armazenar o comprimento da imagem; 
    int canais = 0; // Variável auxiliar para armazenar o número de canais da imagem (como é RGB, terá por default 3 canais);

    int i = 0, j = 0; // Variáveis auxiliares para usar em ciclos;

    int linha_aux = 0, coluna_aux = 0; // Variáveis auxiliares para a leitura e armazenamento dos píxeis durante as imagens;

    int valor_aux = 0; // Será guardado o valor de cada linha lida do texto;

    Pixel **P = NULL; // Matriz em que vai ser armazenada o número de píxeis totais. Declarada como NULL para não haver lixo;

    char RGB[100] = {0}; // String auxiliar, para ler todas as linhas do ficheiro em que se encontram as cores dos píxeis;

    int * blob = NULL; // Array Auxiliar que será para armazenar as informações dos BLOBS;

    Pesquisa *Pesq = NULL; // Cabeça da Lista que foi armazenada as informações;
    Pesquisa *Pnovo = NULL; // Elemento que vai ser adicionado à lista;

    while(fgets(nome_imagem, 256, f))
    {
        nome_imagem[strlen(nome_imagem) - 1] = '\0'; // Como o fgets retorna uma string com "\n" no final, será retirado;
        fgets(buffer_dimensoes, 256, f); // Vamos retirar os valores das dimensões, que se encontram nessa linha;
        token = strtok(buffer_dimensoes, " ");
        largura = atoi(token); // Assim ficamos a saber o número total de linhas que a imagem possui;
        token = strtok(NULL, " ");
        comprimento = atoi(token); // Assim ficamos a saber o número total de colunas que a imagem possui;
        token = strtok(NULL, " ");
        canais = atoi(token); // Assim ficamos a saber o número total de canais que a imagem possui;

        // Será criada a matriz, com o número de linhas correspondentes à largura da imagem;
        P = (Pixel **) malloc(largura * sizeof(Pixel *));

        // Para cada linha, serão criadas X colunas correspondentes ao comprimento da imagem;
        for(i = 0; i < largura; i++)
        {
            P[i] = (Pixel *) malloc(comprimento * sizeof(Pixel));
        }

        // Ambas as variáveis serão inicializadas a 0, pois será feita a leitura de píxeis de uma nova imagem;
        linha_aux = 0;
        coluna_aux = 0;

        for(i = 0, j = 0; i < (largura * comprimento * canais); i++, j++)
        {
            valor_aux = atoi(fgets(RGB, 100, f));

            if(j == 0) // Quando j = 0, significa que se trata o valor "Vermelho" da cor RGB do píxel;
                P[linha_aux][coluna_aux].red = valor_aux;

            if(j == 1) // Quando j = 1, significa que se trata do valor "Verde" da cor RGB do píxel;
                P[linha_aux][coluna_aux].green = valor_aux;

            if(j == 2) // Quando j = 2, significa que se trata do valor "Azul" da cor RGB do píxel;
            {
                P[linha_aux][coluna_aux].blue = valor_aux;
                P[linha_aux][coluna_aux].state = 0; // Significa que o píxel foi encontrado neste momento na pesquisa;
                j = -1; // Como os 3 valores da cor RGB do píxel foram pesquisados, prepara-se a variável j para outro píxel;

                // Caso tenhamos pesquisado o número de colunas igual ao comprimento da imagem, incrementa-se uma linha;
                if(coluna_aux == comprimento-1)
                {
                    coluna_aux = 0;
                    linha_aux++;
                }
                else
                    coluna_aux++;
            }
        }

        /*
            Como o armazenamento da imagem está feito, vamos prosseguir para a pesquisa.

            Resumo: Encontrar o ponto central da imagem com a determinada cor, e calcular o número de píxeis que esta possuí;
        */

       blob = (int *) malloc(3 * sizeof(int));
        for(i = 0; i < largura; i++)
        {
            for(j = 0; j < comprimento; j++)
            {
                /*
                    Se o estado for igual a 1, significa que o píxel já foi pesquisado;
                    Após isso, verifica se o píxel possui as cores que inserimos no inicio do programa;
                */
                if(!P[i][j].state && PossuiCores(colors, P[i][j].red, P[i][j].green, P[i][j].blue))
                {
                    counter = 0;
                    counter_linhas = 0;
                    counter_colunas = 0;

                    findBlob(colors, largura, comprimento, i, j, &P);
                    Pnovo = criarPesquisaBlob(nome_imagem, counter_linhas/counter, counter_colunas/counter, counter);
                    Pesq = inserirPesquisaBlob(Pesq, Pnovo);
                }
            }
        }


        // Como é óbvio, para não armazenar lixo, será apagado todas as linhas da matriz;
        for(i = 0; i < largura; i++)
        {
            free(P[i]);
        }
    }

    // Agora que a pesquisa foi feita com sucesso
    int contador_blobs = imprimirPesquisas(Pesq);

    if(contador_blobs)
        printf("Foram encontrados %d BLOBs com o devido intervalo RGB\n", contador_blobs);

    // Como é óbvio, após terminarmos com o ficheiro texto, vamos fechá-lo;
    fclose(f);

    return 0;
}