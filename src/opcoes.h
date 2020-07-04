#ifndef OPCOES_H
#define OPCOES_H

#include <stdbool.h>
#include <stdio.h>

#include "binario.h"
#include "binarioNaTela.h"
#include "criterio.h"
#include "csv.h"
#include "registro.h"

#define PATH_TAM 100  // Tamanho do nome do arquivo

/**
 * Executa a instrucao de numero n
 * Considera que so foi lido o codigo da operacao
 * e que em seguida estao os dados necessarios para execucao
 * 
 * Retorna se foi possivel ou nao executar essa opcao
 * (Executar a opcao eh diferente de ocorrer um erro durante a execução)
 */
bool opcoes_executar(int n);

#endif