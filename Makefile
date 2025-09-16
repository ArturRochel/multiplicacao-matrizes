all: gerador sequencial paraleloT paraleloP

gerador: 
	gcc programs/gerador.c -o bin/gerador

sequencial:
	gcc programs/sequencial.c -o bin/sequencial

paraleloT:
	gcc programs/paraleloThread.c -o bin/paraleloThread -lpthread

paraleloP:
	gcc programs/paraleloProcessos.c -o bin/paraleloProcessos -lm

clean: cleanprograms cleanresults

cleanprograms:
	rm -f bin/gerador bin/sequencial bin/paraleloThread bin/paraleloProcessos

cleanresults:
	rm -f arquivos/resultadosThreads/*.txt arquivos/resultadosProcessos/*.txt arquivos/resulSequencial.txt arquivos/matrizA.txt arquivos/matrizB.txt

cleanprograms:
	rm -f bin/gerador bin/sequencial bin/paraleloThread bin/paraleloProcessos