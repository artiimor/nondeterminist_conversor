#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "afnd.h"
#include "transformacion.h"

/*Funciones de manipulacion de arrays de enteros*/
int isInArrayEstados(int *arrayEstados, int number, int tamano);
int anadir_estados_array(int *estados_ini, int *estados_extra);
int *copiar_array(int *array_original);

AFND *transformacionEliminaLTransiciones(AFND *AFNDOriginal)
{
    AFND *newAFND;
    int nSimbolos;
    int nEstadosAntes, nEstadosDespues;
    int i, j, k, l, aux;
    int nLambdas = 0;

    int estado_i, estado_j;

    int iArrayEstados = 0;
    int *arrayEstados;

    char *nombre1, *nombre2, *nombre_aux;

    int eliminado = 0;

    if (AFNDOriginal == NULL)
    {
        return NULL;
    }

    nSimbolos = AFNDNumSimbolos(AFNDOriginal);
    nEstadosAntes = AFNDNumEstados(AFNDOriginal);
    nEstadosDespues = nEstadosAntes;

    /*Calculamos el cierre transitivo para comprobar las transiciones lambda*/
    AFNDCierraLTransicion(AFNDOriginal);

    /* Sobre el numero de estados finales:
     * -Si un estado tiene una transicion lambda el numero de estados es uno menor
     * -Si un estado tiene dos transiciones lambda el numero es estados es uno mayor
     * -A partir de dos se suma un estado por cada transicion lambda
     */

    /* Hacemos el bucle encargado de ver el numero de estados al final
     */
    for (i = 0; i < nEstadosAntes; i++)
    {
        for (j = 0; j < nEstadosAntes; j++)
        {
            /*Solo me importa cuando i y j son distintos*/
            if (i != j)
            {
                if (AFNDCierreLTransicionIJ(AFNDOriginal, i, j) == 1)
                {
                    nLambdas++;
                }
            }
        }
        if (nLambdas == 1)
        {
            nEstadosDespues--;
        }
        if (nLambdas > 1)
        {
            nEstadosDespues += (nLambdas - 1);
        }

        nLambdas = 0;
    }

    nEstadosDespues++;

    /*Creamos el nuevo automata finito no determinista*/
    newAFND = AFNDNuevo("SinLambdas", nEstadosDespues, nSimbolos);

    for (i = 0; i < nSimbolos; i++)
    {
        AFNDInsertaSimbolo(newAFND, AFNDSimboloEn(AFNDOriginal, i));
    }

    /*Le añadimos los nuevos estados al automata finito no determinista*/

    /*Creamos un array donde almacenaremos los indices de los estados que
      hemos añadido al AFND nuevo*/
    arrayEstados = (int *)calloc(nEstadosAntes, sizeof(int));

    /*Repetimos el bucle anterior*/
    for (i = 0; i < nEstadosAntes; i++)
    {
        for (j = 0; j < nEstadosAntes; j++)
        {
            if (i != j)
            {
                /*Cuando se de esta condicion tenemos que añadir un nuevo estado*/
                /*Tambien añadimos un nuevo estado al array de estados eliminados*/
                /*TODO añadir conjunto de transiciones de los dos estados*/

                if (AFNDCierreLTransicionIJ(AFNDOriginal, i, j) == 1)
                {
                    nombre1 = AFNDNombreEstadoEn(AFNDOriginal, i);
                    nombre2 = AFNDNombreEstadoEn(AFNDOriginal, j);
                    estado_i = AFNDTipoEstadoEn(AFNDOriginal, i);
                    estado_j = AFNDTipoEstadoEn(AFNDOriginal, j);
                    /*Comparacion para el nombre del estado (alfabetico)*/
                    nombre_aux = (char *)calloc(strlen(nombre1) + strlen(nombre2), sizeof(char *));
                    if (strcmp(nombre1, nombre2) < 0)
                    {
                        strcat(nombre_aux, nombre1);
                        strcat(nombre_aux, nombre2);
                    }
                    else
                    {
                        strcat(nombre_aux, nombre2);
                        strcat(nombre_aux, nombre1);
                    }

                    /*Comprobamos las posibles combinaciones para el tipo de estado*/
                    if ((estado_i == INICIAL && estado_j == FINAL) || (estado_i == FINAL && estado_j == INICIAL))
                    {
                        AFNDInsertaEstado(newAFND, nombre_aux, INICIAL_Y_FINAL);
                    }
                    else if (estado_i == INICIAL || estado_j == INICIAL)
                    {
                        AFNDInsertaEstado(newAFND, nombre_aux, INICIAL);
                    }
                    else if (estado_i == FINAL || estado_j == FINAL)
                    {
                        AFNDInsertaEstado(newAFND, nombre_aux, FINAL);
                    }
                    else
                    {
                        AFNDInsertaEstado(newAFND, nombre_aux, NORMAL);
                    }

                    if (isInArrayEstados(arrayEstados, i, nEstadosAntes) == 0)
                    {
                        arrayEstados[iArrayEstados] = i;
                        iArrayEstados++;
                    }

                    free(nombre_aux);
                }
            }
        }
        /*Si no lo he añadido al array de estados eliminados, añado el estado*/
        if (isInArrayEstados(arrayEstados, i, nEstadosAntes) == 0)
        {
            AFNDInsertaEstado(newAFND, AFNDNombreEstadoEn(AFNDOriginal, i), AFNDTipoEstadoEn(AFNDOriginal, i));
        }
    }

    /*********************************************************************************/
    /*********************************************************************************/
    /*********************************************************************************/
    /*Repetimos el bucle anterior*/
    for (i = 0; i < nEstadosAntes; i++)
    {
        for (j = 0; j < nEstadosAntes; j++)
        {
            if (i != j)
            {
                if (AFNDCierreLTransicionIJ(AFNDOriginal, i, j) == 1)
                {
                    eliminado = 1;
                    nombre1 = AFNDNombreEstadoEn(AFNDOriginal, i);
                    nombre2 = AFNDNombreEstadoEn(AFNDOriginal, j);

                    /*Comparacion para el nombre del estado (alfabetico)*/
                    nombre_aux = (char *)calloc(strlen(nombre1) + strlen(nombre2), sizeof(char *));
                    if (strcmp(nombre1, nombre2) < 0)
                    {
                        strcat(nombre_aux, nombre1);
                        strcat(nombre_aux, nombre2);
                    }
                    else
                    {
                        strcat(nombre_aux, nombre2);
                        strcat(nombre_aux, nombre1);
                    }

                    /*añadimos las transiciones correspondientes al automata*/
                    /*Caso para los estados multiples*/
                    for (k = 0; k < nSimbolos; k++)
                    {
                        for (l = 0; l < nEstadosAntes; l++)
                        {
                            if (AFNDTransicionIndicesEstadoiSimboloEstadof(AFNDOriginal, i, k, l) == 1 || AFNDTransicionIndicesEstadoiSimboloEstadof(AFNDOriginal, j, k, l) == 1)
                            {
                                /*Si no contiene el estado al que transita es que se ha unido a otro*/
                                if (AFNDContieneEstadoNombre(newAFND, AFNDNombreEstadoEn(AFNDOriginal, l)) == -1)
                                {
                                    for (aux = 0; aux < nEstadosDespues; aux++)
                                    {
                                        if (strstr(AFNDNombreEstadoEn(newAFND, aux), AFNDNombreEstadoEn(AFNDOriginal, l)) != NULL)
                                        {
                                            AFNDInsertaTransicion(newAFND, nombre_aux, AFNDSimboloEn(AFNDOriginal, k), AFNDNombreEstadoEn(newAFND, aux));
                                        }
                                    }
                                }
                                /*Si, por el contrario, contiene el estado al que transita, se añade la transicion sin problema*/
                                else
                                {
                                    AFNDInsertaTransicion(newAFND, nombre_aux, AFNDSimboloEn(AFNDOriginal, k), AFNDNombreEstadoEn(AFNDOriginal, l));
                                }
                            }
                        }
                    }
                    free(nombre_aux);
                }
            }
        }
        /*En el caso de que el estado que comprobamos no haya sido eliminado*/
        if (eliminado == 0)
        {
            for (k = 0; k < nSimbolos; k++)
            {
                for (l = 0; l < nEstadosAntes; l++)
                {
                    /*Si el estado i transita a otro*/
                    if (AFNDTransicionIndicesEstadoiSimboloEstadof(AFNDOriginal, i, k, l) == 1)
                    {
                        /*Si no contiene el estado al que transita es que ha sido eliminado por una lambda*/
                        if (AFNDContieneEstadoNombre(newAFND, AFNDNombreEstadoEn(AFNDOriginal, l)) == -1)
                        {
                            for (aux = 0; aux < nEstadosDespues; aux++)
                            {
                                if (strstr(AFNDNombreEstadoEn(newAFND, aux), AFNDNombreEstadoEn(AFNDOriginal, l)) != NULL)
                                {
                                    AFNDInsertaTransicion(newAFND, AFNDNombreEstadoEn(AFNDOriginal, i), AFNDSimboloEn(AFNDOriginal, k), AFNDNombreEstadoEn(newAFND, aux));
                                }
                            }
                        }
                        /*En el caso de que si contenga el estado al que transita*/
                        else
                        {
                            AFNDInsertaTransicion(newAFND, AFNDNombreEstadoEn(AFNDOriginal, i), AFNDSimboloEn(AFNDOriginal, k), AFNDNombreEstadoEn(AFNDOriginal, l));
                        }
                    }
                }
            }
        }
        eliminado = 0;
    }

    AFNDImprime(stdout, newAFND);
    /*AFNDADot(newAFND);*/

    free(arrayEstados);
    AFNDElimina(newAFND);

    return NULL;
}

int isInArrayEstados(int *arrayEstados, int number, int tamano)
{
    int i;

    for (i = 0; i < tamano; i++)
    {
        if (arrayEstados[i] == number)
        {
            return 1;
        }
    }

    return 0;
}

int AFNDContieneEstadoNombre(AFND *AFND, char *nombre)
{
    int nEstados, i;
    nEstados = AFNDNumEstados(AFND);

    for (i = 0; i < nEstados; i++)
    {
        if (strcmp(AFNDNombreEstadoEn(AFND, i), nombre) == 0)
        {
            return i;
        }
    }

    return -1;
}

int *get_estados_destino(AFND *original, int *estado, int n_estados_compruebo, int simbolo)
{
    int n_estados = 0;
    int max_estados;
    int i, j;
    int *estados_final = NULL; /*El conjunto de estados al final*/
    /*Control de errores*/
    if (original == NULL || estado == NULL)
    {
        return NULL;
    }

    max_estados = AFNDNumEstados(original);

    /*Pasamos por todos los estados que quiero comprobar*/
    for (i = 0; i < n_estados_compruebo; i++)
    {

        /*Comprobamos si con el simbolo transiciona a algun estado del original*/
        for (j = 0; j < max_estados; j++)
        {
            /*En el caso de que transicione al simbolo con indice j*/
            if (AFNDTransicionIndicesEstadoiSimboloEstadof(original, estado[i], simbolo, j) == 1)
            {

                /*Si transiciona por primera vez reservamos memoria*/
                if (estados_final == NULL)
                {

                    estados_final = (int *)malloc(sizeof(int));
                    estados_final[n_estados] = j;
                    n_estados++;
                }
                else
                {
                    /*Si el estado no lo hemos añadido antes lo añadimos ahora*/
                    if (isInArrayEstados(estados_final, j, n_estados) != 1)
                    {
                        n_estados++;
                        /*redimensionamos la movida*/
                        estados_final = realloc(estados_final, n_estados * (sizeof(int)));
                        estados_final[n_estados - 1] = j;
                    }
                }
            }
        }
    }

    /*Para recorrer el bucle luego*/
    n_estados++;
    estados_final = realloc(estados_final, n_estados * sizeof(int));
    estados_final[n_estados - 1] = -1;

    return estados_final;
}

int *get_lambda_transition(AFND *original, int estado_input)
{
    int *estados_final = NULL; /*Los estados a los que transiciona*/
    int n_estados = 0;         /*El conjunto de estados que contiene*/
    int estados_max;
    int i;

    if (original == NULL)
    {
        return NULL;
    }

    estados_max = AFNDNumEstados(original);

    AFNDCierraLTransicion(original);

    for (i = 0; i < estados_max; i++)
    {
        if (i != estado_input)
        {
            /*Si el estado a comprobar transiciona al estado con indice i*/
            if (AFNDCierreLTransicionIJ(original, estado_input, i) == 1)
            {
                if (estados_final == NULL)
                {
                    estados_final = (int *)malloc(sizeof(int));
                    estados_final[n_estados] = i;
                    n_estados++;
                }
                else
                {
                    /*redimensionamos la movida*/
                    n_estados++;
                    estados_final = realloc(estados_final, n_estados * sizeof(int));
                    estados_final[n_estados - 1] = i;
                }
            }
        }
    }

    /*Le añado a si mismo*/
    n_estados++;
    estados_final = realloc(estados_final, n_estados * sizeof(int));
    estados_final[n_estados - 1] = estado_input;

    /*Le añado el -1 para conocer el tamaño sin problema*/
    n_estados++;
    estados_final = realloc(estados_final, n_estados * sizeof(int));
    estados_final[n_estados - 1] = -1;

    return estados_final;
}

int *get_estados_destino_with_lambdas(AFND *original, int *estado, int n_estados_compruebo, int simbolo)
{

    int *estados_final = NULL; /*Los estados a los que transiciona*/
    int n_estados = 0;         /*El conjunto de estados que contiene*/
    int estados_max;
    int *estados_output;
    int *estados_lambda_aux;
    int *estados_output_aux;
    int i, check_loop;

    if (original == NULL || estado == NULL)
    {
        return NULL;
    }

    /*Los estados a los que se transiciona mediante el simbolo*/
    estados_output_aux = get_estados_destino(original, estado, n_estados_compruebo, simbolo);
    estados_output = get_estados_destino(original, estado, n_estados_compruebo, simbolo);

    /*Comprobamos los estados a los que se tranciona via lambda*/
    do
    {
        check_loop = 0;
        for (i = 0; estados_output_aux[i] != -1; i++)
        {
            estados_lambda_aux = get_lambda_transition(original, estados_output_aux[i]);
            if (anadir_estados_array(estados_output, estados_lambda_aux) > 0)
            {
                check_loop = 1;
            }
        }

        free(estados_output_aux);
        if (check_loop == 1)
            estados_output_aux = copiar_array(estados_output);
    } while (check_loop == 0);

    return estados_output;
}

/*Check Pending*/
int anadir_estados_array(int *estados_ini, int *estados_extra)
{
    int i, j, aux;
    int ret = 0;
    int no_contiene = 1;
    int estados_ini_aux, n_estados_extra;

    if (estados_ini == NULL || estados_extra == NULL)
    {
        return 0;
    }

    /*Cogemos el numero de estados al principio y al final*/
    for (i = 0; estados_ini[i] != -1; i++)
        ;
    estados_ini_aux = i;

    for (i = 0; estados_extra[i] != -1; i++)
        ;
    n_estados_extra = i;

    /*recorremos el conjunto de estados al final*/
    for (i = 0; i < n_estados_extra; i++)
    {
        no_contiene = 1;
        /*Vemos si el conjunto de estados ini lo contiene*/
        for (j = 0; j < estados_ini_aux && no_contiene == 1; j++)
        {
            /*En el caso de que lo contenga no hay nada que hacer*/
            if (estados_ini[j] == estados_extra[i])
            {
                no_contiene = 0;
            }
        }
        /*Si no lo contenia lo aniadimos*/
        if (no_contiene == 1)
        {

            estados_ini_aux++;
            estados_ini = realloc(estados_ini, estados_ini_aux * sizeof(int));
            estados_ini[estados_ini_aux - 1] = estados_extra[i];
            estados_ini[estados_ini_aux] = -1;
            ret++;
        }

        no_contiene = 1;
    }

    return ret;
}

/*Tira guay*/
int *copiar_array(int *array_original)
{
    int i, aux;
    int *new_array = NULL;

    new_array = (int *)calloc(1, sizeof(int));

    for (i = 0; array_original[i] != -1; i++)
    {
        if (new_array == NULL)
        {
            new_array = (int *)calloc(1, sizeof(int));
            new_array[i] = array_original[i];
        }
        else
        {
            new_array = realloc(new_array, (i + 1) * sizeof(int));
            new_array[i] = array_original[i];
        }
    }

    new_array = realloc(new_array, (i + 1) * sizeof(int));
    new_array[i] = -1;

    return new_array;
}