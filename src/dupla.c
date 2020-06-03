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
 * Funcao hash para facilitar comparacoes de string
 * Somatorio do valor dos caracter
 */
static int hash(char* str) {
    if (!str) return 0;  // Nao recebeu paramentros

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
    if (!campo) {  // Verifica se os parametros foram passados
        if (valor) string_apagar(&valor);
        return NULL;
    }

    Dupla* dupla = malloc(sizeof(Dupla));
    if (!dupla) return NULL;  // Falha ao criar dupla

    dupla->campo = hash(campo);
    string_apagar(&campo);

    switch (dupla_getTipo(dupla)) {
        // Campo Inteiro
        case DUPLA_INTEIRO:
            if (valor) {
                dupla->valor.inteiro = atoi(valor);
                string_apagar(&valor);
            } else {
                dupla->valor.inteiro = INTNULL;
            }
            break;

        // Campo Caracter
        case DUPLA_CARACTER:
            if (valor) {
                dupla->valor.caracter = valor[0];
                string_apagar(&valor);
            } else {
                dupla->valor.caracter = '\0';
            }
            break;

        // Campo String
        case DUPLA_STRING:
            dupla->valor.string = valor;
            string_apagarSeVazio(&dupla->valor.string);
            break;

        default:
            dupla->campo = DUPLA_INVALIDO;
    }

    return dupla;
}

void dupla_apagar(Dupla** dupla) {
    if (!dupla || !*dupla) return;  // Objeto ja foi apagado

    if (dupla_getTipo(*dupla) == DUPLA_STRING) string_apagar(&(*dupla)->valor.string);
    free(*dupla);
    *dupla = NULL;
}

int dupla_getCampo(Dupla* dupla) {
    if (!dupla) return DUPLA_INVALIDO;  // Objeto nao existe

    return dupla->campo;
}

int dupla_getTipo(Dupla* dupla) {
    if (!dupla) return DUPLA_INVALIDO;  // Objeto nao existe

    switch (dupla->campo) {
        // Campos Inteiros
        case DUPLA_IDNASCIMENTO:
        case DUPLA_IDADEMAE:
            return DUPLA_INTEIRO;

        // Campos Caracter
        case DUPLA_SEXOBEBE:
            return DUPLA_CARACTER;

        // Campos String
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