#include "binario.h"

/**
 * Tenta escrever qtde valores do tipo type a parte de ptr em file
 * Se não der certo, desvia para o label fwrite_error
 */
#define TRYFWRITE(ptr, type, qtde, file) \
    if (fwrite((ptr), sizeof(type), (qtde), (file)) != ((size_t)qtde)) goto fwrite_error

/**
 * Tenta ler qtde valores do tipo type a parte de ptr em file
 * Se não der certo, desvia para o label fread_error
 */
#define TRYFREAD(ptr, type, qtde, file) \
    if (fread((ptr), sizeof(type), (qtde), (file)) != ((size_t)qtde)) goto fread_error

Binario* binario_new(char* path) {
    if (!path) return NULL;

    Binario* binario = fopen(path, "wb");
    if (!binario) return NULL;

    int i;  // Iteradores

    char status = '1';
    int rrn = 0;
    int inseridos = 0;
    int removidos = 0;
    int atualizados = 0;
    char lixo[111];
    for (i = 0; i < 111; i++) lixo[i] = LIXO;

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
    if (!path) return NULL;

    Binario* binario = fopen(path, "rb");
    if (!binario) return NULL;

    fseek(binario, 128, SEEK_SET);  // Passa do Cabeçalho
    return binario;
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

    int tamCidadeMae = cidadeMae ? strlen(cidadeMae) : 0;
    int tamCidadeBebe = cidadeBebe ? strlen(cidadeBebe) : 0;
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
    int qtdeLixo = 97 - tamCidadeMae - tamCidadeBebe;

    TRYFWRITE(&tamCidadeMae, int, 1, binario);
    TRYFWRITE(&tamCidadeBebe, int, 1, binario);
    if (cidadeMae) {
        TRYFWRITE(cidadeMae, char, tamCidadeMae, binario);
    }
    if (cidadeBebe) {
        TRYFWRITE(cidadeBebe, char, tamCidadeBebe, binario);
    }
    TRYFWRITE(lixo, char, qtdeLixo, binario);

    TRYFWRITE(&idNascimento, int, 1, binario);
    TRYFWRITE(&idadeMae, int, 1, binario);

    if (dataNascimento) {
        tamDado = strlen(dataNascimento);
        TRYFWRITE(dataNascimento, char, tamDado, binario);
        if (tamDado < 10) {
            TRYFWRITE(&CHARNULO, char, 1, binario);
            qtdeLixo = 10 - tamDado - 1;
            TRYFWRITE(lixo, char, qtdeLixo, binario);
        }
    } else {
        TRYFWRITE(&CHARNULO, char, 1, binario);
        TRYFWRITE(lixo, char, 9, binario);
    }

    sexoBebe = sexoBebe ? sexoBebe : '0';
    TRYFWRITE(&sexoBebe, char, 1, binario);

    if (estadoMae) {
        tamDado = strlen(estadoMae);
        TRYFWRITE(estadoMae, char, tamDado, binario);
        if (tamDado < 2) {
            TRYFWRITE(&CHARNULO, char, 1, binario);
            qtdeLixo = 2 - tamDado - 1;
            TRYFWRITE(lixo, char, qtdeLixo, binario);
        }
    } else {
        TRYFWRITE(&CHARNULO, char, 1, binario);
        TRYFWRITE(lixo, char, 1, binario);
    }

    if (estadoBebe) {
        tamDado = strlen(estadoBebe);
        TRYFWRITE(estadoBebe, char, tamDado, binario);
        if (tamDado < 2) {
            TRYFWRITE(&CHARNULO, char, 1, binario);
            qtdeLixo = 2 - tamDado - 1;
            TRYFWRITE(lixo, char, qtdeLixo, binario);
        }
    } else {
        TRYFWRITE(&CHARNULO, char, 1, binario);
        TRYFWRITE(lixo, char, 1, binario);
    }

    return true;

fwrite_error:
    return false;
}

Registro* binario_leRegistro(Binario* binario, bool* erro) {
    if (!binario) {
        *erro = true;
        return NULL;
    }

    int tamCidadeMae;
    TRYFREAD(&tamCidadeMae, int, 1, binario);

    int tamCidadeBebe;
    TRYFREAD(&tamCidadeBebe, int, 1, binario);

    int qtdeLixo = 97 - tamCidadeMae - tamCidadeBebe;

    char* cidadeMae = (char*)malloc(106 * sizeof(char));
    TRYFREAD(cidadeMae, char, tamCidadeMae, binario);
    cidadeMae[tamCidadeMae] = '\0';

    char* cidadeBebe = (char*)malloc(106 * sizeof(char));
    TRYFREAD(cidadeBebe, char, tamCidadeBebe, binario);
    cidadeBebe[tamCidadeBebe] = '\0';

    fseek(binario, qtdeLixo, SEEK_CUR);

    int idNascimento;
    TRYFREAD(&idNascimento, int, 1, binario);

    int idadeMae;
    TRYFREAD(&idadeMae, int, 1, binario);

    char* dataNascimento = (char*)malloc(11 * sizeof(char));
    TRYFREAD(dataNascimento, char, 10, binario);
    dataNascimento[10] = '\0';

    char sexoBebe;
    TRYFREAD(&sexoBebe, char, 1, binario);

    char* estadoMae = (char*)malloc(3 * sizeof(char));
    TRYFREAD(estadoMae, char, 2, binario);
    estadoMae[2] = '\0';

    char* estadoBebe = (char*)malloc(3 * sizeof(char));
    TRYFREAD(estadoBebe, char, 2, binario);
    estadoBebe[2] = '\0';

    return registro_new(idNascimento,
                        idadeMae, dataNascimento,
                        sexoBebe,
                        estadoMae, estadoBebe,
                        cidadeMae, cidadeBebe);

fread_error:
    *erro = true;
    return NULL;
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
        TRYFWRITE(&inseridos, int, 1, binario);
    } else {
        fseek(binario, sizeof(int), SEEK_CUR);
    }

    if (removidos) {
        TRYFWRITE(&removidos, int, 1, binario);
    } else {
        fseek(binario, sizeof(int), SEEK_CUR);
    }

    if (atualizados) {
        TRYFWRITE(&atualizados, int, 1, binario);
    } else {
        fseek(binario, sizeof(int), SEEK_CUR);
    }

    fclose(binario);
    return true;

fwrite_error:
    fclose(binario);
    return false;
}

bool binario_getCabecalho(char* path,
                          bool* status, int* rrn,
                          int* inseridos, int* removidos, int* atualizados) {
    if (!path) return false;

    Binario* binario = fopen(path, "rb");
    if (!binario) return false;

    if (status) {
        char aux;
        TRYFREAD(&aux, char, 1, binario);
        *status = (aux == '1');
    } else {
        fseek(binario, sizeof(char), SEEK_CUR);
    }

    if (rrn) {
        TRYFREAD(rrn, int, 1, binario);
    } else {
        fseek(binario, sizeof(int), SEEK_CUR);
    }

    if (inseridos) {
        TRYFREAD(inseridos, int, 1, binario);
    } else {
        fseek(binario, sizeof(int), SEEK_CUR);
    }

    if (removidos) {
        TRYFREAD(removidos, int, 1, binario);
    } else {
        fseek(binario, sizeof(int), SEEK_CUR);
    }

    if (atualizados) {
        TRYFREAD(atualizados, int, 1, binario);
    } else {
        fseek(binario, sizeof(int), SEEK_CUR);
    }

    fclose(binario);
    return true;

fread_error:
    fclose(binario);
    return false;
}