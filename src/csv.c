#include "csv.h"

//* ============================ *//
//* ===== Metodos Privados ===== *//
//* ============================ *//

/**
 * Le um campo do CSV (ate uma ',' ou um '\n')
 * Consome a ',' ou '\n'
 */
static void leCampo(CSV* csv, char* str) {
    if (!csv) return;  // Objeto nao existe

    char c;
    int i = 0;

    fscanf(csv, "%c", &c);
    while (!feof(csv) && c != ',' && c != '\n') {
        str[i++] = c;
        fscanf(csv, "%c", &c);
    }
    str[i] = '\0';
    trim(str);
}

/**
 * Le um inteiro do arquivo
 */
static int leInt(CSV* csv) {
    if (!csv) return INTNULL;  // Objeto nao existe

    char str[STR_TAM];

    leCampo(csv, str);

    if (strlen(str) == 0) return INTNULL;

    return atoi(str);
}

/**
 * Le uma string do arquivo e consome a virgula
 * String retornada foi alocada dinamicamente
 */
static char* leStr(CSV* csv) {
    if (!csv) return NULL;  // Objeto nao exite

    char* str = malloc(STR_TAM * sizeof(char));
    if (!str) return NULL;  // Falha ao alocar string

    leCampo(csv, str);

    if (strlen(str) == 0) {
        free(str);
        return NULL;
    }

    return str;
}

/**
 * Le um char do arquivo e consome a virgula
 */
static char leChar(CSV* csv) {
    if (!csv) return '\0';  // Objeto nao existe

    char str[STR_TAM];

    leCampo(csv, str);

    if (strlen(str) != 1) return '\0';

    return str[0];
}

//* ============================ *//
//* ===== Metodos Publicos ===== *//
//* ============================ *//

CSV* csv_abrir(char* path) {
    if (!path) return NULL;  // Objeto nao existe

    CSV* csv = fopen(path, "r");
    if (!csv) return NULL;  // Testa se o arquivo foi aberto com sucesso

    fscanf(csv, "%*[^\n] ");  // Pula a primeira linha do arquivo (e os whitespace subsequente)
    return csv;
}

void csv_fechar(CSV** csv) {
    if (!csv || !*csv) return;  // Objeto ja foi apagado (arquivo ja foi fechado)

    fclose(*csv);
    *csv = NULL;
}

Registro* csv_lerRegistro(CSV* csv) {
    if (!csv) return NULL;  // Objeto nao existe

    char* cidadeMae = leStr(csv);
    if (feof(csv)) return NULL;  // Fim do arquivo ou falha ao ler string

    char* cidadeBebe = leStr(csv);
    if (feof(csv)) {  // Fim do arquivo ou falha ao ler string
        // Desaloca o que ja foi alocado
        free(cidadeMae);
        return NULL;
    }

    int idNascimento = leInt(csv);
    if (feof(csv)) {
        // Desaloca o que ja foi alocado
        free(cidadeMae);
        free(cidadeBebe);
        return NULL;
    }

    int idadeMae = leInt(csv);
    if (feof(csv)) {
        // Desaloca o que ja foi alocado
        free(cidadeMae);
        free(cidadeBebe);
        return NULL;
    }

    char* dataNascimento = leStr(csv);
    if (feof(csv)) {  // Fim do arquivo ou falha ao ler string
        // Desaloca o que ja foi alocado
        free(cidadeMae);
        free(cidadeBebe);
        return NULL;
    }

    char sexoBebe = leChar(csv);
    if (feof(csv)) {
        // Desaloca o que ja foi alocado
        free(cidadeMae);
        free(cidadeBebe);
        free(dataNascimento);
        return NULL;
    }

    char* estadoMae = leStr(csv);
    if (feof(csv)) {  // Fim do arquivo ou falha ao ler string
        // Desaloca o que ja foi alocado
        free(cidadeMae);
        free(cidadeBebe);
        free(dataNascimento);
        return NULL;
    }

    char* estadoBebe = leStr(csv);
    if (feof(csv)) {  // Fim do arquivo ou falha ao ler string
        // Desaloca o que ja foi alocado
        free(cidadeMae);
        free(cidadeBebe);
        free(dataNascimento);
        free(estadoMae);
        return NULL;
    }

    return registro_criar(idNascimento,
                          idadeMae, dataNascimento,
                          sexoBebe,
                          estadoMae, estadoBebe,
                          cidadeMae, cidadeBebe);
}