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

Criterio* criterio_criarDoStdin() {
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

        scanf(" %s", campo);

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

bool criterio_satisfaz(Criterio* criterio, Registro* registro) {
    if (!criterio || !registro) return NULL;  // Algum dos objetos nao existe
    int i;  // Iteradores
    char* reg;  // Auxilia a guardar uma string do registro
    char* dup;  // Auxilia a guardar uma string da dupla

    for (i = 0; i < criterio->tam; i++) {
        switch (dupla_getCampo(criterio->duplas[i])) {
            // Campos Inteiros
            case DUPLA_IDNASCIMENTO:
                if (registro_getIdNascimento(registro) != dupla_getInteiro(criterio->duplas[i])) return false;
                break;

            case DUPLA_IDADEMAE:
                if (registro_getIdadeMae(registro) != dupla_getInteiro(criterio->duplas[i])) return false;
                break;

            // Campos Character
            case DUPLA_SEXOBEBE:
                if (registro_getSexoBebe(registro) != dupla_getCaracter(criterio->duplas[i])) return false;
                break;

            // Campos String
            case DUPLA_DATANASCIMENTO:
                reg = registro_getDataNascimento(registro);
                dup = dupla_getString(criterio->duplas[i]);

                if ((!dup != !reg)) return false;  // (dupla == NULL) xor (campo == NULL)
                if (!reg) continue;  // Ambos sao nulos

                if (strcmp(reg, dup) != 0) return false;
                break;

            case DUPLA_ESTADOMAE:
                reg = registro_getEstadoMae(registro);
                dup = dupla_getString(criterio->duplas[i]);

                if ((!dup != !reg)) return false;  // (dupla == NULL) xor (campo == NULL)
                if (!reg) continue;  // Ambos sao nulos

                if (strcmp(reg, dup) != 0) return false;
                break;

            case DUPLA_ESTADOBEBE:
                reg = registro_getEstadoBebe(registro);
                dup = dupla_getString(criterio->duplas[i]);

                if ((!dup != !reg)) return false;  // (dupla == NULL) xor (campo == NULL)
                if (!reg) continue;  // Ambos sao nulos

                if (strcmp(reg, dup) != 0) return false;
                break;

            case DUPLA_CIDADEMAE:
                reg = registro_getCidadeMae(registro);
                dup = dupla_getString(criterio->duplas[i]);

                if ((!dup != !reg)) return false;  // (dupla == NULL) xor (campo == NULL)
                if (!reg) continue;  // Ambos sao nulos

                if (strcmp(reg, dup) != 0) return false;
                break;

            case DUPLA_CIDADEBEBE:
                reg = registro_getCidadeBebe(registro);
                dup = dupla_getString(criterio->duplas[i]);

                if ((!dup != !reg)) return false;  // (dupla == NULL) xor (campo == NULL)
                if (!reg) continue;  // Ambos sao nulos

                if (strcmp(reg, dup) != 0) return false;
                break;

            default:
                return false;
        }
    }

    return true;
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