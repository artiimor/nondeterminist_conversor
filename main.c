#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "afnd.h"
#include "estado.h"
#include "transformacion.h"

int main(int argc, char **argv)
{
    AFND *AFND, *AFND_aux;
    AFND = AFNDNuevo("automata", 6, 3);
    estado *state;

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
    /*AFNDInsertaLTransicion(AFND, "q4", "q3");*/

    /*ponemos la cadena a procesar
    AFNDInsertaLetra(AFND, char * letra)*/

    AFND_aux = AFNDCierraLTransicion(AFND);
    AFNDImprimeConjuntoEstadosActual(stdout, AFND);

    AFND_obtener_tabla_transicion(AFND);

    /*AFNDADot(AFND_aux);*/
    AFNDADot(AFND);

    /*transformacionEliminaLTransiciones(AFND);*/

    AFNDElimina(AFND);
    return 0;
}
