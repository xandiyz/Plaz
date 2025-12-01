#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAXNOME 64
#define MAXPISTA 128
#define HASH_TAM 101

typedef struct Sala {
    char nome[MAXNOME];
    char pista[MAXPISTA];
    struct Sala *esq;
    struct Sala *dir;
} Sala;

typedef struct PistaNode {
    char pista[MAXPISTA];
    struct PistaNode *esq;
    struct PistaNode *dir;
} PistaNode;

typedef struct HashNode {
    char chave[MAXPISTA];
    char suspeito[MAXNOME];
    struct HashNode *prox;
} HashNode;

Sala* criarSala(const char *nome, const char *pista) {
    Sala *s = (Sala*)malloc(sizeof(Sala));
    strncpy(s->nome, nome, MAXNOME-1); s->nome[MAXNOME-1] = '\0';
    strncpy(s->pista, pista, MAXPISTA-1); s->pista[MAXPISTA-1] = '\0';
    s->esq = s->dir = NULL;
    return s;
}

unsigned long hashString(const char *str) {
    unsigned long h = 5381;
    int c;
    while ((c = (unsigned char)*str++))
        h = ((h << 5) + h) + c;
    return h;
}

void inserirNaHash(HashNode **tabela, const char *pista, const char *suspeito) {
    unsigned long idx = hashString(pista) % HASH_TAM;
    HashNode *n = (HashNode*)malloc(sizeof(HashNode));
    strncpy(n->chave, pista, MAXPISTA-1); n->chave[MAXPISTA-1] = '\0';
    strncpy(n->suspeito, suspeito, MAXNOME-1); n->suspeito[MAXNOME-1] = '\0';
    n->prox = tabela[idx];
    tabela[idx] = n;
}

const char* encontrarSuspeito(HashNode **tabela, const char *pista) {
    unsigned long idx = hashString(pista) % HASH_TAM;
    HashNode *p = tabela[idx];
    while (p) {
        if (strcmp(p->chave, pista) == 0) return p->suspeito;
        p = p->prox;
    }
    return NULL;
}

int buscarPista(PistaNode *raiz, const char *pista) {
    if (!raiz) return 0;
    int cmp = strcmp(pista, raiz->pista);
    if (cmp == 0) return 1;
    if (cmp < 0) return buscarPista(raiz->esq, pista);
    return buscarPista(raiz->dir, pista);
}

PistaNode* inserirPista(PistaNode *raiz, const char *pista) {
    if (raiz == NULL) {
        PistaNode *n = (PistaNode*)malloc(sizeof(PistaNode));
        strncpy(n->pista, pista, MAXPISTA-1); n->pista[MAXPISTA-1] = '\0';
        n->esq = n->dir = NULL;
        return n;
    }
    int cmp = strcmp(pista, raiz->pista);
    if (cmp < 0) raiz->esq = inserirPista(raiz->esq, pista);
    else if (cmp > 0) raiz->dir = inserirPista(raiz->dir, pista);
    return raiz;
}

void exibirPistas(PistaNode *raiz) {
    if (!raiz) return;
    exibirPistas(raiz->esq);
    printf(" - %s\n", raiz->pista);
    exibirPistas(raiz->dir);
}

void explorarSalas(Sala *inicio, PistaNode **raizPistas, HashNode **tabela) {
    if (!inicio) return;
    Sala *atual = inicio;
    char opcao;
    while (1) {
        printf("\nVocê está em: %s\n", atual->nome);

        if (strlen(atual->pista) > 0) {
            printf("Pista encontrada: \"%s\"\n", atual->pista);
            if (!buscarPista(*raizPistas, atual->pista)) {
                *raizPistas = inserirPista(*raizPistas, atual->pista);
                const char *s = encontrarSuspeito(tabela, atual->pista);
                if (s) printf("  (esta pista sugere suspeito: %s)\n", s);
            } else {
                printf("  (pista já coletada anteriormente)\n");
            }
        } else {
            printf("Nenhuma pista nesta sala.\n");
        }

        printf("\nEscolha: (e) esquerda  (d) direita  (s) sair\nOpção: ");
        scanf(" %c", &opcao);
        opcao = tolower(opcao);

        if (opcao == 'e') {
            if (atual->esq) atual = atual->esq;
            else printf("Não há caminho à esquerda!\n");
        } else if (opcao == 'd') {
            if (atual->dir) atual = atual->dir;
            else printf("Não há caminho à direita!\n");
        } else if (opcao == 's') {
            printf("Exploração encerrada.\n");
            return;
        } else {
            printf("Opção inválida!\n");
        }
    }
}

int verificarSuspeitoFinal(PistaNode *raizPistas, HashNode **tabela, const char *acusado) {
    if (!raizPistas) return 0;
    int cnt = 0;
    cnt += verificarSuspeitoFinal(raizPistas->esq, tabela, acusado);
    const char *s = encontrarSuspeito(tabela, raizPistas->pista);
    if (s && strcmp(s, acusado) == 0) cnt++;
    cnt += verificarSuspeitoFinal(raizPistas->dir, tabela, acusado);
    return cnt;
}

void liberarPistas(PistaNode *raiz) {
    if (!raiz) return;
    liberarPistas(raiz->esq);
    liberarPistas(raiz->dir);
    free(raiz);
}

void liberarHash(HashNode **tabela) {
    for (int i = 0; i < HASH_TAM; ++i) {
        HashNode *p = tabela[i];
        while (p) {
            HashNode *t = p;
            p = p->prox;
            free(t);
        }
    }
}

int main(void) {
    HashNode *tabela[HASH_TAM] = {0};

    Sala *hall = criarSala("Hall de Entrada", "Pegada de sapato sujo");
    hall->esq = criarSala("Sala de Estar", "Copo quebrado");
    hall->dir = criarSala("Escritório", "Documento rasgado");
    hall->esq->esq = criarSala("Cozinha", "Faca fora do lugar");
    hall->esq->dir = criarSala("Sala de Jantar", "");
    hall->dir->esq = criarSala("Biblioteca", "Livro fora da prateleira");
    hall->dir->dir = criarSala("Jardim Interno", "Pegadas no barro");

    inserirNaHash(tabela, "Pegada de sapato sujo", "Sr. Silva");
    inserirNaHash(tabela, "Copo quebrado", "Sra. Almeida");
    inserirNaHash(tabela, "Documento rasgado", "Sr. Silva");
    inserirNaHash(tabela, "Faca fora do lugar", "Chefe de Cozinha");
    inserirNaHash(tabela, "Livro fora da prateleira", "Bibliotecário");
    inserirNaHash(tabela, "Pegadas no barro", "Jardineiro");

    PistaNode *arvorePistas = NULL;

    printf("===========================================\n");
    printf("   DETECTIVE QUEST — IDENTIFICAÇÃO DO CULPADO\n");
    printf("===========================================\n");

    explorarSalas(hall, &arvorePistas, tabela);

    printf("\n----------- Pistas coletadas -----------\n");
    if (!arvorePistas)
        printf("Nenhuma pista foi coletada.\n");
    else
        exibirPistas(arvorePistas);

    char acusado[MAXNOME];
    printf("\nQuem você deseja acusar? ");
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
    fgets(acusado, sizeof(acusado), stdin);
    size_t ln = strlen(acusado);
    if (ln && acusado[ln-1] == '\n') acusado[ln-1] = '\0';

    int cont = verificarSuspeitoFinal(arvorePistas, tabela, acusado);
    printf("\nPistas que apontam para \"%s\": %d\n", acusado, cont);

    if (cont >= 2)
        printf("VEREDICTO: Culpado.\n");
    else
        printf("VEREDICTO: Inocente por falta de provas.\n");

    liberarPistas(arvorePistas);
    liberarHash(tabela);

    return 0;
}
