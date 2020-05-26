#ifndef UTIL_H
#define UTIL_H

#include <stdlib.h> // atoi

#define HASH_INVALIDO -1
#define HASH_IDNASCIMENTO 1246
#define HASH_IDADEMAE 778
#define HASH_DATANASCIMENTO 1451
#define HASH_SEXOBEBE 813
#define HASH_ESTADOMAE 915
#define HASH_ESTADOBEBE 1006
#define HASH_CIDADEMAE 877
#define HASH_CIDADEBEBE 968

typedef struct _dupla dupla;
struct _dupla {
    int campo;

    union {
        int inteiro;
        char caracter;
        char* string;
    } valor;
};

#endif