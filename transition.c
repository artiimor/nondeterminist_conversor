#include <stdio.h>
#include <stdlib.h>

#include "transition.h"

struct _transition
{
  int *input_states;  /*Array de estados de entrada*/
  int input_symbol;   /*Simbolo de entrada*/
  int *output_states; /*Conjunto de estados a los que transiciona*/
};

transition *transition_new(int *input_states, int input_symbol, int *output_states)
{

  transition *new_transition;

  if (input_states == NULL || output_states == NULL)
    return NULL;

  new_transition = (transition *)malloc(sizeof(transition));

  if (new_transition == NULL)
    return NULL;

  new_transition->input_states = input_states;
  new_transition->input_symbol = input_symbol;
  new_transition->output_states = output_states;

  return new_transition;
}

int *transition_get_input_states(transition *transition)
{
  if (transition == NULL)
    return NULL;

  return transition->input_states;
}

int transition_get_input_symbol(transition *transition)
{
  if (transition == NULL)
    return -1;

  return transition->input_symbol;
}

int *transition_get_output_states(transition *transition)
{
  if (transition == NULL)
    return NULL;

  return transition->output_states;
}