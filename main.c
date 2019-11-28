#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "afnd.h"
#include "estado.h"
#include "transformacion.h"

int main(int argc, char **argv)
{
    AFND *AFND, *determinista;
    AFND = AFNDNuevo("automata", 8, 2);
    int i;
    int *accesibles;

    /*Insertamos los simbolos*/
    AFNDInsertaSimbolo(AFND, "0");
    AFNDInsertaSimbolo(AFND, "1");

    /*Insertamos los estados*/
    AFNDInsertaEstado(AFND, "A", INICIAL);
    AFNDInsertaEstado(AFND, "B", NORMAL);
    AFNDInsertaEstado(AFND, "D", NORMAL);
    AFNDInsertaEstado(AFND, "E", NORMAL);
    AFNDInsertaEstado(AFND, "F", NORMAL);
    AFNDInsertaEstado(AFND, "G", NORMAL);
    AFNDInsertaEstado(AFND, "H", NORMAL);
    AFNDInsertaEstado(AFND, "C", FINAL);

    /*insertamos las transiciones normales*/
    AFNDInsertaTransicion(AFND, "A", "0", "B");
    AFNDInsertaTransicion(AFND, "A", "1", "F");
    AFNDInsertaTransicion(AFND, "B", "0", "G");
    AFNDInsertaTransicion(AFND, "B", "1", "C");
    AFNDInsertaTransicion(AFND, "C", "1", "C");
    AFNDInsertaTransicion(AFND, "C", "0", "A");
    AFNDInsertaTransicion(AFND, "D", "0", "C");
    AFNDInsertaTransicion(AFND, "D", "1", "G");
    AFNDInsertaTransicion(AFND, "E", "0", "H");
    AFNDInsertaTransicion(AFND, "E", "1", "F");
    AFNDInsertaTransicion(AFND, "F", "0", "C");
    AFNDInsertaTransicion(AFND, "F", "1", "G");
    AFNDInsertaTransicion(AFND, "G", "0", "G");
    AFNDInsertaTransicion(AFND, "G", "1", "E");
    AFNDInsertaTransicion(AFND, "H", "0", "G");
    AFNDInsertaTransicion(AFND, "H", "1", "C");

    /*Insertamos las transiciones lambda*/

    /*determinista = AFND_convertir_a_determinista(AFND);

    AFNDADot(AFND);
    AFNDADot(determinista);*/


    accesibles = get_estados_accesibles(AFND);

    for (i = 0; accesibles[i] != -1; i++)
    {
        printf("%s\n", AFNDNombreEstadoEn(AFND, accesibles[i]));
    }

    AFNDElimina(AFND);
    free(accesibles);

    printf("CHECK FINAL\n");

    return 0;
}