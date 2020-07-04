#ifndef REGISTRO_H
#define REGISTRO_H

#include <stdbool.h>
#include <stdio.h>  // scanf
#include <stdlib.h>
#include <string.h>

#include "alocaString.h"
#include "binarioNaTela.h"
#include "dupla.h"

#define INTNULL -1
#define STR_TAM 97  // Tamanho maximo de um campo string

/**
 * TAD Registro de Dados
 * Armazena dados de um registro de dados
 * 
 * Valores para campos nulos:
 * * Strings: NULL
 * * Inteiros: INTNULL
 * * Chars: '\0'
 */
typedef struct Registro Registro;

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
 * Obs: Depois, use uma funcao para destruir o objeto
 */
Registro* registro_criarVazio();

/**
 * Cria um novo registro pegando dados do stdin
 * Retorna NULL se nao for possivel criar o objeto
 * 
 * cidadeMae[i] cidadeBebe[i] idNascimento[i] idadeMae[i] dataNascimento[i] sexoBebe[i] estadoMae[i] estadoBebe[i]
 * 
 * Obs: Depois, use uma funcao para destruir o objeto
 */
Registro* registro_criarDoStdin();

/**
 * Destroi o objeto
 * Destroi os objetos e desaloca variaveis que estiver dentro dele
 */
void registro_apagar(Registro** registro);

/**
 * Escreve no stdout os dados do registro usando as informacoes mais importantes
 */
void registro_imprimir(Registro* registro);

/**
 * Retorna detro dos parametros passados por referencia
 * os valores armazenado no registro
 * 
 * Se o parametro for NULL, aquele campo nao eh retornado
 */
void registro_extrairDados(Registro* registro,
                           int* idNascimento,
                           int* idadeMae, char** dataNascimento,
                           char* sexoBebe,
                           char** estadoMae, char** estadoBebe,
                           char** cidadeMae, char** cidadeBebe);

//* ============================= *//
//* ===== Getters e Setters ===== *//
//* ============================= *//

// Se registro for NULL, em um getter, retorna uma valor qualquer
// Se registro for NULL, em um setter, nada eh feito

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