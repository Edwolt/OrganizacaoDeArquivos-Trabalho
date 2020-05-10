#ifndef CSV_H
#define CSV_H

#include <stdio.h>
#include <stdlib.h>  // Manipular Arquivos

/**
 * TAD Arquivo CSV
 * Por ser um simplesmente um FILE, não tem motivo para cirar um estrutura
 */
typedef FILE CSV;

/**
 * Abre um arquivo com nome path para leitura
 * Retorna NULL se não for possivel Abrir o arquivo
 * 
 * Obs: Depois, use uma função para destruir o objeto
 */
CSV* csv_open(path);

void csv_del(CSV** csv);

Registro* csv_lerRegistro(CSV* csv);

#endif