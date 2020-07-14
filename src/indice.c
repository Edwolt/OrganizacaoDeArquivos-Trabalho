#include "indice.h"

#define ORDEM 2
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
    int nivel;
    int n;
    Chave chaves[ORDEM + 1];
    int subarvores[ORDEM];
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

inline static esq(int i) { return i; }  // TODO essa funcao pode ser desnecessaria
inline static dir(int i) { return i + 1; }  // TODO essa funcao pode ser desnecessaria

/**
 * Abri um arquivo e salva em indice
 * Se já tiver um arquivo aberto fecha ele
 */
inline static void abrirArquivo(Indice* binario) {
    if (binario->file) fclose(binario->file);
    binario->file = fopen(binario->path, binario->modes);
}

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

static Pagina* pagina_criar() {
    Pagina* pagina = malloc(sizeof(Pagina));
    if (!pagina) return NULL;

    int i;

    pagina->nivel = 0;
    pagina->n = 0;
    for (i = 0; i < ORDEM; i++) {
        pagina->chaves[i].id = RRNNULL;
        pagina->chaves[i].dado = RRNNULL;
        pagina->subarvores[i] = RRNNULL;
    }
    pagina->chaves[ORDEM].id = RRNNULL;
    pagina->chaves[ORDEM].dado = RRNNULL;

    return pagina;
}

static void pagina_apagar(Pagina** pagina) {
    if (!pagina || !*pagina) return;

    free(*pagina);
    *pagina = NULL;
}

static Pagina* lerPagina(Indice* indice) {
    int i;

    Pagina* pagina = pagina_criar();
    if (!pagina) return NULL;

    TRYFREAD(&pagina->nivel, int, 1, indice->file);
    TRYFREAD(&pagina->n, int, 1, indice->file);
    for (i = 0; i < pagina->n; i++) {
        TRYFREAD(&pagina->chaves[i].id, int, 1, indice->file);
        TRYFREAD(&pagina->chaves[i].dado, int, 1, indice->file);
    }
    TRYFREAD(&pagina->subarvores, int, pagina->n + 1, indice->file);  // TODO nao tenho certeza disso

    return pagina;

fread_erro:
    free(pagina);
    return NULL;
}

static bool escreverPagina(Indice* indice, Pagina* pagina) {
    if (!indice || !pagina) return false;

    int i;

    TRYFWRITE(&pagina->nivel, int, 1, indice->file);
    TRYFWRITE(&pagina->n, int, 1, indice->file);
    for (i = 0; i < ORDEM; i++) {
        TRYFWRITE(&pagina->chaves[i].id, int, 1, indice->file);
        TRYFWRITE(&pagina->chaves[i].dado, int, 1, indice->file);
    }
    TRYFWRITE(&pagina->subarvores, int, ORDEM + 1, indice->file);

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
    indice->modes = "wb";

    abrirArquivo(indice);
    if (!indice->file) {  // Falha ao abrir arquivo
        free(indice);
        return NULL;
    }

    // Criando Cabecalho
    indice->status = '0';
    indice->noRaiz = 0;
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
        indice_apontar(indice, rrn, SEEK_SET);
        Pagina* pagina = lerPagina(indice);
        if (!pagina) return RRNNULL;

        l = 0;
        r = pagina->n;

        while (r - l > 1) {
            m = (l + r) / 2;
            if (pagina->chaves[m].id == id) {
                indice_apontar(indice, -1, SEEK_CUR);
                return pagina->chaves[m].dado;
            } else if (pagina->chaves[m].id < id) {
                l = m;
            } else {
                r = m;
            }
        }

        rrn = pagina->subarvores[esq(r)];  // dir(l) == esq(r)

        free(pagina);
        pagina = NULL;
    }

    return RRNNULL;
}

// TODO melhorar nome dos parametros
bool indice_inserir_recursivo(Indice* indice, int atual, int id, int rrn, int* pid, int* prrn, int* rrndir) {
    if (!indice) return false;

    bool ok;

    // Variaveis com alocao dinamica
    Pagina* pagina = NULL;
    Pagina* direita = NULL;
    Pagina* esquerda = NULL;

    indice_apontar(indice, atual, rrn);

    // Busca chave
    int l = 0, r = pagina->n, m;

    while (r - l > 1) {
        m = (l + r) / 2;
        if (pagina->chaves[m].id == id) {  // TODO Tornar mais legivel
            goto falha;  // A chave ja existe
        } else if (pagina->chaves[m].id < id) {
            l = m;
        } else {
            r = m;
        }
    }
    // TODO obs: l < id < r

    // TODO melhorar nome das variaveis
    int Pid;
    int Prrn;
    int Rrndir;
    if (pagina->subarvores[l] != RRNNULL) {  // Existe subarvore para continuar inserindo
        indice_inserir_recursivo(indice, pagina->subarvores[esq(r)], id, rrn, &Pid, &Prrn, &Rrndir);  // TODO virou uma busca
    } else {
        Pid = id;
        Prrn = rrn;
        Rrndir = RRNNULL;
    }

    if (pagina->n == ORDEM) {
        // TODO split
    } else {
        // TODO insere simplesmente
        // Insere {chave: Pid; dado: Prrn} com filho direito Rrndir
    }

    // TODO Inserir de fato

    return true;

falha:
    pagina_apagar(&pagina);
    pagina_apagar(&direita);
    pagina_apagar(&esquerda);
    return false;
}

bool indice_inserir(Indice* indice, int id, int rrn) {  // TODO essa funcao precisa ser recursiva, pois tem que empilhar os pais na busca
    if (!indice) return false;

    int i, j;
    bool ok;

    // Variaveis com alocacao dinamica
    Pagina* pagina = NULL;
    Pagina* direita = NULL;  // Para a situacao que ocorre split
    Pagina* esquerda = NULL;  // Para a situacao que ocorre split

    // Buscando pagina
    if (indice_buscar(indice, id) != RRNNULL) return false;

    // Lendo pagina
    pagina = lerPagina(indice);  // Lendo a mesma pagina de disco 2 vezes seguidas não deveria se tornar um leitura em disco
    if (!pagina) return false;
    indice_apontar(indice, -1, SEEK_CUR);  // Volta indice para o inicio do registro pois ele será escrito

    if (pagina->n == ORDEM) {  // TODO Ocorre Split
        direita = pagina_criar();
        if (!direita) goto falha;
        direita->nivel = pagina->nivel + 1;

        esquerda = pagina_criar();
        if (!esquerda) goto falha;
        esquerda->nivel = pagina->nivel + 1;

        // Distribui uniformemente
        for (i = 0; i < ORDEM / 2; i++) {  // Inicio de pagina vai para esqueda
            esquerda->chaves[i] = pagina->chaves[i];
            esquerda->subarvores[i] = pagina->subarvores[i];
        }
        esquerda->subarvores[i] = pagina->subarvores[i];
        esquerda->n = i;

        // Chave do meio
        int promover = pagina->chaves[i++].id;
        int dir = indice->proxRRN;

        for (j = 0; i < ORDEM; i++, j++) {  // Final de pagina vai para direita
            direita->chaves[j] = pagina->chaves[i];
            direita->subarvores[j] = pagina->subarvores[i];
        }
        direita->subarvores[j] = pagina->subarvores[j];
        direita->n = i;

        ok = escreverPagina(indice, esquerda);
        if (!ok) goto falha;

        indice_apontar(indice, indice->proxRRN++, SEEK_SET);
        ok = escreverPagina(indice, direita);
        if (!ok) goto falha;

        pagina_apagar(&esquerda);
        pagina_apagar(&direita);
        pagina_apagar(&pagina);

        // TODO Insere no nó pai dando split

        return true;
    } else {
        for (i = 0; i < pagina->n; i++) {
            if (id < pagina->chaves[i].id) {  // Insere ordenado
                // Desloca chaves para dar espaco para a nova
                for (j = pagina->n; j > i; j--) {
                    pagina->chaves[j] = pagina->chaves[j - 1];
                }
                pagina->chaves[pagina->n + 1].id = RRNNULL;
                pagina->n++;

                // Insere a pagina
                pagina->chaves[i].id = id;
                pagina->chaves[i].dado = rrn;
                break;
            }

            ok = escreverPagina(indice, pagina);
            if (!ok) goto falha;
            return true;
        }
    }

falha:  // Falha na execucao da funcao
    pagina_apagar(&pagina);
    pagina_apagar(&esquerda);
    pagina_apagar(&direita);
    return false;
}