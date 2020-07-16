#include "indice.h"

#define ORDEM 6
#define RRNNULL -1

typedef struct Pagina Pagina;
typedef struct Chave Chave;

struct Indice {
    FILE* file;
    char* path;
    const char* modes;
    Pagina* raiz;

    // Cabecalho
    char status;
    int noRaiz;
    int nroNiveis;
    int proxRRN;
    int nroChaves;
};

struct Chave {
    int id;
    int dado;
};

struct Pagina {
    int nivel;  // Nivel da pagina
    int n;  // Numero de chaves
    Chave chaves[ORDEM - 1];
    int filhos[ORDEM];
};

//* ====================== *//
//* ===== Constantes ===== *//
//* ====================== *//

#define TAM_REG 72
#define TAM_LIXOC 55

static const char LIXO[TAM_REG + 1] = "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$";

//* ============================ *//
//* ===== Métodos Privados ===== *//
//* ============================ *//

inline static int esq(int i) { return i; }  // Calcula indice do filho a esquerda de i
inline static int dir(int i) { return i + 1; }  // Calcula indice do filho a direita de i

/**
 * Abri um arquivo e salva em indice
 * Se já tiver um arquivo aberto fecha ele
 */
inline static void abrirArquivo(Indice* indice) {
    if (indice->file) fclose(indice->file);
    indice->file = fopen(indice->path, indice->modes);
}

/**
 * Le o cabecalho do arquivo
 * Se já tiver um arquivo aberto fecha ele
 * O valor de file no final será NULL
 * 
 * Retorna false se houver uma falha
 * Retorna false se o arquivo for inconsistente e para a leitura do cabecalho
 */
static bool cabecalhoLeitura(Indice* indice) {
    if (!indice) return false;  // Nao recebeu caminho

    if (indice->file) fclose(indice->file);
    indice->file = NULL;

    indice->file = fopen(indice->path, "rb");
    if (!indice->file) return false;

    // Le cabecalho
    TRYFREAD(&indice->status, char, 1, indice->file);
    if (indice->status == '0') {  // Arquivo inconsistente
        fclose(indice->file);
        indice->file = NULL;
        return false;
    }

    TRYFREAD(&indice->noRaiz, int, 1, indice->file);
    TRYFREAD(&indice->nroNiveis, int, 1, indice->file);
    TRYFREAD(&indice->proxRRN, int, 1, indice->file);
    TRYFREAD(&indice->nroChaves, int, 1, indice->file);

    // Fecha arquivo
    fclose(indice->file);
    indice->file = NULL;
    return true;

fread_erro:  // Falha ao ler do arquivo
    fclose(indice->file);
    indice->file = NULL;
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
static bool cabecalhoEscrita(Indice* indice) {
    if (!indice) return false;  // Nao recebeu caminho

    if (indice->file) fclose(indice->file);
    indice->file = NULL;

    indice->file = fopen(indice->path, "rb+");
    if (!indice->file) return false;

    // Le cabecalho
    TRYFREAD(&indice->status, char, 1, indice->file);
    if (indice->status == '0') {  // Arquivo inconsistente
        fclose(indice->file);
        indice->file = NULL;
        return false;
    }

    TRYFREAD(&indice->noRaiz, int, 1, indice->file);
    TRYFREAD(&indice->nroNiveis, int, 1, indice->file);
    TRYFREAD(&indice->proxRRN, int, 1, indice->file);
    TRYFREAD(&indice->nroChaves, int, 1, indice->file);

    // Marca arquivo como inconsistente
    fseek(indice->file, 0, SEEK_SET);
    indice->status = '0';
    TRYFWRITE(&indice->status, char, 1, indice->file);

    // Fecha arquivo
    fclose(indice->file);
    indice->file = NULL;
    return true;

fwrite_erro:  // Falha ao escrever no arquivo
fread_erro:  // Falha ao ler do arquivo
    fclose(indice->file);
    indice->file = NULL;
    return false;
}

/**
 * Escreve as alteracoes do cabecalho
 * Retorna se a operacao foi bem sucedida
 */
static bool salvarCabecalho(Indice* indice) {
    if (!indice->file) indice->file = fopen(indice->path, "rb+");
    if (!indice->file) return false;
    fseek(indice->file, 0, SEEK_SET);

    indice->status = '1';
    TRYFWRITE(&indice->status, char, 1, indice->file);
    TRYFWRITE(&indice->noRaiz, int, 1, indice->file);
    TRYFWRITE(&indice->nroNiveis, int, 1, indice->file);
    TRYFWRITE(&indice->proxRRN, int, 1, indice->file);
    TRYFWRITE(&indice->nroChaves, int, 1, indice->file);

    fclose(indice->file);
    indice->file = NULL;
    return true;

fwrite_erro:  // Falha ao escrever no arquivo
    fclose(indice->file);
    return false;
}

/**
 * Cria um novo objeto pagina com nenhum dado
 * Retorna NULL se nao for possivel criar o objeto
 */
static Pagina* pagina_criar() {
    Pagina* pagina = malloc(sizeof(Pagina));
    if (!pagina) return NULL;

    int i;

    pagina->nivel = 0;
    pagina->n = 0;
    for (i = 0; i < ORDEM - 1; i++) {
        pagina->chaves[i].id = RRNNULL;
        pagina->chaves[i].dado = RRNNULL;
    }

    for (i = 0; i < ORDEM; i++) {
        pagina->filhos[i] = RRNNULL;
    }

    return pagina;
}

/**
 * Destroi a pagina, os objetos que o pertence e desaloca variveis
 */
static void pagina_apagar(Pagina** pagina) {
    if (!pagina || !*pagina) return;

    free(*pagina);
    *pagina = NULL;
}

/**
 * Le pagina de onde o indice aponta
 */
static Pagina* lerPagina(Indice* indice, int rrn) {
    if (!indice) return NULL;

    int i;

    indice_apontar(indice, rrn, SEEK_SET);  // Aponta para o registro a ser lido

    Pagina* pagina = pagina_criar();
    if (!pagina) {
        return NULL;
    }

    TRYFREAD(&pagina->nivel, int, 1, indice->file);
    TRYFREAD(&pagina->n, int, 1, indice->file);
    for (i = 0; i < ORDEM - 1; i++) {
        TRYFREAD(&pagina->chaves[i].id, int, 1, indice->file);
        TRYFREAD(&pagina->chaves[i].dado, int, 1, indice->file);
    }
    TRYFREAD(&pagina->filhos, int, ORDEM, indice->file);

    return pagina;

fread_erro:
    free(pagina);
    return NULL;
}

/**
 * Escreve pagina onde o indice aponta
 */
static bool escreverPagina(Indice* indice, Pagina* pagina, int rrn) {
    if (!indice || !pagina) return false;

    int i;

    indice_apontar(indice, rrn, SEEK_SET);  // Aponta para o registro a ser escrito

    TRYFWRITE(&pagina->nivel, int, 1, indice->file);
    TRYFWRITE(&pagina->n, int, 1, indice->file);
    for (i = 0; i < ORDEM - 1; i++) {
        TRYFWRITE(&pagina->chaves[i].id, int, 1, indice->file);
        TRYFWRITE(&pagina->chaves[i].dado, int, 1, indice->file);
    }
    TRYFWRITE(&pagina->filhos, int, ORDEM, indice->file);

    return true;

fwrite_erro:
    return false;
}

//* ============================ *//
//* ===== Métodos Publicos ===== *//
//* ============================ *//

Indice* indice_criar(char* path) {
    if (!path) return NULL;  // Nao recebeu parametros

    Indice* indice = malloc(sizeof(Indice));
    if (!indice) return NULL;

    indice->file = NULL;
    indice->path = path;
    indice->modes = "wb+";

    abrirArquivo(indice);
    if (!indice->file) {  // Falha ao abrir arquivo
        free(indice);
        return NULL;
    }

    // Criando Cabecalho
    indice->status = '0';
    indice->noRaiz = RRNNULL;
    indice->nroNiveis = 0;
    indice->proxRRN = 0;
    indice->nroChaves = 0;

    // Escrevendo o cabecalho no arquivo
    TRYFWRITE(&indice->status, char, 1, indice->file);
    TRYFWRITE(&indice->noRaiz, int, 1, indice->file);
    TRYFWRITE(&indice->nroNiveis, int, 1, indice->file);
    TRYFWRITE(&indice->proxRRN, int, 1, indice->file);
    TRYFWRITE(&indice->nroChaves, int, 1, indice->file);
    TRYFWRITE(LIXO, char, TAM_LIXOC, indice->file);

    return indice;

fwrite_erro:  // Falha ao escrever no arquivo
    fclose(indice->file);
    return NULL;
}

Indice* indice_abrir(char* path, bool escrita) {
    if (!path) return NULL;  // Nao recebeu parametros

    bool ok;

    Indice* indice = malloc(sizeof(Indice));
    if (!indice) return NULL;

    indice->file = NULL;
    indice->path = path;
    indice->modes = escrita ? "rb+" : "rb";

    ok = escrita ? cabecalhoEscrita(indice) : cabecalhoLeitura(indice);
    if (!ok) goto falha;

    abrirArquivo(indice);
    if (!indice->file) goto falha;

    indice_apontar(indice, 0, SEEK_SET);
    return indice;

falha:  // Falha na execucao da funcao
    if (indice->file) fclose(indice->file);
    free(indice);
    return NULL;
}

void indice_fechar(Indice** indice) {
    if (!indice || !*indice) return;  // Objeto ja foi apagado (arquivo ja foi fechado)

    // Fecha o arquivo
    if (isEscrita((*indice)->modes)) {
        salvarCabecalho(*indice);
    } else {
        if ((*indice)->file) fclose((*indice)->file);
    }

    free(*indice);
    *indice = NULL;
}

void indice_apontar(Indice* indice, int rrn, int whence) {
    if (whence == SEEK_SET) {
        fseek(indice->file, (rrn + 1) * TAM_REG, SEEK_SET);
    } else if (whence == SEEK_CUR) {
        int sobra = ftell(indice->file) % TAM_REG;  // Quanto o binario esta apontando a frente do inicio do registro
        fseek(indice->file, rrn * TAM_REG - sobra, SEEK_CUR);
    } else if (whence == SEEK_END) {
        fseek(indice->file, rrn * TAM_REG, SEEK_END);
    }
}

int indice_buscar(Indice* indice, int id) {
    int rrn = indice->noRaiz;
    int l, r, m;

    while (rrn != RRNNULL) {
        Pagina* pagina = lerPagina(indice, rrn);
        if (!pagina) return RRNNULL;

        l = 0;
        r = pagina->n;

        while (r - l > 1) {
            m = (l + r) / 2;
            if (pagina->chaves[m].id == id) {
                return pagina->chaves[m].dado;
            } else if (pagina->chaves[m].id < id) {
                l = m;
            } else {
                r = m;
            }
        }

        rrn = pagina->filhos[esq(r)];  // dir(l) == esq(r)

        free(pagina);
        pagina = NULL;
    }

    return RRNNULL;
}

static bool split(Pagina* pagina, Pagina* esquerda, Pagina* direita, Chave* promover) {
    if (!pagina || !esquerda || !direita || !promover) return false;

    int i, j;

    esquerda->nivel = pagina->nivel - 1;
    direita->nivel = direita->nivel - 1;

    // Distribui uniformemente
    esquerda->filhos[esq(0)] = pagina->filhos[esq(0)];
    for (i = 0; i < (ORDEM - 1) / 2; i++) {  // Inicio de pagina vai para esqueda
        esquerda->chaves[i] = pagina->chaves[i];
        esquerda->filhos[dir(i)] = pagina->filhos[dir(i)];
    }
    esquerda->n = i;

    // Chave do meio
    *promover = pagina->chaves[i++];

    direita->filhos[esq(i)] = pagina->filhos[esq(i)];
    for (j = 0; i < (ORDEM - 1); i++, j++) {  // Final de pagina vai para direita
        direita->chaves[j] = pagina->chaves[i];
        direita->filhos[dir(j)] = pagina->filhos[dir(i)];
    }
    direita->n = j;

    return true;
}

static void inserirOrdenado(Pagina* pagina, Chave chave, int flihoDireito) {
    int i, j;

    for (i = 0; i < pagina->n; i++) {
        if (pagina->chaves[i].id > chave.id) break;
    }
    for (j = i + 1; j < pagina->n; j++) {
        pagina->chaves[j] = pagina->chaves[j - 1];
        pagina->filhos[dir(j)] = pagina->filhos[dir(j - 1)];
    }
    pagina->chaves[i] = chave;
    pagina->filhos[dir(i)] = flihoDireito;

    pagina->n++;
}

/**
 * rrn: RRN da pagina onde esta na recursao
 * chave: chave a ser inserida
 * promover: 
 * promoverDir: 
 */
static bool indice_inserir0(Indice* indice, int rrn, Chave chave, Chave* promover, int* promoverDir) {  // TODO separar funcao split
    if (!indice) return false;

    bool ok;

    // Variaveis com alocao dinamica
    Pagina* pagina = NULL;
    Pagina* direita = NULL;
    Pagina* esquerda = NULL;

    // Busca chave
    pagina = lerPagina(indice, rrn);
    if (!pagina) goto falha;

    int l = 0, r = pagina->n, m;
    while (r - l > 1) {
        m = (l + r) / 2;
        if (pagina->chaves[m].id == chave.id) {  // Chave duplicada
            pagina_apagar(&pagina);
            return false;
        } else if (pagina->chaves[m].id < chave.id) {
            l = m;
        } else {
            r = m;
        }
    }

    Chave inserir;  // Chave a ser inserirda em pagina
    int inserirDir;  // Subarvore que deve estar a direita da chave a ser inserida
    if (pagina->filhos[dir(m)] != RRNNULL) {  // Existe subarvore para continuar inserindo
        int a = (pagina->chaves[m].id < chave.id ? dir(m) : esq(m));  // TODO melhorar o nome dessa variavel e explicar o que esta acontecendo
        ok = indice_inserir0(indice, pagina->filhos[a], chave, &inserir, &inserirDir);
        if (!ok) goto falha;
    } else {
        inserir = chave;
        inserirDir = RRNNULL;
    }

    // Insere chave
    if (pagina->n == ORDEM - 1) {  // Nao tem espaco para inserir (Split)
        esquerda = pagina_criar();
        if (!esquerda) goto falha;

        direita = pagina_criar();
        if (!direita) goto falha;

        // Distribui uniformemente
        split(pagina, esquerda, direita, promover);
        *promoverDir = indice->proxRRN;

        ok = escreverPagina(indice, esquerda, rrn);
        if (!ok) goto falha;

        ok = escreverPagina(indice, direita, indice->proxRRN++);
        if (!ok) goto falha;

        pagina_apagar(&esquerda);
        pagina_apagar(&direita);
    } else {  // Tem espaco para inserir
        // inseri Chave inserir tendo filho direito inserirDir
        inserirOrdenado(pagina, inserir, inserirDir);

        ok = escreverPagina(indice, pagina, rrn);
        if (!ok) goto falha;

        promover->id = RRNNULL;
        promover->dado = RRNNULL;
        *promoverDir = RRNNULL;
    }
    pagina_apagar(&pagina);
    return true;

falha:  // Falha na execucao da funcao
    pagina_apagar(&pagina);
    pagina_apagar(&direita);
    pagina_apagar(&esquerda);
    return false;
}

bool indice_inserir(Indice* indice, int id, int dado) {
    if (!indice) return false;
    if (id == RRNNULL) return false;

    bool ok;

    // Variaveis com alocacao dinamica
    Pagina* pagina = NULL;

    // Inserindo chave
    Chave chave = {id, dado};

    if (indice->noRaiz == RRNNULL) {  // Se nao tem no raiz
        indice->noRaiz = indice->proxRRN++;  // Cria um novo no vazio e chama de raiz
        pagina = pagina_criar();
        if (!pagina) goto falha;

        pagina->nivel = 1;
        escreverPagina(indice, pagina, indice->noRaiz);
    }

    Chave promover;
    int promoverDir;
    ok = indice_inserir0(indice, indice->noRaiz, chave, &promover, &promoverDir);
    if (!ok) goto falha;  // Falha ao inserir chave

    if (promover.id == RRNNULL) {
        indice->nroChaves++;
        return true;  // Não houve split na raiz
    }

    pagina = pagina_criar();
    if (!pagina) goto falha;
    pagina->nivel = ++indice->nroNiveis;

    pagina->chaves[0] = promover;
    pagina->filhos[esq(0)] = indice->noRaiz;
    pagina->filhos[dir(0)] = promoverDir;

    indice->noRaiz = indice->proxRRN++;
    escreverPagina(indice, pagina, indice->noRaiz);

    indice->nroChaves++;
    pagina_apagar(&pagina);
    return true;

falha:  // Falha na execucao da funcao
    pagina_apagar(&pagina);
    return false;
}