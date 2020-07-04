#include "cabecalho.h"

struct Cabecalho {
    bool status;  // Status
    int rrn;  // RRNProxRegistro
    int inseridos;  // numeroRegistrosInseridos
    int removidos;  // numeroRegistrosRemovidos
    int atualizados;  //numeroRegistrosAtualizado
};

Cabecalho* cabecalho_criar(bool status, int rrn,
                           int inseridos, int removidos, int atualizados) {
    Cabecalho* cabecalho = malloc(sizeof(Cabecalho));
    if (!cabecalho) return NULL;

    cabecalho->status = status;
    cabecalho->rrn = rrn;
    cabecalho->inseridos = inseridos;
    cabecalho->removidos = removidos;
    cabecalho->atualizados = atualizados;

    return cabecalho;
}

Cabecalho* cabecalho_criarVazio() {
    return cabecalho_criar(true, 0, 0, 0, 0);
}

void cabecalho_apagar(Cabecalho** cabecalho) {
    if (!cabecalho || !*cabecalho) return;

    free(*cabecalho);
    *cabecalho = NULL;
}

void cabecalho_extrairDados(Cabecalho* cabecalho,
                            bool* status, int* rrn,
                            int* inseridos, int* removidos, int* atualizados) {
    if (!cabecalho) return;

    if (status) *status = cabecalho->status;
    if (rrn) *rrn = cabecalho->rrn;
    if (inseridos) *inseridos = cabecalho->inseridos;
    if (removidos) *removidos = cabecalho->removidos;
    if (atualizados) *atualizados = cabecalho->atualizados;
}

void cabecalho_inserir(Cabecalho* cabecalho) {
    if (!cabecalho) return;
    cabecalho->inseridos++;
    cabecalho->rrn++;
}

void cabecalho_remover(Cabecalho* cebecalho) {
    if (!cebecalho) return;
    cebecalho->removidos++;
    cebecalho->inseridos--;
}

void cabecalho_atualizar(Cabecalho* cabecalho) {
    if (!cabecalho) return;
    cabecalho->atualizados++;
}

//* ============================= *//
//* ===== Getters e Setters ===== *//
//* ============================= *//

// Se cabecalho for NULL, em um getter, retorna uma valor qualquer
// Se cabecalho for NULL, em um setter, nada eh feito

bool cabecalho_getStatus(Cabecalho* cabecalho) {
    if (!cabecalho) return false;
    return cabecalho->status;
}
void cabecalho_setStatus(Cabecalho* cabecalho, bool status) {
    if (!cabecalho) return;
    cabecalho->status = status;
}

int cabecalho_getRRN(Cabecalho* cabecalho) {
    if (!cabecalho) return INTNULL;
    return cabecalho->rrn;
}
void cabecalho_setRRN(Cabecalho* cabecalho, int rrn) {
    if (!cabecalho) return;
    cabecalho->rrn = rrn;
}

int cabecalho_getInseridos(Cabecalho* cabecalho) {
    if (!cabecalho) return INTNULL;
    return cabecalho->inseridos;
}
void cabecalho_setInseridos(Cabecalho* cabecalho, int inseridos) {
    if (!cabecalho) return;
    cabecalho->inseridos = inseridos;
}

int cabecalho_getRemovidos(Cabecalho* cabecalho) {
    if (!cabecalho) return INTNULL;
    return cabecalho->removidos;
}
void cabecalho_setRemovidos(Cabecalho* cabecalho, int removidos) {
    if (!cabecalho) return;
    cabecalho->removidos = removidos;
}

int cabecalho_getAtualizados(Cabecalho* cabecalho) {
    if (!cabecalho) return INTNULL;
    return cabecalho->atualizados;
}
void cabecalho_setAtualizados(Cabecalho* cabecalho, int atualizados) {
    if (!cabecalho) return;
    cabecalho->atualizados = atualizados;
}