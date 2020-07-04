#ifndef CRITERIO_H
#define CRITERIO_H

#include <stdio.h>
#include <stdlib.h>

#include "alocaString.h"
#include "binarioNaTela.h"  // trim
#include "dupla.h"
#include "registro.h"  // STR_TAM

/**
 * TAD Criterio: Vetor de Duplas
 */
typedef struct Criterio Criterio;

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
 * 
 * Obs: Depois, use uma funcao para destruir o objeto
 */
Criterio* criterio_criarDoStdin();

/**
 * Destroi o objeto
 * Destroi os objetos e desaloca variaveis que estiver dentro dele
 * 
 * Os arrays retornado pelo getArray deixa de ter valor valido e nao deve ser mais usado
 */
void criterio_apagar(Criterio** criterio);

/**
 * Retorna se um registro satisfaz o criterio
 * Para cada dupla no criterio o registro precisa no campo descrito pela dupla ter o valor que esta na dupla
 * 
 * Retorna false se registro ou duplas for NULL
 */
bool criterio_satisfaz(Criterio* criterio, Registro* registro);

/**
 * Atualiza o registro com os dados salvo em criterio
 * e apaga o objeto criterio
 */
void criterio_atualizarRegistro(Criterio** criterio, Registro* registro);

//* ============================= *//
//* ===== Getters e Setters ===== *//
//* ============================= *//

/**
 * Retorna um array com todas as duplas
 * Retorna em n o tamanho do array
 * 
 * Se alterar algo no array retornado tambem altera os dados armazenado
 */
Dupla** criterio_getArray(Criterio* criterio, int* n);

/**
 * Muda o array
 * 
 * Se houver um array armazenado ele eh apagado
 * Os arrays retornado pelo getArray deixa de ter valor valido e nao deve ser mais usado
 * Se você quer evitar isso faça uma copia dos dados do array antes
 */
void criterio_setArray(Criterio* criterio, Dupla** duplas, int n);

#endif