#include "binario.h"

//* ====================== *//
//* ===== Constantes ===== *//
//* ====================== *//

#define REMOVIDO -1  // Valor do campo 1 se o registro estiver logicamente removido

#define TAM_REG 128  // Tamanho do registro
#define TAM_CVAR 97  // Tamanho resevado para campos variaveis (excluido indicadores de tamanho)
#define TAM_ESTADO 2  // Tamanho da string estado
#define TAM_DATA 10  //Tamanho da string data
#define TAM_LIXOC 111  //Tamanho do espaco vazio no final do registro cabecalho

/**
 * Vetor com $ para preencher espacos vazios
 * O tamanho da string eh do tamanho de um registro para ter certeza que sera suficiente
 */
static const char LIXO[TAM_REG + 1] = "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$";

//* ================== *//
//* ===== Macros ===== *//
//* ================== *//
// Para reduzir trechos de código repetitivos

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

Binario* binario_criar(char* path) {
    if (!path) return NULL;  // Nao recebeu parametros

    Binario* binario = fopen(path, "wb");
    if (!binario) return NULL;  // Falha ao abrir arquivo

    // Criando Cabecalho
    char status = '1';
    int rrn = 0;
    int inseridos = 0;
    int removidos = 0;
    int atualizados = 0;

    // Escrevendo o cabecalho no arquivo
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

Binario* binario_abrirLeitura(char* path) {
    if (!path) return NULL;  // Nao recebeu parametros

    Binario* binario = fopen(path, "rb");
    if (!binario) return NULL;  // Falha ao abrir arquivo

    binario_apontar(binario, 1, SEEK_SET);  // Pula o registro cabecalho
    return binario;
}

Binario* binario_abrirEscrita(char* path) {
    if (!path) return NULL;  // Nao recebeu paramentros

    Binario* binario = fopen(path, "rb+");
    if (!binario) return NULL;  // Falha ao abrir arquivo

    binario_apontar(binario, 1, SEEK_SET);  // Pula o registro cabecalho
    return binario;
}

void binario_fechar(Binario** binario) {
    if (!binario || !*binario) return;  // Objeto ja foi apagado (arquivo ja foi fechado)

    // Fecha o arquivo
    fclose(*binario);
    *binario = NULL;
}

bool binario_inserir(Binario* binario, Registro** registros, int n) {
    if (!binario || !registros) return false;  // Objeto nao existe ou nao recebeu parametros

    int i;  // Iteradores

    bool ok;
    for (i = 0; i < n; i++) {
        ok = binario_escreverRegistro(binario, registros[i]);
        if (!ok) return false;
    }

    return true;
}

bool binario_remover(Binario* binario) {
    if (!binario) return false;  // Objeto nao existe

    int removido = REMOVIDO;
    TRYFWRITE(&removido, int, 1, binario);
    binario_apontar(binario, 1, SEEK_CUR);

    return true;

fwrite_error:
    return false;
}

Registro* binario_buscar(Binario* binario, int rrn, bool* erro) {
    binario_apontar(binario, rrn, SEEK_SET);
    return binario_leRegistro(binario, erro);
}

void binario_apontar(Binario* binario, int rrn, int whence) {
    if (whence == SEEK_CUR) {
        int here = ftell(binario);
        here = here % TAM_REG;
        fseek(binario, rrn * TAM_REG - here, SEEK_CUR);
    } else {
        fseek(binario, rrn * TAM_REG, whence);
    }
}

Registro* binario_leRegistro(Binario* binario, bool* erro) {
    if (!binario) {  // Objeto nao existe
        *erro = true;
        return NULL;
    }

    *erro = false;  // Salva erro como false, pois caso nao ocorra nenhum erro deve valer false

    // Le Campos Variaveis
    int tamCidadeMae;
    TRYFREAD(&tamCidadeMae, int, 1, binario);

    if (tamCidadeMae == REMOVIDO) {  // Registro foi removido
        binario_apontar(binario, 1, SEEK_CUR);  // Pula registro
        return NULL;
    }

    int tamCidadeBebe;
    TRYFREAD(&tamCidadeBebe, int, 1, binario);

    char* cidadeMae = NULL;
    if (tamCidadeMae > 0) {
        cidadeMae = malloc((TAM_CVAR + 1) * sizeof(char));
        if (!cidadeMae) {  // Falha ao alocar string
            *erro = true;
            return NULL;
        }
        TRYFREAD(cidadeMae, char, tamCidadeMae, binario);
        cidadeMae[tamCidadeMae] = '\0';  // Poe '\0' no fim da string. Se ela for menor ela nao sera alterada (O arquivo tera o '\0')
    }  // Se tem uma cidade armazenada ela foi lida, senao cidadeMae eh NULL

    char* cidadeBebe = NULL;
    if (tamCidadeBebe > 0) {
        cidadeBebe = malloc((TAM_CVAR + 1) * sizeof(char));
        if (!cidadeBebe) {  // Falha ao alocar string
            // Desaloca o que ja foi alocado
            free(cidadeMae);

            *erro = true;
            return NULL;
        }
        TRYFREAD(cidadeBebe, char, tamCidadeBebe, binario);
        cidadeBebe[tamCidadeBebe] = '\0';  // Poe '\0' no fim da string. Se ela for menor ela nao sera alterada (O arquivo tera o '\0')
    }  // Se tem uma cidade armazenada ela foi lida, senao cidadeBebe eh NULL

    // Pula lixo
    int qtdeLixo = TAM_CVAR - tamCidadeMae - tamCidadeBebe;
    fseek(binario, qtdeLixo, SEEK_CUR);

    // Le campos fixos
    int idNascimento;
    TRYFREAD(&idNascimento, int, 1, binario);

    int idadeMae;
    TRYFREAD(&idadeMae, int, 1, binario);

    char* dataNascimento = malloc((TAM_DATA + 1) * sizeof(char));
    if (!dataNascimento) {  // Falha ao alocar string
        // Desaloca o que ja foi alocado
        free(cidadeMae);
        free(cidadeBebe);

        *erro = true;
        return NULL;
    }
    TRYFREAD(dataNascimento, char, TAM_DATA, binario);
    dataNascimento[TAM_DATA] = '\0';  // Poe '\0' no fim da string. Se ela for menor ela nao sera alterada (O arquivo tera o '\0')
    if (strlen(dataNascimento) == 0) {
        free(dataNascimento);
        dataNascimento = NULL;
    }

    char sexoBebe;
    TRYFREAD(&sexoBebe, char, 1, binario);

    char* estadoMae = malloc((TAM_ESTADO + 1) * sizeof(char));
    if (!estadoMae) {  // Falha ao alocar string
        // Desaloca o que ja foi alocado
        free(cidadeMae);
        free(cidadeBebe);
        free(dataNascimento);

        *erro = true;
        return NULL;
    }
    TRYFREAD(estadoMae, char, TAM_ESTADO, binario);
    estadoMae[TAM_ESTADO] = '\0';  // Poe '\0' no fim da string. Se ela for menor ela nao sera alterada (O arquivo tera o '\0')
    if (strlen(estadoMae) == 0) {
        free(estadoMae);
        estadoMae = NULL;
    }

    char* estadoBebe = malloc((TAM_ESTADO + 1) * sizeof(char));
    if (!estadoBebe) {  // Falha ao alocar string
        // Desaloca o que ja foi alocado
        free(cidadeMae);
        free(cidadeBebe);
        free(dataNascimento);
        free(estadoMae);

        *erro = true;
        return NULL;
    }
    TRYFREAD(estadoBebe, char, TAM_ESTADO, binario);
    estadoBebe[TAM_ESTADO] = '\0';  // Poe '\0' no fim da string. Se ela for menor ela nao sera alterada (O arquivo tera o '\0')
    if (strlen(estadoBebe) == 0) {
        free(estadoBebe);
        estadoBebe = NULL;
    }

    // Cria o objeto registro e retorna
    return registro_criar(idNascimento,
                          idadeMae, dataNascimento,
                          sexoBebe,
                          estadoMae, estadoBebe,
                          cidadeMae, cidadeBebe);

fread_error:  // Tratando erros ao ler do arquivo
    *erro = true;
    return NULL;
}

bool binario_escreverRegistro(Binario* binario, Registro* registro) {
    if (!binario || !registro) return false;  // Objeto nao existe ou nao recebeu os paramentro

    const char CHARNULO = '\0';
    int tamDado;  // Tamanho do dado escrito para calcular quanto lixo escrever
    int qtdeLixo;  // Guardar a qunatidade de espaco vazio para preencher com lixo

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
    // Mesmo que tamCidadee = (cidade != NULL ? strlen(cidade) : 0);
    int tamCidadeMae = (cidadeMae ? strlen(cidadeMae) : 0);
    int tamCidadeBebe = (cidadeBebe ? strlen(cidadeBebe) : 0);

    // Escrevendo campos variaveis
    qtdeLixo = TAM_CVAR;  // Espaco disponivel para campos variaveis
    TRYFWRITE(&tamCidadeMae, int, 1, binario);
    TRYFWRITE(&tamCidadeBebe, int, 1, binario);
    if (cidadeMae) TRYFWRITE(cidadeMae, char, tamCidadeMae, binario);
    if (cidadeBebe) TRYFWRITE(cidadeBebe, char, tamCidadeBebe, binario);

    // Preenche o espaço que sobrou com lixo o restante
    qtdeLixo -= tamCidadeMae + tamCidadeBebe;
    TRYFWRITE(LIXO, char, qtdeLixo, binario);

    // Escrevendo campos fixos
    TRYFWRITE(&idNascimento, int, 1, binario);
    TRYFWRITE(&idadeMae, int, 1, binario);

    qtdeLixo = TAM_DATA;
    if (dataNascimento) {  // dataNascimento != NULL
        tamDado = strlen(dataNascimento);
        TRYFWRITE(dataNascimento, char, tamDado, binario);
        qtdeLixo -= tamDado;  // Descontando o que foi escrito

        if (qtdeLixo > 0) {  // Se sobrou espaço vazio
            // Escreve '\0', e se tiver sobrado escreve lixo
            TRYFWRITE(&CHARNULO, char, 1, binario);
            qtdeLixo--;
            TRYFWRITE(LIXO, char, qtdeLixo, binario);
        }
    } else {  // dataNascimento == NULL
        // Escreve '\0', e se tiver sobrado escreve lixo
        TRYFWRITE(&CHARNULO, char, 1, binario);
        qtdeLixo--;
        TRYFWRITE(LIXO, char, qtdeLixo, binario);
    }

    TRYFWRITE(&sexoBebe, char, 1, binario);

    qtdeLixo = TAM_ESTADO;
    if (estadoMae) {  // estadoMae != NULL
        tamDado = strlen(estadoMae);
        TRYFWRITE(estadoMae, char, tamDado, binario);
        qtdeLixo -= tamDado;  // Descontando o que foi escrito

        if (qtdeLixo > 0) {  // Se sobrou espaco vazio
            // Escreve '\0', e se tiver sobrado escreve lixo
            TRYFWRITE(&CHARNULO, char, 1, binario);
            qtdeLixo--;
            TRYFWRITE(LIXO, char, qtdeLixo, binario);
        }
    } else {  // estadoMae == NULL
        // Escreve '\0', e se tiver sobrado escreve lixo
        TRYFWRITE(&CHARNULO, char, 1, binario);
        qtdeLixo--;
        TRYFWRITE(LIXO, char, qtdeLixo, binario);
    }

    qtdeLixo = TAM_ESTADO;
    if (estadoBebe) {  // estadoBebe != NULL
        tamDado = strlen(estadoBebe);
        TRYFWRITE(estadoBebe, char, tamDado, binario);
        qtdeLixo -= tamDado;  // Descontando o que foi escrito

        if (qtdeLixo > 0) {  // Se sobrou espaco vazio
            // Escreve '\0', e se tiver sobrado escreve lixo
            TRYFWRITE(&CHARNULO, char, 1, binario);
            qtdeLixo--;
            TRYFWRITE(LIXO, char, qtdeLixo, binario);
        }
    } else {  // estadoBebe == NULL
        // Escreve '\0', e se tiver sobrado escreve lixo
        TRYFWRITE(&CHARNULO, char, 1, binario);
        qtdeLixo--;
        TRYFWRITE(LIXO, char, qtdeLixo, binario);
    }

    return true;

fwrite_error:  // Tratando erros ao escrever no arquivo
    return false;
}

//* ============================== *//
//* ===== Registro Cabecalho ===== *//
//* ============================== *//

bool binario_getCabecalho(char* path,
                          bool* status, int* rrn,
                          int* inseridos, int* removidos, int* atualizados) {
    if (!path) return false;  // Nao recebeu caminho

    Binario* binario = fopen(path, "rb");
    if (!binario) return false;  // Falha ao abrir arquivo

    if (status) {  // Deve ler o status
        char aux;
        TRYFREAD(&aux, char, 1, binario);
        *status = (aux == '1');
    } else {
        fseek(binario, sizeof(char), SEEK_CUR);  // Pula o status
    }

    if (rrn) {  // Deve ler o rrn
        TRYFREAD(rrn, int, 1, binario);
    } else {
        fseek(binario, sizeof(int), SEEK_CUR);  // Pula o status
    }

    if (inseridos) {  // Deve ler inseridos
        TRYFREAD(inseridos, int, 1, binario);
    } else {
        fseek(binario, sizeof(int), SEEK_CUR);  // Pula inseridos
    }

    if (removidos) {  // Deve ler removidos
        TRYFREAD(removidos, int, 1, binario);
    } else {
        fseek(binario, sizeof(int), SEEK_CUR);  // Pula o removidos
    }

    if (atualizados) {  // Deve ler atualizados
        TRYFREAD(atualizados, int, 1, binario);
    } else {
        fseek(binario, sizeof(int), SEEK_CUR);  // Pula o atualizdos
    }

    fclose(binario);
    return true;

fread_error:  // Tratando erros ao ler do arquivo
    fclose(binario);
    return false;
}

bool binario_setCabecalho(char* path,
                          bool* status, int* rrn,
                          int* inseridos, int* removidos, int* atualizados) {
    if (!path) return false;  // Nao recebeu path

    Binario* binario = fopen(path, "rb+");
    if (!binario) return false;  // Falha ao abrir arquivo

    if (status) {  // Recebeu status
        char aux = (status ? '1' : '0');
        TRYFWRITE(&aux, char, 1, binario);
    } else {
        fseek(binario, sizeof(char), SEEK_CUR);  // Pula o status
    }

    if (rrn) {  // Recebeu rrn
        TRYFWRITE(rrn, int, 1, binario);
    } else {
        fseek(binario, sizeof(int), SEEK_CUR);  // Pula o rrn
    }

    if (inseridos) {  // Recebeu inseridos
        TRYFWRITE(inseridos, int, 1, binario);
    } else {
        fseek(binario, sizeof(int), SEEK_CUR);  // Pula inseridos
    }

    if (removidos) {  // Recebeu removidos
        TRYFWRITE(removidos, int, 1, binario);
    } else {
        fseek(binario, sizeof(int), SEEK_CUR);  // Pula removidos
    }

    if (atualizados) {  // Recebeu o atualizados
        TRYFWRITE(atualizados, int, 1, binario);
    } else {
        fseek(binario, sizeof(int), SEEK_CUR);  // Pula atualizados
    }

    fclose(binario);
    return true;

fwrite_error:  // Tratando erros ao escrever no arquivo
    fclose(binario);
    return false;
}