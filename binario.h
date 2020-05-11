#ifndef BINARIO_H
#define BINARIO_H

#include <stdbool.h>
#include <stdio.h>  // Manipular arquivos
#include <stdlib.h>

#include "registro.h"

#define LIXO '$';

/**
 * TAD Arquivo binario
 * Por ser um simplesmente um FILE, nao tem motivo para cirar um estrutura
 */
typedef FILE Binario;

/**
 * Cria um novo arquivo com nome path
 * Se o arquivo ja existir cria, sobrescreve ele com um arquivo novo
 * Retorna NULL se nao for possivel criar o arquivo
 * 
 * Obs: Depois, use uma função para destruir o objeto
 */
Binario* binario_new(char* path);

/**
 * Abre um arquivo com nome path para leitura
 * Retorna NULL se nao for possivel Abrir o arquivo
 * 
 * Obs: Depois, use uma funcao para destruir o objeto
 */
Binario* binario_open(char* path);

/**
 * Destroi o objeto binario
 * (Não apaga o arquivo criado, apenas desaloca memoria)
 */
void binario_del(Binario** binario);

/**
 * Insere um registro no arquivo
 */
bool binario_inserir(Binario* binario, Registro* registro);

/**
 * Le um registro do arquivo binario
 * 
 * Obs: Destrua o registro depois de usar
 */
Registro* binario_leRegistro(Binario* binario);

/**
 * Atuliza o cabeçalho do arquivo
 * O valor que for NULL não será atualizado
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

#endif