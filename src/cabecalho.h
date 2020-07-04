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

Cabecalho* cabecalho_criar(bool status, int rrn,
                           int inseridos, int removidos, int atualizados);

Cabecalho* cabecalho_criarVazio();

void cabecalho_apagar(Cabecalho** cabecalho);

void cabecalho_extrairDados(Cabecalho* cabecalho,
                            bool* status, int* rrn,
                            int* inseridos, int* removidos, int* atualizados);

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