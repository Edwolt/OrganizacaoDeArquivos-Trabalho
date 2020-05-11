#include "csv.h"

#include "binarioNaTela.h"

#define STR_TAM 106  // Maior String que pode ser lida

//* ============================ *//
//* ===== Metodos Privados ===== *//
//* ============================ *//

/**
 * Le um inteiro do arquivo e consome a virgula
 */
int leInt(CSV* csv) {
    if (!csv) return INTNULL;

    char str[STR_TAM];

    fscanf(csv, " %[^,\n]%*c", str);
    trim(str);

    if (strlen(str) == 0) return INTNULL;

    return atoi(str);
}

/**
 * Le uma string do arquivo e consome a virgula
 * usa alocacao dinamica
 */
char* leStr(CSV* csv) {
    if (!csv) return NULL;

    char* str = (char*)malloc(STR_TAM * sizeof(char));

    fscanf(csv, " %[^,\n]%*c", str);
    trim(str);

    if (strlen(str) == 0) {
        free(str);
        return NULL;
    }

    return str;
}

/**
 * Le um char do arquivo e consome a virgula
 */
char leChar(CSV* csv) {
    if (!csv) return '\0';

    char str[STR_TAM];

    fscanf(csv, " %[^,\n]%*c", str);
    trim(str);

    if (strlen(str) != 1) return '\0';

    return str[0];
}

//* ============================ *//
//* ===== Metodos Publicos ===== *//
//* ============================ *//

CSV* csv_open(char* path) {
    if (!path) return NULL;

    CSV* csv = fopen(path, "r");
    if (!csv) return NULL; // Testa se o arquivo foi aberto com sucesso

    fscanf(csv, "%*[^\n] ");  // Le uma linha do arquivo sem salvar o valor
    return csv;
    return NULL;
}

void csv_del(CSV** csv) {
    // Verifica se objeto já foi apagado
    if (!csv) return;
    if (!*csv) {
        *csv = NULL;
        return;
    }

    fclose(*csv);
    *csv = NULL;
}

Registro* csv_lerRegistro(CSV* csv) {
    char* cidadeMae = leStr(csv);
    if (!cidadeMae) return NULL;

    char* cidadeBebe = leStr(csv);
    if (!cidadeBebe) {
        free(cidadeMae);
        return NULL;
    }

    int idNascimento = leInt(csv);
    if (idNascimento == INTNULL) {
        free(cidadeMae);
        free(cidadeBebe);
        return NULL;
    }

    int idadeMae = leInt(csv);
    if (idadeMae == INTNULL) {
        free(cidadeMae);
        free(cidadeBebe);
        return NULL;
    }

    char* dataNascimento = leStr(csv);
    if (!dataNascimento) {
        free(cidadeMae);
        free(cidadeBebe);
        return NULL;
    }

    char sexoBebe = leChar(csv);
    if (sexoBebe == '\0') {
        free(cidadeMae);
        free(cidadeBebe);
        free(dataNascimento);
        return NULL;
    }

    char* estadoMae = leStr(csv);
    if (!estadoMae) {
        free(cidadeMae);
        free(cidadeBebe);
        free(dataNascimento);
        return NULL;
    }

    char* estadoBebe = leStr(csv);
    if (!estadoBebe) {
        free(cidadeMae);
        free(cidadeBebe);
        free(dataNascimento);
        free(estadoMae);
        return NULL;
    }

    return registro_new(idNascimento,
                        idadeMae, dataNascimento,
                        sexoBebe,
                        estadoMae, estadoBebe,
                        cidadeMae, cidadeBebe);
}