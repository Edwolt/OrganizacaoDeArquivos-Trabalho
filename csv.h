#ifndef CSV_H
#define CSV_H

#include <stdio.h>  // Manipular Arquivos
#include <stdlib.h> // Alocacao Dinamica
#include <string.h>

#include "binarioNaTela.h" // Funcao trim()
#include "registro.h"

/**
 * TAD Arquivo CSV
 * Manipula um arquivo .csv com os dados do registro
 * Use para extrair as informações do arquivo CSV
 * 
 * Por ser simples, nao tem motivo para cirar uma estrutura
 * por isso usei apenas um typedef
 */
typedef FILE CSV;

/**
 * Abre um arquivo csv com os dados do registro com nome path para leitura
 * Obs: Nao eh necessario pula a primeira linha que contem os nomes das colunas
 * 
 * Retorna NULL se nao for possivel abrir o arquivo
 * Retorna NULL se nao receber um path
 * 
 * Obs: Depois, use uma função para destruir o objeto
 */
CSV* csv_abrir(char* path);

/**
 * Destroi o objeto
 * Obs: Não apaga o arquivo, apenas desaloca memoria
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