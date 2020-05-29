#ifndef CRITERIO_H
#define CRITERIO_H

#include <stdio.h>
#include <stdlib.h>

#include "dupla.h"
#include "registro.h"  // STR_TAM

/**
 * TAD Criterio: Vetor de Duplas
 */
typedef struct _criterio Criterio;

/**
 * Cria um novo objeto criterio
 * 
 * Retorna NULL se nao for possivel criar o objeto
 * 
 * Obs: Depois, use uma funcao para destruir o objeto
 */
Criterio* criterio_criar();

/**
 * Le dados do stdin, e retorna um objeto criterio com os dados lidos
 * A entrada deve ser:
 * n campo[1] valor[2] .. campo[n] valor[n]
 */
Criterio* criterio_criarStdin();

/**
 * Destroi o objeto
 * Destroi os objetos e desaloca variaveis que estiver dentro dele
 *!Os arrays retornado pelo getArray deixa de ter valor valido e nao deve ser mais usado
 */
void criterio_apagar(Criterio** criterio);

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
 * Se houver um array armazenado ele eh apagado (inclusive s)
 *!Os arrays retornado pelo getArray deixa de ter valor valido e nao deve ser mais usado
 *!Se você quer evitar isso faça uma copia dos dados do array antes
 */
void criterio_setArray(Dupla** duplas, int n);

#endif