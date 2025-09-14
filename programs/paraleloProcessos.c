#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <math.h>


int main(int argc, char *argv[]){
    if(argc != 4){
        fprintf(stderr, "Uso: %s <matriz1.txt> <matriz2.txt> <P>\n", argv[0]);
        return 1;
    }
    //armazenado argumento
    char caminhoM1[100];
    char caminhoM2[100];
    //caminho para os arquivos das matrizes
    sprintf(caminhoM1, "../arquivos/%s", argv[1]);
    sprintf(caminhoM2, "../arquivos/%s", argv[2]);

    int P = atoi(argv[3]);
    if(P <= 0){
        fprintf(stderr, "Valor de P tem que ser maior que zero!");
        return 1;
    }

    // ler o arquivos das matrizes
    int n1, m1, n2, m2;
    FILE *f1 = fopen(caminhoM1, "r");
    //verificando se foi aberto
    if (!f1) {
        perror("Erro ao abrir matriz 1");
        return 1;
    }
    fscanf(f1, "%d %d", &n1, &m1);
    int **M1 = malloc(n1 * sizeof(int*));
    for(int i = 0; i < n1; i++){
        M1[i] = malloc(m1 * sizeof(int));
        for(int j = 0; j < m1; j++){
            fscanf(f1, "%d", &M1[i][j]);
        }
    }
    fclose(f1);

    FILE *f2 = fopen(caminhoM2, "r");

    if (!f2) {
        perror("Erro ao abrir matriz 2  ");
        return 1;
    }

    fscanf(f2, "%d %d", &n2, &m2);
    int **M2 = malloc(n2 * sizeof(int*));
    for(int i = 0; i < n2; i++){
        M2[i] = malloc(m2 * sizeof(int));
        for(int j = 0; j < m2; j++){
            fscanf(f2, "%d", &M2[i][j]);
        }
    }
    fclose(f2);

    // verificando se é possivel multiplicar
    if(m1 != n2){
        fprintf(stderr, "Não é possível multiplicar.\n");
        for (int i = 0; i < n1; i++) free(M1[i]);
        free(M1);
        for (int i = 0; i < n2; i++) free(M2[i]);
        free(M2);
        return 1;
    }
    

    // preparando para divisão do trabalho
    int totalElements = n1 * m2;
    int numProcesses = (int)ceil((double)totalElements / P);

    // criando os processos
    for(int k = 0; k < numProcesses; k++){
        pid_t pid = fork();
        // processo filho
        if(pid == 0){ 
            struct timespec start, end;
            
            // criando arquivo de saída para este filho
            clock_gettime(CLOCK_MONOTONIC, &start);
            
            char filename[50];
            sprintf(filename, "../arquivos/resultadosProcessos/resultBlock-%d.txt", k);
            FILE *out = fopen(filename, "w");
            if (!out) {
                perror("Erro ao criar arquivo de saída");
                exit(1);
            }

            fprintf(out, "%d %d\n", n1, m2);

            // calculando intervalo desse processo
            int startIdx = k * P;
            int endIdx = (k+1) * P;
            if(endIdx > totalElements) {
                endIdx = totalElements;
            }

            for(int idx = startIdx; idx < endIdx; idx++){
                int i = idx / m2;
                int j = idx % m2;

                int sum = 0;
                for(int t = 0; t < m1; t++){
                    sum += M1[i][t] * M2[t][j];
                }
                fprintf(out, "%d %d %d\n", i, j, sum);
            }


            clock_gettime(CLOCK_MONOTONIC, &end);
            double elapsed = ((end.tv_sec - start.tv_sec) * 1000.0) + ((end.tv_nsec - start.tv_nsec) / 1000000.0);

            fprintf(out, "\nTempo gasto: %.3f ms\n", elapsed);
            fclose(out);
            exit(0);
        }
    }
    for(int k = 0; k < numProcesses; k++){
        wait(NULL);
    }

    for (int i = 0; i < n1; i++) free(M1[i]);
    free(M1);
    for (int i = 0; i < n2; i++) free(M2[i]);
    free(M2);
    return 0;
}