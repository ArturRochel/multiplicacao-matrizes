all: gerador

gerador: 
	gcc programs/gerador.c -o bin/gerador

sequencial:
	gcc programs/sequencial.c -o bin/sequencial

paraleloT:
	gcc programs/paraleloThread.c -o bin/paraleloThread

paraleloP:
	gcc programs/paraleloProcessos.c -o bin/paraleloProcessos -lm

cleanprograms:
	rm -f bin/gerador bin/sequencial bin/paraleloThread

cleantest:
	rm -f arquivos/resultadosThreads/*.txt