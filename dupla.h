#ifndef DUPLA_H
#define DUPLA_H

#include <stdlib.h>  // atoi

#define DUPLA_INVALIDO -1
#define DUPLA_IDNASCIMENTO 1246
#define DUPLA_IDADEMAE 778
#define DUPLA_DATANASCIMENTO 1451
#define DUPLA_SEXOBEBE 813
#define DUPLA_ESTADOMAE 915
#define DUPLA_ESTADOBEBE 1006
#define DUPLA_CIDADEMAE 877
#define DUPLA_CIDADEBEBE 968

#define DUPLA_INT 0
#define DUPLA_CHAR 1
#define DUPLA_STRING 2

/**
 * TAD Dupla campo valor
 * Representando a dupla de parametros campo valor
 */
typedef struct _dupla Dupla;

Dupla* dupla_criar(char* campo, char* valor);

void dupla_apagar(Dupla* dupla);
void dupla_destruir(Dupla* dupla);

int dupla_tipo(Dupla* dupla);

#endif