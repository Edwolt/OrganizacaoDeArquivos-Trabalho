#ifndef UTIL_H
#define UTIL_H

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

//* ================== *//
//* ===== Macros ===== *//
//* ================== *//
/**
 * Tenta escrever qtde valores do tipo type a partir ponteiro ptr no arquivo file
 * Se não der certo, desvia para o label fwrite_error
 */
#define TRYFWRITE(ptr, type, qtde, file) \
    if (fwrite((ptr), sizeof(type), (qtde), (file)) != ((size_t)qtde)) goto fwrite_erro

/**
 * Tenta ler qtde valores do tipo type para o ponteiro ptr no arquivo file
 * Se não der certo, desvia para o label fread_error
 */
#define TRYFREAD(ptr, type, qtde, file) \
    if (fread((ptr), sizeof(type), (qtde), (file)) != ((size_t)qtde)) goto fread_erro

//* =================== *//
//* ===== Funcoes ===== *//
//* =================== *//

/**
 * Retorna se binario eh para arquivo de escrita
 */
bool isEscrita(const char* modes);

//* ================== *//
//* ===== String ===== *//
//* ================== *//

/*
Funcoes para auxiliar na alocacao dinamica de strings
*/

/**
 * Aloca uma string dinamicamente
 * Retorna NULL se nao for possivel alocar
 */
inline static char* string_criar(int tam) { return malloc(tam * sizeof(char)); }

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