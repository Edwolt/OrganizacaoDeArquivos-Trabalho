#include "util.h"

//* =================== *//
//* ===== Funcoes ===== *//
//* =================== *//

bool ehEscrita(const char* modes) {
    int i;
    for (i = 0; modes[i] != '\0'; i++) {
        if (modes[i] == 'w' ||
            modes[i] == 'a' ||
            modes[i] == '+') {
            return true;
        }
    }
    return false;
}

//* ================== *//
//* ===== String ===== *//
//* ================== *//

void string_apagar(char** str) {
    if (!str || !*str) return;  // String ja desalocada

    free(*str);
    *str = NULL;
}

void string_apagarSeVazio(char** str) {
    if (!str || !*str) return;  // String ja desalocada

    if (strlen(*str) != 0) return;  // String nao vazia

    free(*str);
    *str = NULL;
}