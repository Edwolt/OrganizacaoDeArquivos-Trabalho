#ifndef ARQUIVO_H
#define ARQUIVO_H

#include <stdbool.h>

#include "registro.h"

typedef struct _arquivo Arquivo;

Arquivo* arquivo_new(char* path);
bool arquivo_setStatus(Arquivo* arquivo, bool status);
void arquivo_salvar(Arquivo* arquivo);
arquivo_inserir(Registro* registro);
arquivo_criar(Arquivo* arquivos);
#endif