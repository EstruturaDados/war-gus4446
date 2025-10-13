#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Define o tamanho padrão para strings de nome e cor
#define TAM_STRING_NOME 30
#define TAM_STRING_COR 10
#define TAM_STRING_MISSAO 100

// Estrutura para representar um território no mapa
typedef struct {
    char nome[TAM_STRING_NOME];
    char cor[TAM_STRING_COR];
    int tropas;
} Territorio;

// --- Protótipos das Funções ---

// Funções de utilidade
void limparBufferEntrada();

// Funções de gerenciamento do jogo
void cadastrarTerritorios(Territorio* mapa, int n);
void listarTerritorios(Territorio* mapa, int n);
void atacar(Territorio* atacante, Territorio* defensor);

// Funções do sistema de missões
void atribuirMissao(char* destino, const char* missoes[], int totalMissoes);
int verificarMissao(const char* missao, const char* corDoJogador, Territorio* mapa, int tamanho);
void exibirMissao(const char* missao);

// Função de gerenciamento de memória
void liberarMemoria(Territorio* mapa, char* missao);

// --- Implementação das Funções ---

/**
 * @brief Limpa o buffer de entrada do teclado.
 * Essencial para evitar que `scanf` e `fgets` interfiram um com o outro.
 */
void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/**
 * @brief Cadastra 'n' territórios no mapa do jogo.
 * @param mapa Ponteiro para o vetor de territórios.
 * @param n Número total de territórios a serem cadastrados.
 */
void cadastrarTerritorios(Territorio* mapa, int n) {
    for (int i = 0; i < n; i++) {
        printf("\n--- Cadastro do Território %d ---\n", i + 1);
        printf("Digite o nome do país: ");
        fgets(mapa[i].nome, TAM_STRING_NOME, stdin);
        mapa[i].nome[strcspn(mapa[i].nome, "\n")] = '\0';

        printf("Digite a cor do exército: ");
        fgets(mapa[i].cor, TAM_STRING_COR, stdin);
        mapa[i].cor[strcspn(mapa[i].cor, "\n")] = '\0';

        printf("Digite o número de tropas: ");
        scanf("%d", &mapa[i].tropas);
        limparBufferEntrada();
    }
}

/**
 * @brief Lista todos os territórios cadastrados, exibindo seus status.
 * @param mapa Ponteiro para o vetor de territórios.
 * @param n Número total de territórios.
 */
void listarTerritorios(Territorio* mapa, int n) {
    printf("\n--- Lista de Territórios ---\n");
    for (int i = 0; i < n; i++) {
        printf("[%d] Nome: %s | Cor: %s | Tropas: %d\n",
               i, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
}

/**
 * @brief Sorteia e atribui uma missão para o jogador.
 * @param destino Ponteiro para a string que armazenará a missão (passagem por referência).
 * @param missoes Vetor de strings com todas as missões possíveis.
 * @param totalMissoes Número total de missões disponíveis.
 */
void atribuirMissao(char* destino, const char* missoes[], int totalMissoes) {
    int indiceSorteado = rand() % totalMissoes;
    strcpy(destino, missoes[indiceSorteado]);
}

/**
 * @brief Exibe a missão do jogador.
 * @param missao A string da missão a ser exibida (passagem por valor).
 */
void exibirMissao(const char* missao) {
    printf("\n=========================================\n");
    printf("Sua missão secreta é:\n");
    printf(">> %s <<\n", missao);
    printf("=========================================\n");
}

/**
 * @brief Simula um ataque entre dois territórios.
 * A lógica segue o requisito: se o atacante vencer, o defensor é conquistado.
 * Caso contrário, o atacante perde uma tropa.
 * @param atacante Ponteiro para o território atacante.
 * @param defensor Ponteiro para o território defensor.
 */
void atacar(Territorio* atacante, Territorio* defensor) {
    int dadoAtacante = (rand() % 6) + 1;
    int dadoDefensor = (rand() % 6) + 1;

    printf("\nAtaque de %s (%s) contra %s (%s)\n",
           atacante->nome, atacante->cor, defensor->nome, defensor->cor);
    printf("Dado atacante: %d | Dado defensor: %d\n", dadoAtacante, dadoDefensor);

    if (dadoAtacante > dadoDefensor) {
        printf("Resultado: O ATACANTE VENCEU!\n");
        printf("O território %s foi conquistado!\n", defensor->nome);
        
        // Transfere a cor e metade das tropas, como especificado
        strcpy(defensor->cor, atacante->cor);
        defensor->tropas = atacante->tropas / 2;
        atacante->tropas /= 2;
        if (atacante->tropas < 1) { // Garante que o atacante fique com ao menos 1 tropa
            atacante->tropas = 1;
        }

    } else {
        printf("Resultado: O DEFENSOR RESISTIU! O atacante perde 1 tropa.\n");
        if (atacante->tropas > 1) {
            atacante->tropas--;
        }
    }
}

/**
 * @brief Verifica se a missão do jogador foi cumprida.
 * @param missao String contendo a missão do jogador.
 * @param corDoJogador String com a cor do jogador.
 * @param mapa Ponteiro para o vetor de territórios.
 * @param tamanho Número total de territórios no mapa.
 * @return 1 se a missão foi cumprida, 0 caso contrário.
 */
int verificarMissao(const char* missao, const char* corDoJogador, Territorio* mapa, int tamanho) {
    // Lógica para "CONQUISTAR 3 TERRITORIOS"
    if (strcmp(missao, "CONQUISTAR 3 TERRITORIOS") == 0) {
        int contagem = 0;
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, corDoJogador) == 0) {
                contagem++;
            }
        }
        return contagem >= 3;
    }

    // Lógica para "ELIMINAR O EXERCITO VERMELHO"
    if (strcmp(missao, "ELIMINAR O EXERCITO VERMELHO") == 0) {
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, "VERMELHO") == 0) {
                return 0; // Se encontrar ao menos um, a missão não foi cumprida
            }
        }
        return 1; // Se o loop terminar, não há mais exércitos vermelhos
    }

    // Lógica para "ELIMINAR O EXERCITO AZUL"
    if (strcmp(missao, "ELIMINAR O EXERCITO AZUL") == 0) {
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, "AZUL") == 0) {
                return 0;
            }
        }
        return 1;
    }

    // Adicione outras lógicas de missão aqui...

    return 0; // Nenhuma condição de vitória conhecida foi atingida
}

/**
 * @brief Libera toda a memória alocada dinamicamente.
 * @param mapa Ponteiro para o vetor de territórios.
 * @param missao Ponteiro para a string da missão do jogador.
 */
void liberarMemoria(Territorio* mapa, char* missao) {
    printf("Liberando memória alocada...\n");
    free(mapa);
    free(missao);
}


// --- Função Principal ---
int main() {
    // Inicializa o gerador de números aleatórios
    srand(time(NULL));

    // Vetor de missões estratégicas disponíveis no jogo
    const char* missoes[] = {
        "CONQUISTAR 3 TERRITORIOS",
        "ELIMINAR O EXERCITO VERMELHO",
        "ELIMINAR O EXERCITO AZUL"
        // Adicione mais missões aqui
    };
    int totalMissoes = sizeof(missoes) / sizeof(missoes[0]);

    int totalTerritorios;
    int opcao;
    char corDoJogador[TAM_STRING_COR];

    printf("=========================================\n");
    printf("         Jogo WAR Estruturado\n");
    printf("=========================================\n");

    // Definição do jogador
    printf("Digite a cor que você irá controlar (ex: VERDE): ");
    fgets(corDoJogador, TAM_STRING_COR, stdin);
    corDoJogador[strcspn(corDoJogador, "\n")] = '\0';
    
    // Alocação dinâmica da memória para a missão do jogador
    char* missaoDoJogador = (char*) malloc(sizeof(char) * TAM_STRING_MISSAO);
    if (missaoDoJogador == NULL) {
        printf("Erro ao alocar memória para a missão.\n");
        return 1;
    }
    
    // Atribuição e exibição da missão
    atribuirMissao(missaoDoJogador, missoes, totalMissoes);
    exibirMissao(missaoDoJogador);
    
    // Setup do mapa
    printf("\nQuantos territórios deseja cadastrar? ");
    scanf("%d", &totalTerritorios);
    limparBufferEntrada();

    Territorio* mapa = (Territorio*) calloc(totalTerritorios, sizeof(Territorio));
    if (mapa == NULL) {
        printf("Erro ao alocar memória para o mapa.\n");
        free(missaoDoJogador); // Libera a memória já alocada
        return 1;
    }

    cadastrarTerritorios(mapa, totalTerritorios);

    // Loop principal do jogo
    do {
        printf("\n================ MENU =================\n");
        printf("1 - Listar territórios\n");
        printf("2 - Realizar ataque\n");
        printf("0 - Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        limparBufferEntrada();

        switch (opcao) {
            case 1:
                listarTerritorios(mapa, totalTerritorios);
                break;

            case 2: {
                listarTerritorios(mapa, totalTerritorios);
                int at, def;
                printf("\nEscolha o índice do atacante: ");
                scanf("%d", &at);
                printf("Escolha o índice do defensor: ");
                scanf("%d", &def);
                limparBufferEntrada();

                // Validações do ataque
                if (at < 0 || at >= totalTerritorios || def < 0 || def >= totalTerritorios) {
                    printf("Índice inválido!\n");
                    break;
                }
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
                listarTerritorios(mapa, totalTerritorios);

                // Verificação da missão após o ataque
                if (verificarMissao(missaoDoJogador, corDoJogador, mapa, totalTerritorios)) {
                    printf("\n\n****************************************\n");
                    printf("  PARABÉNS! VOCÊ CUMPRIU SUA MISSÃO!\n");
                    printf("         >> %s <<\n", missaoDoJogador);
                    printf("****************************************\n\n");
                    opcao = 0; // Força a saída do jogo
                }
                break;
            }

            case 0:
                printf("Encerrando o jogo...\n");
                break;

            default:
                printf("Opção inválida!\n");
        }

    } while (opcao != 0);

    // Libera toda a memória alocada antes de encerrar
    liberarMemoria(mapa, missaoDoJogador);
    return 0;
}