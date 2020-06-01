#ifndef ALOCA_STRING_H
#define ALOCA_STRING_H

/*
Funcoes para auxiliar na alocacao dinamica de strings
*/

#include <stdlib.h>

/**
 * Aloca uma string dinamicamente
 * Retorna NULL se nao for possivel alocar
 */
char* string_criar(int tam);

/**
 * Desaloca uma string
 * o parametro passado por referencia passa a valer NULL
 */
void string_apagar(char** str);

/**
 * Desaloca uma string se ela for vazia (tamanho 0)
 * Se a string for vazia, o parametro passado por referencia passa a valer NULL
 */
void string_apagarSeVazio(char** str);

#endif