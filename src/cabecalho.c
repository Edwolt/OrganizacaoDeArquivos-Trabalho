#include "cabecalho.h"

struct Cabecalho {
    bool status;
    int rrn;
    int inseridos;
    int removidos;
    int atualizados;
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