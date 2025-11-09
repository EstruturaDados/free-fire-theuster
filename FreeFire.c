#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_ITENS 10
#define MAX_COMPONENTES 20

// ---------------------- Structs ----------------------
typedef struct {
    char nome[30];
    char tipo[20];
    int quantidade;
} Item;

typedef struct No {
    Item dados;
    struct No *prox;
} No;

typedef struct {
    char nome[30];
    char tipo[20];
    int prioridade;
} Componente;

// ---------------------- Variáveis Globais ----------------------
Item mochila[MAX_ITENS];
int qtdItens = 0;
No *inicio = NULL;

// ---------------------- Funções Utilitárias ----------------------
void lerString(char *str, int tam) {
    fgets(str, tam, stdin);
    str[strcspn(str, "\n")] = 0;
}

int stricmp_simple(const char *a, const char *b) {
    while (*a && *b) {
        char ca = tolower(*a++), cb = tolower(*b++);
        if (ca != cb) return (ca < cb) ? -1 : 1;
    }
    return *a ? 1 : (*b ? -1 : 0);
}

// ---------------------- Mochila (Vetor) ----------------------
void inserirVetor(Item item) {
    if (qtdItens >= MAX_ITENS) {
        printf("Mochila cheia.\n");
        return;
    }
    mochila[qtdItens++] = item;
    printf("Item inserido.\n");
}

void listarVetor() {
    printf("\n--- Mochila (Vetor) ---\n");
    for (int i = 0; i < qtdItens; i++)
        printf("[%d] Nome: %s | Tipo: %s | Quant: %d\n", i, mochila[i].nome, mochila[i].tipo, mochila[i].quantidade);
    if (qtdItens == 0) printf("Vazia.\n");
}

int buscarVetor(char *nome, long *comp) {
    *comp = 0;
    for (int i = 0; i < qtdItens; i++) {
        (*comp)++;
        if (stricmp_simple(mochila[i].nome, nome) == 0) return i;
    }
    return -1;
}

void removerVetor(char *nome) {
    long comp;
    int idx = buscarVetor(nome, &comp);
    if (idx < 0) {
        printf("Item não encontrado.\n");
        return;
    }
    for (int i = idx; i < qtdItens - 1; i++)
        mochila[i] = mochila[i + 1];
    qtdItens--;
    printf("Item removido.\n");
}

// ---------------------- Mochila (Lista Encadeada) ----------------------
void inserirLista(Item item) {
    No *novo = malloc(sizeof(No));
    novo->dados = item;
    novo->prox = inicio;
    inicio = novo;
    printf("Item inserido na lista.\n");
}

void listarLista() {
    printf("\n--- Mochila (Lista) ---\n");
    No *aux = inicio;
    int i = 0;
    while (aux) {
        printf("[%d] Nome: %s | Tipo: %s | Quant: %d\n", i++, aux->dados.nome, aux->dados.tipo, aux->dados.quantidade);
        aux = aux->prox;
    }
    if (!inicio) printf("Vazia.\n");
}

void removerLista(char *nome) {
    No *atual = inicio, *ant = NULL;
    while (atual) {
        if (stricmp_simple(atual->dados.nome, nome) == 0) {
            if (ant) ant->prox = atual->prox;
            else inicio = atual->prox;
            free(atual);
            printf("Item removido da lista.\n");
            return;
        }
        ant = atual;
        atual = atual->prox;
    }
    printf("Item não encontrado.\n");
}

No *buscarLista(char *nome, long *comp) {
    No *aux = inicio;
    *comp = 0;
    while (aux) {
        (*comp)++;
        if (stricmp_simple(aux->dados.nome, nome) == 0) return aux;
        aux = aux->prox;
    }
    return NULL;
}

void limparLista() {
    while (inicio) {
        No *tmp = inicio->prox;
        free(inicio);
        inicio = tmp;
    }
}

// ---------------------- Ordenação e Busca Binária ----------------------
long bubbleSort(Componente v[], int n) {
    long comp = 0;
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - i - 1; j++) {
            comp++;
            if (stricmp_simple(v[j].nome, v[j + 1].nome) > 0) {
                Componente t = v[j];
                v[j] = v[j + 1];
                v[j + 1] = t;
            }
        }
    return comp;
}

long insertionSort(Componente v[], int n) {
    long comp = 0;
    for (int i = 1; i < n; i++) {
        Componente key = v[i];
        int j = i - 1;
        while (j >= 0 && stricmp_simple(v[j].tipo, key.tipo) > 0) {
            v[j + 1] = v[j];
            j--;
            comp++;
        }
        v[j + 1] = key;
    }
    return comp;
}

long selectionSort(Componente v[], int n) {
    long comp = 0;
    for (int i = 0; i < n - 1; i++) {
        int min = i;
        for (int j = i + 1; j < n; j++) {
            comp++;
            if (v[j].prioridade < v[min].prioridade) min = j;
        }
        Componente t = v[i];
        v[i] = v[min];
        v[min] = t;
    }
    return comp;
}

int buscaBinaria(Componente v[], int n, char *nome, long *comp) {
    int ini = 0, fim = n - 1;
    *comp = 0;
    while (ini <= fim) {
        int m = (ini + fim) / 2;
        (*comp)++;
        int r = stricmp_simple(v[m].nome, nome);
        if (r == 0) return m;
        else if (r > 0) fim = m - 1;
        else ini = m + 1;
    }
    return -1;
}

// ---------------------- Módulo de Componentes ----------------------
void mostrarComp(Componente v[], int n) {
    printf("\n--- Componentes ---\n");
    for (int i = 0; i < n; i++)
        printf("[%d] Nome: %s | Tipo: %s | Prioridade: %d\n", i, v[i].nome, v[i].tipo, v[i].prioridade);
}

void moduloComponentes() {
    Componente v[MAX_COMPONENTES];
    int n = 0;
    char op[10], nome[30];
    printf("Qtd de componentes (max 20): ");
    fgets(op, 10, stdin);
    n = atoi(op);
    if (n < 1) return;

    for (int i = 0; i < n; i++) {
        printf("\nComponente %d\n", i + 1);
        printf("Nome: "); lerString(v[i].nome, 30);
        printf("Tipo: "); lerString(v[i].tipo, 20);
        printf("Prioridade (1-10): ");
        fgets(op, 10, stdin);
        v[i].prioridade = atoi(op);
    }

    int ordenadoNome = 0;
    while (1) {
        printf("\n1-Listar\n2-Bubble (nome)\n3-Insertion (tipo)\n4-Selection (prioridade)\n5-Busca binária (nome)\n0-Voltar\nOpcao: ");
        fgets(op, 10, stdin);
        if (op[0] == '0') break;

        if (op[0] == '1') mostrarComp(v, n);
        else if (op[0] == '2') {
            long c; clock_t t1 = clock();
            c = bubbleSort(v, n);
            printf("Ordenado por nome. Comparações: %ld Tempo: %.6f s\n", c, (double)(clock()-t1)/CLOCKS_PER_SEC);
            mostrarComp(v, n);
            ordenadoNome = 1;
        } else if (op[0] == '3') {
            long c; clock_t t1 = clock();
            c = insertionSort(v, n);
            printf("Ordenado por tipo. Comparações: %ld Tempo: %.6f s\n", c, (double)(clock()-t1)/CLOCKS_PER_SEC);
            mostrarComp(v, n);
            ordenadoNome = 0;
        } else if (op[0] == '4') {
            long c; clock_t t1 = clock();
            c = selectionSort(v, n);
            printf("Ordenado por prioridade. Comparações: %ld Tempo: %.6f s\n", c, (double)(clock()-t1)/CLOCKS_PER_SEC);
            mostrarComp(v, n);
            ordenadoNome = 0;
        } else if (op[0] == '5') {
            if (!ordenadoNome) { printf("Ordene por nome antes.\n"); continue; }
            printf("Nome a buscar: "); lerString(nome, 30);
            long c; int idx = buscaBinaria(v, n, nome, &c);
            if (idx >= 0)
                printf("Encontrado: %s | Tipo: %s | Prioridade: %d\n", v[idx].nome, v[idx].tipo, v[idx].prioridade);
            else printf("Não encontrado.\n");
            printf("Comparações: %ld\n", c);
        }
    }
}

// ---------------------- Menu Mochila ----------------------
void menuMochila() {
    Item it; char nome[30], op[10];
    while (1) {
        printf("\n1-Add Vetor\n2-Remover Vetor\n3-Listar Vetor\n4-Buscar Vetor\n5-Add Lista\n6-Remover Lista\n7-Listar Lista\n8-Buscar Lista\n0-Voltar\nOpcao: ");
        fgets(op, 10, stdin);
        if (op[0] == '0') break;

        if (op[0] == '1') {
            printf("Nome: "); lerString(it.nome, 30);
            printf("Tipo: "); lerString(it.tipo, 20);
            printf("Quantidade: "); fgets(op, 10, stdin);
            it.quantidade = atoi(op);
            inserirVetor(it);
        } else if (op[0] == '2') {
            printf("Nome: "); lerString(nome, 30);
            removerVetor(nome);
        } else if (op[0] == '3') listarVetor();
        else if (op[0] == '4') {
            printf("Nome: "); lerString(nome, 30);
            long c; int i = buscarVetor(nome, &c);
            if (i >= 0) printf("Encontrado: %s | %s | %d\n", mochila[i].nome, mochila[i].tipo, mochila[i].quantidade);
            else printf("Não encontrado.\n");
            printf("Comparações: %ld\n", c);
        } else if (op[0] == '5') {
            printf("Nome: "); lerString(it.nome, 30);
            printf("Tipo: "); lerString(it.tipo, 20);
            printf("Quantidade: "); fgets(op, 10, stdin);
            it.quantidade = atoi(op);
            inserirLista(it);
        } else if (op[0] == '6') {
            printf("Nome: "); lerString(nome, 30);
            removerLista(nome);
        } else if (op[0] == '7') listarLista();
        else if (op[0] == '8') {
            printf("Nome: "); lerString(nome, 30);
            long c; No *res = buscarLista(nome, &c);
            if (res) printf("Encontrado: %s | %s | %d\n", res->dados.nome, res->dados.tipo, res->dados.quantidade);
            else printf("Não encontrado.\n");
            printf("Comparações: %ld\n", c);
        }
    }
}

// ---------------------- Função Principal ----------------------
int main() {
    char op[10];
    while (1) {
        printf("\n=== MENU PRINCIPAL ===\n");
        printf("1 - Mochila (vetor e lista)\n2 - Componentes (ordenacao e busca)\n0 - Sair\nOpcao: ");
        fgets(op, 10, stdin);
        if (op[0] == '0') break;
        if (op[0] == '1') menuMochila();
        else if (op[0] == '2') moduloComponentes();
    }
    limparLista();
    return 0;
}
