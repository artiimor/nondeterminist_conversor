#ifndef STATE_H
#define STATE_H

/**
  * Struct 
  * 
  * Representa un estado del AFND. Contiene un array de indices de estados en el AFND y el nombre.
  */
typedef struct _state state;

/**
  * Crea un estado con nombre a NULL
  * 
  * @param states Array del indice que tienen los estados en el AFND
  * 
  * @return el estado creado
  */
state *new_state(int *states);

/**
  * Libera memoria de un estado, con su nombre y su array incluidos
  * 
  * @param state, el estado a liberar
  */

void free_state(state *state);

int state_calculate_name(state *state, AFND *AFND);

#endif

