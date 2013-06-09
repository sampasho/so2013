gestore_coperanti: gestore_coperanti.c util.h
	cc -o bin/gestore_coperanti gestore_coperanti.c util.h

gestore_repository: gestore_repository.c util.c util.h
	cc -o bin/gestore_repository gestore_repository.c util.c util.h

cooperante: cooperante.c util.h
	cc -o bin/cooperante cooperante.c util.c util.h

clean: gestore_coperanti.c util.h gestore_repository.c cooperante.c
	ipcclean

all:  gestore_coperanti gestore_repository cooperante

#all: gestore_coperanti gestore_repository cooperante clear


clear:
	rm gestore_coperanti gestore_repository cooperante
