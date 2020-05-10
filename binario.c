#include "binario.h"

Binario* binario_new(char* path) {
    if (!path) return NULL;
    return fopen(path, "wb");
}

Binario* binario_open(char* path) {
    if (path) return NULL;
    return fopen(path, "rb");
}

void binario_del(Binario** binario) {
    // Verifica se objeto já foi apagado
    if (!binario) return;
    if (!*binario) {
        *binario = NULL;
        return;
    }

    if (binario && *binario) fclose(*binario);
    *binario = NULL;
}

bool binario_inserir(Binario* binario, Registro* registro);
Registro* binario_leRegistro(Binario* binario);
bool binario_setStatus(char* path, bool status);
