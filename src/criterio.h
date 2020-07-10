#ifndef CRITERIO_H
#define CRITERIO_H

#include <stdio.h>
#include <stdlib.h>

#include "binarioNaTela.h"  // trim
#include "dupla.h"
#include "registro.h"  // STR_TAM
#include "util.h"

/**
 * TAD Criterio: Vetor de Duplas
 */
typedef struct Criterio Criterio;

/**
 * Cria um criterio
 * Retorna NULL se nao for possivel criar o objeto
 */
Criterio* criterio_criar(Dupla** duplas, int n);

/**
 * Le dados do stdin, e retorna um objeto criterio com os dados lidos
 * A entrada deve ser:
 * n campo[1] valor[1] .. campo[n] valor[n]
 */
Criterio* criterio_criarDoStdin();

/**
 * Destroi o objeto, os objetos que o pertence e desaloca variveis
 * Os arrays retornado pelo getArray deixam de ter valor valido
 */
void criterio_apagar(Criterio** criterio);

/**
 * Retorna se um registro satisfaz o criterio
 * Para cada dupla no criterio o registro precisa no campo descrito pela dupla ter o valor que esta na dupla
 * 
 * Retorna false se registro for NULL
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
 * Se houver um array armazenado ele eh apagado
 * Os arrays retornado pelo getArray deixa de ter valor valido
 */
void criterio_setArray(Criterio* criterio, Dupla** duplas, int n);

/**
 * Retorna se o criterio só pode satisfazer um registro
 * Para isso acontecer uma das duplas tem que ser idNascimento
 */
bool criterio_unico(Criterio* criterio);

#endif