/*
    INCLUDE PARA FUNÇÕES DO PROGRAMA!

    Entre estas estão:

    - Calcular Intervalo de Tolerância das Cores;
    - Verificar se as Cores de um determinado Píxel pertencem ao intervalo de tolerância;
    - Pesquisar os BLOBs;
    - Criar um elemento para a Lista de Pesquisa;
    - Inserir um elemento para a Lista de Pesquisa (insere no final da Lista);
*/


/*
    Função que calcula o intervalo de tolerância das cores RGB:
        - aux: Array com as cores RGB que inserimos no início do programa;
        - tolerancia: Valor da tolerância que pretendemos dar às cores RGB;

    Return: Array Bidimensional com as respetivas cores RGB, com o intervalo de tolerância pretendido;
*/
int ** IntervaloTolerancia(int aux[], int tolerancia)
{
    int **colors = (int **) malloc(3 * sizeof(int));
    int i = 0;

    for(i = 0; i < 3; i++)
    {
        colors[i] = (int *) malloc(2 * sizeof(int));
        colors[i][0] = aux[i] - tolerancia;

        if(colors[i][0] < 0)
            colors[i][0] = 0;

        colors[i][1] = aux[i] + tolerancia;

        if(colors[i][1] > 255)
            colors[i][1] = 255;
    }

    return colors;
}

/*
    Função que verifica se as cores RGB de um determinado píxel se encontram no intervalo:
        - colors: Array Bidimensional com as cores RGB que pretendemos pesquisar, com o determinado intervalo;
        - red: Cor RGB Vermelha do Píxel;
        - green: Cor RGB Verde do Píxel;
        - blue: Cor RGB Azul do Píxel;

    Return: 1 caso pertença, 0 caso não pertença;
*/
int PossuiCores(int ** colors, int red, int green, int blue)
{
    if(((red >= colors[0][0]) && (red <= colors[0][1]))
        && ((green >= colors[1][0]) && (green <= colors[1][1]))
            && ((blue >= colors[2][0]) && (blue <= colors[2][1])))
                return 1;

    return 0;
}

int findBlob(int ** cores, int largura, int comprimento, int linha, int coluna, Pixel *** P)
{
    if(!(*P)[linha][coluna].state) // Caso o pixel seja igual a 1, significa que já foi pesquisado, que é o que não pretendemos;
    {
        if(PossuiCores(cores, (*P)[linha][coluna].red, (*P)[linha][coluna].green, (*P)[linha][coluna].blue))
        {
            (*P)[linha][coluna].state = 1; // Colocamos a 1, pois neste momento está a ser pesquisado;

            counter++; // Como possui as cores dentro do intervalo de tolerância, será incrementado ao nº de píxeis pesquisados;
            counter_linhas += linha; // Como possui as cores, será adicionado ao valor das linhas o correspondente à linha atual;
            counter_colunas += coluna; // Como possui as cores, será adicionado ao valor das colunas o correspondente à coluna atual;

            // Caso não esteja na primeira coluna (à esquerda da imagem, vai procurar outra vez à esquerda;
            if(coluna != 0)
                findBlob(cores, largura, comprimento, linha, coluna - 1, P);

            // Caso não esteja na última coluna (à direita da imagem), vai procurar outra vez à direita;
            if(coluna != comprimento - 1)
                findBlob(cores, largura, comprimento, linha, coluna + 1, P);

            // Caso não esteja na primeira linha (no topo da imagem), vai procurar outra vez, mais para cima;
            if(linha != 0)
                findBlob(cores, largura, comprimento, linha - 1, coluna, P);

            // Caso não esteja na última linha (no fundo da imagem), vai procurar outra vez, mais para baixo;
            if(linha != largura - 1)
                findBlob(cores, largura, comprimento, linha + 1, coluna, P);
        }
    }

    return 1;
}

Pesquisa * criarPesquisaBlob(char * nome_imagem, int x, int y, int pixeis)
{
    Pesquisa *pv = (Pesquisa *) malloc(sizeof(Pesquisa));

    pv->ID = 0;
    strcpy(pv->nome, nome_imagem);
    pv->x = x;
    pv->y = y;
    pv->pixeis = pixeis;
    pv->Pseg = NULL;
    pv->Pant = NULL; 

    return pv;
}

Pesquisa * inserirPesquisaBlob_Primeiro(Pesquisa *P, Pesquisa *Pnovo)
{
    Pnovo->Pseg = P;
    Pnovo->Pant = NULL;
    P->Pant = Pnovo;

    return Pnovo;
}

Pesquisa * inserirPesquisaBlob_Ultimo(Pesquisa *P, Pesquisa *Pnovo)
{    
    if(P == NULL)
        return(Pnovo);

    Pesquisa *aux = P;

    while(aux->Pseg != NULL)
        aux = aux->Pseg;

    aux->Pseg = Pnovo;
    Pnovo->Pant = aux;

    return P;
}
  

Pesquisa * inserirPesquisaBlob(Pesquisa * P, Pesquisa * Pnovo)
{
    Pesquisa *aux = P;

    while(aux != NULL)
    {
        if(aux->pixeis < Pnovo->pixeis)
            break;
        aux = aux->Pseg;
    }

    if(aux == NULL)
    {
        return(inserirPesquisaBlob_Ultimo(P, Pnovo));
    }

    if(aux->Pant == NULL)
    {
        return(inserirPesquisaBlob_Primeiro(P, Pnovo));
    }

    Pnovo->Pseg = aux;
    Pnovo->Pant = aux->Pant;
    aux->Pant = Pnovo;
    Pnovo->Pant->Pseg = Pnovo;

    return P;
}

int imprimirPesquisas(FILE *fout, Pesquisa *P)
{
    if(P == NULL)
    {
        printf("Não foram encontrados BLOBs com o intervalo RGB\n");
        return 0;
    }

    Pesquisa *aux = P;
    int counter = 0;

    while(aux != NULL)
    {
        fprintf(fout, "%s (%d, %d): %d pixeis\n", aux->nome, aux->x, aux->y, aux->pixeis);
        counter++;
        aux = aux->Pseg;
    }

    return counter;
}