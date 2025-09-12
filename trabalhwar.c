#include <stdio.h>
#include <string.h>

struct territorio{
    char nome[30];
    char cor[10];
    int tropas;
};

int main() {
    struct territorio territorios[5];

    printf("=== Cadastro de territórios ===\n\n");
    for(int i = 0; i < 5; i++) {
        printf("Território %d: \n", i+ 1);

        printf("Digite o nome do território: ");
        scanf("%s", territorios[i].nome);

        printf("Digite a cor do exercito: ");
        scanf("%s", territorios[i].cor);

        printf("Digite a quantidade de tropas: ");
        scanf("%s",territorios[i].cor);

        printf("--------------------------------");
    }

    printf("\n=== Territorios cadastrados ===\n\n");
    for(int i = 0; i < 5; i++){
        printf("Territorio %d: \n", i + 1);
        printf("nome: %s\n", territorios[i].nome);
        printf("Cor do exercito: %s\n", territorios[i].cor);
        printf("Quantidade de tropas: %d\n", territorios[i].tropas);
        printf("-------------------------------------------");
    
    }
    return 0;
    
}