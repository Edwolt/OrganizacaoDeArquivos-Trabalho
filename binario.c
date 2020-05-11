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
    for (i = 0; i < 111; i++) lixo[i] = LIXO;

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

bool binario_inserir(Binario* binario, Registro* registro) {
    if (!binario) return false;

    int i;  // Iteradores

    // Extraindo dados do registro
    char* cidadeMae = registro_getCidadeMae(registro);
    int tamCidadeMae = strlen(cidadeMae);
    char* cidadeBebe = registro_getCidadeBebe(registro);
    int tamCidadeBebe = strlen(cidadeBebe);
    int idNascimento = registro_getIdNascimento(registro);
    int idadeMae = registro_getIdadeMae(registro);
    char* dataNascimento = registro_getDataNascimento(registro);
    char sexoBebe = registro_getSexoBebe(registro);
    char* estadoMae = registro_getEstadoMae(registro);
    char* estadoBebe = registro_getEstadoBebe(registro);

    // string para preencher com lixo
    // 97 por que eh o tamanho maximo de lixo possivel
    char lixo[97];
    for (i = 0; i < 97; i++) lixo[i] = LIXO;

    // Quantidade de lixo no campo variável
    int qtdeLixo = 97 - tamCidadeBebe - tamCidadeMae;

    if (fwrite(&tamCidadeMae, sizeof(int), 1, binario) != 1) return false;
    if (fwrite(&tamCidadeBebe, sizeof(int), 1, binario) != 1) return false;
    if (fwrite(cidadeMae, sizeof(char), tamCidadeMae, binario) != tamCidadeMae) return false;
    if (fwrite(cidadeBebe, sizeof(char), tamCidadeBebe, binario) != tamCidadeBebe) return false;
    if (fwrite(lixo, sizeof(char), qtdeLixo, binario) != qtdeLixo) return false;
    if (fwrite(&idNascimento, sizeof(int), 1, binario) != 1) return false;
    if (fwrite(&idadeMae, sizeof(int), 1, binario) != 1) return false;
    if (strlen(dataNascimento) != 0) {
        if (fwrite(dataNascimento, sizeof(char), 1, binario) != 1) return false;
    } else {
        if (fwrite(dataNascimento, sizeof(char), 1, binario)) return false;
        if (fwrite(lixo, sizeof(char), 1, binario)) return false;
    }
    if (fwrite(sexoBebe, sizeof(char), 1, binario) != 1) return false;
    // TODO terminar
}

Registro* binario_leRegistro(Binario* binario) {
}

bool binario_atualizaCabecalho(char* path,
                               bool* status, int* rrn,
                               int* inseridos, int* removidos, int* atualizados) {
    if (!path) return false;

    Binario* binario = fopen(path, "rb+");
    if (!binario) return false;

    if (status) {
        char aux = (status ? '1' : '0');
        if (fwrite(&aux, sizeof(aux), 1, binario) != 1) goto fwrite_erro;
    } else {
        fseek(binario, sizeof(char), SEEK_CUR);
    }

    if (rrn) {
        if (fwrite(rrn, sizeof(int), 1, binario) != 1) goto fwrite_erro;
    } else {
        fseek(binario, sizeof(int), SEEK_CUR);
    }

    if (inseridos) {
        if (fwrite(inseridos, sizeof(int), 1, binario) != 1) goto fwrite_erro;
    } else {
        fseek(binario, sizeof(int), SEEK_CUR);
    }

    if (removidos) {
        if (fwrite(removidos, sizeof(int), 1, binario) != 1) goto fwrite_erro;
    } else {
        fseek(binario, sizeof(int), SEEK_CUR);
    }

    if (atualizados) {
        if (fwrite(atualizados, sizeof(int), 1, binario) != 1) goto fwrite_erro;
    } else {
        fseek(binario, sizeof(int), SEEK_CUR);
    }

    fclose(binario);
    return true;

    // TODO sumir com o goto
fwrite_erro:
    fclose(binario);
    return false;
}