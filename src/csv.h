#ifndef CSV_H
#define CSV_H

#include <stdio.h>  // Manipular Arquivos
#include <stdlib.h>  // Alocacao Dinamica

#include "binarioNaTela.h"  // Funcao trim()
#include "registro.h"
#include "string.h"

/**
 * TAD Arquivo CSV
 * Manipula um arquivo .csv com os dados do registro
 * Extrai as informações de arquivo .csv com dados de registros 
 * 
 * Por ser simples, nao tem motivo para cirar uma estrutura
 * por isso eh apenas um typedef
 */
typedef FILE CSV;

/**
 * Retorna o arquivo csv com nome path aberto para leitura
 * Obs: Nao eh necessario pula a primeira linha, que contem os nomes das colunas
 * 
 * Retorna NULL se nao for possivel abrir o arquivo
 * Retorna NULL se nao receber um path
 */
CSV* csv_abrir(char* path);

/**
 * Fecha o arquivo
 */
void csv_fechar(CSV** csv);

/**
 * Le um registro do arquivo csv
 * 
 * Retorna NULL se nao foi possivel ler o registro 
 * pode ser causado pelo fim do arquivo ou por um erro durante a leitura
 * 
 * A ordem dos dados do arquivo csv deve ser a seguinte:
 * cidadeMae, cidadeBebe, idNascimento, idadeMae, dataNascimento, sexoBebe, estadoMae, estadoBebe 
 */
Registro* csv_lerRegistro(CSV* csv);

#endif