#ifndef TRANSITION_H
#define TRANSITION_H

/**
  * Struct 
  * 
  * Representa un caso para la funcion de transicion
  */
typedef struct _transition transition;

/**
  * Crea un caso de la funcion de transicion
  * 
  * @param input_state Array del indice que tienen los estados en el AFND. Entrada
  * @param imput_symbol Indice del simbolo de entrada en el AFND
  * @param res_states Array del indice que tienen los estados en el AFND. Salida
  */
transition *transition_new(int *input_state, int input_symbol, int *output_states);

/**
  * Metodo getter del conjunto de estados de entrada del caso de la funcion de transicion.
  * 
  * @param El caso de la funcion de transicion
  * @return Array con el indice que ocupan los estados de entrada en el AFND
  */
int *transition_get_input_state(transition *transition);

/**
  * Metodo getter del simbolo de entrada para el caso de la funcion de transicion.
  * 
  * @param El caso de la funcion de transicion
  * @return El indice que ocupa el simbolo de entrada
  */
int transition_get_input_symbol(transition *transition);

/**
  * Metodo getter del conjunto de estados de salida del caso de la funcion de transicion.
  * 
  * @param El caso de la funcion de transicion
  * @return Array con el indice que ocupan los estados de salida en el AFND
  */
int *transition_get_output_state(transition *transition);

#endif
