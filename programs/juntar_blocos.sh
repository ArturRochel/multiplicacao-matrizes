\#!/bin/bash

# Verifica se o usuário passou o parâmetro
if [ $# -ne 1 ]; then
    echo "Uso: $0 [processos|threads]"
    exit 1
fi

# Define a pasta com base no parâmetro
tipo=$1
if [ "$tipo" = "processos" ]; then
    pasta="../arquivos/resultadosProcessos"
elif [ "$tipo" = "threads" ]; then
    pasta="../arquivos/resultadosThreads"
else
    echo "Opção inválida: $tipo"
    echo "Use 'processos' ou 'threads'"
    exit 1
fi

# Arquivo final dentro da mesma pasta
arquivoFinal="$pasta/resultadoFinal.txt"

# Lendo as dimensões da matriz do primeiro arquivo de bloco
read n1 m2 < "$pasta/resultBlock-0.txt"

# Criando o arquivo final e escrevendo as dimensões
echo "$n1 $m2" > "$arquivoFinal"

# Criando uma matriz auxiliar (array associativo) para armazenar resultados
declare -A matriz

# Preenche a matriz a partir dos arquivos
for f in "$pasta"/resultBlock-*.txt; do
    while read -r a b c; do
        # Ignora linhas inválidas
        if [[ "$a" =~ ^[0-9]+$ && "$b" =~ ^[0-9]+$ && "$c" =~ ^[0-9]+$ ]]; then
            matriz["$a,$b"]=$c
        fi
    done < <(grep -v "Tempo" "$f" | tail -n +2)
done

# Escreve a matriz no arquivo final na ordem correta
for ((i=0; i<n1; i++)); do
    linha=""
    for ((j=0; j<m2; j++)); do
        linha="$linha${matriz[$i,$j]} "
    done
    echo "$linha" >> "$arquivoFinal"
done

# Pegando o maior tempo entre todas as execuções (em ms)
tempoTotal=$(grep "Tempo gasto" "$pasta"/resultBlock-*.txt | awk '{if ($3 > max) max=$3} END {print max}')
echo "Tempo total de execução: $tempoTotal ms" >> "$arquivoFinal"