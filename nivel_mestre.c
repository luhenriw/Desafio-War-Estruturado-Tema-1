/* DESAFIO NIVEL MESTRE */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_NOME 30
#define MAX_COR  10

/* Estrutura de território pedida */
typedef struct {
    char nome[MAX_NOME];
    char cor[MAX_COR];   // cor/dono do territorio (ex: "Vermelho", "Azul", "Neutro")
    int tropas;
} Territorio;

/* ---------- PROTÓTIPOS (modularização) ---------- */
void atribuirMissao(char* destino, char* missoes[], int totalMissoes);
int verificarMissao(char* missao, Territorio* mapa, int tamanho, char* corJogador);
void exibirMissao(const char* missao); // "por valor" na exibição (const char* é passagem por valor do ponteiro)
void atacar(Territorio* atacante, Territorio* defensor);
void exibirMapa(Territorio* mapa, int tamanho);
void liberarMemoria(Territorio* mapa, int nTerritorios, char* missoesJogador[], int nPlayers);

/* Funções auxiliares */
int pertenceAoJogador(Territorio* t, const char* cor);
int possuiConsecutivos(Territorio* mapa, int tamanho, char* cor, int qtd);

/* ---------- IMPLEMENTAÇÕES ---------- */

/*
  atribuirMissao:
  - Sorteia uma missão do vetor missoes[] de tamanho totalMissoes
  - Copia a string sorteada para destino (destino deve apontar para memória válida)
*/
void atribuirMissao(char* destino, char* missoes[], int totalMissoes) {
    if (!destino || !missoes || totalMissoes <= 0) return;
    int idx = rand() % totalMissoes;
    strcpy(destino, missoes[idx]);
}

/*
  exibirMissao:
  - Mostra a missão do jogador (apenas chamado no início)
  - Recebe const char* por valor (o ponteiro é passado por valor)
*/
void exibirMissao(const char* missao) {
    printf("\n=== MISSÃO SORTEADA ===\n");
    printf("%s\n", missao);
    printf("=======================\n\n");
}

/*
  verificarMissao:
  - Avalia se 'corJogador' cumpriu a missão descrita em 'missao' no 'mapa' de tamanho 'tamanho'
  - Retorna 1 se cumprida, 0 caso contrário
  - Implementa lógica simples baseada no texto exato das missões
*/
int verificarMissao(char* missao, Territorio* mapa, int tamanho, char* corJogador) {
    if (!missao || !mapa || tamanho <= 0 || !corJogador) return 0;

    // Missões conhecidas (devemos comparar exatamente as strings usadas)
    if (strcmp(missao, "Conquistar 3 territorios seguidos") == 0) {
        // verifica se o jogador tem 3 territórios consecutivos no vetor
        return possuiConsecutivos(mapa, tamanho, corJogador, 3) ? 1 : 0;
    }

    if (strcmp(missao, "Eliminar todas as tropas da cor Vermelho") == 0) {
        // verifica se não existe nenhum territorio com cor "Vermelho"
        for (int i = 0; i < tamanho; ++i) {
            if (strcmp(mapa[i].cor, "Vermelho") == 0 && mapa[i].tropas > 0) return 0;
        }
        // se não encontrou tropas vermelhas
        return 1;
    }

    if (strcmp(missao, "Possuir pelo menos 15 tropas no total") == 0) {
        int soma = 0;
        for (int i = 0; i < tamanho; ++i) {
            if (strcmp(mapa[i].cor, corJogador) == 0) soma += mapa[i].tropas;
        }
        return (soma >= 15) ? 1 : 0;
    }

    if (strcmp(missao, "Conquistar 5 territorios quaisquer") == 0) {
        int cont = 0;
        for (int i = 0; i < tamanho; ++i) {
            if (strcmp(mapa[i].cor, corJogador) == 0) cont++;
        }
        return (cont >= 5) ? 1 : 0;
    }

    if (strcmp(missao, "Ter o maior numero de territorios") == 0) {
        int meu = 0;
        int maior = 0;
        for (int i = 0; i < tamanho; ++i) {
            if (strcmp(mapa[i].cor, corJogador) == 0) meu++;
        }
        // calcula maior entre outros jogadores (simples: compara com o maior atual geral)
        for (int i = 0; i < tamanho; ++i) {
            int cont = 0;
            for (int j = 0; j < tamanho; ++j) {
                if (strcmp(mapa[j].cor, mapa[i].cor) == 0) cont++;
            }
            if (cont > maior) maior = cont;
        }
        return (meu >= maior && meu > 0) ? 1 : 0;
    }

    // Missão desconhecida -> retorna 0
    return 0;
}

/*
  atacar:
  - Simula um ataque: rola um dado para atacante e defensor (1..6)
  - Se atacante vencer: transfere cor para defensor e metade das tropas do atacante -> defensor,
    atualizando tropas; se perder, atacante perde 1 tropa.
  - Observação: validação se é ataque válido (invasão) é feita antes de chamar atacar
*/
void atacar(Territorio* atacante, Territorio* defensor) {
    if (!atacante || !defensor) return;

    int dadoA = rand() % 6 + 1;
    int dadoD = rand() % 6 + 1;

    printf("Rolagem: Atacante(%s)=%d x Defensor(%s)=%d\n", atacante->nome, dadoA, defensor->nome, dadoD);

    if (dadoA > dadoD) {
        // atacante vence: transfere cor e metade das tropas
        int transfer = atacante->tropas / 2;
        if (transfer < 1) transfer = 1; // garante pelo menos 1 tropa transferida
        printf("Atacante venceu! Transferindo %d tropas para %s e mudando sua cor para %s\n",
               transfer, defensor->nome, atacante->cor);

        // transferir tropas e cor
        defensor->tropas = transfer;
        strcpy(defensor->cor, atacante->cor);

        // reduzir tropas do atacante
        atacante->tropas -= transfer;
        if (atacante->tropas < 0) atacante->tropas = 0;
    } else {
        // defensor vence ou empate: atacante perde 1 tropa
        atacante->tropas -= 1;
        if (atacante->tropas < 0) atacante->tropas = 0;
        printf("Atacante perdeu 1 tropa. Tropas restantes no atacante: %d\n", atacante->tropas);
    }
}

/*
  exibirMapa:
  - Mostra o mapa (vetor de territorios) com index, nome, cor, tropas
*/
void exibirMapa(Territorio* mapa, int tamanho) {
    printf("\n====== MAPA ======\n");
    for (int i = 0; i < tamanho; ++i) {
        printf("[%d] %s | Cor: %s | Tropas: %d\n",
               i, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
    printf("==================\n");
}

/*
  liberarMemoria:
  - libera o vetor de territorios e o vetor de strings das missões alocadas por jogador
*/
void liberarMemoria(Territorio* mapa, int nTerritorios, char* missoesJogador[], int nPlayers) {
    (void)nTerritorios; // não usado, mantido por clareza
    if (mapa) free(mapa);
    for (int i = 0; i < nPlayers; ++i) {
        if (missoesJogador[i]) free(missoesJogador[i]);
    }
}

/* ---------- Auxiliares ---------- */

/* verifica se o território pertence à cor informada */
int pertenceAoJogador(Territorio* t, const char* cor) {
    return strcmp(t->cor, cor) == 0;
}

/* verifica se existe uma sequência de 'qtd' territórios consecutivos no vetor pertencentes a 'cor' */
int possuiConsecutivos(Territorio* mapa, int tamanho, char* cor, int qtd) {
    if (qtd <= 0) return 0;
    for (int i = 0; i + qtd - 1 < tamanho; ++i) {
        int ok = 1;
        for (int j = 0; j < qtd; ++j) {
            if (strcmp(mapa[i+j].cor, cor) != 0) { ok = 0; break; }
        }
        if (ok) return 1;
    }
    return 0;
}

/* ---------- MAIN (jogo simplificado) ---------- */

int main() {
    srand((unsigned)time(NULL));

    // vetor com pelo menos 5 missões
    char* missoesDisponiveis[] = {
        "Conquistar 3 territorios seguidos",
        "Eliminar todas as tropas da cor Vermelho",
        "Possuir pelo menos 15 tropas no total",
        "Conquistar 5 territorios quaisquer",
        "Ter o maior numero de territorios"
    };
    int totalMissoes = sizeof(missoesDisponiveis) / sizeof(missoesDisponiveis[0]);

    // Definindo jogadores (duas cores)
    int nPlayers = 2;
    char* coresPlayers[] = {"Vermelho", "Azul"};

    // Aloca dinamicamente espaço para armazenar a missão de cada jogador
    char* missoesJogador[2];
    for (int i = 0; i < nPlayers; ++i) {
        // aloca espaço suficiente para a maior missão conhecida (mais 1 para '\0')
        missoesJogador[i] = malloc(100 * sizeof(char));
        if (!missoesJogador[i]) {
            fprintf(stderr, "Erro de alocação\n");
            return 1;
        }
    }

    // Atribui missão para cada jogador (passagem por referência indireta: destino aponta para malloc)
    for (int i = 0; i < nPlayers; ++i) {
        atribuirMissao(missoesJogador[i], missoesDisponiveis, totalMissoes);
        printf("Jogador %d (%s) recebeu sua missão: \n", i+1, coresPlayers[i]);
        exibirMissao(missoesJogador[i]); // exibe apenas uma vez no início
    }

    // Criação do mapa: aloca dinamicamente territórios
    int nTerritorios;
    printf("Quantos territorios no mapa? (sugestao 7-12): ");
    if (scanf("%d", &nTerritorios) != 1 || nTerritorios <= 0) {
        printf("Entrada inválida. Usando 8 territorios por default.\n");
        nTerritorios = 8;
    }

    Territorio* mapa = calloc(nTerritorios, sizeof(Territorio));
    if (!mapa) { fprintf(stderr, "Erro de alocação mapa\n"); return 1; }

    // Inicializa mapa com nomes, cores e tropas (exemplo simples)
    // Para facilitar, alternamos cores e colocamos alguns territorios neutros
    for (int i = 0; i < nTerritorios; ++i) {
        snprintf(mapa[i].nome, MAX_NOME, "Territorio-%d", i);
        if (i % 3 == 2) {
            strcpy(mapa[i].cor, "Neutro");
            mapa[i].tropas = 2 + rand() % 4;
        } else {
            // alterna entre as cores dos jogadores
            strcpy(mapa[i].cor, coresPlayers[i % nPlayers]);
            mapa[i].tropas = 3 + rand() % 5;
        }
    }

    // Loop de jogo simplificado por turnos
    int turno = 0;
    int vencedor = -1;
    while (1) {
        int jogadorAtual = turno % nPlayers;
        printf("\n--- Turno %d: Jogador %d (%s) ---\n", turno+1, jogadorAtual+1, coresPlayers[jogadorAtual]);
        exibirMapa(mapa, nTerritorios);

        // Pergunta ao jogador se deseja atacar (simplificado: manual input)
        printf("Deseja atacar? 1-Sim 0-Nao: ");
        int querAtacar;
        if (scanf("%d", &querAtacar) != 1) querAtacar = 0;

        if (querAtacar == 1) {
            int idxAt, idxDef;
            printf("Digite indice do territorio atacante: ");
            scanf("%d", &idxAt);
            printf("Digite indice do territorio defensor: ");
            scanf("%d", &idxDef);

            // valida índices
            if (idxAt < 0 || idxAt >= nTerritorios || idxDef < 0 || idxDef >= nTerritorios) {
                printf("Indices invalidos.\n");
            } else if (!pertenceAoJogador(&mapa[idxAt], coresPlayers[jogadorAtual])) {
                printf("Voce so pode atacar com territorios que voce possui (cor %s).\n", coresPlayers[jogadorAtual]);
            } else if (strcmp(mapa[idxAt].cor, mapa[idxDef].cor) == 0) {
                printf("Nao pode atacar um territorio com a mesma cor.\n");
            } else if (mapa[idxAt].tropas <= 0) {
                printf("Territorio atacante nao possui tropas suficientes.\n");
            } else {
                // realiza ataque
                atacar(&mapa[idxAt], &mapa[idxDef]);
            }
        } else {
            printf("Jogador pulou ataque.\n");
        }

        // Ao final do turno, verificar silenciosamente se missão de algum jogador foi cumprida
        for (int j = 0; j < nPlayers; ++j) {
            if (verificarMissao(missoesJogador[j], mapa, nTerritorios, coresPlayers[j])) {
                vencedor = j;
                break;
            }
        }
        if (vencedor != -1) break;

        turno++;
        if (turno > 200) { // segurança para não virar loop infinito
            printf("Limite de turnos atingido. Empate.\n");
            break;
        }
    }

    if (vencedor != -1) {
        printf("\n*** JOGADOR %d (%s) CUMPRIU SUA MISSÃO E VENCE! ***\n", vencedor+1, coresPlayers[vencedor]);
        printf("Missao cumprida: %s\n", missoesJogador[vencedor]);
    } else {
        printf("\nNenhum vencedor definido.\n");
    }

    // liberar memória alocada dinamicamente
    liberarMemoria(mapa, nTerritorios, missoesJogador, nPlayers);

    return 0;
}