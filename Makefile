all: gerador

gerador: 
	gcc utilitario/main.c -o gerador

sequencial:
	gcc sequencial/main.c -o sequencial

clean:
	rm -f gerador sequencial