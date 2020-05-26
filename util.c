#include "util.h"

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

dupla numCampo(char* campo, char* valor) {
    dupla res;
    res.campo = hash(campo);

    switch (res.campo) {
        // Campo Inteiro
        case HASH_IDNASCIMENTO:
        case HASH_IDADEMAE:
            res.valor.inteiro = atoi(valor);
            break;

        // Campo String
        case HASH_DATANASCIMENTO:
        case HASH_ESTADOMAE:
        case HASH_ESTADOBEBE:
        case HASH_CIDADEMAE:
        case HASH_CIDADEBEBE:
            res.valor.string = valor;
            break;

        // Campo Char
        case HASH_SEXOBEBE:
            res.valor.caracter = valor[0];
            break;

        default:
            res.campo = HASH_INVALIDO;
    }

    return res;
}