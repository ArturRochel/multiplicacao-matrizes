# Multiplicação de Matrizes - Sequencial vs. Paralelo

Este projeto foi desenvolvido para a disciplina de **Sistemas Operacionais** do Instituto Metrópole Digital (IMD) da Universidade Federal do Rio Grande do Norte (UFRN).

O objetivo do trabalho é implementar e analisar o desempenho da multiplicação de matrizes em diferentes cenários: de forma sequencial, e de forma paralela utilizando threads e processos. [cite\_start]A análise comparativa busca observar os ganhos de performance obtidos com a paralelização em relação à abordagem convencional. [cite: 8, 9, 25]

## Estrutura do Projeto

O projeto é composto pelos seguintes programas em C e um script auxiliar:

  * **Gerador (`gerador.c`):** Um programa auxiliar que gera duas matrizes com dimensões especificadas via linha de comando e as salva em `arquivos/matrizA.txt` e `arquivos/matrizB.txt`.
  * **Sequencial (`sequencial.c`):** Realiza a multiplicação das duas matrizes de forma sequencial. O resultado e o tempo de execução são salvos em `arquivos/resulSequencial.txt`.
  * **Paralelo com Threads (`paraleloThread.c`):** Implementa a multiplicação de matrizes utilizando threads. O trabalho é dividido entre um número de threads, onde cada uma calcula uma parte (P) do resultado. Os resultados parciais e os tempos de cada thread são salvos em arquivos separados no diretório `arquivos/resultadosThreads/`.
  * **Paralelo com Processos (`paraleloProcessos.c`):** Similar à versão com threads, mas utiliza a criação de novos processos (`fork()`) para dividir a tarefa de multiplicação. Os resultados parciais são salvos no diretório `arquivos/resultadosProcessos/`.
  * **Juntar Blocos (`juntar_blocos.sh`):** Um script de shell para consolidar os múltiplos arquivos de resultado gerados pelas versões paralelas em um único arquivo final, `resultadoFinal.txt`, e calcular o tempo total aproximado.

## Como Compilar e Executar

### Pré-requisitos

  - Compilador GCC
  - Make
  - pthreads (para a versão com threads)
  - Biblioteca de matemática (`-lm`) (para a versão com processos)

### 1\. Compilação

Para compilar todos os programas, utilize o `Makefile` disponibilizado. Execute o seguinte comando na raiz do projeto:

```bash
make all
```

Os arquivos executáveis serão gerados no diretório `bin/`.

### 2\. Geração das Matrizes

Antes de executar os programas de multiplicação, gere as matrizes de entrada:

```bash
./bin/gerador [n1] [m1] [n2] [m2]
```

Onde `[n1] [m1]` são as dimensões da Matriz A e `[n2] [m2]` são as dimensões da Matriz B. As matrizes serão salvas em `arquivos/matrizA.txt` e `arquivos/matrizB.txt`.

**Exemplo:**

```bash
./bin/gerador 100 100 100 100
```

### 3\. Execução dos Programas de Multiplicação

  * **Sequencial:**

    ```bash
    ./bin/sequencial matrizA.txt matrizB.txt
    ```

    O resultado será salvo em `arquivos/resulSequencial.txt`.

  * **Paralelo com Threads:**

    ```bash
    ./bin/paraleloThread matrizA.txt matrizB.txt [P]
    ```

    Onde `[P]` é o número de elementos da matriz resultado que cada thread irá calcular.

  * **Paralelo com Processos:**

    ```bash
    ./bin/paraleloProcessos matrizA.txt matrizB.txt [P]
    ```

    Onde `[P]` é o número de elementos da matriz resultado que cada processo irá calcular.

### 4\. Juntando os Resultados Paralelos

Após executar as versões paralelas, utilize o script `juntar_blocos.sh` para unir os resultados parciais.

  * **Para resultados com Threads:**

    ```bash
    ./programs/juntar_blocos.sh threads
    ```

  * **Para resultados com Processos:**

    ```bash
    ./programs/juntar_blocos.sh processos
    ```

O resultado consolidado será salvo como `resultadoFinal.txt` dentro da respectiva pasta (`arquivos/resultadosThreads` ou `arquivos/resultadosProcessos`).

### 5\. Limpeza

Para remover os arquivos compilados e os resultados gerados, execute:

```bash
make clean
```

Para remover apenas os programas compilados:

```bash
make cleanprograms
```

Para remover apenas os arquivos de resultado:

```bash
make cleanresults
```
