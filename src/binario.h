#ifndef BINARIO_H
#define BINARIO_H

#include <stdbool.h>
#include <stdio.h>  // Arquivos
#include <stdlib.h>  // Alocacao Dinamica

#include "csv.h"
#include "registro.h"

/**
 * TAD Arquivo binario
 * Manipula um arquivo binario de registros
 * 
 * Por ser simples, nao tem motivo para cirar uma estrutura
 * por isso usei apenas um typedef
 */
typedef FILE Binario;

/**
 * Cria um novo arquivo binario com nome path
 * Cria nele o registro cabecalho
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
 * Cria um novo arquivo binário com nome path
 * a partir dos dados de um arquivo .csv usando o TAD CSV
 * 
 * Se o arquivo ja existir, sobrescreve ele com um arquivo novo
 * 
 * Retorna se foi possivel gerar
 */
bool binario_gerarDoCSV(char* path, CSV* csv);

/**
 * Abre um arquivo binario com nome path para leitura
 * Obs: Nao eh necessario pular o registro cabecalho
 * 
 * Retorna NULL se nao for possivel Abrir o arquivo
 * Retorna NULL se nao receber um path
 * 
 * Obs: Depois, use uma funcao para destruir o objeto
 */
Binario* binario_abrirLeitura(char* path);

/**
 * Abre um arquivo binario com nome path para escrita
 * O arquivo tambem permite leitura
 * Obs: Nao eh necessario pular o registro cabecalho
 * 
 * Retorna NULL se nao for possivel abrir o arquivo
 * Retorna NULL se nao receber um path
 * 
 * Obs: Depois, use uma funcao para destruir o objeto
 */
Binario* binario_abrirEscrita(char* path);

/**
 * Destroi o objeto
 * 
 * Obs: Não apaga o arquivo, apenas desaloca memoria
 * Obs: Se for um arquivo de escrita, depois da execucao (senao ocorre erros) arquivo estara em disco
 */
void binario_fechar(Binario** binario);

/**
 * Insere n registros no arquivo binario
 * Retorna se a operacao foi possivel
 */
bool binario_inserir(Binario* binario, Registro** registros, int n);

/**
 * Remove o registro para o qual o arquivo esta apontando
 */
bool binario_remover(Binario* binario);

/**
 * Retorna o registro que esta no rrn
 * Retorna NULL se o registro estiver logicamente removido
 * 
 * Salva em erro se ocorreu um erro
 * Retorna NULL se ocorrer um erro
 */
Registro* binario_buscar(Binario* binario, int rrn, bool* erro);

/**
 * Faz o binario passar a apontar para o rrn dado
 * 
 * whence segue a mesma logica que o fseek:
 * * SEEK_SET: Vai para o registro com o rrn passado
 * * SEEK_CUR: Anda rrn registros a partir do registro dado
 * * SEEK_END: Anda rrn registros a partir do fim do arquivo
 */
void binario_apontar(Binario* binario, int rrn, int whence);

/**
 * Le um registro do arquivo binario
 * Retorna o Registro lido
 * Retorna NULL se o registro estiver logicamente removido
 * 
 * Salva em erro se ocorreu um erro
 * Retorna NULL se ocorrer um erro
 * 
 * Obs: Destrua o registro depois de usar
 */
Registro* binario_leRegistro(Binario* binario, bool* erro);

/**
 * Insere um registro no arquivo
 * Obs: Nao verifica se esta no final do arquivo
 * 
 * Retorna se foi possivel inserir o registro no arquivo
 */
bool binario_escreverRegistro(Binario* binario, Registro* registro);

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
bool binario_setCabecalho(char* path,
                          bool* status, int* rrn,
                          int* inseridos, int* removidos, int* atualizados);

/**
 * Pega os valores do cabecalho de um arquivo
 * O valor que for NULL nao sera lido do arquivo
 * 
 * status      = Status
 * rrn         = RRNProxRegistro (Quantidade de registros contando os removidos e descontando o cabecalho)
 * inseridos   = numeroRegistrosInseridos
 * removidos   = numeroRegistrosRemovidos  
 * atualizados = numeroRegistrosAtualizado
 */
bool binario_getCabecalho(char* path,
                          bool* status, int* rrn,
                          int* inseridos, int* removidos, int* atualizados);

#endif