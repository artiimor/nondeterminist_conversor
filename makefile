CC = gcc -g -ansi
CFLAGS = -Wall
EXE = main

all : $(EXE)

.PHONY : clean	

$(EXE) : % : %.o afnd.o transformacion.o estado.o transicion.o
	@echo "#---------------------------"
	@echo "# Generando $@ "
	@echo "# Depende de $^"
	@echo "# Ha cambiado $<"
	$(CC) $(CFLAGS) -o $@ $@.o afnd.o transformacion.o estado.o transicion.o

afnd.o : afnd.c afnd.h
	@echo "#---------------------------"
	@echo "# Generando $@"
	@echo "# Depende de $^"
	@echo "# Ha cambiado $<"
	$(CC) $(CFLAGS) -c $<

transformacion.o : transformacion.c transformacion.h transicion.h afnd.h
	@echo "#---------------------------"
	@echo "# Generando $@"
	@echo "# Depende de $^"
	@echo "# Ha cambiado $<"
	$(CC) $(CFLAGS) -c $<

transicion.o : transicion.c transicion.h afnd.h
	@echo "#---------------------------"
	@echo "# Generando $@"
	@echo "# Depende de $^"
	@echo "# Ha cambiado $<"
	$(CC) $(CFLAGS) -c $<

estado.o : estado.c estado.h afnd.h
	@echo "#---------------------------"
	@echo "# Generando $@"
	@echo "# Depende de $^"
	@echo "# Ha cambiado $<"
	$(CC) $(CFLAGS) -c $<

clean :
	rm -rf *o
	rm -rf main
	rm -rf *.dot
	rm -rf *.pdf

dot :
	dot -Tpdf determinista.dot -o automata.pdf

runv:
	valgrind --leak-check=full --track-origins=yes ./main