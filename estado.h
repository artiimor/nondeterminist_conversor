#ifndef ESTADO_H
#define ESTADO_H

/**
  * Struct 
  * 
  * Representa un estado del AFND. Contiene un array de indices de estados en el AFND y el nombre.
  */
typedef struct _estado estado;

/**
  * Crea un estado con nombre a NULL
  * 
  * @param estados Array del indice que tienen los estados en el AFND
  * 
  * @return el estado creado
  */
estado *new_estado(int *estados);

/**
  * Libera memoria de un estado, con su nombre y su array incluidos
  * 
  * @param estado, el estado a liberar
  */

void free_estado(estado *estado);

/**
  * Calcula el nombre del estado
  * 
  * @param estado el estado del que calculamos el nombre
  * @param AFND el automata finito no determinista original al que pertenecen sus estados
  * 
  * @return nada, tipo void
  */
void estado_calculate_name(estado *estado, AFND *AFND);

/**
  * Calcula el tipo de un estado
  * 
  * @param estado el estado del que calculamos el tipo
  * @param AFND el automata finito no determinista original al que pertenecen sus estados
  * 
  * @return nada, tipo void
  */
void estado_calculate_tipo_estado(estado *estado, AFND *AFND);

/**
  * Metodo getter del nombre del estado
  * 
  * @param estado el estado del que queremos conocer el nombre
  * 
  * @return el nombre del estado, NULL si algo va mal
  */

char *estado_get_nombre(estado *estado);

/**
  * Metodo getter del tipo del estado
  * 
  * @param estado el estado del que queremos conocer el tipo
  * 
  * @return Entero que representa el tipo del estado, -1 si algo va mal
  */
int estado_get_tipo_estado(estado *estado);



#endif

