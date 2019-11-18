#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "afnd.h"
#include "estado.h"

struct _estado
{
    int *estados;  /*Array de estados "Simultaneos"*/
    char *nombre; /*Simbolo de entrada*/
    int tipo_estado;
};

estado *new_estado(int *estados)
{

    estado *new_estado = NULL;

    if (estados == NULL)
    {
        return NULL;
    }

    new_estado = (estado *)malloc(sizeof(estado));

    if (new_estado == NULL)
    {
        return NULL;
    }

    new_estado->estados = estados;
    new_estado->nombre = NULL;
    new_estado->tipo_estado = NORMAL;

    return new_estado;
}

void free_estado(estado *estado)
{
    if (estado == NULL)
    {
        return;
    }

    if (estado->nombre != NULL)
    {
        free(estado->nombre);
    }

    if (estado->estados != NULL)
    {
        free(estado->estados);
    }

    free(estado);
    estado = NULL;
}

void estado_calculate_name(estado *estado, AFND *AFND)
{
    char *nombre;
    char *nombre_aux;
    int i = 0;
    int len_aux;

    if (estado == NULL || AFND == NULL || estado->estados == NULL)
    {
        return;
    }
printf("HOLA\n");
    printf("%d\n",estado->estados[0]);
    
    nombre_aux = AFNDNombreEstadoEn(AFND, estado->estados[i]);
    len_aux = strlen(nombre_aux);
    
    /*nombre tiene el nombre del primer estado*/
    nombre = (char *)calloc(len_aux, sizeof(char *));
    strcpy(nombre, nombre_aux);

    /*Vamos añadiendo los nombre de los otro estados*/
    for (i = 1; estado->estados[i] != -1; i++)
    {
        /*Cogemos el nombre que corresponde*/
        nombre_aux = AFNDNombreEstadoEn(AFND, estado->estados[i]);

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

    estado->nombre = nombre;

    printf("El nombre to guapo que le hemos puesto es: %s\n", estado->nombre);

    return;
}

void estado_calculate_tipo_estado(estado *estado, AFND *AFND)
{
    int i;

    if (estado == NULL || AFND == NULL)
    {
        return;
    }

    /*Bucle con el que miramos los estados de todos los estados*/
    /*Si un estado no es normal su tipo se "añade" al estado que estamos analizando*/
    for (i = 0; estado->estados[i] != -1; i++)
    {
        if (AFNDTipoEstadoEn(AFND, estado->estados[i]) == FINAL)
        {
            if (estado->tipo_estado == INICIAL)
            {
                estado->tipo_estado = INICIAL_Y_FINAL;
                return;
            }
            estado->tipo_estado = FINAL;
        }
        else if (AFNDTipoEstadoEn(AFND, estado->estados[i] == INICIAL))
        {
            if (estado->tipo_estado == FINAL)
            {
                estado->tipo_estado = INICIAL_Y_FINAL;
                return;
            }
            estado->tipo_estado = INICIAL;
        }
        else if (AFNDTipoEstadoEn(AFND, estado->estados[i]) == INICIAL_Y_FINAL)
        {
            estado->tipo_estado = INICIAL_Y_FINAL;
            return;
        }
    }
}

char *estado_get_nombre(estado *estado)
{
    if (estado == NULL)
    {
        return NULL;
    }

    return estado->nombre;
}

int estado_get_tipo_estado(estado *estado)
{
    if (estado == NULL)
    {
        return -1;
    }

    return estado->tipo_estado;
}
