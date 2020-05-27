#include "dupla.h"

struct _dupla {
    int campo;

    union {
        int inteiro;
        char caracter;
        char* string;
    } valor;
};

//* ============================ */
//* ===== Metodos Privados ===== */
//* ============================ */

/**
 * Função hash para facilitar comparações
 */
static int hash(char* str) {
    int res = 0;
    for (int i = 0; str[i] != '\0'; i++) {
        res += str[i];
    }
    return res;
}

//* ============================ */
//* ===== Metodos Publicos ===== */
//* ============================ */

Dupla* dupla_criar(char* campo, char* valor) {
    // Parametros nao foram passados
    if (!campo) {
        if (valor) free(valor);
        return NULL;
    }

    if (!valor) {
        if (campo) free(campo);
        return NULL;
    }

    Dupla* dupla = (Dupla*)malloc(sizeof(Dupla));
    if (!dupla) return NULL;

    dupla->campo = hash(campo);

    switch (dupla_getTipo(dupla)) {
        // Campo Inteiro
        case DUPLA_INTEIRO:
            dupla->valor.inteiro = atoi(valor);
            free(valor);
            break;

        // Campo Caracter
        case DUPLA_CARACTER:
            dupla->valor.caracter = valor[0];
            free(valor);
            break;

        // Campo String
        case DUPLA_STRING:
            dupla->valor.string = valor;
            break;

        default:
            dupla->campo = DUPLA_INVALIDO;
    }

    return dupla;
}

void dupla_apagar(Dupla** dupla) {
    // Verifica se registro já foi apagado
    if (!dupla || !*dupla) return;

    if (dupla_getTipo(*dupla) == DUPLA_STRING) free((*dupla)->valor.string);
    free(*dupla);
    *dupla = NULL;
}

int dupla_getCampo(Dupla* dupla) {
    if (!dupla) return DUPLA_INVALIDO;

    return dupla->campo;
}

int dupla_getTipo(Dupla* dupla) {
    if (!dupla) return DUPLA_INVALIDO;

    switch (dupla->campo) {
        // Campo Inteiro
        case DUPLA_IDNASCIMENTO:
        case DUPLA_IDADEMAE:
            return DUPLA_INTEIRO;

        // Campo Char
        case DUPLA_SEXOBEBE:
            return DUPLA_CARACTER;

        // Campo String
        case DUPLA_DATANASCIMENTO:
        case DUPLA_ESTADOMAE:
        case DUPLA_ESTADOBEBE:
        case DUPLA_CIDADEMAE:
        case DUPLA_CIDADEBEBE:
            return DUPLA_STRING;

        default:
            return DUPLA_INVALIDO;
    }
}

//* =================== *//
//* ===== Getters ===== *//
//* =================== *//

int dupla_getInteiro(Dupla* dupla) {
    if (!dupla) return 0;
    return dupla->valor.inteiro;
}

char dupla_getCaracter(Dupla* dupla) {
    if (!dupla) return '\0';
    return dupla->valor.caracter;
}

char* dupla_getString(Dupla* dupla) {
    if (!dupla) return NULL;
    return dupla->valor.string;
}