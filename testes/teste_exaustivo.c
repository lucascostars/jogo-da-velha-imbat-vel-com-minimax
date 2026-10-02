/*
 * Explora todas as sequências de jogadas humanas contra a IA
 * determinística, considerando os dois possíveis participantes iniciais.
 *
 * Compilar: gcc testes/teste_exaustivo.c -o teste
 * Executar: ./teste
 */

#define main jogo_main
#include "../jogo_da_velha.c"
#undef main

static long partidas = 0;
static long derrotasIA = 0;

void explora(char tabuleiro[TAMANHO][TAMANHO], int vez) {
    if (verificaVitoria(tabuleiro, PLAYER)) {
        partidas++;
        derrotasIA++;
        return;
    }

    if (verificaVitoria(tabuleiro, COMPUTER)) {
        partidas++;
        return;
    }

    if (checaEmpate(tabuleiro)) {
        partidas++;
        return;
    }

    if (vez == VEZ_COMPUTADOR) {
        char copia[TAMANHO][TAMANHO];
        memcpy(copia, tabuleiro, sizeof(copia));
        jogadaComputador(copia);
        explora(copia, VEZ_PLAYER);
        return;
    }

    for (int i = 0; i < TAMANHO; i++) {
        for (int j = 0; j < TAMANHO; j++) {
            if (tabuleiro[i][j] != EMPTY) continue;

            tabuleiro[i][j] = PLAYER;
            explora(tabuleiro, VEZ_COMPUTADOR);
            tabuleiro[i][j] = EMPTY;
        }
    }
}

int main(void) {
    char tabuleiro[TAMANHO][TAMANHO];
    long totalDerrotasIA = 0;

    inicializaTabuleiro(tabuleiro);
    explora(tabuleiro, VEZ_PLAYER);
    printf("Jogador começa: %ld partidas, IA perdeu %ld\n",
           partidas, derrotasIA);
    totalDerrotasIA += derrotasIA;

    partidas = derrotasIA = 0;

    inicializaTabuleiro(tabuleiro);
    explora(tabuleiro, VEZ_COMPUTADOR);
    printf("IA começa:      %ld partidas, IA perdeu %ld\n",
           partidas, derrotasIA);
    totalDerrotasIA += derrotasIA;

    return totalDerrotasIA != 0;
  
}
