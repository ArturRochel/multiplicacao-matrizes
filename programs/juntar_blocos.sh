#!/bin/bash

# Pasta onde estão os arquivos de threads
pasta="../arquivos/resultadosThreads"

# Arquivo final dentro da mesma pasta
arquivoFinal="$pasta/resultadoFinal.txt"

# Lendo as dimensões da matriz do primeiro arquivo de bloco
read n1 m2 < "$pasta/resultBlock-0.txt"

# Pegando todos os números (ignorando a primeira linha e a linha de tempo)
numeros=$(for f in "$pasta"/resultBlock-*.txt; do
    tail -n +2 "$f" | grep -v "Tempo"
done)

# Criando o arquivo final e escrevendo as dimensões
echo "$n1 $m2" > "$arquivoFinal"

# Organizando os números no formato correto da matriz
count=0
linha=""
for num in $numeros; do
    linha="$linha$num "
    count=$((count+1))
    if [ $count -eq $m2 ]; then
        echo "$linha" >> "$arquivoFinal"
        linha=""
        count=0
    fi
done

# Somando os tempos de todas as threads
tempoTotal=$(grep "Tempo gasto" "$pasta"/resultBlock-*.txt | awk '{s+=$4} END {print s}')
echo "Tempo total de execução: $tempoTotal segundos"
