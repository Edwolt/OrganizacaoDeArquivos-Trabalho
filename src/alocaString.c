#include "alocaString.h"

char* string_criar(int tam) {
    return malloc(tam * sizeof(char));
}

void string_apagar(char** str) {
    if (!str || !*str) return;  // String ja desalocada

    free(*str);
    *str = NULL;
}

void string_apagarSeVazio(char** str) {
    if (!str || !*str) return;  // String ja desalocada

    if (*str[0] != '\0') return;  // String nao vazia

    free(*str);
    *str = NULL;
}