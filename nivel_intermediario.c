/* DESAFIO INTERMEDIARIO */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

// ---------- FUNÇÕES ----------
void cadastrarTerritorios(Territorio* mapa, int n) {
    printf("\n--- Cadastro de Territórios ---\n");
    for (int i = 0; i < n; i++) {
        printf("\nTerritório %d:\n", i+1);

        printf("Nome: ");
        scanf(" %[^\n]", mapa[i].nome);

        printf("Cor (exército): ");
        scanf(" %[^\n]", mapa[i].cor);

        printf("Tropas: ");
        scanf("%d", &mapa[i].tropas);
    }
}

void exibirTerritorios(Territorio* mapa, int n) {
    printf("\n--- Territórios ---\n");
    for (int i = 0; i < n; i++) {
        printf("\n[%d] %s | Cor: %s | Tropas: %d\n",
               i, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
}

void atacar(Territorio* atacante, Territorio* defensor) {
    printf("\n--- ATAQUE ---\n");
    printf("Atacante: %s (%s) com %d tropas\n",
           atacante->nome, atacante->cor, atacante->tropas);
    printf("Defensor: %s (%s) com %d tropas\n\n",
           defensor->nome, defensor->cor, defensor->tropas);

    int dadoAtacante = (rand() % 6) + 1;
    int dadoDefensor = (rand() % 6) + 1;

    printf("Dado atacante: %d\n", dadoAtacante);
    printf("Dado defensor: %d\n", dadoDefensor);

    if (dadoAtacante > dadoDefensor) {
        printf("\n➡ O atacante venceu!\n");

        strcpy(defensor->cor, atacante->cor);   // muda dono
        defensor->tropas = atacante->tropas / 2; // metade das tropas entra

        printf("O território %s foi conquistado!\n", defensor->nome);
    } 
    else {
        printf("\n➡ O defensor venceu!\n");
        atacante->tropas -= 1;  // atacante perde 1 tropa

        if (atacante->tropas < 0)
            atacante->tropas = 0;

        printf("%s perdeu 1 tropa.\n", atacante->nome);
    }
}

void liberarMemoria(Territorio* mapa) {
    free(mapa);
}

// ---------- MAIN ----------
int main() {
    srand(time(NULL));

    int n;
    printf("Quantos territórios deseja cadastrar? ");
    scanf("%d", &n);

    Territorio* mapa = (Territorio*) calloc(n, sizeof(Territorio));

    cadastrarTerritorios(mapa, n);

    int opcao;
    do {
        printf("\n--- MENU ---\n");
        printf("1 - Exibir territórios\n");
        printf("2 - Atacar\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        if (opcao == 1) {
            exibirTerritorios(mapa, n);
        }
        else if (opcao == 2) {
            exibirTerritorios(mapa, n);

            int at, df;

            printf("\nEscolha o índice do território atacante: ");
            scanf("%d", &at);

            printf("Escolha o índice do território defensor: ");
            scanf("%d", &df);

            // Validações
            if (at < 0 || at >= n || df < 0 || df >= n) {
                printf("Índice inválido!\n");
                continue;
            }

            if (strcmp(mapa[at].cor, mapa[df].cor) == 0) {
                printf("Você não pode atacar seu próprio território!\n");
                continue;
            }

            atacar(&mapa[at], &mapa[df]);
        }

    } while (opcao != 0);

    liberarMemoria(mapa);
    printf("\nMemória liberada. Encerrando programa...\n");

    return 0;
}