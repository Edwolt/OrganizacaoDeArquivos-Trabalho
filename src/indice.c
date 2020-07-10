#include "indice.h"

#define CHAVES_NUM 2

typedef struct Pagina Pagina;

struct Indice {
    FILE* file;
    char* path;
    const char* modes;
    Pagina* raiz;

    // Cabecalho
    char status;
    int noRaiz;
    int nroNiveis;
    int proxRRN;
    int nroChaves;
};

struct Pagina {
    int nivel;
    int n;
    int chaves[CHAVES_NUM];
    int ponteiro[CHAVES_NUM - 1];
};

//* ====================== *//
//* ===== Constantes ===== *//
//* ====================== *//

#define TAM_REG 72
#define TAM_LIXOC 55

static const char LIXO[TAM_REG + 1] = "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$";

//* ============================ *//
//* ===== Métodos Privados ===== *//
//* ============================ *//

/**
 * Abri um arquivo e salva em indice
 * Se já tiver um arquivo aberto fecha ele
 */
inline static void abrirArquivo(Indice* binario) {
    if (binario->file) fclose(binario->file);
    binario->file = fopen(binario->path, binario->modes);
}

//* ============================ *//
//* ===== Métodos Publicos ===== *//
//* ============================ *//

Indice* indice_criar(char* path) {
    Indice* indice = malloc(sizeof(Indice));
    if (!indice) return NULL;

    indice->file = NULL;
    indice->path = path;
    indice->modes = "wb";

    abrirArquivo(indice);
    if (!indice->file) {
        free(indice);
        return NULL;
    }

    indice->status = '0';
    indice->noRaiz = 0;
    indice->nroNiveis = 0;
    indice->proxRRN = 0;
    indice->nroChaves = 0;

    return NULL;
}