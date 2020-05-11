#include "csv.h"

#include "binarioNaTela.h"

#define STR_TAM 106  // Maior String que pode ser lida

//* ============================ *//
//* ===== Metodos Privados ===== *//
//* ============================ *//

void leCampo(CSV* csv, char* str) {
    char c;
    int i = 0;

    fscanf(csv, "%c", &c);
    while (!feof(csv) && c != ',' && c != '\n') {
        str[i++] = c;
        fscanf(csv, "%c", &c);
    }
    str[i] = '\0';
}

/**
 * Le um inteiro do arquivo e consome a virgula
 */
int leInt(CSV* csv) {
    if (!csv) return INTNULL;

    char str[STR_TAM];

    leCampo(csv, str);
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

    leCampo(csv, str);
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

    leCampo(csv, str);
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
    if (!csv) return NULL;  // Testa se o arquivo foi aberto com sucesso

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
    if (feof(csv)) return NULL;

    char* cidadeBebe = leStr(csv);
    if (feof(csv)) {
        free(cidadeMae);
        return NULL;
    }

    int idNascimento = leInt(csv);
    if (feof(csv)) {
        free(cidadeMae);
        free(cidadeBebe);
        return NULL;
    }

    int idadeMae = leInt(csv);
    if (feof(csv)) {
        free(cidadeMae);
        free(cidadeBebe);
        return NULL;
    }

    char* dataNascimento = leStr(csv);
    if (feof(csv)) {
        free(cidadeMae);
        free(cidadeBebe);
        return NULL;
    }

    char sexoBebe = leChar(csv);
    if (feof(csv)) {
        free(cidadeMae);
        free(cidadeBebe);
        free(dataNascimento);
        return NULL;
    }

    char* estadoMae = leStr(csv);
    if (feof(csv)) {
        free(cidadeMae);
        free(cidadeBebe);
        free(dataNascimento);
        return NULL;
    }

    char* estadoBebe = leStr(csv);
    if (feof(csv)) {
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