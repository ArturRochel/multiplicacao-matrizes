#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

//struct para agilizar e vacilitar a passagem de informações
typedef struct {
    int **M1;
    int **M2;
    int n1, m1, n2, m2;
    int startIndex;
    int P;
    int threadID;
} ThreadData;

void *multiplyBlock(void *arg){
    ThreadData *data = (ThreadData *)arg;

    int n1 = data->n1;
    int m1 = data->m1;
    int n2 = data->n2;
    int m2 = data->m2;
    int **M1 = data->M1;
    int **M2 = data->M2;
    int start = data->startIndex;
    int P = data->P;
    int threadID = data->threadID;

    //abrindo arquivo de saída para este bloco
    char filename[100];
    sprintf(filename, "arquivos/resultadosThreads/resultBlock-%d.txt", threadID);
    FILE *fout = fopen(filename, "w");
    if(!fout){
        printf("Erro ao criar arquivo %s\n", filename);
        pthread_exit(NULL);
    }

    //escrevendo o tamanho total de matriz
    fprintf(fout, "%d %d\n", n1, m2);

    struct timespec inicio, fim;
    clock_gettime(CLOCK_MONOTONIC, &inicio);

    int total_elements = n1 * m2;
    int count = 0;
    int idx = start;

    while(count < P && idx < total_elements){
        int row = idx / m2;
        int col = idx % m2;

        // multiplicação das matrizes
        int sum = 0;
        for(int k = 0; k < m1; k++){
            sum += M1[row][k] * M2[k][col];
        }

        fprintf(fout, "%d %d %d\n", row, col, sum);

        count++;
        idx++;
    }

    clock_gettime(CLOCK_MONOTONIC, &fim);
    double elapsed = (fim.tv_sec - inicio.tv_sec) * 1000.0;
    elapsed += (fim.tv_nsec - inicio.tv_nsec) / 1000000.0;
    fprintf(fout, "Tempo(ms): %.3f\n", elapsed);

    fclose(fout);

    pthread_exit(NULL);
}

int main(int argc, char *argv[]){
    // verificando quantidade de argumentos
    if(argc != 4){
        printf("Uso correto: %s <arquivo M1> <arquivo M2> <P>\n", argv[0]);
        return 1;
    }
    // armazenando informações
    char caminhoM1[100];
    char caminhoM2[100];
    
    sprintf(caminhoM1, "arquivos/%s", argv[1]);
    sprintf(caminhoM2, "arquivos/%s", argv[2]);

    int P = atoi(argv[3]);

    //verificando P
    if(P <= 0){
        printf("P deve ser um valor inteiro positivo.\n");
        return 1;
    }


     
    int n1, m1;
    // abrindo arquivo
    FILE *f1 = fopen(caminhoM1, "r");
    if(f1 == NULL){
        printf("Erro ao abrir o arquivo %s\n", caminhoM1);
        return 1;
    }
    // lendo as dimenções da matriz     
    fscanf(f1, "%d %d", &n1, &m1);

    //alocando dinamicamente
    int  **M1 = malloc (n1 * sizeof(int *));
    for(int i = 0; i < n1; i++){
        M1[i] = malloc(m1 * sizeof(int));
    }

    for(int i = 0; i < n1; i++){
        for(int j = 0; j < m1; j++){
            fscanf(f1, "%d", &M1[i][j]);
        }
    }

    fclose(f1);

    int n2, m2;

    //abrindo segundo arquivo
    FILE *f2 = fopen(caminhoM2, "r");
    if(f2 == NULL){
        printf("Erro ao abrir o arquivo %s\n", caminhoM2);
        return 1;
    }
    // lendo as dimensões da segunda matriz
    fscanf(f2, "%d %d", &n2, &m2);

    //alocando dinamicamente
    int **M2 = malloc(n2 *sizeof(int *));
    for(int i = 0; i < n2; i++){
        M2[i] = malloc(m2 * sizeof(int));
    }

    for(int i = 0; i < n2; i++){
        for(int j = 0; j < m2; j++){
            fscanf(f2, "%d", &M2[i][j]);
        }
    }

    fclose(f2);

    // verificando se é possivel fazer a multiplicação
    if(m1 != n2){
        printf("Não é possível fazer a multiplicação.\n");
        return 1;
    }

    // calculando as threads
    int totalElementos = n1*m2;
    int numeroThreads = (totalElementos + P -1)/P; // função teto

    pthread_t threads[numeroThreads];
    ThreadData threadData[numeroThreads];

    for(int t = 0; t < numeroThreads; t++){
        threadData[t].M1 = M1;
        threadData[t].M2 = M2;
        threadData[t].n1 = n1;
        threadData[t].m1 = m1;
        threadData[t].n2 = n2;
        threadData[t].m2 = m2;
        threadData[t].startIndex = t * P;
        threadData[t].P = P;
        threadData[t].threadID = t;

        pthread_create(&threads[t], NULL, multiplyBlock, (void *)&threadData[t]);
    }

    for(int t = 0; t < numeroThreads; t++){
        pthread_join(threads[t], NULL);
    }

    for(int i = 0; i < n1; i++) free(M1[i]);
    free(M1);

    for(int i = 0; i < n2; i++) free(M2[i]);
    free(M2);

    return 0;
}