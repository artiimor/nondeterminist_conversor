#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "afnd.h"
#include "state.h"

struct _state
{
    int *states;  /*Array de states "Simultaneos"*/
    char *nombre; /*Simbolo de entrada*/
};

state *new_state(int *states)
{

    state *new_state = NULL;

    if (states == NULL)
    {
        return NULL;
    }

    new_state = (state *)malloc(sizeof(state));

    if (new_state == NULL)
    {
        return NULL;
    }

    new_state->states = states;
    new_state->nombre = NULL;

    return new_state;
}

void free_state(state *state)
{
    if (state == NULL)
    {
        return;
    }

    if (state->nombre != NULL)
    {
        free(state->nombre);
    }

    if (state->states != NULL)
    {
        free(state->states);
    }

    free(state);
    state = NULL;
}

int state_calculate_name(state *state, AFND *AFND)
{
    char *nombre;
    char *nombre_aux;
    int i = 0;
    int len_aux;

    if (state == NULL || AFND == NULL || state->states == NULL)
    {
        return -1;
    }

    nombre_aux = AFNDNombreEstadoEn(AFND, state->states[i]);
    len_aux = strlen(nombre_aux);

    /*nombre tiene el nombre del primer estado*/
    nombre = (char *)calloc(len_aux, sizeof(char *));
    strcpy(nombre, nombre_aux);

    /*Vamos añadiendo los nombre de los otro estados*/
    for (i = 1; state->states[i] != -1; i++)
    {
        /*Cogemos el nombre que corresponde*/
        nombre_aux = AFNDNombreEstadoEn(AFND, state->states[i]);

        /*Si el nombre no lo hemos añadido ya*/
        if (strstr(nombre, nombre_aux) == NULL)
        {
            /*Redimensionamos*/
            len_aux += strlen(nombre_aux);
            nombre = realloc(nombre, len_aux * sizeof(char *));

            /*añadimos la cadena*/
            strcat(nombre, nombre_aux);
        }
    }

    state->nombre = nombre;

    printf("El nombre to guapo que le hemos puesto es: %s\n", state->nombre);

    return 1;
}
