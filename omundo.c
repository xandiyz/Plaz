#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

void exibirMapa(Territorio* mapa, int qtd) {
    printf("\n========= MAPA ATUAL =========\n");
    for (int i = 0; i < qtd; i++) {
        printf("\nTerritorio %d\n", i + 1);
        printf("Nome: %s\n", mapa[i].nome);
        printf("Cor: %s\n", mapa[i].cor);
        printf("Tropas: %d\n", mapa[i].tropas);
    }
}

void cadastrarTerritorios(Territorio* mapa, int qtd) {
    for (int i = 0; i < qtd; i++) {
        printf("\n--- Cadastro do Territorio %d ---\n", i + 1);
        printf("Nome: ");
        scanf("%s", mapa[i].nome);
        printf("Cor do exercito: ");
        scanf("%s", mapa[i].cor);
        printf("Numero de tropas: ");
        scanf("%d", &mapa[i].tropas);
    }
}

void atacar(Territorio* atacante, Territorio* defensor) {
    int d1 = rand() % 6 + 1;
    int d2 = rand() % 6 + 1;

    printf("\n--- %s (%s) atacando %s (%s) ---\n",
           atacante->nome, atacante->cor,
           defensor->nome, defensor->cor);

    printf("Dado atacante: %d | Dado defensor: %d\n", d1, d2);

    if (d1 > d2) {
        printf("O atacante venceu!\n");
        strcpy(defensor->cor, atacante->cor);
        defensor->tropas /= 2;
    } else {
        printf("O ataque falhou!\n");
        if (atacante->tropas > 1)
            atacante->tropas -= 1;
    }
}

void exibirMissao(char* missao) {
    printf("\n=== SUA MISSAO ===\n");
    printf("%s\n", missao);
}

void atribuirMissao(char* destino, char* missoes[], int total) {
    int sorteio = rand() % total;
    strcpy(destino, missoes[sorteio]);
}

int verificarMissao(char* missao, Territorio* mapa, int tamanho) {
    if (strstr(missao, "Conquistar") != NULL) {
        int count = 0;
        for (int i = 0; i < tamanho; i++)
            if (strcmp(mapa[i].cor, "Azul") == 0)
                count++;
        if (count >= 2)
            return 1;
    }

    if (strstr(missao, "Eliminar") != NULL) {
        int existeVermelho = 0;
        for (int i = 0; i < tamanho; i++)
            if (strcmp(mapa[i].cor, "Vermelho") == 0)
                existeVermelho = 1;
        if (!existeVermelho)
            return 1;
    }

    return 0;
}

void liberarMemoria(Territorio* mapa, char* missaoJogador) {
    free(mapa);
    free(missaoJogador);
}

int main() {
    srand(time(NULL));

    char* missoes[] = {
        "Conquistar 2 territorios.",
        "Eliminar todas as tropas da cor Vermelho.",
        "Conquistar 3 territorios seguidos.",
        "Eliminar qualquer exercito inimigo.",
        "Controlar mais territorios que os outros jogadores."
    };

    int totalMissoes = 5;

    char* missaoJogador = (char*) malloc(100 * sizeof(char));
    if (!missaoJogador) return 1;

    atribuirMissao(missaoJogador, missoes, totalMissoes);
    exibirMissao(missaoJogador);

    int qtd;
    printf("\nQuantos territorios deseja cadastrar? ");
    scanf("%d", &qtd);

    Territorio* mapa = (Territorio*) calloc(qtd, sizeof(Territorio));
    if (!mapa) return 1;

    cadastrarTerritorios(mapa, qtd);

    int opcao;
    while (1) {
        exibirMapa(mapa, qtd);

        printf("\n1 - Realizar ataque\n0 - Encerrar\n> ");
        scanf("%d", &opcao);
        if (opcao == 0) break;

        int iAtq, iDef;
        printf("Atacante (1-%d): ", qtd);
        scanf("%d", &iAtq);
        printf("Defensor (1-%d): ", qtd);
        scanf("%d", &iDef);

        if (iAtq < 1 || iAtq > qtd || iDef < 1 || iDef > qtd || iAtq == iDef) {
            printf("Escolha invalida!\n");
            continue;
        }

        Territorio* A = &mapa[iAtq - 1];
        Territorio* D = &mapa[iDef - 1];

        if (strcmp(A->cor, D->cor) == 0) {
            printf("Nao pode atacar territorios da mesma cor!\n");
            continue;
        }

        atacar(A, D);

        if (verificarMissao(missaoJogador, mapa, qtd)) {
            printf("\n========================================\n");
            printf("PARABÉNS! VOCÊ CUMPRIU SUA MISSÃO!\n");
            printf("Missao: %s\n", missaoJogador);
            printf("VOCÊ VENCEU O JOGO!\n");
            printf("========================================\n");
            break;
        }
    }

    liberarMemoria(mapa, missaoJogador);
    printf("\nMemoria liberada. Fim do programa.\n");

    return 0;
}

