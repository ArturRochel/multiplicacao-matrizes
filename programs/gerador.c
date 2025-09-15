#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]) {

    if(argc != 5) {
        printf("Insira os argumentos corretamente: n1 m1 n2 m2\n");
        return 1;
    }
    
    srand(time(NULL));

    int n1 = atoi(argv[1]);
    int m1 = atoi(argv[2]);
    int n2 = atoi(argv[3]);
    int m2 = atoi(argv[4]);

    int **matrizA = malloc(n1 * sizeof(int *));
    for(int i = 0; i < n1; i++)
        matrizA[i] = malloc(m1 * sizeof(int));

    int **matrizB = malloc(n2 * sizeof(int *));
    for(int i = 0; i < n2; i++)
        matrizB[i] = malloc(m2 * sizeof(int));


    for(int n = 0; n < n1; n++)
        for(int i = 0; i < m1; i++)
            matrizA[n][i] = rand() % 100;

    for(int n = 0; n < n2; n++)
        for(int i = 0; i < m2; i++)
            matrizB[n][i] = rand() % 100;

    FILE *arquivoA = fopen("arquivos/matrizA.txt", "w");
    if(arquivoA == NULL) {
        printf("Erro na criação do arquivo MatrizA\n");
        return 1;
    }

    fprintf(arquivoA, "%d %d\n", n1, m1);
    for(int n = 0; n < n1; n++) {
        for(int i = 0; i < m1; i++)
            fprintf(arquivoA, "%d ", matrizA[n][i]);
        fprintf(arquivoA, "\n");
    }
    fclose(arquivoA);

    FILE *arquivoB = fopen("arquivos/matrizB.txt", "w");
    if(arquivoB == NULL) {
        printf("Erro na criação do arquivo MatrizB\n");
        return 1;
    }

    fprintf(arquivoB, "%d %d\n", n2, m2);
    for(int n = 0; n < n2; n++) {
        for(int i = 0; i < m2; i++)
            fprintf(arquivoB, "%d ", matrizB[n][i]);
        fprintf(arquivoB, "\n");
    }
    fclose(arquivoB);

    // Libera a memória
    for(int i = 0; i < n1; i++)
        free(matrizA[i]);
    free(matrizA);

    for(int i = 0; i < n2; i++)
        free(matrizB[i]);
    free(matrizB);

    return 0;
}
