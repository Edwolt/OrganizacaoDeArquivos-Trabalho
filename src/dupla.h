#ifndef DUPLA_H
#define DUPLA_H

#include <stdlib.h>  // atoi

#include "registro.h"  // INTNULL
#include "util.h"

// Valores da hash de nome do campo
#define DUPLA_INVALIDO -1
#define DUPLA_IDNASCIMENTO 1246
#define DUPLA_IDADEMAE 778
#define DUPLA_DATANASCIMENTO 1451
#define DUPLA_SEXOBEBE 813
#define DUPLA_ESTADOMAE 915
#define DUPLA_ESTADOBEBE 1006
#define DUPLA_CIDADEMAE 877
#define DUPLA_CIDADEBEBE 968

// Constantes para saber qual o tipo de dados
// #define DUPLA_INVALIDO -1
#define DUPLA_INTEIRO 0
#define DUPLA_CARACTER 1
#define DUPLA_STRING 2

/**
 * TAD Dupla campo(string) valor
 * Representando a dupla de parametros campo valor
 */
typedef struct Dupla Dupla;

/**
 * Cria um novo objeto dupla a partir de duas strings
 * a string campo e valor devem ter sido alocada dinamicamente
 * depois de criar a dupla, as strings sao desalocadas
 * Obs: Depois de usar os valores da variavel campo e valor ficarao inacessiveis
 * 
 * Retorna NULL se nao for possivel criar o objeto
 * 
 * Obs: Depois, use uma funcao para destruir o objeto
 */
Dupla* dupla_criar(char* campo, char* valor);

/**
 * Destroi o objeto, os objetos que o pertence e desaloca variveis
 */
void dupla_apagar(Dupla** dupla);

/**
 * Destroi o objeto, mas nao os objetos que o pertence, nem desaloca variveis
 * O motivo dessa instrução existir eh poder desalocar a dupla e ainda ter a string utilizavel
 */
void dupla_destruir(Dupla** dupla);

//* =================== *//
//* ===== Getters ===== *//
//* =================== *//

/**
 * Retorna qual o valor do dupla
 * Se dupla for NULL retorna DUPLA_INVALIDO
 */
int dupla_getCampo(Dupla* dupla);

/**
 * Retorn qual tipo de dado esta em valor
 * Se dupla for NULL retorna DUPLA_INVALIDO
 */
int dupla_getTipo(Dupla* dupla);

/**
 * Retorna o valor inteiro que esta armazenado
 * Se nao estiver armazenado um valor inteiro retorna um valor qualquer
 */
int dupla_getInteiro(Dupla* dupla);

/**
 * Retorna o valor caracter que esta armazenado
 * Se nao estiver armazenado um valor caracter retorna um valor qualquer
 */
char dupla_getCaracter(Dupla* dupla);

/**
 * Retorna o valor string que esta armazenado
 * Se nao estiver armazenado um valor string retorna um valor qualquer
 */
char* dupla_getString(Dupla* dupla);

#endif