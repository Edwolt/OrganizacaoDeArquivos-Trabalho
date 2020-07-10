#ifndef UTIL_H
#define UTIL_H

#include <stdbool.h>

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
bool ehEscrita(const char* modes);

#endif