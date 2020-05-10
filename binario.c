#include "binario.h"

Binario* binario_new(char* path) {
    if (!path) return NULL;
    Binario* binario = fopen(path, "wb");

    char status = '1';
    int rrn = 0;
    int inseridos = 0;
    int removidos = 0;
    int atualizados = 0;
    char lixo[111];
    for (int i = 0; i < 111; i++) lixo[i] = LIXO;

    // TODO sumir com o goto
    if (fwrite(&status, sizeof(char), 1, binario) != 1) goto fwrite_erro;      // Status
    if (fwrite(&rrn, sizeof(int), 1, binario) != 1) goto fwrite_erro;          // RRNProxRegistro
    if (fwrite(&inseridos, sizeof(int), 1, binario) != 1) goto fwrite_erro;    // numeroRegistrosInseridos
    if (fwrite(&removidos, sizeof(int), 1, binario) != 1) goto fwrite_erro;    // numeroRegistrosRemovidos
    if (fwrite(&atualizados, sizeof(int), 1, binario) != 1) goto fwrite_erro;  // numeroRegistrosAtualizado
    if (fwrite(lixo, sizeof(char), 111, binario) != 111) goto fwrite_erro;     // lixo (caracter '$')

    return binario;

fwrite_erro:
    printf("Falha no carregamento do arquivo.");
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
