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
    Dupla* dupla = (Dupla*)malloc(sizeof(Dupla));
    if (!dupla) return NULL;

    dupla->campo = hash(campo);

    switch (dupla->campo) {
        // Campo Inteiro
        case DUPLA_IDNASCIMENTO:
        case DUPLA_IDADEMAE:
            dupla->valor.inteiro = atoi(valor);
            break;

        // Campo String
        case DUPLA_DATANASCIMENTO:
        case DUPLA_ESTADOMAE:
        case DUPLA_ESTADOBEBE:
        case DUPLA_CIDADEMAE:
        case DUPLA_CIDADEBEBE:
            dupla->valor.string = valor;
            break;

        // Campo Char
        case DUPLA_SEXOBEBE:
            dupla->valor.caracter = valor[0];
            break;

        default:
            dupla->campo = DUPLA_INVALIDO;
    }

    return dupla;
}

dupla_apagar(Dupla* dupla) {
    if (dupla_tipo(dupla) == DUPLA_STRING) free(dupla->valor.string);
    free(dupla);
}

dupla_destruir(Dupla* dupla) {
    free(dupla);
}

dupla_tipo(Dupla* dupla) {
    // TODO Implementar
    return DUPLA_STRING;
}