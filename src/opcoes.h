#ifndef OPCOES_H
#define OPCOES_H

#include <stdbool.h>
#include <stdio.h>

#include "binario.h"
#include "binarioNaTela.h"
#include "criterio.h"
#include "csv.h"
#include "indice.h"
#include "registro.h"

#define PATH_TAM 100  // Tamanho do nome do arquivo

/**
 * Executa a instrucao de numero n
 * Considera que so foi lido o codigo da operacao
 * e que em seguida estao os dados necessarios para execucao
 * 
 * Retorna false se n for numero invalido de opcao
 */
bool opcoes_executar(int n);

#endif