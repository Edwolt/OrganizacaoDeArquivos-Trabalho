#ifndef OPCOES_H
#define OPCOES_H

#include <stdbool.h>
#include <stdio.h>

#include "binario.h"
#include "binarioNaTela.h"
#include "csv.h"
#include "dupla.h"
#include "registro.h"
#include "criterio.h"

/**
 * Executa a instrucao de numero n
 * Retorna se foi possivel executar essa opcao
 */
bool opcoes_executar(int n);

#endif