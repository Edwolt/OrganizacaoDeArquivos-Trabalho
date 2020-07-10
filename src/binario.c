#include "binario.h"

struct Binario {
    FILE* file;
    char* path;
    const char* modes;

    // Cabecalho
    char status;  // Status
    int rrnProx;  // RRNProxRegistro
    int inseridos;  // numeroRegistrosInseridos
    int removidos;  // numeroRegistrosRemovidos
    int atualizados;  //numeroRegistrosAtualizado
};

//* ====================== *//
//* ===== Constantes ===== *//
//* ====================== *//

#define REMOVIDO -1  // Valor do campo 1 se o registro estiver logicamente removido

#define TAM_REG 128  // Tamanho do registro
#define TAM_CVAR 97  // Tamanho resevado para campos variaveis, desconsiderando indicadores de tamanho
#define TAM_ESTADO 2  // Tamanho reservado para string estado
#define TAM_DATA 10  // Tamanho reservado para string data
#define TAM_LIXOC 111  // Espaco vazio no final do registro cabecalho

/**
 * Vetor com $ para preencher espacos vazios
 * O tamanho da string eh do tamanho de um registro para ter certeza que sera suficiente
 */
static const char LIXO[TAM_REG + 1] = "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$";

//* ============================ *//
//* ===== Métodos Privados ===== *//
//* ============================ *//

/**
 * Abri um arquivo e salva em binario
 * Se já tiver um arquivo aberto fecha ele
 */
inline static void abrirArquivo(Binario* binario) {
    if (binario->file) fclose(binario->file);
    binario->file = fopen(binario->path, binario->modes);
}

/**
 * Le o cabecalho do arquivo
 * Se já tiver um arquivo aberto fecha ele
 * O valor de file no final será NULL
 * 
 * Retorna false se houver uma falha
 * Retorna false se o arquivo for inconsistente e para a leitura do cabecalho
 */
static bool cabecalhoLeitura(Binario* binario) {
    if (!binario) return false;  // Nao recebeu caminho

    if (binario->file) fclose(binario->file);
    binario->file = NULL;

    binario->file = fopen(binario->path, "rb");
    if (!binario->file) return false;

    TRYFREAD(&binario->status, char, 1, binario->file);
    if (binario->status == '0') {  // Arquivo inconsistente
        fclose(binario->file);
        binario->file = NULL;
        return false;
    }

    TRYFREAD(&binario->rrnProx, int, 1, binario->file);
    TRYFREAD(&binario->inseridos, int, 1, binario->file);
    TRYFREAD(&binario->removidos, int, 1, binario->file);
    TRYFREAD(&binario->atualizados, int, 1, binario->file);
    fclose(binario->file);
    binario->file = NULL;

    return true;

fread_erro:  // Falha ao ler do arquivo
    fclose(binario->file);
    binario->file = NULL;
    return false;
}

/**
 * Le o cabecalho do arquivo e o marca como inconsistente
 * Se já tiver um arquivo aberto fecha ele
 * O valor de file no final será NULL
 * 
 * Retorna false se houver uma falha
 * Retorna false se o arquivo ja estava inconsistente e para a leitura do cabecalho
 */
static bool cabecalhoEscrita(Binario* binario) {
    if (!binario) return false;  // Nao recebeu caminho

    if (binario->file) fclose(binario->file);
    binario->file = NULL;

    binario->file = fopen(binario->path, "rb+");
    if (!binario->file) return false;

    TRYFREAD(&binario->status, char, 1, binario->file);
    if (binario->status == '0') {  // Arquivo inconsistente
        fclose(binario->file);
        binario->file = NULL;
        return false;
    }

    TRYFREAD(&binario->rrnProx, int, 1, binario->file);
    TRYFREAD(&binario->inseridos, int, 1, binario->file);
    TRYFREAD(&binario->removidos, int, 1, binario->file);
    TRYFREAD(&binario->atualizados, int, 1, binario->file);

    fseek(binario->file, 0, SEEK_SET);
    binario->status = '0';
    TRYFWRITE(&binario->status, char, 1, binario->file);

    fclose(binario->file);
    binario->file = NULL;

    return true;

fwrite_erro:  // Falha ao escrever no arquivo
fread_erro:  // Falha ao ler do arquivo
    fclose(binario->file);
    binario->file = NULL;
    return false;
}

/**
 * Escreve as alteracoes do cabecalho
 * Retorna se a operacao foi bem sucedida
 */
static bool salvarCabecalho(Binario* binario) {
    if (!binario->file) binario->file = fopen(binario->path, "rb+");
    if (!binario->file) return false;
    fseek(binario->file, 0, SEEK_SET);

    binario->status = '1';
    TRYFWRITE(&binario->status, char, 1, binario->file);
    TRYFWRITE(&binario->rrnProx, int, 1, binario->file);
    TRYFWRITE(&binario->inseridos, int, 1, binario->file);
    TRYFWRITE(&binario->removidos, int, 1, binario->file);
    TRYFWRITE(&binario->atualizados, int, 1, binario->file);

    fclose(binario->file);
    binario->file = NULL;

    return true;

fwrite_erro:  // Falha ao escrever no arquivo
    fclose(binario->file);
    return false;
}

/**
 * Escreve um string onde o binario aponta
 * e preenche o espaco que faltou com lixo
 *
 * maxTam: tamanho reservado para string
 * Retorna se a operacao foi sucedida
 */
static bool escreveString(Binario* binario, char* str, int maxTam) {
    const char CHARNULO = '\0';
    int qtdeLixo = maxTam;
    int len;

    if (str) {  // str != NULL
        len = strlen(str);
        TRYFWRITE(str, char, len, binario->file);
        qtdeLixo -= len;  // Descontando o que foi escrito

        if (qtdeLixo > 0) {  // Sobrou espaço vazio
            // Escreve '\0', e se tiver sobrado escreve lixo
            TRYFWRITE(&CHARNULO, char, 1, binario->file);
            qtdeLixo--;
            TRYFWRITE(LIXO, char, qtdeLixo, binario->file);
        }
    } else {  // dataNascimento == NULL
        // Escreve '\0', e se tiver sobrado escreve lixo
        TRYFWRITE(&CHARNULO, char, 1, binario->file);
        qtdeLixo--;
        TRYFWRITE(LIXO, char, qtdeLixo, binario->file);
    }
    return true;

fwrite_erro:  // Falha ao escrever no arquivo
    return false;
}

/**
 * Escreve um string onde o binario aponta
 * maxTam: tamanho reservado para string

 * Retorna se a operacao foi sucedida
 */
static bool atualizaString(Binario* binario, char* str, int maxTam, bool* erro) {
    const char CHARNULO = '\0';
    int espaco = maxTam;
    int len;

    *erro = false;
    if (str) {  // str != NULL
        len = strlen(str);
        TRYFWRITE(str, char, len, binario->file);
        espaco -= len;

        if (espaco > 0) {  // Sobrou espaco vazio
            TRYFWRITE(&CHARNULO, char, 1, binario->file);  // Escreve '\0'
            espaco--;
            fseek(binario->file, espaco, SEEK_CUR);
        }
    } else {  // str == NULL
        TRYFWRITE(&CHARNULO, char, 1, binario->file);  // Escreve '\0'
        espaco--;
        fseek(binario->file, espaco, SEEK_CUR);
    }
    return true;

fwrite_erro:  // Falha ao escrever no arquivo
    return false;
}

/**
 * Escreve o registro onde o binario aponta
 */
static bool binario_escreverRegistro(Binario* binario, Registro* registro) {
    if (!binario || !registro) return false;  // Objeto nao existe ou nao recebeu os paramentro

    int qtdeLixo;

    // Extraindo dados do registro
    int idNascimento;
    int idadeMae;
    char* dataNascimento;
    char sexoBebe;
    char *estadoMae, *estadoBebe;
    char *cidadeMae, *cidadeBebe;
    registro_extrairDados(registro,
                          &idNascimento,
                          &idadeMae, &dataNascimento,
                          &sexoBebe,
                          &estadoMae, &estadoBebe,
                          &cidadeMae, &cidadeBebe);

    sexoBebe = (sexoBebe != '\0' ? sexoBebe : '0');  // Se sexoBebe for nulo ele recebe '0'

    // Calculando tamanho dos campos variaveis
    // Mesmo que tamCidadee = (cidade != NULL ? strlen(cidade) : 0);
    int tamCidadeMae = (cidadeMae ? strlen(cidadeMae) : 0);
    int tamCidadeBebe = (cidadeBebe ? strlen(cidadeBebe) : 0);

    // Escrevendo campos variaveis
    qtdeLixo = TAM_CVAR;  // Espaco disponivel para campos variaveis
    TRYFWRITE(&tamCidadeMae, int, 1, binario->file);
    TRYFWRITE(&tamCidadeBebe, int, 1, binario->file);
    if (cidadeMae) TRYFWRITE(cidadeMae, char, tamCidadeMae, binario->file);
    if (cidadeBebe) TRYFWRITE(cidadeBebe, char, tamCidadeBebe, binario->file);

    // Preenche o espaço que sobrou com lixo o restante
    qtdeLixo -= tamCidadeMae + tamCidadeBebe;
    TRYFWRITE(LIXO, char, qtdeLixo, binario->file);

    // Escrevendo campos fixos
    TRYFWRITE(&idNascimento, int, 1, binario->file);
    TRYFWRITE(&idadeMae, int, 1, binario->file);

    bool ok;
    ok = escreveString(binario, dataNascimento, TAM_DATA);
    if (!ok) goto fwrite_erro;  // Falha ao escrever no arquivo

    TRYFWRITE(&sexoBebe, char, 1, binario->file);

    ok = escreveString(binario, estadoMae, TAM_ESTADO);
    if (!ok) goto fwrite_erro;  // Falha ao ecrever no arquivo

    ok = escreveString(binario, estadoBebe, TAM_ESTADO);
    if (!ok) goto fwrite_erro;  // Falha ao escrever no arquivo

    return true;

fwrite_erro:  // Falha ao escrever no arquivo
    return false;
}

static bool binario_atualizarRegistro(Binario* binario, Registro* registro) {
    if (!binario || !registro) return false;  // Objeto nao existe ou nao recebeu os paramentro

    int espaco;
    bool erro;

    // Extraindo dados do registro
    int idNascimento;
    int idadeMae;
    char* dataNascimento;
    char sexoBebe;
    char *estadoMae, *estadoBebe;
    char *cidadeMae, *cidadeBebe;
    registro_extrairDados(registro,
                          &idNascimento,
                          &idadeMae, &dataNascimento,
                          &sexoBebe,
                          &estadoMae, &estadoBebe,
                          &cidadeMae, &cidadeBebe);

    sexoBebe = (sexoBebe != '\0' ? sexoBebe : '0');  // Se sexoBebe for nulo ele recebe '0'

    // Calculando tamanho dos campos variaveis
    // Mesmo que tamCidadee = (cidade != NULL ? strlen(cidade) : 0);
    int tamCidadeMae = (cidadeMae ? strlen(cidadeMae) : 0);
    int tamCidadeBebe = (cidadeBebe ? strlen(cidadeBebe) : 0);

    // Escrevendo campos variaveis
    TRYFWRITE(&tamCidadeMae, int, 1, binario->file);
    TRYFWRITE(&tamCidadeBebe, int, 1, binario->file);
    if (cidadeMae) TRYFWRITE(cidadeMae, char, tamCidadeMae, binario->file);
    if (cidadeBebe) TRYFWRITE(cidadeBebe, char, tamCidadeBebe, binario->file);

    // Pula lixo
    espaco = TAM_CVAR - tamCidadeBebe - tamCidadeMae;
    fseek(binario->file, espaco, SEEK_CUR);

    // Escrevendo campos fixos
    TRYFWRITE(&idNascimento, int, 1, binario->file);
    TRYFWRITE(&idadeMae, int, 1, binario->file);

    atualizaString(binario, dataNascimento, TAM_DATA, &erro);
    if (erro) goto fwrite_erro;  // Falha ao escrever no arquivo

    TRYFWRITE(&sexoBebe, char, 1, binario->file);

    atualizaString(binario, estadoMae, TAM_ESTADO, &erro);
    if (erro) goto fwrite_erro;  // Falha ao escrever no arquivo

    atualizaString(binario, estadoBebe, TAM_ESTADO, &erro);
    if (erro) goto fwrite_erro;  // Falha ao escrever no arquivo

    return true;

fwrite_erro:  // Falha ao escrever no arquivo
    return false;
}

//* ============================ *//
//* ===== Métodos Publicos ===== *//
//* ============================ *//

Binario* binario_criar(char* path) {
    Binario* binario = malloc(sizeof(Binario));
    if (!binario) return NULL;

    binario->file = NULL;
    binario->path = path;
    binario->modes = "wb";

    abrirArquivo(binario);
    if (!binario->file) {  // Falha ao abrir arquivo
        free(binario);
        return NULL;
    }

    // Criando Cabecalho
    binario->status = '0';
    binario->rrnProx = 0;
    binario->inseridos = 0;
    binario->removidos = 0;
    binario->atualizados = 0;

    // Escrevendo o cabecalho no arquivo
    TRYFWRITE(&binario->status, char, 1, binario->file);
    TRYFWRITE(&binario->rrnProx, int, 1, binario->file);
    TRYFWRITE(&binario->inseridos, int, 1, binario->file);
    TRYFWRITE(&binario->removidos, int, 1, binario->file);
    TRYFWRITE(&binario->atualizados, int, 1, binario->file);
    TRYFWRITE(LIXO, char, TAM_LIXOC, binario->file);

    return binario;

fwrite_erro:  // Falha ao escrever no arquivo
    fclose(binario->file);
    return NULL;
}

Binario* binario_abrirLeitura(char* path) {
    if (!path) return NULL;  // Nao recebeu parametros

    Binario* binario = malloc(sizeof(Binario));
    if (!binario) return NULL;

    binario->file = NULL;
    binario->path = path;
    binario->modes = "rb";

    bool ok = cabecalhoLeitura(binario);
    if (!ok) {
        goto falha;
    }

    abrirArquivo(binario);
    if (!binario->file) goto falha;  // Falha ao abrir arquivo

    binario_apontar(binario, 0, SEEK_SET);  // Vai para o primeiro registro do arquivo
    return binario;

falha:  // Falha na execucao da funcao
    if (binario->file) fclose(binario->file);
    free(binario);
    return NULL;
}

Binario* binario_abrirEscrita(char* path) {
    if (!path) return NULL;  // Nao recebeu paramentros

    Binario* binario = malloc(sizeof(Binario));
    if (!binario) return NULL;

    binario->file = NULL;
    binario->path = path;
    binario->modes = "rb+";

    bool ok = cabecalhoEscrita(binario);
    if (!ok) goto falha;

    abrirArquivo(binario);
    if (!binario->file) goto falha;  // Falha ao abrir arquivo

    binario_apontar(binario, 0, SEEK_SET);  // Vai para o primeiro registro do arquivo
    return binario;

falha:  // Falha na execucao da funcao
    if (binario->file) fclose(binario->file);
    free(binario);
    return NULL;
}

void binario_fechar(Binario** binario) {
    if (!binario || !*binario) return;  // Objeto ja foi apagado (arquivo ja foi fechado)

    // Fecha o arquivo
    if (ehEscrita((*binario)->modes)) {
        salvarCabecalho(*binario);
    } else {
        if ((*binario)->file) fclose((*binario)->file);
    }
    free(*binario);
    *binario = NULL;
}

void binario_apontar(Binario* binario, int rrn, int whence) {
    if (whence == SEEK_SET) {
        fseek(binario->file, (rrn + 1) * TAM_REG, SEEK_SET);
    } else if (whence == SEEK_CUR) {
        int sobra = ftell(binario->file) % TAM_REG;  // Quanto o binario esta apontando a frente do inicio do registro
        fseek(binario->file, rrn * TAM_REG - sobra, SEEK_CUR);
    } else if (whence == SEEK_END) {
        fseek(binario->file, rrn * TAM_REG, SEEK_END);
    }
}

Registro* binario_lerRegistro(Binario* binario, bool* erro) {
    if (!binario) {  // Objeto nao existe
        *erro = true;
        return NULL;
    }

    // Variaveis com alocação dinamica
    char* cidadeMae = NULL;
    char* cidadeBebe = NULL;
    char* dataNascimento = NULL;
    char* estadoMae = NULL;
    char* estadoBebe = NULL;

    *erro = false;  // Salva erro como false, pois caso nao ocorra nenhum erro deve valer false

    // Le Campos Variaveis
    int tamCidadeMae;
    TRYFREAD(&tamCidadeMae, int, 1, binario->file);

    if (tamCidadeMae == REMOVIDO) {  // Registro foi removido
        binario_apontar(binario, 1, SEEK_CUR);  // Pula registro
        return NULL;
    }

    int tamCidadeBebe;
    TRYFREAD(&tamCidadeBebe, int, 1, binario->file);

    cidadeMae = NULL;
    if (tamCidadeMae > 0) {
        cidadeMae = string_criar(TAM_CVAR + 1);
        if (!cidadeMae) goto falha;  // Falha ao alocar string

        TRYFREAD(cidadeMae, char, tamCidadeMae, binario->file);
        cidadeMae[tamCidadeMae] = '\0';  // Poe '\0' no fim da string. Se ela for menor ela nao sera alterada (O arquivo tera o '\0')
    }  // Se tem uma cidade armazenada ela foi lida, senao cidadeMae eh NULL

    cidadeBebe = NULL;
    if (tamCidadeBebe > 0) {
        cidadeBebe = string_criar(TAM_CVAR + 1);
        if (!cidadeBebe) goto falha;  // Falha ao alocar string

        TRYFREAD(cidadeBebe, char, tamCidadeBebe, binario->file);
        cidadeBebe[tamCidadeBebe] = '\0';  // Poe '\0' no fim da string. Se ela for menor ela nao sera alterada (O arquivo tera o '\0')
    }  // Se tem uma cidade armazenada ela foi lida, senao cidadeBebe eh NULL

    // Pula lixo
    int qtdeLixo = TAM_CVAR - tamCidadeMae - tamCidadeBebe;
    fseek(binario->file, qtdeLixo, SEEK_CUR);

    // Le campos fixos
    int idNascimento;
    TRYFREAD(&idNascimento, int, 1, binario->file);

    int idadeMae;
    TRYFREAD(&idadeMae, int, 1, binario->file);

    dataNascimento = string_criar(TAM_DATA + 1);
    if (!dataNascimento) goto falha;  // Falha ao alocar string

    TRYFREAD(dataNascimento, char, TAM_DATA, binario->file);
    dataNascimento[TAM_DATA] = '\0';  // Poe '\0' no fim da string. Se ela for menor ela nao sera alterada (O arquivo tera o '\0')
    string_apagarSeVazio(&dataNascimento);

    char sexoBebe;
    TRYFREAD(&sexoBebe, char, 1, binario->file);

    estadoMae = string_criar(TAM_ESTADO + 1);
    if (!estadoMae) goto falha;  // Falha ao alocar string

    TRYFREAD(estadoMae, char, TAM_ESTADO, binario->file);
    estadoMae[TAM_ESTADO] = '\0';  // Poe '\0' no fim da string. Se ela for menor ela nao sera alterada (O arquivo tera o '\0')
    string_apagarSeVazio(&estadoMae);

    estadoBebe = string_criar(TAM_ESTADO + 1);
    if (!estadoBebe) goto falha;  // Falha ao alocar string

    TRYFREAD(estadoBebe, char, TAM_ESTADO, binario->file);
    estadoBebe[TAM_ESTADO] = '\0';  // Poe '\0' no fim da string. Se ela for menor ela nao sera alterada (O arquivo tera o '\0')
    string_apagarSeVazio(&estadoBebe);

    // Cria o objeto registro e retorna
    return registro_criar(idNascimento,
                          idadeMae, dataNascimento,
                          sexoBebe,
                          estadoMae, estadoBebe,
                          cidadeMae, cidadeBebe);

falha:  // Falha na execucao da funcao
fread_erro:  // Erro ao ler do arquivo
    string_apagar(&cidadeMae);
    string_apagar(&cidadeBebe);
    string_apagar(&dataNascimento);
    string_apagar(&estadoMae);
    string_apagar(&estadoBebe);

    *erro = true;
    return NULL;
}

Registro* binario_buscar(Binario* binario, int rrn, bool* erro) {
    binario_apontar(binario, rrn, SEEK_SET);
    return binario_lerRegistro(binario, erro);
}

bool binario_inserir(Binario* binario, Registro* registro) {
    binario_apontar(binario, binario->rrnProx, SEEK_SET);
    bool ok = binario_escreverRegistro(binario, registro);
    if (!ok) return false;

    binario->rrnProx++;
    binario->inseridos++;
    return true;
}

bool binario_inserirVarios(Binario* binario, Registro** registros, int n) {
    if (!binario || !registros) return false;  // Objeto nao existe ou nao recebeu parametros

    int i;
    bool ok;
    for (i = 0; i < n; i++) {
        ok = binario_inserir(binario, registros[i]);
        if (!ok) return false;
    }

    return true;
}

bool binario_atualizar(Binario* binario, Registro* registro) {
    bool ok = binario_atualizarRegistro(binario, registro);
    if (!ok) return false;

    binario->atualizados++;
    return true;
}

bool binario_remover(Binario* binario) {
    if (!binario) return false;  // Objeto nao existe

    const int REM = REMOVIDO;
    TRYFWRITE(&REM, int, 1, binario->file);
    binario_apontar(binario, 1, SEEK_CUR);  // Pula para o proximo registro

    binario->inseridos--;
    binario->removidos++;
    return true;

fwrite_erro:  // Falha ao escrever no arquivo
    return false;
}

//* ============================== *//
//* ===== Registro Cabecalho ===== *//
//* ============================== *//

int binario_getRRNProx(Binario* binario) { return binario->rrnProx; }
bool binario_estaVazio(Binario* binario) { return binario->inseridos == 0; }