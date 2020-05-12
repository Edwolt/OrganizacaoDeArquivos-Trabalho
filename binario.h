#ifndef BINARIO_H
#define BINARIO_H

#include <stdbool.h>
#include <stdio.h>  // Manipular arquivos
#include <stdlib.h>

#include "csv.h"
#include "registro.h"

// TODO atualizar documentacao

/**
 * TAD Arquivo binario
 * Manipula um arquivo binario de registros
 * 
 * Por ser simples, nao tem motivo para cirar uma estrutura
 * por isso usei apenas um typedef
 */
typedef FILE Binario;

/**
 * Cria um novo arquivo binário de registros com nome path ja com o registro cabecalho
 * Se o arquivo ja existir, sobrescreve ele com um arquivo novo
 * 
 * Retorna o arquivo para escrita
 * Obs: Nao eh necessario pular o registro cabecalho
 * 
 * Retorna NULL se nao for possivel criar o arquivo
 * Retorna NULL se nao receber um path
 * 
 * Obs: Depois, use uma função para destruir o objeto
 */
Binario* binario_criar(char* path);

/**
 * Cria um novo arquivo binário de registros
 * a partir dos dados de um arquivo .csv usando o TAD CSV
 * 
 * Se o arquivo ja existir, sobrescreve ele com um arquivo novo
 * 
 * Retorna o arquivo para escrita
 * Retorna NULL se nao for possivel criar o arquivo
 * Retorna NULL se nao receber um path
 * 
 * Obs: Depois, use uma função para destruir o objeto
 */
Binario* binario_gerarDoCSV(char* path, CSV* csv);

/**
 * Abre um arquivo binario de registros com nome path para leitura
 * Obs: Nao eh necessario pular o registro cabecalho
 * 
 * Retorna NULL se nao for possivel Abrir o arquivo
 * Retorna NULL se nao receber um path
 * 
 * Obs: Depois, use uma funcao para destruir o objeto
 */
Binario* binario_abrirLeitura(char* path);

Binario* binario_abrirEscrita(char* path);

/**
 * Destroi o objeto
 * 
 * Obs: Não apaga o arquivo, apenas desaloca memoria
 * Obs: Se for um arquivo de escrita, depois da execucao (senao ocorre erros) arquivo estara em disco
 */
void binario_fechar(Binario** binario);

/**
 * Insere um registro no arquivo
 * Obs: Nao verifica se esta no final do arquivo
 * 
 * Retorna se foi possivel inserir o registro no arquivo
 */
bool binario_inserir(Binario* binario, Registro* registro);

/**
 * Le um registro do arquivo binario
 * 
 * Salva em erro se ocorreu um erro
 * Retorna NULL se ocorrer um erro
 * 
 * Retorna o Registro lido
 * Retorna NULL se o registro estiver logicamente removido
 * 
 * Obs: Destrua o registro depois de usar
 */
Registro* binario_leRegistro(Binario* binario, bool* erro);

//* ============================== *//
//* ===== Registro Cabecalho ===== *//
//* ============================== *//

/**
 * Atuliza o cabeçalho do arquivo
 * O valor que for NULL nao sera atualizado
 * 
 * status      = Status
 * rrn         = RRNProxRegistro
 * inseridos   = numeroRegistrosInseridos
 * removidos   = numeroRegistrosRemovidos  
 * atualizados = numeroRegistrosAtualizado
 */
bool binario_atualizaCabecalho(char* path,
                               bool* status, int* rrn,
                               int* inseridos, int* removidos, int* atualizados);

/**
 * Pega os valores do cabecalho de um arquivo
 * O valor que for NULL nao sera lido do arquivo
 * 
 * status      = Status
 * rrn         = RRNProxRegistro
 * inseridos   = numeroRegistrosInseridos
 * removidos   = numeroRegistrosRemovidos  
 * atualizados = numeroRegistrosAtualizado
 */
bool binario_getCabecalho(char* path,
                          bool* status, int* rrn,
                          int* inseridos, int* removidos, int* atualizados);

#endif