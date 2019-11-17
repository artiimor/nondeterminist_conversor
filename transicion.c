#include <stdio.h>
#include <stdlib.h>

#include "transicion.h"
#include "afnd.h"

struct _transicion
{
  int *input_states;  /*Array de estados de entrada*/
  int input_symbol;   /*Simbolo de entrada*/
  int *output_states; /*Conjunto de estados a los que transiciona*/
};

transicion *transicion_new(int *input_states, int input_symbol, int *output_states)
{

  transicion *new_transicion;

  if (input_states == NULL || output_states == NULL)
    return NULL;

  new_transicion = (transicion *)malloc(sizeof(transicion));

  if (new_transicion == NULL)
    return NULL;

  new_transicion->input_states = input_states;
  new_transicion->input_symbol = input_symbol;
  new_transicion->output_states = output_states;

  return new_transicion;
}

int *transicion_get_input_states(transicion *transicion)
{
  if (transicion == NULL)
    return NULL;

  return transicion->input_states;
}

int transicion_get_input_symbol(transicion *transicion)
{
  if (transicion == NULL)
    return -1;

  return transicion->input_symbol;
}

int *transicion_get_output_states(transicion *transicion)
{
  if (transicion == NULL)
    return NULL;

  return transicion->output_states;
}

void transicion_debug(AFND *AFND, transicion *transicion)
{
  int i;

  if (transicion == NULL)
  {
    return;
  }
  printf("ESTADOS INICIALES: \n");
  for (i=0; transicion->input_states[i] != -1; i++){
    printf("\t%s \n",AFNDNombreEstadoEn(AFND, transicion->input_states[i]));
  }
  printf("\nSIMBOLO: \n");
  printf("\t%s\n",AFNDSimboloEn(AFND, transicion->input_symbol));

  printf("ESTADOS FINALES: \n");
  for (i=0; transicion->output_states[i] != -1; i++){
    printf("\t%s \n",AFNDNombreEstadoEn(AFND, transicion->output_states[i]));
  }
  printf("\n\n\n");
}