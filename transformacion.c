#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "afnd.h"
#include "transicion.h"
#include "estado.h"
#include "transformacion.h"

/*Funciones de manipulacion de arrays de enteros*/
int isInArrayEstados(int *arrayEstados, int number, int tamano);
int anadir_estados_array(int **estados_ini, int *estados_extra);
int *copiar_array(int *array_original);
int contar_array(int *array_original);
int is_content_equal(int *array_1, int *array_2);
int n_estados_insertados = 0;

AFND *AFND_convertir_a_determinista(AFND *original)
{
    transicion **tabla_transicion;
    int n_estados, n_simbolos, i, j, condicion_input, condicion_output;
    int *input, *output;
    int tipo_input, tipo_output;
    char *nombre_input, *nombre_output, *nombre_simbolo;
    AFND *determinista;
    int *estados_1, *estados_2;

    estados_1 = (int *)malloc(10 * sizeof(int));
    estados_1[0] = 1;
    estados_1[1] = 2;
    estados_1[2] = 3;
    estados_1[3] = 4;
    estados_1[4] = 5;
    estados_1[5] = 6;
    estados_1[6] = 7;
    estados_1[7] = 8;
    estados_1[8] = 9;
    estados_1[9] = -1;

    estados_2 = (int *)malloc(5 * sizeof(int));
    estados_2[0] = 10;
    estados_2[1] = 2;
    estados_2[2] = 23;
    estados_2[3] = 4;
    estados_2[4] = -1;

    anadir_estados_array(&estados_1, estados_2);

    for (i = 0; estados_1[i] != -1; i++)
    {
        printf("\n%d\n", estados_1[i]);
    }

    free(estados_1);
    free(estados_2);

    return NULL;

    n_simbolos = AFNDNumSimbolos(original);

    tabla_transicion = AFND_obtener_tabla_transicion(original, &n_estados);

    determinista = AFNDNuevo("determinista", n_estados, n_simbolos);

    for (i = 0; i < n_simbolos; i++)
    {
        AFNDInsertaSimbolo(determinista, AFNDSimboloEn(original, i));
    }

    for (i = 0; tabla_transicion[i] != NULL; i++)
    {

        condicion_input = 0;
        condicion_output = 0;
        transicion_debug(original, tabla_transicion[i]);

        input = transicion_get_input_states(tabla_transicion[i]);
        output = transicion_get_output_states(tabla_transicion[i]);
        nombre_input = transicion_get_input_state_name(original, tabla_transicion[i]);
        nombre_output = transicion_get_output_state_name(original, tabla_transicion[i]);
        nombre_simbolo = AFNDSimboloEn(determinista, transicion_get_input_symbol(tabla_transicion[i]));
        tipo_input = transicion_get_input_state_type(original, tabla_transicion[i]);
        tipo_output = transicion_get_output_state_type(original, tabla_transicion[i]);
        /*añadimos los estados si no se encontraban antes*/
        for (j = 0; j < n_estados_insertados; j++)
        {
            if (strcmp(nombre_input, AFNDNombreEstadoEn(determinista, j)) == 0)
            {
                condicion_input = 1;
            }
            if (strcmp(nombre_output, AFNDNombreEstadoEn(determinista, j)) == 0)
            {
                condicion_output = 1;
            }
        }
        if (condicion_input == 0)
        {
            n_estados_insertados++;
            AFNDInsertaEstado(determinista, nombre_input, tipo_input);
        }
        if (condicion_output == 0)
        {
            n_estados_insertados++;
            AFNDInsertaEstado(determinista, nombre_output, tipo_output);
        }

        /*Solo queda añadir la transicion*/
        AFNDInsertaTransicion(determinista, nombre_input, nombre_simbolo, nombre_output);
    }

    /*free tabla_transicion*/
    for (i = 0; tabla_transicion[i] != NULL; i++)
    {
        free(tabla_transicion[i]);
    }
    free(tabla_transicion);
    return determinista;
}

transicion **AFND_obtener_tabla_transicion(AFND *AFND, int *n_estados)
{
    transicion **tabla_transicion; /*Array de transiciones*/
    transicion *transicion_aux;
    int ini, i, j, count, condicion, n_transiciones = 1;
    int n_simbolos;
    int *estados_ini, *estados_aux; /*Arrays de estados*/
    int **estados_pendientes;       /*Array de estados a analizar*/
    int estado_revisado = 0;        /*Estado que estamos analizando*/
    int len_estados = 1;

    if (AFND == NULL)
    {
        return NULL;
    }

    /*INICIALIZACIONES*/
    n_simbolos = AFNDNumSimbolos(AFND);
    estados_pendientes = (int **)malloc(sizeof(int *));
    estados_pendientes[0] = NULL;
    tabla_transicion = (transicion **)malloc(sizeof(transicion *));
    tabla_transicion[0] = NULL;

    /*Primero cogemos el estado inicial y vemos a cuales transita con lambda*/
    ini = AFNDIndiceEstadoInicial(AFND);
    estados_ini = get_lambda_transition(AFND, ini);

    /*Metemos en la lista a analizar a estados_ini*/
    len_estados++;
    estados_pendientes = realloc(estados_pendientes, len_estados * sizeof(int *));
    estados_pendientes[len_estados - 2] = estados_ini;
    estados_pendientes[len_estados - 1] = NULL;

    /*Bucle principal*/
    while (estados_pendientes[estado_revisado] != NULL)
    {
        /*Miramos las posibles transiciones*/
        for (i = 0; i < n_simbolos; i++)
        {
            count = contar_array(estados_pendientes[estado_revisado]);
            estados_aux = get_estados_destino_with_lambdas(AFND, estados_pendientes[estado_revisado], count, i);

            /*añadimos el resultado al array de pendientes*/
            condicion = 0;
            if (estados_aux != NULL)
            {
                /*Primero lo añado a las transiciones*/
                transicion_aux = transicion_new(estados_pendientes[estado_revisado], i, estados_aux);
                n_transiciones++;
                tabla_transicion = realloc(tabla_transicion, n_transiciones * sizeof(transicion *));
                tabla_transicion[n_transiciones - 2] = transicion_aux;
                tabla_transicion[n_transiciones - 1] = NULL;

                /*Si transiciona lo añadimos al array que revisa*/
                /*comprobar que el array no estaba antes*/
                for (j = 0; estados_pendientes[j] != NULL && condicion == 0; j++)
                {
                    if (is_content_equal(estados_aux, estados_pendientes[j]))
                    {

                        condicion = 1;
                    }
                }

                if (condicion == 0)
                {

                    len_estados++;
                    estados_pendientes = realloc(estados_pendientes, len_estados * sizeof(int *));
                    estados_pendientes[len_estados - 2] = estados_aux;
                    estados_pendientes[len_estados - 1] = NULL;
                }
            }
        }
        estado_revisado++;
    }

    *n_estados = len_estados - 1;

    return tabla_transicion;
}

int isInArrayEstados(int *arrayEstados, int number, int tamano)
{
    int i;

    for (i = 0; i < tamano && arrayEstados[i] != -1; i++)
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

    if (n_estados == 0)
    {
        return NULL;
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
    int estados_max;
    int *estados_output;
    int *estados_lambda_aux;
    int *estados_output_aux;
    int i, check_loop;
    int count = 0;

    if (original == NULL || estado == NULL)
    {
        return NULL;
    }

    /*Los estados a los que se transiciona mediante el simbolo*/
    estados_output_aux = get_estados_destino(original, estado, n_estados_compruebo, simbolo);
    estados_output = get_estados_destino(original, estado, n_estados_compruebo, simbolo);
    if (estados_output == NULL)
    {
        return NULL;
    }
    /*Comprobamos los esget_estados_destino_with_lambdastados a los que se tranciona via lambda*/
    do
    {
        check_loop = 0;
        for (i = 0; estados_output_aux[i] != -1; i++)
        {
            estados_lambda_aux = get_lambda_transition(original, estados_output_aux[i]);
            if (anadir_estados_array(estados_output, estados_lambda_aux) > 0)
            {
                count = 1;
                check_loop = 1;
            }
        }

        /*free(estados_output_aux);*/
        if (check_loop == 1)
        {
            estados_output_aux = copiar_array(estados_output);
        }

    } while (check_loop == 1);

    return estados_output;
}

/*Check Pending*/
int anadir_estados_array(int **estados_ini, int *estados_extra)
{
    int i, j, aux;
    int ret = 0;
    int no_contiene = 1;
    int estados_ini_aux, n_estados_extra;
    int *estados_pointer = NULL;

    if (estados_ini == NULL || estados_extra == NULL)
    {
        return 0;
    }

    /*Cogemos el numero de estados al principio y al final*/
    for (i = 0; (*estados_ini)[i] != -1; i++)
        ;
    estados_ini_aux = i + 1;
    printf("estadosiniaux: %d\n", estados_ini_aux);

    for (i = 0; estados_extra[i] != -1; i++)
        ;
    n_estados_extra = i + 1;

    /*recorremos el conjunto de estados al final*/
    for (i = 0; i < n_estados_extra; i++)
    {
        no_contiene = 1;
        /*Vemos si el conjunto de estados ini lo contiene*/
        for (j = 0; j < estados_ini_aux && no_contiene == 1; j++)
        {
            /*En el caso de que lo contenga no hay nada que hacer*/
            if ((*estados_ini)[j] == estados_extra[i])
            {
                no_contiene = 0;
            }
        }
        /*Si no lo contenia lo aniadimos*/
        if (no_contiene == 1)
        {
            printf("estados_ini_aux = %d\n", estados_ini_aux);
            estados_ini_aux++;
            printf("estados_ini_aux = %d\n", estados_ini_aux);
            estados_pointer = realloc((*estados_ini), estados_ini_aux * sizeof(int));
            *estados_ini = estados_pointer;
            (*estados_ini)[estados_ini_aux - 2] = estados_extra[i];
            (*estados_ini)[estados_ini_aux - 1] = -1;
            ret++;
        }

        no_contiene = 1;
    }
    printf("PRECOPIA\n");
    for (i = 0; (*estados_ini)[i] != -1; i++)
    {
        printf("\n%d\n", (*estados_ini)[i]);
    }
    printf("FINITO\n");
    return ret;
}

/*Tira guay*/
int *copiar_array(int *array_original)
{
    int i, size;
    int *new_array = NULL;

    size = contar_array(array_original);

    new_array = (int *)calloc((size + 1), sizeof(int));

    for (i = 0; i < size; i++)
    {
        new_array[i] = array_original[i];
    }

    new_array[i] = -1;

    return new_array;
}

int contar_array(int *array_original)
{
    int count;

    if (array_original == NULL)
    {
        return 0;
    }

    for (count = 0; array_original[count] != -1; count++)
        ;

    return count;
}

int is_content_equal(int *array_1, int *array_2)
{
    int count1, count2, i;

    if (array_1 == NULL || array_2 == NULL)
    {
        return 0;
    }

    count1 = contar_array(array_1);
    count2 = contar_array(array_2);

    if (count1 != count2)
    {
        return 0;
    }

    for (i = 0; i < count1; i++)
    {
        if (isInArrayEstados(array_2, array_1[i], count1) == 0)
        {
            return 0;
        }
    }

    return 1;
}
