#include "binario.h"

Binario* binario_new(char* path) {
    if (!path) return NULL;
    Binario* binario = fopen(path, "wb");

    char status = '1';
    int rrn = 0;
    int inseridos = 0;
    int removidos = 0;
    int atualizados = 0;
    char lixo = LIXO;

    // TODO tirar goto
    if (fwrite(status, sizeof(status), 1, binario) != 1) goto write_error;  // Status
    if (fwrite(rrn, sizeof(rrn), 1, binario) != 1) goto write_error;  // Status
    if (fwrite(inseridos, sizeof(inseridos), 1, binario) != 1) goto write_error;  // Status
    if (fwrite(removidos, sizeof(removidos), 1, binario) != 1) goto write_error;  // Status
    if (fwrite(atualizados, sizeof(atualizados), 1, binario) != 1) goto write_error;  // Status


    return binario;

write_error:
    if (fwrite(binario, '0') != 1) {
        printf("Falha no carregamento do arquivo.")
    }  // Status
    fclose(binario);
    return NULL;
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

bool binario_setStatus(char* path, bool status) {
}
