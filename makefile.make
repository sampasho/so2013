gestore_coperanti: gestore_coperanti.c util.c
	cc -o bin/gestore_coperanti gestore_coperanti.c util.c 

gestore_repository: gestore_repository.c util.c
	cc -o bin/gestore_repository gestore_repository.c util.c 
	
cooperante: cooperante.c util.c
	cc -o bin/cooperante cooperante.c util.c 
	
clean: gestore_coperanti.c util.c gestore_repository.c cooperante.c 
	ipcclean

all: gestore_coperanti gestore_repository cooperante

#all: gestore_coperanti gestore_repository cooperante clear


clear:
	rm gestore_coperanti gestore_repository cooperante
