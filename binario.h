#ifndef ARQUIVO_H
#define ARQUIVO_H

#include <stdbool.h>

#include "registro.h"

/**
 * TAD Arquivo binario
 * Por ser um simplesmente um FILE, não tem motivo para cirar um estrutura
 */
typedef FILE Binario;

/**
 * Cria um novo arquivo com nome path
 * Se o arquivo já existir cria, sobrescreve ele com um arquivo novo
 * Retorna NULL se não for possivel criar o arquivo
 * 
 * Obs: Depois, use uma função para destruir o objeto
 */
Binario* binario_new(char* path);

/**
 * Abre um arquivo com nome path para leitura
 * Retorna NULL se não for possivel Abrir o arquivo
 * 
 * Obs: Depois, use uma função para destruir o objeto
 */
Binario* binario_open(char* path);

/**
 * Destroi o objeto binario
 * (Não apaga o arquivo criado, apenas desaloca memória)
 */
void binario_del(Binario* binario);

/**
 * Insere um registro no arquivo
 */
bool binario_inserir(Binario* binario, Registro* registro);

/**
 * Le um registro do arquivo binário
 * 
 * Obs: Destrua o registro depois de usar
 */
Registro* binario_leRegistro(Binario* binario);

/**
 * Muda o status do arquivo no caminho path
 */
bool binario_setStatus(char* path, bool status);

#endif