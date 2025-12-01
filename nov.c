#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM_FILA 5
#define TAM_PILHA 3

typedef struct {
    char nome;
    int id;
} Peca;

char tipos[] = {'I', 'O', 'T', 'L'};
int geradorID = 0;

Peca gerarPeca() {
    Peca p;
    p.nome = tipos[rand() % 4];
    p.id = geradorID++;
    return p;
}

typedef struct {
    Peca itens[TAM_FILA];
    int inicio, fim, quantidade;
} Fila;

typedef struct {
    Peca itens[TAM_PILHA];
    int topo;
} Pilha;

void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->fim = 0;
    f->quantidade = 0;
    for (int i = 0; i < TAM_FILA; i++) {
        f->itens[f->fim] = gerarPeca();
        f->fim = (f->fim + 1) % TAM_FILA;
        f->quantidade++;
    }
}

void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

int filaVazia(Fila *f) {
    return f->quantidade == 0;
}

int pilhaVazia(Pilha *p) {
    return p->topo == -1;
}

int pilhaCheia(Pilha *p) {
    return p->topo == TAM_PILHA - 1;
}

Peca removerFila(Fila *f) {
    Peca removida = f->itens[f->inicio];
    f->inicio = (f->inicio + 1) % TAM_FILA;
    f->quantidade--;
    f->itens[f->fim] = gerarPeca();
    f->fim = (f->fim + 1) % TAM_FILA;
    f->quantidade++;
    return removida;
}

int adicionarPilha(Pilha *p, Peca x) {
    if (pilhaCheia(p)) return 0;
    p->topo++;
    p->itens[p->topo] = x;
    return 1;
}

Peca removerPilha(Pilha *p) {
    return p->itens[p->topo--];
}

void trocarTopo(Fila *f, Pilha *p) {
    Peca temp = f->itens[f->inicio];
    f->itens[f->inicio] = p->itens[p->topo];
    p->itens[p->topo] = temp;
}

void trocaMultiplas(Fila *f, Pilha *p) {
    for (int i = 0; i < 3; i++) {
        int pos = (f->inicio + i) % TAM_FILA;
        Peca temp = f->itens[pos];
        f->itens[pos] = p->itens[p->topo - i];
        p->itens[p->topo - i] = temp;
    }
}

void exibirEstado(Fila *f, Pilha *p) {
    printf("\nFila de peças\t");
    int idx = f->inicio;
    for (int i = 0; i < f->quantidade; i++) {
        printf("[%c %d] ", f->itens[idx].nome, f->itens[idx].id);
        idx = (idx + 1) % TAM_FILA;
    }

    printf("\nPilha de reserva\t(Topo -> Base): ");
    for (int i = p->topo; i >= 0; i--) {
        printf("[%c %d] ", p->itens[i].nome, p->itens[i].id);
    }
    printf("\n");
}

int main() {
    srand((unsigned) time(NULL));
    Fila fila;
    Pilha pilha;
    inicializarFila(&fila);
    inicializarPilha(&pilha);

    int opcao;

    do {
        exibirEstado(&fila, &pilha);

        printf("\nOpções:\n");
        printf("1 - Jogar peça\n");
        printf("2 - Reservar peça\n");
        printf("3 - Usar peça da pilha\n");
        printf("4 - Trocar topo da pilha com frente da fila\n");
        printf("5 - Troca múltipla (3 peças)\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        if (scanf("%d", &opcao) != 1) {
            int c; while ((c = getchar()) != '\n' && c != EOF) {}
            printf("Entrada inválida\n");
            continue;
        }

        switch (opcao) {
            case 1:
                if (!filaVazia(&fila)) removerFila(&fila);
                break;

            case 2:
                if (!filaVazia(&fila)) {
                    if (!pilhaCheia(&pilha)) {
                        Peca p = removerFila(&fila);
                        adicionarPilha(&pilha, p);
                    }
                }
                break;

            case 3:
                if (!pilhaVazia(&pilha)) removerPilha(&pilha);
                break;

            case 4:
                if (!filaVazia(&fila) && !pilhaVazia(&pilha)) trocarTopo(&fila, &pilha);
                break;

            case 5:
                if (fila.quantidade >= 3 && pilha.topo >= 2) trocaMultiplas(&fila, &pilha);
                break;
        }

    } while (opcao != 0);

    return 0;
}
