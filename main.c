#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "afnd.h"
#include "estado.h"
#include "transformacion.h"

int main(int argc, char **argv)
{
    AFND *AFND, *determinista;
    AFND = AFNDNuevo("automata", 6, 3);

    /*Insertamos los simbolos*/
    AFNDInsertaSimbolo(AFND, "+");
    AFNDInsertaSimbolo(AFND, "0");
    AFNDInsertaSimbolo(AFND, ".");

    /*Insertamos los estados*/
    AFNDInsertaEstado(AFND, "q0", INICIAL);
    AFNDInsertaEstado(AFND, "q1", NORMAL);
    AFNDInsertaEstado(AFND, "q2", NORMAL);
    AFNDInsertaEstado(AFND, "q3", NORMAL);
    AFNDInsertaEstado(AFND, "q4", NORMAL);
    AFNDInsertaEstado(AFND, "q5", FINAL);

    /*insertamos las transiciones normales*/
    AFNDInsertaTransicion(AFND, "q0", "+", "q1");
    AFNDInsertaTransicion(AFND, "q1", ".", "q2");
    AFNDInsertaTransicion(AFND, "q1", "0", "q1");
    AFNDInsertaTransicion(AFND, "q1", "0", "q4");
    AFNDInsertaTransicion(AFND, "q2", "0", "q3");
    AFNDInsertaTransicion(AFND, "q4", ".", "q3");
    AFNDInsertaTransicion(AFND, "q3", "0", "q3");

    /*Insertamos las transiciones lambda*/
    AFNDInsertaLTransicion(AFND, "q0", "q1");
    AFNDInsertaLTransicion(AFND, "q3", "q5");

    AFNDCierraLTransicion(AFND);
    AFNDImprimeConjuntoEstadosActual(stdout, AFND);

    determinista = AFND_convertir_a_determinista(AFND);

    AFNDADot(AFND);
    AFNDADot(determinista);

    /*transformacionEliminaLTransiciones(AFND);*/

    AFNDElimina(AFND);
    AFNDElimina(determinista);

    printf("CHECK FINAL\n");
    return 0;
}
