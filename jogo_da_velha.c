#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <time.h>

#define TAMANHO 3
#define PLAYER 'X'
#define COMPUTER 'O'
#define EMPTY '.'

#define VEZ_PLAYER 0
#define VEZ_COMPUTADOR 1

#define PONTUACAO_VITORIA 10
#define INFINITO 1000

void inicializaTabuleiro(char tabuleiro[TAMANHO][TAMANHO]);
void mostraTabuleiro(char tabuleiro[TAMANHO][TAMANHO]);
void mostraResultado(char tabuleiro[TAMANHO][TAMANHO], const char *mensagem);
int verificaVitoria(char tabuleiro[TAMANHO][TAMANHO], char jogador);
int checaEmpate(char tabuleiro[TAMANHO][TAMANHO]);
int casaDisponivel(char tabuleiro[TAMANHO][TAMANHO], int linha, int coluna);
int jogadaPlayer(char tabuleiro[TAMANHO][TAMANHO]);
void jogadaComputador(char tabuleiro[TAMANHO][TAMANHO]);
int minimax(char tabuleiro[TAMANHO][TAMANHO], int profundidade, int ehMaximizador);
void lerEntrada(char *entrada, int tamanho);
int lerInteiro(void);
int sorteiaVez(void);
int reiniciarJogo(char tabuleiro[TAMANHO][TAMANHO]);
int novaPartida(void);
void menu(void);

int main(void) {
    setlocale(LC_ALL, "");
    srand(time(NULL));
    menu();
    return 0;
}

/* ---------- Fluxo do jogo ---------- */

void menu(void) {
    int opcao;

    do {
        printf("\nJogo da Velha!\n");
        printf("1. Iniciar Jogo\n");
        printf("2. Sair\n");
        opcao = lerInteiro();

        switch (opcao) {
            case 1:
                /* novaPartida() retorna 1 enquanto o jogador quiser jogar de novo */
                while (novaPartida());
                break;
            case 2:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida!\n");
        }
    } while (opcao != 2);
}

int novaPartida(void) {
    char tabuleiro[TAMANHO][TAMANHO];
    inicializaTabuleiro(tabuleiro);

    int vez = sorteiaVez();

    while (1) {
        mostraTabuleiro(tabuleiro);

        if (vez == VEZ_PLAYER) {
            printf("Sua vez!\n");

            if (!jogadaPlayer(tabuleiro)) {
                vez = reiniciarJogo(tabuleiro);
                continue;
            }
            if (verificaVitoria(tabuleiro, PLAYER)) {
                /* Com o minimax correto, este caso nunca deve acontecer */
                mostraResultado(tabuleiro, "VOCÊ VENCEU!");
                break;
            }
            vez = VEZ_COMPUTADOR;
        } else {
            printf("Vez do computador!\n");

            jogadaComputador(tabuleiro);
            if (verificaVitoria(tabuleiro, COMPUTER)) {
                mostraResultado(tabuleiro, "VOCÊ PERDEU!");
                break;
            }
            vez = VEZ_PLAYER;
        }

        if (checaEmpate(tabuleiro)) {
            mostraResultado(tabuleiro, "EMPATE!");
            break;
        }
    }

    printf("\n1. Iniciar nova partida\n2. Voltar ao menu\n");
    return lerInteiro() == 1;
}

int reiniciarJogo(char tabuleiro[TAMANHO][TAMANHO]) {
    printf("Reiniciando o jogo...\n");
    inicializaTabuleiro(tabuleiro);
    return sorteiaVez();
}

int sorteiaVez(void) {
    return rand() % 2;
}

/* ---------- Tabuleiro ---------- */

void inicializaTabuleiro(char tabuleiro[TAMANHO][TAMANHO]) {
    for (int i = 0; i < TAMANHO; i++) {
        for (int j = 0; j < TAMANHO; j++) {
            tabuleiro[i][j] = EMPTY;
        }
    }
}

void mostraTabuleiro(char tabuleiro[TAMANHO][TAMANHO]) {
    printf("\n");
    for (int i = 0; i < TAMANHO; i++) {
        for (int j = 0; j < TAMANHO; j++) {
            printf(" %c ", tabuleiro[i][j]);
            if (j < TAMANHO - 1) printf("|");
        }
        printf("\n");
        if (i < TAMANHO - 1) printf("---|---|---\n");
    }
    printf("\n");
}

void mostraResultado(char tabuleiro[TAMANHO][TAMANHO], const char *mensagem) {
    mostraTabuleiro(tabuleiro);
    printf("%s\n", mensagem);
}

int casaDisponivel(char tabuleiro[TAMANHO][TAMANHO], int linha, int coluna) {
    return linha >= 0 && linha < TAMANHO
        && coluna >= 0 && coluna < TAMANHO
        && tabuleiro[linha][coluna] == EMPTY;
}

int verificaVitoria(char tabuleiro[TAMANHO][TAMANHO], char jogador) {
    /* Linhas e colunas */
    for (int i = 0; i < TAMANHO; i++) {
        if (tabuleiro[i][0] == jogador && tabuleiro[i][1] == jogador && tabuleiro[i][2] == jogador)
            return 1;
        if (tabuleiro[0][i] == jogador && tabuleiro[1][i] == jogador && tabuleiro[2][i] == jogador)
            return 1;
    }

    /* Diagonais */
    if (tabuleiro[0][0] == jogador && tabuleiro[1][1] == jogador && tabuleiro[2][2] == jogador)
        return 1;
    if (tabuleiro[0][2] == jogador && tabuleiro[1][1] == jogador && tabuleiro[2][0] == jogador)
        return 1;

    return 0;
}

int checaEmpate(char tabuleiro[TAMANHO][TAMANHO]) {
    for (int i = 0; i < TAMANHO; i++) {
        for (int j = 0; j < TAMANHO; j++) {
            if (tabuleiro[i][j] == EMPTY) return 0;
        }
    }
    return 1;
}

/* ---------- Jogadas ---------- */

/* Retorna 1 se a jogada foi feita e 0 se o jogador pediu para reiniciar */
int jogadaPlayer(char tabuleiro[TAMANHO][TAMANHO]) {
    char entrada[64];
    int linha, coluna;

    while (1) {
        printf("Escolha linha e coluna (1-3), ou R para reiniciar: ");
        lerEntrada(entrada, sizeof(entrada));

        if (entrada[0] == 'r' || entrada[0] == 'R') return 0;

        if (sscanf(entrada, "%d %d", &linha, &coluna) != 2) {
            printf("Entrada inválida. Digite dois números, por exemplo: 2 3\n");
            continue;
        }

        linha--;
        coluna--;

        if (casaDisponivel(tabuleiro, linha, coluna)) break;
        printf("Jogada inválida: casa ocupada ou fora do tabuleiro.\n");
    }

    tabuleiro[linha][coluna] = PLAYER;
    return 1;
}

void jogadaComputador(char tabuleiro[TAMANHO][TAMANHO]) {
    int melhorValor = -INFINITO;
    int melhorLinha = -1, melhorColuna = -1;

    for (int i = 0; i < TAMANHO; i++) {
        for (int j = 0; j < TAMANHO; j++) {
            if (tabuleiro[i][j] != EMPTY) continue;

            tabuleiro[i][j] = COMPUTER;
            int valor = minimax(tabuleiro, 0, 0);
            tabuleiro[i][j] = EMPTY;

            if (valor > melhorValor) {
                melhorValor = valor;
                melhorLinha = i;
                melhorColuna = j;
            }
        }
    }

    tabuleiro[melhorLinha][melhorColuna] = COMPUTER;
}

/* ---------- Minimax ---------- */

/*
 * Pontuação do ponto de vista do computador:
 *   vitória do computador -> positiva (maior quanto mais cedo)
 *   vitória do jogador    -> negativa (menos negativa quanto mais tarde)
 *   empate                -> 0
 */
int minimax(char tabuleiro[TAMANHO][TAMANHO], int profundidade, int ehMaximizador) {
    if (verificaVitoria(tabuleiro, COMPUTER)) return PONTUACAO_VITORIA - profundidade;
    if (verificaVitoria(tabuleiro, PLAYER)) return profundidade - PONTUACAO_VITORIA;
    if (checaEmpate(tabuleiro)) return 0;

    if (ehMaximizador) {
        int melhorValor = -INFINITO;
        for (int i = 0; i < TAMANHO; i++) {
            for (int j = 0; j < TAMANHO; j++) {
                if (tabuleiro[i][j] != EMPTY) continue;

                tabuleiro[i][j] = COMPUTER;
                int valor = minimax(tabuleiro, profundidade + 1, 0);
                tabuleiro[i][j] = EMPTY;

                if (valor > melhorValor) melhorValor = valor;
            }
        }
        return melhorValor;
    } else {
        int melhorValor = INFINITO;
        for (int i = 0; i < TAMANHO; i++) {
            for (int j = 0; j < TAMANHO; j++) {
                if (tabuleiro[i][j] != EMPTY) continue;

                tabuleiro[i][j] = PLAYER;
                int valor = minimax(tabuleiro, profundidade + 1, 1);
                tabuleiro[i][j] = EMPTY;

                if (valor < melhorValor) melhorValor = valor;
            }
        }
        return melhorValor;
    }
}

/* ---------- Entrada de dados ---------- */

/* Lê uma linha inteira e descarta o que passar do tamanho do buffer */
void lerEntrada(char *entrada, int tamanho) {
    if (fgets(entrada, tamanho, stdin) == NULL) {
        printf("\nEntrada encerrada. Saindo...\n");
        exit(0);
    }

    if (strchr(entrada, '\n') == NULL) {
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
    }
}

/* Retorna o número digitado, ou -1 se a entrada não for um número */
int lerInteiro(void) {
    char entrada[64];
    int valor;

    lerEntrada(entrada, sizeof(entrada));
    if (sscanf(entrada, "%d", &valor) != 1) return -1;
    return valor;
}
