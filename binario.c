#include "binario.h"

/**
 * Vetor com $ para preencher espaços vazios
 * O tamanho da string eh 127 para 
 */
const char LIXO[128] = "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$";

//* ================== *//
//* ===== Macros ===== *//
//* ================== *//

/*
Macros para reduzir trechos de código repetitivo
*/

/**
 * Tenta escrever qtde valores do tipo type a partir ponteiro ptr no arquivo file
 * Se não der certo, desvia para o label fwrite_error
 */
#define TRYFWRITE(ptr, type, qtde, file) \
    if (fwrite((ptr), sizeof(type), (qtde), (file)) != ((size_t)qtde)) goto fwrite_error

/**
 * Tenta ler qtde valores do tipo type para o ponteiro ptr no arquivo file
 * Se não der certo, desvia para o label fread_error
 */
#define TRYFREAD(ptr, type, qtde, file) \
    if (fread((ptr), sizeof(type), (qtde), (file)) != ((size_t)qtde)) goto fread_error

//* ============================ *//
//* ===== Métodos Publicos ===== *//
//* ============================ *//

Binario* binario_new(char* path) {
    if (!path) return NULL;  // Verifica se recebeu o caminho

    Binario* binario = fopen(path, "wb");
    if (!binario) return NULL;  // Verifica se o arquivo foi aberto com sucesso

    // Criando Cabeçalho
    char status = '1';
    int rrn = 0;
    int inseridos = 0;
    int removidos = 0;
    int atualizados = 0;

    // Escrevendo o cabeçalho no arquivo
    TRYFWRITE(&status, char, 1, binario);
    TRYFWRITE(&rrn, int, 1, binario);
    TRYFWRITE(&inseridos, int, 1, binario);
    TRYFWRITE(&removidos, int, 1, binario);
    TRYFWRITE(&atualizados, int, 1, binario);
    TRYFWRITE(LIXO, char, 111, binario);

    return binario;

fwrite_error:  // Tratando erros ao escrever no arquivo
    fclose(binario);
    return NULL;
}

Binario* binario_open(char* path) {
    if (!path) return NULL;  // Verifica se recebeu o caminho

    Binario* binario = fopen(path, "rb");
    if (!binario) return NULL;  // Verifica se o arquivo foi aberto com sucesso

    fseek(binario, 128, SEEK_SET);  // Pula o Registro Cabeçalho
    return binario;
}

void binario_del(Binario** binario) {
    // Verifica se objeto já foi apagado
    if (!binario) return;
    if (!*binario) {
        *binario = NULL;
        return;
    }

    // Fecha o arquivo
    if (binario && *binario) fclose(*binario);
    *binario = NULL;
}

bool binario_inserir(Binario* binario, Registro* registro) {
    if (!binario) return false;

    const char CHARNULO = '\0';  // Guarda '\0'
    int tamDado;  // Variável para guardar o tamanho do dado escrito para calcular quanto lixo escrever
    int qtdeLixo;  // Variável para guardar a qunatidade de espaco vazio para preencher com lixo

    // Extraindo dados do registro
    char* cidadeMae = registro_getCidadeMae(registro);
    char* cidadeBebe = registro_getCidadeBebe(registro);
    int idNascimento = registro_getIdNascimento(registro);
    int idadeMae = registro_getIdadeMae(registro);
    char* dataNascimento = registro_getDataNascimento(registro);
    char sexoBebe = registro_getSexoBebe(registro);
    sexoBebe = (sexoBebe != '\0' ? sexoBebe : '0');  // Se sexoBebe for nulo ele recebe '0'
    char* estadoMae = registro_getEstadoMae(registro);
    char* estadoBebe = registro_getEstadoBebe(registro);

    // Calculando tamanho dos campos variaveis
    // Mesmo que if(cidade != NULL) tamCidade = strlen(cidade) else tamCidade = 0
    int tamCidadeMae = cidadeMae ? strlen(cidadeMae) : 0;
    int tamCidadeBebe = cidadeBebe ? strlen(cidadeBebe) : 0;

    // Escrevendo campos variaveis
    qtdeLixo = 97;  // Espaco disponivel para campos variaveis
    TRYFWRITE(&tamCidadeMae, int, 1, binario);  // Escreve o tamnho da cidadeMae
    TRYFWRITE(&tamCidadeBebe, int, 1, binario);  // Escreve o tamnho da cidadeBebe
    if (cidadeMae) TRYFWRITE(cidadeMae, char, tamCidadeMae, binario);  // Escreve cidadeMae se for diferente de NULL
    if (cidadeBebe) TRYFWRITE(cidadeBebe, char, tamCidadeBebe, binario);  // Escreve cidadeBebe se for diferente de NULL

    qtdeLixo -= tamCidadeMae + tamCidadeBebe;
    TRYFWRITE(LIXO, char, qtdeLixo, binario);  // Preenche com lixo o restante

    // Escrevendo campos inteiros
    TRYFWRITE(&idNascimento, int, 1, binario);
    TRYFWRITE(&idadeMae, int, 1, binario);

    qtdeLixo = 10;  // Tamanho maximo de dataNascimento
    if (dataNascimento) {  // Se dataNascimento não for NULL
        tamDado = strlen(dataNascimento);
        TRYFWRITE(dataNascimento, char, tamDado, binario);
        qtdeLixo -= tamDado;

        if (qtdeLixo > 0) {  // Se sobrou espaço vazio
            TRYFWRITE(&CHARNULO, char, 1, binario);
            qtdeLixo--;  // Desconta caracter '\0'
            TRYFWRITE(LIXO, char, qtdeLixo, binario);  // Escreve lixo
        }
    } else {  // Se dataNascimento for NULL
        TRYFWRITE(&CHARNULO, char, 1, binario);
        qtdeLixo--;  // Desconta caracter '\0'
        TRYFWRITE(LIXO, char, qtdeLixo, binario);  // Escreve lixo
    }

    TRYFWRITE(&sexoBebe, char, 1, binario);

    qtdeLixo = 2;  // Tamanho maximo de estadoMae
    if (estadoMae) {  // Se estadoMae nao for NULL
        tamDado = strlen(estadoMae);
        TRYFWRITE(estadoMae, char, tamDado, binario);
        qtdeLixo -= tamDado;

        if (qtdeLixo > 0) {  // Se sobrou espaco vazio
            TRYFWRITE(&CHARNULO, char, 1, binario);
            qtdeLixo--;  // Desconta caracter '\0'
            TRYFWRITE(LIXO, char, qtdeLixo, binario);
        }
    } else {  // Se estadoMae for NULL
        TRYFWRITE(&CHARNULO, char, 1, binario);
        qtdeLixo--;  // Desconta caracter '\0'
        TRYFWRITE(LIXO, char, qtdeLixo, binario);
    }

    qtdeLixo = 2;  // Tamanho maximo de estadoBebe
    if (estadoBebe) {  // Se estadoBebe nao for NULL
        tamDado = strlen(estadoBebe);
        TRYFWRITE(estadoBebe, char, tamDado, binario);

        if (qtdeLixo > 0) {  // Se sobrou espaco vazio
            TRYFWRITE(&CHARNULO, char, 1, binario);
            qtdeLixo--;  // Desconta caracter '\0'
            TRYFWRITE(LIXO, char, qtdeLixo, binario);
        }
    } else {
        TRYFWRITE(&CHARNULO, char, 1, binario);
        qtdeLixo--;  // Desconta caracter '\0'
        TRYFWRITE(LIXO, char, qtdeLixo, binario);
    }

    return true;

fwrite_error:  // Tratando erros ao escrever no arquivo
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

    char* cidadeMae = NULL;
    if (tamCidadeMae > 0) {
        cidadeMae = (char*)malloc(106 * sizeof(char));
        TRYFREAD(cidadeMae, char, tamCidadeMae, binario);
        cidadeMae[tamCidadeMae] = '\0';
    }

    char* cidadeBebe = NULL;
    if (tamCidadeBebe > 0) {
        cidadeBebe = (char*)malloc(106 * sizeof(char));
        TRYFREAD(cidadeBebe, char, tamCidadeBebe, binario);
        cidadeBebe[tamCidadeBebe] = '\0';
    }

    fseek(binario, qtdeLixo, SEEK_CUR);

    int idNascimento;
    TRYFREAD(&idNascimento, int, 1, binario);

    int idadeMae;
    TRYFREAD(&idadeMae, int, 1, binario);

    char* dataNascimento = (char*)malloc(11 * sizeof(char));
    TRYFREAD(dataNascimento, char, 10, binario);
    if (strlen(dataNascimento) > 0) {
        dataNascimento[10] = '\0';
    } else {
        free(dataNascimento);
        dataNascimento = NULL;
    }

    char sexoBebe;
    TRYFREAD(&sexoBebe, char, 1, binario);

    char* estadoMae = (char*)malloc(3 * sizeof(char));
    TRYFREAD(estadoMae, char, 2, binario);
    estadoMae[2] = '\0';
    if (strlen(estadoMae) == 0) {
        free(estadoMae);
        estadoMae = NULL;
    }

    char* estadoBebe = (char*)malloc(3 * sizeof(char));
    TRYFREAD(estadoBebe, char, 2, binario);
    estadoBebe[2] = '\0';
    if (strlen(estadoBebe) == 0) {
        free(estadoBebe);
        estadoBebe = NULL;
    }

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