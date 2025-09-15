#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]) {
    if(argc != 3) {
        printf("O número de parâmetros está errado \n");
        return 1;
    }

    char caminhoA[100];
    char caminhoB[100];

    sprintf(caminhoA, "arquivos/%s", argv[1]);
    sprintf(caminhoB, "arquivos/%s", argv[2]);
    
    FILE *arquivoA = fopen(caminhoA, "r");
    FILE *arquivoB = fopen(caminhoB, "r");

    if(arquivoA == NULL || arquivoB == NULL) {
        printf("Arquivos não foram abertos.\n");
        return 1;
    }

    int linhaA = 0, colunaA = 0;
    int linhaB = 0, colunaB = 0;

    if(fscanf(arquivoA, "%d %d", &linhaA, &colunaA) != 2) {
        printf("Erro na leitura do arquivo A\n");
        return 1;
    }

    if(fscanf(arquivoB, "%d %d", &linhaB, &colunaB) != 2) {
        printf("Erro na leitura do arquivo B\n");
        return 1;
    }

    if(colunaA != linhaB) {
        printf("As matrizes não podem ser multiplicadas\n");
        return 1;
    }

    // Alocação dinâmica da matriz A
    int **matrizA = malloc(linhaA * sizeof(int *));
    for(int i = 0; i < linhaA; i++)
        matrizA[i] = malloc(colunaA * sizeof(int));

    // Alocação dinâmica da matriz B
    int **matrizB = malloc(linhaB * sizeof(int *));
    for(int i = 0; i < linhaB; i++)
        matrizB[i] = malloc(colunaB * sizeof(int));

    // Alocação dinâmica da matriz Resultado
    int **matrizResultado = malloc(linhaA * sizeof(int *));
    for(int i = 0; i < linhaA; i++)
        matrizResultado[i] = malloc(colunaB * sizeof(int));

    // Leitura da matriz A
    for(int i = 0; i < linhaA; i++)
        for(int j = 0; j < colunaA; j++)
            fscanf(arquivoA, "%d", &matrizA[i][j]);
    fclose(arquivoA);

    // Leitura da matriz B
    for(int i = 0; i < linhaB; i++)
        for(int j = 0; j < colunaB; j++)
            fscanf(arquivoB, "%d", &matrizB[i][j]);
    fclose(arquivoB);

    // Multiplicação sequencial
    clock_t inicio = clock();

    for(int i = 0; i < linhaA; i++) {
        for(int j = 0; j < colunaB; j++) {
            matrizResultado[i][j] = 0;
            for(int k = 0; k < colunaA; k++) {
                matrizResultado[i][j] += matrizA[i][k] * matrizB[k][j];
            }
        }
    }

    clock_t fim = clock();
    double duracao = (((double)(fim - inicio)) / CLOCKS_PER_SEC) * 1000;

    // Gravação do resultado
    FILE *resultadoSequencial = fopen("arquivos/resulSequencial.txt", "w");
    if(resultadoSequencial == NULL) {
        printf("Erro na criação do arquivo de resultado sequencial\n");
        return 1;
    }

    fprintf(resultadoSequencial, "%d %d\n", linhaA, colunaB);
    for(int i = 0; i < linhaA; i++) {
        for(int j = 0; j < colunaB; j++)
            fprintf(resultadoSequencial, "%d ", matrizResultado[i][j]);
        fprintf(resultadoSequencial, "\n");
    }
    fprintf(resultadoSequencial, "\n%f", duracao);
    fclose(resultadoSequencial);

    // Liberação da memória
    for(int i = 0; i < linhaA; i++)
        free(matrizA[i]);
    free(matrizA);

    for(int i = 0; i < linhaB; i++)
        free(matrizB[i]);
    free(matrizB);

    for(int i = 0; i < linhaA; i++)
        free(matrizResultado[i]);
    free(matrizResultado);

    return 0;
}