#ifndef INDICE_H
#define INDICE_H

#include <stdbool.h>
#include <stdio.h>  // Arquivos
#include <stdlib.h>  // Alocao Dinamica

#include "registro.h"
#include "util.h"

/**
 * TAD Indices para arquivo binario
 * Implementa um arquivo de indices com Arvore B
 */
typedef struct Indice Indice;

/**
 * Cria um arquivo de indice vazio para escrita com nome path
 * No arquivo ja eh escrito o registro cabecalho marcando o arquivo como inconsistente
 * O arquivo aponta para o primeiro ?
 * 
 * Se o arquivo ja existir, sobrescreve ele com um arquivo novo
 * Retorna NULL se nao for possivel criar o arquivo
 */
Indice* indice_criar(char* path);

Indice* indice_abrirLeitura(char* path);

Indice* indice_abrirEscrita(char* path);

void indice_fechar(Indice** indice);

void indice_apontar(Indice* indice, int rrn, int whence);

Registro* indice_buscar(char* path);

#endif