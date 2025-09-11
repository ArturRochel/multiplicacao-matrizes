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
        printf("Arquivos não foram abertos.");
        return 1;
    }

    int linhaA = 0;
    int colunaA = 0;
    int linhaB = 0;
    int colunaB = 0;

    int itensLidos = 0;

    if (fscanf(arquivoA, "%d %d", &linhaA, &colunaA) != 2) {
    printf("Erro na leitura do arquivo A\n");
    return 1;
    }

    if (fscanf(arquivoB, "%d %d", &linhaB, &colunaB) != 2) {
    printf("Erro na leitura do arquivo B\n");
    return 1;
    }

    if(colunaA != linhaB) {
        printf("As matrizes não podem ser multiplicadas");
        return 1;
    }

    int matrizA[linhaA][colunaA];
    int matrizB[linhaB][colunaB];
    int matrizResultado[linhaA][colunaB];

    for(int i = 0; i<linhaA; i++) {
        for(int j = 0; j<colunaA; j++) {
            fscanf(arquivoA, "%d", &matrizA[i][j]);
        }
    }

    fclose(arquivoA);

    for(int i = 0; i<linhaB; i++) {
        for(int j = 0; j<colunaB; j++) {
            fscanf(arquivoB, "%d", &matrizB[i][j]);
        }
    }

    fclose(arquivoB);

    clock_t inicio;
    inicio = clock();

    for(int i = 0; i<linhaA; i++) {
        for(int j = 0; j<colunaB; j++) {
            matrizResultado[i][j] = 0;
            for(int k = 0; k<colunaA; k++) {
                matrizResultado[i][j] = matrizResultado[i][j] + matrizA[i][k] * matrizB[k][j];
            }
        }
    }

    clock_t fim;
    fim = clock();

    double duracao = (((double)(fim-inicio)) / CLOCKS_PER_SEC) * 1000;

    // Abrir arquivo para gravar os resultados
    FILE *resultadoSequencial = fopen("arquivos/resulSequencial.txt", "w");

    if(resultadoSequencial == NULL) {
        printf("Erro na criação do arquivo de resultado sequencial");
        return 1;
    }
    
    // Linha e Coluna
    fprintf(resultadoSequencial, "%d %d \n", linhaA, colunaB);


    for(int i = 0; i<linhaA; i++) {
        for(int j = 0; j<colunaB; j++) {
            fprintf(resultadoSequencial, "%d ", matrizResultado[i][j]);
        }
        fprintf(resultadoSequencial, "\n");
    }

    fprintf(resultadoSequencial, "\n");
    fprintf(resultadoSequencial, "%f", duracao);

    fclose(resultadoSequencial);

    return 0;
}