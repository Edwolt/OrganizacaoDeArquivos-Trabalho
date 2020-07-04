#ifndef OPCOES_H
#define OPCOES_H

#include <stdbool.h>
#include <stdio.h>
#include "binario.h"
#include "binarioNaTela.h"
#include "criterio.h"
#include "csv.h"
#include "registro.h"

/**
 * Executa a instrucao de numero n
 * Considera que so foi lido o codigo da operacao
 * e que em seguida estao os dados para execucao
 * 
 * Retorna se foi possivel executar essa opcao
 */
bool opcoes_executar(int n);

#endif