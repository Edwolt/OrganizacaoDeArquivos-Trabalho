#include "csv.h"

#include "binarioNaTela.h"

#define STR_TAM 106  // Maior String que pode ser lida

//* ============================ *//
//* ===== Metodos Privados ===== *//
//* ============================ *//

/**
 * 
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
 * 
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
 * 
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
    if (path) {
        CSV* csv = fopen(path, "r");
        fscanf(csv, "%*[^\n] ");  // Le uma linha do arquivo sem salvar o valor
        return csv;
    };
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
    char* cidadeBebe = leStr(csv);
    int idNascimento = leInt(csv);
    int idadeMae = leInt(csv);
    char* dataNascimento = leStr(csv);
    char sexoBebe = leChar(csv);
    char* estadoMae = leStr(csv);
    char* estadoBebe = leStr(csv);

    return registro_new(idNascimento,
                        idadeMae, dataNascimento,
                        sexoBebe,
                        estadoMae, estadoBebe,
                        cidadeMae, cidadeBebe);
}