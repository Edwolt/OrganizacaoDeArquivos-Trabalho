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
 * Le um inteiro do arquivo e consome a virgula
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
 * 
 * Retorna NULL se nao tiver um string para o campo no CSV
 */
static char* leStr(CSV* csv) {
    if (!csv) return NULL;  // Objeto nao exite

    char* str = string_criar(STR_TAM);
    if (!str) return NULL;  // Falha ao alocar string

    leCampo(csv, str);

    string_apagarSeVazio(&str);

    return str;
}

/**
 * Le um char do arquivo e consome a virgula
 */
static char leChar(CSV* csv) {
    if (!csv) return '\0';  // Objeto nao existe

    char str[STR_TAM];

    leCampo(csv, str);

    return (strlen(str) == 1 ? str[0] : '\0');
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

    // Variaveis com alocacao dincamica
    char* cidadeMae = NULL;
    char* cidadeBebe = NULL;
    char* dataNascimento = NULL;
    char* estadoMae = NULL;
    char* estadoBebe = NULL;

    // Leituras
    cidadeMae = leStr(csv);
    if (feof(csv)) goto falha;  // Fim do arquivo ou falha ao ler string

    cidadeBebe = leStr(csv);
    if (feof(csv)) goto falha;  // Fim do arquivo ou falha ao ler string

    int idNascimento = leInt(csv);
    if (feof(csv)) goto falha;  // Fim do arquivo ou falha ao ler string

    int idadeMae = leInt(csv);
    if (feof(csv)) goto falha;  // Fim do arquivo ou falha ao ler string

    dataNascimento = leStr(csv);
    if (feof(csv)) goto falha;  // Fim do arquivo ou falha ao ler string

    char sexoBebe = leChar(csv);
    if (feof(csv)) goto falha;  // Fim do arquivo ou falha ao ler string

    estadoMae = leStr(csv);
    if (feof(csv)) goto falha;  // Fim do arquivo ou falha ao ler string

    estadoBebe = leStr(csv);
    if (feof(csv)) goto falha;  // Fim do arquivo ou falha ao ler string

    return registro_criar(idNascimento,
                          idadeMae, dataNascimento,
                          sexoBebe,
                          estadoMae, estadoBebe,
                          cidadeMae, cidadeBebe);

falha:  // Falha na execucao da funcao
    string_apagar(&cidadeMae);
    string_apagar(&cidadeBebe);
    string_apagar(&dataNascimento);
    string_apagar(&estadoMae);
    string_apagar(&estadoBebe);
    return NULL;
}