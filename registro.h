#ifndef REGISTRO_H
#define REGISTRO_H

typedef struct _registro Registro;

Registro* registro_criar();

void registro_apagar(Registro* registro);

#endif