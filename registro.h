#ifndef REGISTRO_H
#define REGISTRO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INTNULL -1
#define STR_TAM 97  // Tamanho maximo de um campo string


/**
 * TAD Registro
 * Armazena dados de um registro
 * 
 * Valores para campos nulos:
 * - Strings: NULL
 * - Inteiros: INTNULL
 * - Chars: '\0'
 */
typedef struct _registro Registro;

/**
 * Cria um novo registro
 * Retorna NULL se nao for possivel criar o objeto
 * 
 * Obs: Depois, use uma funcao para destruir o objeto
 */
Registro* registro_criar(int idNascimento,
                         int idadeMae, char* dataNascimento,
                         char sexoBebe,
                         char* estadoMae, char* estadoBebe,
                         char* cidadeMae, char* cidadeBebe);

/**
 * Cria um novo registro sem nenhum dado
 * Retorna NULL se nao for possivel criar o objeto
 * 
 * Obs: Depois, use uma funcao para destruir o objeto
 */
Registro* registro_criarVazio();

/**
 * Destroi o objeto
 * Destroi os objetos e desaloca variaveis que estiver dentro dele
 */
void registro_apagar(Registro** registro);

/**
 * Escreve no stdin os dados do registro usando as informacoes mais importantes
 */
void registro_imprimir();

//* ============================ *//
//* ===== Getter e Setters ===== *//
//* ============================ *//

int registro_getIdNascimento(Registro* registro);
void registro_setIdNascimento(Registro* registro, int idNascimento);

int registro_getIdadeMae(Registro* registro);
void registro_setIdadeMae(Registro* registro, int idadeMae);

char* registro_getDataNascimento(Registro* registro);
void registro_setDataNascimento(Registro* registro, char* dataNascimento);

char registro_getSexoBebe(Registro* registro);
void registro_setSexoBebe(Registro* registro, char sexoBebe);

char* registro_getEstadoMae(Registro* registro);
void registro_setEstadoMae(Registro* registro, char* estadoMae);

char* registro_getEstadoBebe(Registro* registro);
void registro_setEstadoBebe(Registro* registro, char* estadoBebe);

char* registro_getCidadeMae(Registro* registro);
void registro_setCidadeMae(Registro* registro, char* cidadeMae);

char* registro_getCidadeBebe(Registro* registro);
void registro_setCidadeBebe(Registro* registro, char* cidadeBebe);

#endif