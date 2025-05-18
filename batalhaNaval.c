/*  Batalha Naval – Nível MESTRE
    ► Tabuleiro 10 × 10 com 4 navios de tamanho 3
    ► Três habilidades especiais (Cone, Cruz, Octaedro) de tamanho 5 × 5
        · Água   = 0
        · Navio  = 3
        · Área   = 5   (marcada apenas sobre a água – navios continuam 3)
*/

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define BOARD_SIZE   10      /* tabuleiro 10 × 10                 */
#define SHIP_SIZE     3      /* cada navio ocupa 3 células        */
#define ABIL_SIZE     5      /* matrizes de habilidade 5 × 5      */
#define WATER         0
#define SHIP          3
#define AREA          5

/* ---------- Prototipação ---------- */
void inicializarTabuleiro(int tab[BOARD_SIZE][BOARD_SIZE]);
bool posicionarHorizontal (int tab[BOARD_SIZE][BOARD_SIZE], int linha, int col_ini);
bool posicionarVertical   (int tab[BOARD_SIZE][BOARD_SIZE], int linha_ini, int coluna);
bool posicionarDiagDown   (int tab[BOARD_SIZE][BOARD_SIZE], int linha_ini, int col_ini);
bool posicionarDiagUp     (int tab[BOARD_SIZE][BOARD_SIZE], int linha_ini, int col_ini);

void gerarCone     (int abil[ABIL_SIZE][ABIL_SIZE]);
void gerarCruz     (int abil[ABIL_SIZE][ABIL_SIZE]);
void gerarOctaedro (int abil[ABIL_SIZE][ABIL_SIZE]);

void aplicarHabilidade(int tab[BOARD_SIZE][BOARD_SIZE],
                       int abil[ABIL_SIZE][ABIL_SIZE],
                       int origem_linha, int origem_coluna);

void imprimirTabuleiro(int tab[BOARD_SIZE][BOARD_SIZE]);

/* ============================================================= */
int main(void)
{
    int tab[BOARD_SIZE][BOARD_SIZE];
    inicializarTabuleiro(tab);

    /* -------- 1. Posiciona quatro navios (mesmos do nível Aventureiro) ------- */
    if (!posicionarHorizontal(tab, 1, 2)) { puts("Erro navio horizontal"); return 1; }
    if (!posicionarVertical  (tab, 6, 5)) { puts("Erro navio vertical");   return 1; }
    if (!posicionarDiagDown  (tab, 2, 7)) { puts("Erro navio \\");          return 1; }
    if (!posicionarDiagUp    (tab, 4, 2)) { puts("Erro navio /");           return 1; }

    /* -------- 2. Constrói matrizes de habilidade ----------------------------- */
    int cone[ABIL_SIZE][ABIL_SIZE];
    int cruz[ABIL_SIZE][ABIL_SIZE];
    int octa[ABIL_SIZE][ABIL_SIZE];

    gerarCone(cone);
    gerarCruz(cruz);
    gerarOctaedro(octa);

    /* -------- 3. Aplica habilidades em pontos de origem fixos ---------------- */
    aplicarHabilidade(tab, cone, 0, 4);    /* topo do cone em (0,4)              */
    aplicarHabilidade(tab, cruz, 5, 5);    /* centro da cruz em (5,5)            */
    aplicarHabilidade(tab, octa, 7, 2);    /* centro do octaedro em (7,2)        */

    /* -------- 4. Exibe o resultado ------------------------------------------ */
    puts("=== Tabuleiro com Habilidades ===");
    imprimirTabuleiro(tab);

    return 0;
}

/* ============================================================================ */
/*                           FUNÇÕES DE TABULEIRO                               */
/* ============================================================================ */
void inicializarTabuleiro(int tab[BOARD_SIZE][BOARD_SIZE])
{
    for (int i = 0; i < BOARD_SIZE; ++i)
        for (int j = 0; j < BOARD_SIZE; ++j)
            tab[i][j] = WATER;
}

bool posicionarHorizontal(int tab[BOARD_SIZE][BOARD_SIZE], int linha, int col_ini)
{
    if (linha < 0 || linha >= BOARD_SIZE)                    return false;
    if (col_ini < 0 || col_ini + SHIP_SIZE - 1 >= BOARD_SIZE) return false;
    for (int j = 0; j < SHIP_SIZE; ++j)
        if (tab[linha][col_ini + j] != WATER)                return false;

    for (int j = 0; j < SHIP_SIZE; ++j)
        tab[linha][col_ini + j] = SHIP;
    return true;
}

bool posicionarVertical(int tab[BOARD_SIZE][BOARD_SIZE], int linha_ini, int coluna)
{
    if (coluna < 0 || coluna >= BOARD_SIZE)                   return false;
    if (linha_ini < 0 || linha_ini + SHIP_SIZE - 1 >= BOARD_SIZE) return false;
    for (int i = 0; i < SHIP_SIZE; ++i)
        if (tab[linha_ini + i][coluna] != WATER)              return false;

    for (int i = 0; i < SHIP_SIZE; ++i)
        tab[linha_ini + i][coluna] = SHIP;
    return true;
}

bool posicionarDiagDown(int tab[BOARD_SIZE][BOARD_SIZE], int linha_ini, int col_ini)
{
    if (linha_ini < 0 || col_ini < 0)                                     return false;
    if (linha_ini + SHIP_SIZE - 1 >= BOARD_SIZE)                          return false;
    if (col_ini  + SHIP_SIZE - 1 >= BOARD_SIZE)                           return false;
    for (int k = 0; k < SHIP_SIZE; ++k)
        if (tab[linha_ini + k][col_ini + k] != WATER)                     return false;

    for (int k = 0; k < SHIP_SIZE; ++k)
        tab[linha_ini + k][col_ini + k] = SHIP;
    return true;
}

bool posicionarDiagUp(int tab[BOARD_SIZE][BOARD_SIZE], int linha_ini, int col_ini)
{
    if (linha_ini < 0 || col_ini >= BOARD_SIZE)                           return false;
    if (linha_ini + SHIP_SIZE - 1 >= BOARD_SIZE)                          return false;
    if (col_ini  - SHIP_SIZE + 1 < 0)                                     return false;
    for (int k = 0; k < SHIP_SIZE; ++k)
        if (tab[linha_ini + k][col_ini - k] != WATER)                     return false;

    for (int k = 0; k < SHIP_SIZE; ++k)
        tab[linha_ini + k][col_ini - k] = SHIP;
    return true;
}

/* ============================================================================ */
/*                           MATRIZES DE HABILIDADE                             */
/* ============================================================================ */

/* Cone: topo na primeira linha, expandindo-se para baixo.
 * Para size 5 ->        0 0 1 0 0
 *                       0 1 1 1 0
 *                       1 1 1 1 1
 *                       0 0 0 0 0
 *                       0 0 0 0 0
 */
void gerarCone(int abil[ABIL_SIZE][ABIL_SIZE])
{
    int half = ABIL_SIZE / 2;
    for (int i = 0; i < ABIL_SIZE; ++i)
        for (int j = 0; j < ABIL_SIZE; ++j)
            abil[i][j] = 0;

    for (int row = 0; row <= half; ++row)
        for (int col = half - row; col <= half + row; ++col)
            abil[row][col] = 1;
}

/* Cruz: linha e coluna centrais preenchidas. */
void gerarCruz(int abil[ABIL_SIZE][ABIL_SIZE])
{
    int half = ABIL_SIZE / 2;
    for (int i = 0; i < ABIL_SIZE; ++i)
        for (int j = 0; j < ABIL_SIZE; ++j)
            abil[i][j] = (i == half || j == half) ? 1 : 0;
}

/* Octaedro (losango): |i-half| + |j-half| ≤ half */
void gerarOctaedro(int abil[ABIL_SIZE][ABIL_SIZE])
{
    int half = ABIL_SIZE / 2;
    for (int i = 0; i < ABIL_SIZE; ++i)
        for (int j = 0; j < ABIL_SIZE; ++j)
            abil[i][j] = (abs(i - half) + abs(j - half) <= half) ? 1 : 0;
}

/* ============================================================================ */
/*                     SOBREPOSIÇÃO DA HABILIDADE NO TABULEIRO                  */
/* ============================================================================ */
void aplicarHabilidade(int tab[BOARD_SIZE][BOARD_SIZE],
                       int abil[ABIL_SIZE][ABIL_SIZE],
                       int origem_linha, int origem_coluna)
{
    int half = ABIL_SIZE / 2;
    for (int i = 0; i < ABIL_SIZE; ++i)
    {
        for (int j = 0; j < ABIL_SIZE; ++j)
        {
            if (abil[i][j] == 0) continue;           /* célula não afetada */

            int lin_tab = origem_linha + (i - half);
            int col_tab = origem_coluna + (j - half);

            /* Mantém dentro dos limites do tabuleiro */
            if (lin_tab < 0 || lin_tab >= BOARD_SIZE) continue;
            if (col_tab < 0 || col_tab >= BOARD_SIZE) continue;

            if (tab[lin_tab][col_tab] == WATER)       /* só marca sobre a água */
                tab[lin_tab][col_tab] = AREA;
        }
    }
}

/* ============================================================================ */
/*                                IMPRESSÃO                                     */
/* ============================================================================ */
void imprimirTabuleiro(int tab[BOARD_SIZE][BOARD_SIZE])
{
    for (int i = 0; i < BOARD_SIZE; ++i)
    {
        for (int j = 0; j < BOARD_SIZE; ++j)
            printf("%d ", tab[i][j]);
        putchar('\n');
    }
}
