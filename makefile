CC = gcc -ansi
CFLAGS = -Wall
EXE = main

all : $(EXE)

.PHONY : clean	

$(EXE) : % : %.o afnd.o transformacion.o state.o
	@echo "#---------------------------"
	@echo "# Generando $@ "
	@echo "# Depende de $^"
	@echo "# Ha cambiado $<"
	$(CC) $(CFLAGS) -o $@ $@.o afnd.o transformacion.o state.o

afnd.o : afnd.c afnd.h
	@echo "#---------------------------"
	@echo "# Generando $@"
	@echo "# Depende de $^"
	@echo "# Ha cambiado $<"
	$(CC) $(CFLAGS) -c $<

transformacion.o : transformacion.c transformacion.h afnd.h
	@echo "#---------------------------"
	@echo "# Generando $@"
	@echo "# Depende de $^"
	@echo "# Ha cambiado $<"
	$(CC) $(CFLAGS) -c $<

transition.o : transition.c transition.h
	@echo "#---------------------------"
	@echo "# Generando $@"
	@echo "# Depende de $^"
	@echo "# Ha cambiado $<"
	$(CC) $(CFLAGS) -c $<

state.o : state.c state.h afnd.h
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
	dot -Tpdf automata.dot -o automata.pdf

runv:
	valgrind --leak-check=full ./main