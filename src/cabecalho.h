#ifndef CABECALHO_H
#define CABECALHO_H

#include <stdbool.h>
#include <stdlib.h>

#include "registro.h"  // INTNULL

/**
 * TAD Registro Cabecalho
 * Armazena dados de um registro cabecalho
 */
typedef struct Cabecalho Cabecalho;

/**
 * Cria um novo objeto cabecalho
 * Retorna NULL se nao for possivel criar o objeto
 * 
 * Obs: Depois, use uma funcao para destruir o objeto
 */
Cabecalho* cabecalho_criar(bool status, int rrn,
                           int inseridos, int removidos, int atualizados);

/**
 * Cria um novo objeto cabecalho
 * com status consistente e outros campos zerados
 * Retorna NULL se nao for possivel criar o objeto
 * 
 * Obs: Depois, use uma funcao para destruir o objeto
 */
Cabecalho* cabecalho_criarVazio();

/**
 * Destroi o objeto, os objetos que o pertence e desaloca variveis
 */
void cabecalho_apagar(Cabecalho** cabecalho);

/**
 * Retorna dentro dos parametros passados por referencia
 * os valores armazenado no registro
 * 
 * Se o parametro for NULL, aquele campo nao eh retornado
 */
void cabecalho_extrairDados(Cabecalho* cabecalho,
                            bool* status, int* rrn,
                            int* inseridos, int* removidos, int* atualizados);

/**
 * Atualiza o cabecalho considerando que um registro foi inserido:
 * rrn++; inseridos++;
 */
void cabecalho_inserir(Cabecalho* cabecalho);

/**
 * Atualiza o cabecalho considerando que um registro foi removido:
 * removidos++; inseridos--;
 */
void cabecalho_remover(Cabecalho* cebecalho);

/**
 * Atualiza o cabecalho considerando que um registro foi atualizado:
 * atualizados++;
 */
void cabecalho_atualizar(Cabecalho* cabecalho);

//* ============================= *//
//* ===== Getters e Setters ===== *//
//* ============================= *//

// Se cabecalho for NULL, em um getter, retorna uma valor qualquer
// Se cabecalho for NULL, em um setter, nada eh feito

bool cabecalho_getStatus(Cabecalho* cabecalho);
void cabecalho_setStatus(Cabecalho* cabecalho, bool status);

int cabecalho_getRRN(Cabecalho* cabecalho);
void cabecalho_setRRN(Cabecalho* cabecalho, int rrn);

int cabecalho_getInseridos(Cabecalho* cabecalho);
void cabecalho_setInseridos(Cabecalho* cabecalho, int inseridos);

int cabecalho_getRemovidos(Cabecalho* cabecalho);
void cabecalho_setRemovidos(Cabecalho* cabecalho, int removidos);

int cabecalho_getAtualizados(Cabecalho* cabecalho);
void cabecalho_setAtualizados(Cabecalho* cabecalho, int atualizados);

#endif