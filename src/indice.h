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
 * O arquivo aponta para a primeira pagina
 * 
 * Se o arquivo ja existir, sobrescreve ele com um arquivo novo
 * Retorna NULL se nao for possivel criar o arquivo
 */
Indice* indice_criar(char* path);

Indice* indice_abrir(char* path, bool escrita);

Indice* indice_abrirLeitura(char* path) { return indice_abrir(path, false); }

Indice* indice_abrirEscrita(char* path) { return indice_abrir(path, true); }

void indice_fechar(Indice** indice);

void indice_apontar(Indice* indice, int rrn, int whence);

int indice_buscar(Indice* indice, int id);

bool indice_inserir(Indice* indice, int id, int dado);

#endif