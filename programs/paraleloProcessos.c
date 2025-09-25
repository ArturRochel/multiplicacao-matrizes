#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <math.h>


int main(int argc, char *argv[]){
    // verificando quantidade de argumentos
    if(argc != 4){
        fprintf(stderr, "Uso: %s <matriz1.txt> <matriz2.txt> <P>\n", argv[0]);
        return 1;
    }
    //armazenado argumento
    char caminhoM1[100];
    char caminhoM2[100];
    //caminho para os arquivos das matrizes
    sprintf(caminhoM1, "arquivos/%s", argv[1]);
    sprintf(caminhoM2, "arquivos/%s", argv[2]);

    int P = atoi(argv[3]);
    if(P <= 0){
        fprintf(stderr, "Valor de P tem que ser maior que zero!");
        return 1;
    }

    // ler o arquivos das matrizes
    int n1, m1, n2, m2;
    FILE *arquivo1 = fopen(caminhoM1, "r");
    //verificando se foi aberto
    if (!arquivo1) {
        perror("Erro ao abrir matriz 1");
        return 1;
    }
    fscanf(arquivo1, "%d %d", &n1, &m1);
    int **matrizA = malloc(n1 * sizeof(int*));
    for(int i = 0; i < n1; i++){
        matrizA[i] = malloc(m1 * sizeof(int));
        for(int j = 0; j < m1; j++){
            fscanf(arquivo1, "%d", &matrizA[i][j]);
        }
    }
    fclose(arquivo1);

    FILE *arquivo2 = fopen(caminhoM2, "r");

    if (!arquivo2) {
        perror("Erro ao abrir matriz 2  ");
        return 1;
    }

    fscanf(arquivo2, "%d %d", &n2, &m2);
    int **matrizB = malloc(n2 * sizeof(int*));
    for(int i = 0; i < n2; i++){
        matrizB[i] = malloc(m2 * sizeof(int));
        for(int j = 0; j < m2; j++){
            fscanf(arquivo2, "%d", &matrizB[i][j]);
        }
    }
    fclose(arquivo2);

    // verificando se é possivel multiplicar
    if(m1 != n2){
        fprintf(stderr, "Não é possível multiplicar.\n");
        for (int i = 0; i < n1; i++) free(matrizA[i]);
        free(matrizA);
        for (int i = 0; i < n2; i++) free(matrizB[i]);
        free(matrizB);
        return 1;
    }
    

    // preparando para divisão do trabalho
    int totalElementos = n1 * m2;
    int quantidadeDeProcessos = (int)ceil((double)totalElementos / P);

    // criando os processos
    for(int i = 0; i < quantidadeDeProcessos; i++){
        pid_t pid = fork();
        // processo filho
        if(pid == 0){ 
            struct timespec inicio, fim;
            
            // iniciando contagem
            clock_gettime(CLOCK_MONOTONIC, &inicio);
            
            char nomeArquivo[50];
            sprintf(nomeArquivo, "arquivos/resultadosProcessos/resultBlock-%d.txt", i);
            FILE *arquivoSaida = fopen(nomeArquivo, "w");
            if (!arquivoSaida) {
                perror("Erro ao criar arquivo de saída");
                exit(1);
            }

            fprintf(arquivoSaida, "%d %d\n", n1, m2);

            // calculando intervalo desse processo
            int indiceInicio = i * P;
            int indiceFim = (i+1) * P;
            if(indiceFim > totalElementos) {
                indiceFim = totalElementos;
            }

            for(int idx = indiceInicio; idx < indiceFim; idx++){
                int linha = idx / m2;
                int coluna = idx % m2;

                int soma = 0;
                for(int t = 0; t < m1; t++){
                    soma += matrizA[linha][t] * matrizB[t][coluna];
                }
                fprintf(arquivoSaida, "%d %d %d\n", linha, coluna, soma);
            }


            clock_gettime(CLOCK_MONOTONIC, &fim);
            double tempo = ((fim.tv_sec - inicio.tv_sec) * 1000.0) + ((fim.tv_nsec - inicio.tv_nsec) / 1000000.0);

            fprintf(arquivoSaida, "\nTempo gasto: %.3f ms\n", tempo);
            fclose(arquivoSaida);
            exit(0);
        }
    }
    for(int i = 0; i < quantidadeDeProcessos; i++){
        wait(NULL);
    }

    for (int i = 0; i < n1; i++) free(matrizA[i]);
    free(matrizA);
    for (int i = 0; i < n2; i++) free(matrizB[i]);
    free(matrizB);
    return 0;
}