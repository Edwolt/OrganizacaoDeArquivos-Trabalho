#include "criterio.h"

struct _criterio {
    Dupla** duplas;
    int tam;
};

Criterio* criterio_criar(Dupla** duplas, int n) {
    if (!duplas) return NULL;  // Nao recebeu parametro

    Criterio* criterio = (Criterio*)malloc(sizeof(Criterio));
    if (!criterio) return NULL;  // Falha ao criar criterios

    criterio->tam = n;
    criterio->duplas = duplas;

    return criterio;
}

Criterio* criterio_criarVazio() {
    Criterio* criterio = (Criterio*)malloc(sizeof(Criterio));
    if (!criterio) return NULL;  // Falha ao criar criterios

    criterio->tam = 0;
    criterio->duplas = NULL;

    return criterio;
}

Criterio* criterio_criarStdin() {
    int i;  //Iteradores

    Criterio* criterio = (Criterio*)malloc(sizeof(Criterio));
    if (!criterio) return NULL;  // Falha ao criar criterios

    scanf("%d", &criterio->tam);

    char* campo;
    char* valor;

    criterio->duplas = (Dupla**)malloc(criterio->tam * sizeof(Dupla*));
    if (!criterio) {  // Falha ao alocar vetor de duplas
        return NULL;
    }

    for (i = 0; i < criterio->tam; i++) {
        campo = (char*)malloc(STR_TAM * sizeof(char));
        if (!campo) {  // Falha ao alocar string campo
            return NULL;
        }

        valor = (char*)malloc(STR_TAM * sizeof(char));
        if (!valor) {  // Falha ao alocar string valor
            free(campo);  // Falha ao alocar string campo
            return NULL;
        }

        scanf("%s ", campo);
        trim(campo);

        scan_quote_string(valor);
        trim(valor);

        criterio->duplas[i] = dupla_criar(campo, valor);
        if (!criterio->duplas[i]) {  // Se nao foi possivel alocar
            for (i--; i >= 0; i--) {  // Apaga o que ja tinha sido alocado
                dupla_apagar(&criterio->duplas[i]);
            }
            return NULL;
        }
    }

    return criterio;
}

void criterio_apagar(Criterio** criterio) {
    if (!criterio || !*criterio) return;  // Objeto ja foi apagado

    int i;  //Iteradores

    for (i = 0; i < (*criterio)->tam; i++) dupla_apagar(&(*criterio)->duplas[i]);
    free((*criterio)->duplas);
    free(*criterio);
    *criterio = NULL;
}

//* ============================= *//
//* ===== Getters e Setters ===== *//
//* ============================= *//

Dupla** criterio_getArray(Criterio* criterio, int* n) {
    if (!criterio) {  // Objeto nao existe
        if (n) *n = 0;
        return NULL;
    }

    if (n) *n = criterio->tam;
    return criterio->duplas;
}

void criterio_setArray(Criterio* criterio, Dupla** duplas, int n) {
    int i;  //Iteradores;

    for (i = 0; i < criterio->tam; i++) dupla_apagar(&criterio->duplas[i]);  // Apaga duplas
    criterio->duplas = duplas;
    criterio->tam = n;
}