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
//* ===== Métodos Privados ===== *//
//* ============================ *//

/**
 * Insere um registro no arquivo
 * Obs: Nao verifica se esta no final do arquivo
 * 
 * Retorna se foi possivel inserir o registro no arquivo
 */
static bool escreverRegistro(Binario* binario, Registro* registro) {
    if (!binario) return false;  // Verifica se recebeu um arquivo

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
    // Mesmo que tamCidadee = (cidade != NULL ? strlen(cidade) : 0);
    int tamCidadeMae = (cidadeMae ? strlen(cidadeMae) : 0);
    int tamCidadeBebe = (cidadeBebe ? strlen(cidadeBebe) : 0);

    // Escrevendo campos variaveis
    qtdeLixo = TAM_CVAR;  // Espaco disponivel para campos variaveis
    TRYFWRITE(&tamCidadeMae, int, 1, binario);  // Escreve o tamnho da cidadeMae
    TRYFWRITE(&tamCidadeBebe, int, 1, binario);  // Escreve o tamnho da cidadeBebe
    if (cidadeMae) TRYFWRITE(cidadeMae, char, tamCidadeMae, binario);  // Escreve cidadeMae se for diferente de NULL
    if (cidadeBebe) TRYFWRITE(cidadeBebe, char, tamCidadeBebe, binario);  // Escreve cidadeBebe se for diferente de NULL

    qtdeLixo -= tamCidadeMae + tamCidadeBebe;
    TRYFWRITE(LIXO, char, qtdeLixo, binario);  // Preenche com lixo o restante

    // Escrevendo campos fixos
    TRYFWRITE(&idNascimento, int, 1, binario);
    TRYFWRITE(&idadeMae, int, 1, binario);

    qtdeLixo = TAM_DATA;
    if (dataNascimento) {  // Se dataNascimento nao for NULL
        tamDado = strlen(dataNascimento);
        TRYFWRITE(dataNascimento, char, tamDado, binario);
        qtdeLixo -= tamDado;  // Descontando o que foi escrito

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

    qtdeLixo = TAM_ESTADO;
    if (estadoMae) {  // Se estadoMae nao for NULL
        tamDado = strlen(estadoMae);
        TRYFWRITE(estadoMae, char, tamDado, binario);
        qtdeLixo -= tamDado;  // Descontando o que foi escrito

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

    qtdeLixo = TAM_ESTADO;
    if (estadoBebe) {  // Se estadoBebe nao for NULL
        tamDado = strlen(estadoBebe);
        TRYFWRITE(estadoBebe, char, tamDado, binario);
        qtdeLixo -= tamDado;  // Descontando o que foi escrito

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

//* ============================ *//
//* ===== Métodos Publicos ===== *//
//* ============================ *//

Binario* binario_criar(char* path) {
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

bool binario_gerarDoCSV(char* path, CSV* csv) {
    // Criar arquivo com o regitro cabecalho
    Binario* binario = binario_criar(path);
    if (!binario) return false;  // Verifica se o arquivo foi criado
    binario_fechar(&binario);

    Registro* registro;  // Variavel para armazenar registro lido
    int cont = 0;  // Conta quantos registro foram escritos
    bool ok;  // Armazena se o setCabecalho deu cert

    // Coloca status como inconsistente
    bool status = false;
    ok = binario_setCabecalho(path, &status, NULL, NULL, NULL, NULL);
    if (!ok) return false;

    // Escreve registros no arquivo
    binario = binario_abrirEscrita(path);
    while ((registro = csv_lerRegistro(csv))) {  // Enquanto houver registros para ler
        escreverRegistro(binario, registro);
        registro_apagar(&registro);
        cont++;
    }
    binario_fechar(&binario);

    // Coloca status como consistente
    status = true;
    ok = binario_setCabecalho(path, &status, &cont, &cont, NULL, NULL);
    if (!ok) return false;

    return true;
}

Binario* binario_abrirLeitura(char* path) {
    if (!path) return NULL;  // Verifica se recebeu o caminho

    Binario* binario = fopen(path, "rb");
    if (!binario) return NULL;  // Verifica se o arquivo foi aberto com sucesso

    fseek(binario, TAM_REG, SEEK_SET);  // Pula o registro cabecalho
    return binario;
}

Binario* binario_abrirEscrita(char* path) {
    if (!path) return NULL;  // Verifica se recebeu o caminho

    Binario* binario = fopen(path, "rb+");
    if (!binario) return NULL;  // Verifica se o arquivo foi aberto com sucesso

    fseek(binario, TAM_REG, SEEK_SET);  // Pula o registro cabecalho
    return binario;
}

void binario_fechar(Binario** binario) {
    // Verifica se objeto já foi apagado
    if (!binario || !*binario) return;

    // Fecha o arquivo
    if (binario && *binario) fclose(*binario);
    *binario = NULL;
}

bool binario_inserir(char* path, Registro* registro) {
    bool status;
    int rrn;
    binario_getCabecalho(path, &status, &rrn, NULL, NULL, NULL);

    if (!status) return false;  // Arquivo inconsistente

    // Coloca status como inconsistente
    status = false;
    binario_setCabecalho(path, &status, NULL, NULL, NULL, NULL);

    Binario* binario = binario_abrirEscrita(path);
    fseek(binario, rrn * TAM_REG, SEEK_SET);

    bool ok = escreverRegistro(binario, registro);
    if (!ok) return false;

    // Atualiza RRNProx e coloca status como consistente
    rrn++;
    status = true;
    binario_setCabecalho(path, &status, &rrn, NULL, NULL, NULL);

    return true;
}

Registro* binario_leRegistro(Binario* binario, bool* erro) {
    if (!binario) {  // Verifica se recebeu um arquivo
        *erro = true;
        return NULL;
    }

    *erro = false;  // Salva erro como false, pois caso nao ocarra nenhum erro deve valer false

    // Le Campos Variaveis
    int tamCidadeMae;
    TRYFREAD(&tamCidadeMae, int, 1, binario);

    if (tamCidadeMae == REMOVIDO) return NULL;  // Verifica se o registro esta removido

    int tamCidadeBebe;
    TRYFREAD(&tamCidadeBebe, int, 1, binario);

    char* cidadeMae = NULL;
    if (tamCidadeMae > 0) {
        cidadeMae = (char*)malloc((TAM_CVAR + 1) * sizeof(char));
        TRYFREAD(cidadeMae, char, tamCidadeMae, binario);
        cidadeMae[tamCidadeMae] = '\0';  // Poe '\0' no fim da string. Se ela for menor ela nao sera altarada (O arquivo tera o '\0')
    }  // Se tem uma cidade armazenada ela foi lida, senao cidadeMae eh NULL

    char* cidadeBebe = NULL;
    if (tamCidadeBebe > 0) {
        cidadeBebe = (char*)malloc((TAM_CVAR + 1) * sizeof(char));
        TRYFREAD(cidadeBebe, char, tamCidadeBebe, binario);
        cidadeBebe[tamCidadeBebe] = '\0';  // Poe '\0' no fim da string. Se ela for menor ela nao sera altarada (O arquivo tera o '\0')
    }  // Se tem uma cidade armazenada ela foi lida, senao cidadeBebe eh NULL

    // Pula lixo
    int qtdeLixo = TAM_CVAR - tamCidadeMae - tamCidadeBebe;
    fseek(binario, qtdeLixo, SEEK_CUR);

    // Le campos fixos
    int idNascimento;
    TRYFREAD(&idNascimento, int, 1, binario);

    int idadeMae;
    TRYFREAD(&idadeMae, int, 1, binario);

    char* dataNascimento = (char*)malloc((TAM_DATA + 1) * sizeof(char));
    TRYFREAD(dataNascimento, char, TAM_DATA, binario);
    dataNascimento[TAM_DATA] = '\0';  // Poe '\0' no fim da string. Se ela for menor ela nao sera altarada (O arquivo tera o '\0')
    if (strlen(dataNascimento) == 0) {  // Se a string for vazia
        free(dataNascimento);
        dataNascimento = NULL;
    }

    char sexoBebe;
    TRYFREAD(&sexoBebe, char, 1, binario);

    char* estadoMae = (char*)malloc((TAM_ESTADO + 1) * sizeof(char));
    TRYFREAD(estadoMae, char, TAM_ESTADO, binario);
    estadoMae[TAM_ESTADO] = '\0';  // Poe '\0' no fim da string. Se ela for menor ela nao sera altarada (O arquivo tera o '\0')
    if (strlen(estadoMae) == 0) {  // Se a string for vazia
        free(estadoMae);
        estadoMae = NULL;
    }

    char* estadoBebe = (char*)malloc((TAM_ESTADO + 1) * sizeof(char));
    TRYFREAD(estadoBebe, char, TAM_ESTADO, binario);
    estadoBebe[TAM_ESTADO] = '\0';  // Poe '\0' no fim da string. Se ela for menor ela nao sera altarada (O arquivo tera o '\0')
    if (strlen(estadoBebe) == 0) {  // Se a string for vazia
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

//* ============================== *//
//* ===== Registro Cabecalho ===== *//
//* ============================== *//

bool binario_setCabecalho(char* path,
                          bool* status, int* rrn,
                          int* inseridos, int* removidos, int* atualizados) {
    if (!path) return false;  // Verifica se recebeu o caminho

    Binario* binario = fopen(path, "rb+");
    if (!binario) return false;  // Verifica se o arquivo foi aberto

    if (status) {  // Verifica se recebeu status
        char aux = (status ? '1' : '0');
        TRYFWRITE(&aux, char, 1, binario);
    } else {
        fseek(binario, sizeof(char), SEEK_CUR);  // Pula o status no arquivos
    }

    if (rrn) {  // Verifica se recebeu o rrn
        TRYFWRITE(rrn, int, 1, binario);
    } else {
        fseek(binario, sizeof(int), SEEK_CUR);  // Pula o rrn no arquivos
    }

    if (inseridos) {  // Verifica se recebeu inseridos
        TRYFWRITE(inseridos, int, 1, binario);
    } else {
        fseek(binario, sizeof(int), SEEK_CUR);  // Pula inseridos no arquivos
    }

    if (removidos) {  // Verifica se recebeu removidos
        TRYFWRITE(removidos, int, 1, binario);
    } else {
        fseek(binario, sizeof(int), SEEK_CUR);  // Pula removidos no arquivos
    }

    if (atualizados) {  // Verifica se recebeu o atualizados
        TRYFWRITE(atualizados, int, 1, binario);
    } else {
        fseek(binario, sizeof(int), SEEK_CUR);  // Pula atualizados no arquivos
    }

    fclose(binario);
    return true;

fwrite_error:  // Tratando erros ao escrever no arquivo
    fclose(binario);
    return false;
}

bool binario_getCabecalho(char* path,
                          bool* status, int* rrn,
                          int* inseridos, int* removidos, int* atualizados) {
    if (!path) return false;  // Verifica se recebeu um caminho

    Binario* binario = fopen(path, "rb");
    if (!binario) return false;  // Ve se o arquivo foi aberto

    if (status) {  // Verifica se deve ler o status
        char aux;
        TRYFREAD(&aux, char, 1, binario);
        *status = (aux == '1');
    } else {
        fseek(binario, sizeof(char), SEEK_CUR);  // Pula o status no arquivos
    }

    if (rrn) {  // Verifica se deve ler o rrn
        TRYFREAD(rrn, int, 1, binario);
    } else {  // Pula o status no arquivos
        fseek(binario, sizeof(int), SEEK_CUR);
    }

    if (inseridos) {  // Verifica se deve ler inseridos
        TRYFREAD(inseridos, int, 1, binario);
    } else {  // Pula inseridos no arquivos
        fseek(binario, sizeof(int), SEEK_CUR);
    }

    if (removidos) {  // Verifica se deve ler removidos
        TRYFREAD(removidos, int, 1, binario);
    } else {  // Pula o removidos no arquivos
        fseek(binario, sizeof(int), SEEK_CUR);
    }

    if (atualizados) {  // Verifica se deve ler atualizados
        TRYFREAD(atualizados, int, 1, binario);
    } else {  // Pula o atualizdos no arquivos
        fseek(binario, sizeof(int), SEEK_CUR);
    }

    fclose(binario);
    return true;

fread_error:  // Tratando erros ao ler do arquivo
    fclose(binario);
    return false;
}