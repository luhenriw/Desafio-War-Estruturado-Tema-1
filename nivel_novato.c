/* DESAFIO NOVATO */

#include <stdio.h>
#include <string.h>

typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

int main() {
    // Vetor fixo com 5 territórios (requisito do nível novato)
    Territorio lista[5];

    printf("=== Cadastro de Territórios ===\n");

    // Entrada de dados dos 5 territórios
    for (int i = 0; i < 5; i++) {
        printf("\n--- Território %d ---\n", i + 1);

        printf("Nome: ");
        scanf(" %[^\n]", lista[i].nome);  // Lê nome com espaços

        printf("Cor do exército: ");
        scanf(" %[^\n]", lista[i].cor);

        printf("Quantidade de tropas: ");
        scanf("%d", &lista[i].tropas);
    }

    // Exibição dos dados cadastrados
    printf("\n\n=== Territórios cadastrados ===\n");
    for (int i = 0; i < 5; i++) {
        printf("\nTerritório %d:\n", i + 1);
        printf("Nome: %s\n", lista[i].nome);
        printf("Cor: %s\n", lista[i].cor);
        printf("Tropas: %d\n", lista[i].tropas);
    }

    return 0;
}