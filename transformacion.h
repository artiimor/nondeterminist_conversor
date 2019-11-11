#ifndef TRANSFORMACION_H
#define TRANSFORMACION_H

#include <stdio.h>
#include <stdlib.h>

#include "afnd.h"

/**
 * devuelve un automata finito no determinista donde se han eliminado las transiciones lambda
 * 
 * @param AFND el automata finito no determinista a transformar
 * 
 * @return el automata finito no determinista sin transiciones lambda
 */
AFND *transformacionEliminaLTransiciones(AFND *AFND);

/**
 * Devuelve un array de enteros, donde cada entero hace referencia al estado al que se transiciona.
 * 
 * @param original el AFND original
 * @param estado el conjunto de estados a comprobar (estamos en no determinista y podemos estar en varios a la vez)
 * @param simbolo entero que corresponde a un simbolo en el AFND original.
 * 
 * @return un array de enteros con los estados a los que se transiciona
 */
int *get_estados_destino(AFND *original, int *estado, int n_estados_compruebo, int simbolo);

/**
 * Devuelve el conjunto de estados al que transiciona un estado via transiciones lambda
 * 
 * @param original el AFND original
 * @param estado_input el estado que queremos comprobar
 * 
 * @return el conjunto de estados al que transiciona via lambda o NULL si no transiciona
 */
int *get_lambda_transition(AFND *original, int estado_input);

#endif