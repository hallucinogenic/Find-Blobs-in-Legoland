/*
    INCLUDE PARA AS ESTRUTURAS DO PROGRAMA!
*/

typedef struct PESQUISA
{
    int ID; // Chave de Ordenação;
    char nome[256]; // Nome da Imagem;
    int x, y; // Coordenadas centrais que foram encontradas na Pesquisa;
    int pixeis; // Número de píxeis encontrados na Pesquisa;
    struct PESQUISA *Pseg; // Imagem seguinte na lista;
    struct PESQUISA *Pant; // Imagem anterior na lista;
}Pesquisa;

typedef struct PIXEL
{
    int ID; // Chave de Ordenação;
    int red; // Cor RGB Vermelha do Píxeil (entre 0 e 255); 
    int green; // Cor RGB Verde do Píxel (entre 0 e 255);
    int blue; // Cor RGB Azul do Píxel (entre 0 e 255);
    int state; // Estado do píxel: 0 se for encontrado na pesquisa, 1 se já foi pesquisado;
}Pixel;