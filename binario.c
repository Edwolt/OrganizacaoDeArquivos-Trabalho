#include "binario.h"

/**
 * Tenta escrever qtde valores do tipo type a parte de ptr em file
 * Se não der certo, desvia para o label fwrite_error
 */
#define TRYFWRITE(ptr, type, qtde, file) \
    if (fwrite((ptr), sizeof(type), (qtde), (file)) != ((size_t)qtde)) goto fwrite_error

Binario* binario_new(char* path) {
    if (!path) return NULL;
    Binario* binario = fopen(path, "wb");
    int i;  // Iteradores

    char status = '1';
    int rrn = 0;
    int inseridos = 0;
    int removidos = 0;
    int atualizados = 0;
    char lixo[111];
    for (i = 0; i < 111; i++) lixo[i] = LIXO;

    // TODO sumir com o goto
    TRYFWRITE(&status, char, 1, binario);
    TRYFWRITE(&rrn, int, 1, binario);
    TRYFWRITE(&inseridos, int, 1, binario);
    TRYFWRITE(&removidos, int, 1, binario);
    TRYFWRITE(&atualizados, int, 1, binario);
    TRYFWRITE(lixo, char, 111, binario);
    return binario;

fwrite_error:
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
    char CHARNULO = '\0';
    int tamDado;

    // Extraindo dados do registro
    char* cidadeMae = registro_getCidadeMae(registro);
    char* cidadeBebe = registro_getCidadeBebe(registro);
    int tamCidadeMae = strlen(cidadeMae);
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

    TRYFWRITE(&tamCidadeMae, int, 1, binario);
    TRYFWRITE(&tamCidadeBebe, int, 1, binario);
    TRYFWRITE(cidadeMae, char, tamCidadeMae, binario);
    TRYFWRITE(cidadeBebe, int, tamCidadeBebe, binario);
    TRYFWRITE(lixo, char, qtdeLixo, binario);
    TRYFWRITE(&idNascimento, int, 1, binario);
    TRYFWRITE(&idadeMae, int, 1, binario);

    tamDado = strlen(dataNascimento);
    TRYFWRITE(dataNascimento, char, tamDado, binario);
    if (tamDado < 10) {
        TRYFWRITE(&CHARNULO, char, 1, binario);
        qtdeLixo = 10 - tamDado - 1;
        TRYFWRITE(lixo, char, qtdeLixo, binario);
    }

    TRYFWRITE(&sexoBebe, char, 1, binario);

    tamDado = strlen(estadoMae);
    TRYFWRITE(estadoMae, char, tamDado, binario);
    if (tamDado < 2) {
        TRYFWRITE(&CHARNULO, char, 1, binario);
        qtdeLixo = 2 - tamDado - 1;
        TRYFWRITE(lixo, char, qtdeLixo, binario);
    }

    tamDado = strlen(estadoMae);
    TRYFWRITE(estadoBebe, char, tamDado, binario);
    if (tamDado < 2) {
        TRYFWRITE(&CHARNULO, char, 1, binario);
        qtdeLixo = 2 - tamDado - 1;
        TRYFWRITE(lixo, char, qtdeLixo, binario);
    }

    return true;

fwrite_error:
    return false;
}

Registro* binario_leRegistro(Binario* binario) {
    binario;
    return registro_newVazio();
}

bool binario_atualizaCabecalho(char* path,
                               bool* status, int* rrn,
                               int* inseridos, int* removidos, int* atualizados) {
    if (!path) return false;

    Binario* binario = fopen(path, "rb+");
    if (!binario) return false;

    if (status) {
        char aux = (status ? '1' : '0');
        TRYFWRITE(&aux, char, 1, binario);
    } else {
        fseek(binario, sizeof(char), SEEK_CUR);
    }

    if (rrn) {
        TRYFWRITE(rrn, int, 1, binario);
    } else {
        fseek(binario, sizeof(int), SEEK_CUR);
    }

    if (inseridos) {
        TRYFWRITE(inseridos, int, 1, binario);
    } else {
        fseek(binario, sizeof(int), SEEK_CUR);
    }

    if (removidos) {
        TRYFWRITE(removidos, int, 1, binario);
    } else {
        fseek(binario, sizeof(int), SEEK_CUR);
    }

    if (atualizados) {
        TRYFWRITE(atualizados, int, 1, binario);
    } else {
        fseek(binario, sizeof(int), SEEK_CUR);
    }

    fclose(binario);
    return true;

    // TODO sumir com o goto
fwrite_error:
    fclose(binario);
    return false;
}