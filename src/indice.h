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

/**
 * TODO colocar documentacao aqui
 */
Indice* indice_abrir(char* path, bool escrita);

/**
 * Abre arquivo para leitura
 * Wrapper para indice_abrir
 */
inline static Indice* indice_abrirLeitura(char* path) { return indice_abrir(path, false); }

/**
 * Abre arquivo para escrita
 * Wrapper para indice_abrir
 */
inline static Indice* indice_abrirEscrita(char* path) { return indice_abrir(path, true); }

/**
 * TODO colocar documentacao aqui
 */
void indice_fechar(Indice** indice);

/**
 * TODO colocar documentacao aqui
 */
void indice_apontar(Indice* indice, int rrn, int whence);

/**
 * TODO colocar documentacao aqui
 */
int indice_buscar(Indice* indice, int id);

/**
 * TODO colocar documentacao aqui
 */
bool indice_inserir(Indice* indice, int id, int dado);

#endif