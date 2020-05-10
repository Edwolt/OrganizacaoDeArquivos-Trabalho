#ifndef CSV_H
#define CSV_H

#include <stdio.h>
#include <stdlib.h>  // Manipular Arquivos

#include "registro.h"

/**
 * TAD Arquivo CSV
 * Por ser um simplesmente um FILE, nao tem motivo para cirar um estrutura
 */
typedef FILE CSV;

/**
 * Abre um arquivo com nome path para leitura
 * Retorna NULL se nao for possivel abrir o arquivo
 * 
 * Obs: Depois, use uma função para destruir o objeto
 */
CSV* csv_open(char* path);

void csv_del(CSV** csv);

Registro* csv_lerRegistro(CSV* csv);

#endif