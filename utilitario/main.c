#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]) {

    if(argc != 5) {
        printf("Insira os argumentos corretamente. n1 m1 n2 m2");
        return 1;
    };
    
    srand(time(NULL));

    int n1 = atoi(argv[1]);
    int m1 = atoi(argv[2]);
    int n2 = atoi(argv[3]);
    int m2 = atoi(argv[4]);

    // Inicializa as matrizes
    int matrizA[n1][m1];
    int matrizB[n2][m2];

    // Preenche a matrizA
    for(int n = 0; n<n1; n++) {
        for(int i = 0; i<m1; i++) {
            matrizA[n][i] = rand() % 100;
        }
    }

    // Preenche a matrizB
    for(int n = 0; n<n2; n++){
        for(int i = 0; i<m2; i++){
            matrizB[n][i] = rand() % 100;
        }
    }

    // Puxa o arquivo referente a matriz A
    FILE *arquivoA = fopen("matrizA.txt", "w");

    // Verifica se o arquivo está vazio
    if(arquivoA == NULL) {
        printf("Erro na criação do arquivo MatrizA");
        return 1;
    }
    
    fprintf(arquivoA, "%d %d \n", n1, m1);

    return 0;
}