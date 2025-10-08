#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TAM_STRING 100

typedef struct {
    char nome[TAM_STRING];
    char cor[TAM_STRING];
    int tropas;
} Territorio;

void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void cadastrarTerritorios(Territorio* mapa, int n) {
    for (int i = 0; i < n; i++) {
        printf("\n--- Cadastro do Território %d ---\n", i + 1);
        printf("Digite o nome do país: ");
        fgets(mapa[i].nome, TAM_STRING, stdin);
        mapa[i].nome[strcspn(mapa[i].nome, "\n")] = '\0';

        printf("Digite a cor do exército: ");
        fgets(mapa[i].cor, TAM_STRING, stdin);
        mapa[i].cor[strcspn(mapa[i].cor, "\n")] = '\0';

        printf("Digite o número de tropas: ");
        scanf("%d", &mapa[i].tropas);
        limparBufferEntrada();
    }
}

void listarTerritorios(Territorio* mapa, int n) {
    printf("\n--- Lista de Territórios ---\n");
    for (int i = 0; i < n; i++) {
        printf("[%d] Nome: %s | Cor: %s | Tropas: %d\n",
               i, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
}

void atacar(Territorio* atacante, Territorio* defensor) {
    int dadoAtacante = (rand() % 6) + 1;
    int dadoDefensor = (rand() % 6) + 1;

    printf("\nAtaque de %s (%s) contra %s (%s)\n",
           atacante->nome, atacante->cor, defensor->nome, defensor->cor);
    printf("Dado atacante: %d | Dado defensor: %d\n", dadoAtacante, dadoDefensor);

    if (dadoAtacante > dadoDefensor) {
        printf("Resultado: o atacante venceu!\n");
        defensor->tropas = atacante->tropas / 2;
        strcpy(defensor->cor, atacante->cor);
        atacante->tropas /= 2;
    } else {
        printf("Resultado: o defensor resistiu! O atacante perde 1 tropa.\n");
        if (atacante->tropas > 0)
            atacante->tropas--;
    }
}

int main() {
    srand(time(NULL));
    int total;
    int opcao;

    printf("=========================================\n");
    printf("             Jogo WAR Estruturado\n");
    printf("=========================================\n");
    printf("Quantos territórios deseja cadastrar? ");
    scanf("%d", &total);
    limparBufferEntrada();

    Territorio* mapa = (Territorio*) calloc(total, sizeof(Territorio));
    if (mapa == NULL) {
        printf("Erro ao alocar memória.\n");
        return 1;
    }

    cadastrarTerritorios(mapa, total);

    do {
        printf("\n=========================================\n");
        printf("1 - Listar territórios\n");
        printf("2 - Realizar ataque\n");
        printf("0 - Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        limparBufferEntrada();

        switch (opcao) {
            case 1:
                listarTerritorios(mapa, total);
                break;

            case 2: {
                listarTerritorios(mapa, total);
                int at, def;
                printf("\nEscolha o índice do atacante: ");
                scanf("%d", &at);
                printf("Escolha o índice do defensor: ");
                scanf("%d", &def);
                limparBufferEntrada();

                if (at == def) {
                    printf("Um território não pode atacar a si mesmo!\n");
                    break;
                }
                if (strcmp(mapa[at].cor, mapa[def].cor) == 0) {
                    printf("Você não pode atacar um território da mesma cor!\n");
                    break;
                }
                if (mapa[at].tropas < 2) {
                    printf("O atacante precisa ter pelo menos 2 tropas!\n");
                    break;
                }

                atacar(&mapa[at], &mapa[def]);
                printf("\n--- Estado atualizado ---\n");
                listarTerritorios(mapa, total);
                break;
            }

            case 0:
                printf("Encerrando o jogo...\n");
                break;

            default:
                printf("Opção inválida!\n");
        }

    } while (opcao != 0);

    free(mapa);
    return 0;
}
