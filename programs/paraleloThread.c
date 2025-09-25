#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

//struct para agilizar e vacilitar a passagem de informações
typedef struct {
    int **matrizA;
    int **matrizB;
    int linha1, coluna1, linha2, coluna2;
    int indiceInicio;
    int P;
    int threadID;
} ThreadData;

void *multiplicarBloco(void *arg){
    ThreadData *data = (ThreadData *)arg;

    int linha1 = data->linha1;
    int coluna1 = data->coluna1;
    int linha2 = data->linha2;
    int coluna2 = data->coluna2;
    int **matrizA = data->matrizA;
    int **matrizB = data->matrizB;
    int start = data->indiceInicio;
    int P = data->P;
    int threadID = data->threadID;

    //abrindo arquivo de saída para este bloco
    char nomeArquivo[100];
    sprintf(nomeArquivo, "arquivos/resultadosThreads/resultBlock-%d.txt", threadID);
    FILE *arquivoSaida = fopen(nomeArquivo, "w");
    if(!arquivoSaida){
        printf("Erro ao criar arquivo %s\n", nomeArquivo);
        pthread_exit(NULL);
    }

    //escrevendo o tamanho total de matriz
    fprintf(arquivoSaida, "%d %d\n", linha1, coluna2);

    struct timespec inicio, fim;
    clock_gettime(CLOCK_MONOTONIC, &inicio);

    int total_elements = linha1 * coluna2;
    int contador = 0;
    int idx = start;

    while(contador < P && idx < total_elements){
        int linha = idx / coluna2;
        int coluna = idx % coluna2;

        // multiplicação das matrizes
        int soma = 0;
        for(int k = 0; k < coluna1; k++){
            soma += matrizA[linha][k] * matrizB[k][coluna];
        }

        fprintf(arquivoSaida, "%d %d %d\n", linha, coluna, soma);

        contador++;
        idx++;
    }

    clock_gettime(CLOCK_MONOTONIC, &fim);
    double tempo = (fim.tv_sec - inicio.tv_sec) * 1000.0;
    tempo += (fim.tv_nsec - inicio.tv_nsec) / 1000000.0;
    fprintf(arquivoSaida, "Tempo(ms): %.3f\n", tempo);

    fclose(arquivoSaida);

    pthread_exit(NULL);
}

int main(int argc, char *argv[]){
    // verificando quantidade de argumentos
    if(argc != 4){
        printf("Uso correto: %s <arquivo matrizA> <arquivo matrizB> <P>\n", argv[0]);
        return 1;
    }
    // armazenando informações
    char caminhoMatrizA[100];
    char caminhoMatrizB[100];
    
    sprintf(caminhoMatrizA, "arquivos/%s", argv[1]);
    sprintf(caminhoMatrizB, "arquivos/%s", argv[2]);

    int P = atoi(argv[3]);

    //verificando P
    if(P <= 0){
        printf("P deve ser um valor inteiro positivo.\n");
        return 1;
    }


     
    int linha1, coluna1;
    // abrindo arquivo
    FILE *arquivoMatrizA = fopen(caminhoMatrizA, "r");
    if(arquivoMatrizA == NULL){
        printf("Erro ao abrir o arquivo %s\n", caminhoMatrizA);
        return 1;
    }
    // lendo as dimenções da matriz     
    fscanf(arquivoMatrizA, "%d %d", &linha1, &coluna1);

    //alocando dinamicamente
    int  **matrizA = malloc (linha1 * sizeof(int *));
    for(int i = 0; i < linha1; i++){
        matrizA[i] = malloc(coluna1 * sizeof(int));
    }

    for(int i = 0; i < linha1; i++){
        for(int j = 0; j < coluna1; j++){
            fscanf(arquivoMatrizA, "%d", &matrizA[i][j]);
        }
    }

    fclose(arquivoMatrizA);

    int linha2, coluna2;

    //abrindo segundo arquivo
    FILE *arquivoMatriz2 = fopen(caminhoMatrizB, "r");
    if(arquivoMatriz2 == NULL){
        printf("Erro ao abrir o arquivo %s\n", caminhoMatrizB);
        return 1;
    }
    // lendo as dimensões da segunda matriz
    fscanf(arquivoMatriz2, "%d %d", &linha2, &coluna2);

    //alocando dinamicamente
    int **matrizB = malloc(linha2 *sizeof(int *));
    for(int i = 0; i < linha2; i++){
        matrizB[i] = malloc(coluna2 * sizeof(int));
    }

    for(int i = 0; i < linha2; i++){
        for(int j = 0; j < coluna2; j++){
            fscanf(arquivoMatriz2, "%d", &matrizB[i][j]);
        }
    }

    fclose(arquivoMatriz2);

    // verificando se é possivel fazer a multiplicação
    if(coluna1 != linha2){
        printf("Não é possível fazer a multiplicação.\n");
        return 1;
    }

    // calculando as threads
    int totalElementos = linha1*coluna2;
    int numeroThreads = (totalElementos + P -1)/P; // função teto

    pthread_t threads[numeroThreads];
    ThreadData threadData[numeroThreads];

    for(int t = 0; t < numeroThreads; t++){
        threadData[t].matrizA = matrizA;
        threadData[t].matrizB = matrizB;
        threadData[t].linha1 = linha1;
        threadData[t].coluna1 = coluna1;
        threadData[t].linha2 = linha2;
        threadData[t].coluna2 = coluna2;
        threadData[t].indiceInicio = t * P;
        threadData[t].P = P;
        threadData[t].threadID = t;

        pthread_create(&threads[t], NULL, multiplicarBloco, (void *)&threadData[t]);
    }

    for(int t = 0; t < numeroThreads; t++){
        pthread_join(threads[t], NULL);
    }

    for(int i = 0; i < linha1; i++) free(matrizA[i]);
    free(matrizA);

    for(int i = 0; i < linha2; i++) free(matrizB[i]);
    free(matrizB);

    return 0;
}