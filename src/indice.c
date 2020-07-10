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

static bool cabecalhoLeitura(Indice* indice) {
    if (!indice) return false;  // Nao recebeu caminho

    if (indice->file) fclose(indice->file);
    indice->file = NULL;

    indice->file = fopen(indice->path, "rb");
    if (!indice->file) return false;

    // Le cabecalho
    TRYFREAD(&indice->status, char, 1, indice->file);
    if (indice->status == '0') {  // Arquivo inconsistente
        fclose(indice->file);
        indice->file = NULL;
        return false;
    }

    TRYFREAD(&indice->noRaiz, int, 1, indice->file);
    TRYFREAD(&indice->nroNiveis, int, 1, indice->file);
    TRYFREAD(&indice->proxRRN, int, 1, indice->file);
    TRYFREAD(&indice->nroChaves, int, 1, indice->file);

    // Fecha arquivo
    fclose(indice->file);
    indice->file = NULL;
    return true;

fread_erro:  // Falha ao ler do arquivo
    fclose(indice->file);
    indice->file = NULL;
    return false;
}

static bool cabecalhoEscrita(Indice* indice) {
    if (!indice) return false;  // Nao recebeu caminho

    if (indice->file) fclose(indice->file);
    indice->file = NULL;

    indice->file = fopen(indice->path, "rb+");
    if (!indice->file) return false;

    // Le cabecalho
    TRYFREAD(&indice->status, char, 1, indice->file);
    if (indice->status == '0') {  // Arquivo inconsistente
        fclose(indice->file);
        indice->file = NULL;
        return false;
    }

    TRYFREAD(&indice->noRaiz, int, 1, indice->file);
    TRYFREAD(&indice->nroNiveis, int, 1, indice->file);
    TRYFREAD(&indice->proxRRN, int, 1, indice->file);
    TRYFREAD(&indice->nroChaves, int, 1, indice->file);

    // Marca arquivo como inconsistente
    fseek(indice->file, 0, SEEK_SET);
    indice->status = '0';
    TRYFWRITE(&indice->status, char, 1, indice->file);

    // Fecha arquivo
    fclose(indice->file);
    indice->file = NULL;
    return true;

fwrite_erro:  // Falha ao escrever no arquivo
fread_erro:  // Falha ao ler do arquivo
    fclose(indice->file);
    indice->file = NULL;
    return false;
}

static bool salvarCabecalho(Indice* indice) {
    if (!indice->file) indice->file = fopen(indice->path, "rb+");
    if (!indice->file) return false;
    fseek(indice->file, 0, SEEK_SET);

    indice->status = '1';
    TRYFWRITE(&indice->status, char, 1, indice->file);
    TRYFWRITE(&indice->noRaiz, int, 1, indice->file);
    TRYFWRITE(&indice->nroNiveis, int, 1, indice->file);
    TRYFWRITE(&indice->proxRRN, int, 1, indice->file);
    TRYFWRITE(&indice->nroChaves, int, 1, indice->file);

    fclose(indice->file);
    indice->file = NULL;
    return true;

fwrite_erro:  // Falha ao escrever no arquivo
    fclose(indice->file);
    return false;
}

//* ============================ *//
//* ===== Métodos Publicos ===== *//
//* ============================ *//

Indice* indice_criar(char* path) {
    if (!path) return NULL;  // Nao recebeu parametros

    Indice* indice = malloc(sizeof(Indice));
    if (!indice) return NULL;

    indice->file = NULL;
    indice->path = path;
    indice->modes = "wb";

    abrirArquivo(indice);
    if (!indice->file) {  // Falha ao abrir arquivo
        free(indice);
        return NULL;
    }

    // Criando Cabecalho
    indice->status = '0';
    indice->noRaiz = 0;
    indice->nroNiveis = 0;
    indice->proxRRN = 0;
    indice->nroChaves = 0;

    // Escrevendo o cabecalho no arquivo
    TRYFWRITE(&indice->status, char, 1, indice->file);
    TRYFWRITE(&indice->noRaiz, int, 1, indice->file);
    TRYFWRITE(&indice->nroNiveis, int, 1, indice->file);
    TRYFWRITE(&indice->proxRRN, int, 1, indice->file);
    TRYFWRITE(&indice->nroChaves, int, 1, indice->file);
    TRYFWRITE(LIXO, char, TAM_LIXOC, indice->file);

    return indice;

fwrite_erro:  // Falha ao escrever no arquivo
    fclose(indice->file);
    return NULL;
}

Indice* indice_abrirLeitura(char* path) {
    if (!path) return NULL;  // Nao recebeu parametros

    Indice* indice = malloc(sizeof(Indice));
    if (!indice) return NULL;

    indice->file = NULL;
    indice->path;
    indice->modes = "rb";

    bool ok = cabecalhoLeitura(indice);
    if (!ok) goto falha;

    abrirArquivo(indice);
    if (!indice->file) goto falha;

    indice_apontar(indice, 0, SEEK_SET);
    return indice;

falha:  // Falha na execucao da funcao
    if (indice->file) fclose(indice->file);
    free(indice);
    return NULL;
}

Indice* indice_abrirEscrita(char* path) {
    if (!path) return NULL;

    Indice* indice = malloc(sizeof(Indice));
    if (!indice) return NULL;

    indice->file = NULL;
    indice->path;
    indice->modes = "rb+";

    bool ok = cabecalhoEscrita(indice);
    if (!ok) goto falha;

falha:
}

void indice_fechar(Indice** indice) {
    if (!indice || !*indice) return;  // Objeto ja foi apagado (arquivo ja foi fechado)

    // Fecha o arquivo
    if (ehEscrita((*indice)->modes)) {
        salvarCabecalho(*indice);
    } else {
        if ((*indice)->file) fclose((*indice)->file);
    }

    free(*indice);
    *indice = NULL;
}

void indice_apontar(Indice* indice, int rrn, int whence) {
    if (whence == SEEK_SET) {
        fseek(indice->file, (rrn + 1) * TAM_REG, SEEK_SET);
    } else if (whence == SEEK_CUR) {
        int sobra = ftell(indice->file) % TAM_REG;  // Quanto o binario esta apontando a frente do inicio do registro
        fseek(indice->file, rrn * TAM_REG - sobra, SEEK_CUR);
    } else if (whence == SEEK_END) {
        fseek(indice->file, rrn * TAM_REG, SEEK_END);
    }
}