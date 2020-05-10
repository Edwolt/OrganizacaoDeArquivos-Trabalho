#ifndef CSV_H
#define CSV_H

#include <stdlib.h>

typedef struct _CSV CSV;

/**
 * Aloca objeto do tipo CSV dinamicamente
 * Retorna NULL se não for possivel
 * 
 * Obs: É necessário usar uma função que desaloca memória
 */
CSV* csv_new(char* path);

/**
 * Apaga Estrutura e objetos que estão nela
 * 
 * Obs: Desaloca memória
 */
void csv_del(CSV* csv);

/**
 * Apaga Estrutura sem apagar objetos que estão nela
 * 
 * Obs: Desaloca memória
 */
void csv_rmv(CSV* csv);

Registro* csv_lerRegistro(CSV* csv)

#endif