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

    int linhaA = 0;
    int colunaA = 0;
    int linhaB = 0;
    int colunaB = 0;

    int itensLidos = 0;

    fscanf(arquivoA, "%d %d", linhaA, colunaA);
    fscanf(arquivoB, "%d %d", linhaB, colunaB);

    return 0;
}