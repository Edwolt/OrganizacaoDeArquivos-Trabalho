#ifndef CRITERIO_H
#define CRITERIO_H

#include <stdio.h>
#include <stdlib.h>

#include "dupla.h"

/**
 * TAD Criterio: Vetor de Duplas
 */
typedef struct _criterio Criterio;

Criterio* criterio_criar();

/**
 * Le dados do stdin, e retorna um objeto criterio com os dados lidos
 * A entrada deve ser:
 * n campo[1] valor[2] .. campo[n] valor[n]
 */
Criterio* criterio_scan();

/**
 * Desaloca criterio do stdin
 */
void criterio_apagar();

//* ============================= *//
//* ===== Getters e Setters ===== *//
//* ============================= *//

/**
 * Retorna um array com todas as duplas
 * Retorna em n o tamanho do array
 * 
 * Se alterar algo no array retornado tambem altera os dados armazenado
 */
Dupla** criterio_getArray(int* n);

/**
 * Muda o array
 * 
 * Se houver um array armazenado ele eh apagado
 *!Se você quer evitar isso use o getArray e faça uma copia dos dados retornados
 */
void criterio_setArray(Dupla** duplas, int n);

#endif