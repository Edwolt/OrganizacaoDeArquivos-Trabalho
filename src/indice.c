#include "indice.h"

#define CHAVES_NUM 2

struct Pagina {
    int nivel;
    int n;
    int chaves[CHAVES_NUM];
    int ponteiro[CHAVES_NUM - 1];
};