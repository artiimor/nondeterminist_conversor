#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "afnd.h"
#include "transformacion.h"

int main(int argc, char **argv)
{
    AFND *AFND, *determinista;
    AFND = AFNDNuevo("automata", 8, 2);
    int i, j;
    int *accesibles;
    int **matriz;

    /*Insertamos los simbolos*/
    AFNDInsertaSimbolo(AFND, "0");
    AFNDInsertaSimbolo(AFND, "1");

    /*Insertamos los estados*/
    AFNDInsertaEstado(AFND, "A", INICIAL);
    AFNDInsertaEstado(AFND, "B", NORMAL);
    AFNDInsertaEstado(AFND, "C", FINAL);
    AFNDInsertaEstado(AFND, "D", NORMAL);
    AFNDInsertaEstado(AFND, "E", NORMAL);
    AFNDInsertaEstado(AFND, "F", NORMAL);
    AFNDInsertaEstado(AFND, "G", NORMAL);
    AFNDInsertaEstado(AFND, "H", NORMAL);

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

    matriz = (int **)calloc(8, sizeof(int *));
    for (i = 0; i < 8; i++)
    {
        matriz[i] = (int *)calloc(8, sizeof(int));
    }
    for (i = 0; i < 8; i++)
    {
        for (j = 0; j < 8; j++)
        {
            if (AFNDTipoEstadoEn(AFND, i) == FINAL || AFNDTipoEstadoEn(AFND, j) == FINAL)
            {
                matriz[i][j] = 1;
            }
        }
    }

    printf("RETORNO: %d\n",comprobar_distinguibles(AFND, matriz, 0, 3));

    for (i = 0; i < 8; i++)
    {
        for (j = 0; j < 8; j++)
        {
            printf("|%d ", matriz[i][j]);
        }
        printf("\n");
    }

    AFNDElimina(AFND);

    for (i = 0; i < 8; i++)
    {
         free(matriz[i]);
    }
    free(matriz);

    printf("CHECK FINAL\n");

    return 0;
}