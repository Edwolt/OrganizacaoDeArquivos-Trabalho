#include "registro.h"

struct _registro {
    int idNascimento;      // Int
    int idadeMae;          // Int
    char* dataNascimento;  // string: AAAA-MM-DD
    char sexoBebe;         // '0', '1', ou '2'
    char* estadoMae;       // 2 chars
    char* estadoBebe;      // 2 chars

    // Campos de Tamanho Variável
    char* cidadeMae;   // string, maximo 105
    char* cidadeBebe;  // string, maximo 105
};

Registro* registro_new(int idNascimento,
                       int idadeMae, char* dataNascimento,
                       char sexoBebe,
                       char* estadoMae, char* estadoBebe,
                       char* cidadeMae, char* cidadeBebe) {
    Registro* registro = (Registro*)malloc(sizeof(Registro));
    if (registro) {
        registro->idNascimento = idNascimento;
        registro->idadeMae = idadeMae;
        registro->dataNascimento = dataNascimento;
        registro->sexoBebe = sexoBebe;
        registro->estadoMae = estadoMae;
        registro->estadoBebe = estadoBebe;
        registro->cidadeMae = cidadeMae;
        registro->cidadeBebe = cidadeBebe;
    }
    return registro;
}

Registro* registro_new() {
    Registro* registro = (Registro*)malloc(sizeof(Registro));
    if (registro) {
        registro->idNascimento = INTNULL;
        registro->idadeMae = INTNULL;
        registro->dataNascimento = NULL;
        registro->sexoBebe = '\0';
        registro->estadoMae = NULL;
        registro->estadoBebe = NULL;
        registro->cidadeMae = NULL;
        registro->cidadeBebe = NULL;
    }
    return registro;
}

//* ============================ *//
//* ===== Getter e Setters ===== *//
//* ============================ *//

int registro_getIdNascimento(Registro* registro) { return registro->idNascimento; }
void registro_setIdNascimento(Registro* registro, int idNascimento) { registro->idNascimento = idNascimento; }

int registro_getIdadeMae(Registro* registro) { return registro->idadeMae; }
void registro_setIdadeMae(Registro* registro, int idadeMae) { registro->idadeMae = idadeMae; }

char* registro_getDataNascimento(Registro* registro) { return registro->dataNascimento; }
void registro_setDataNascimento(Registro* registro, char* dataNascimento) { registro->dataNascimento = dataNascimento; }

char registro_getSexoBebe(Registro* registro) { return registro->sexoBebe; }
void registro_setSexoBebe(Registro* registro, char sexoBebe) { registro->sexoBebe = sexoBebe; }

char* registro_getEstadoMae(Registro* registro) { return registro->estadoMae; }
void registro_setEstadoMae(Registro* registro, char* estadoMae) { registro->estadoMae = estadoMae; }

char* registro_getEstadoBebe(Registro* registro) { return registro->estadoBebe; }
void registro_setEstadoBebe(Registro* registro, char* estadoBebe) { registro->estadoBebe = estadoBebe; }

char* registro_getCidadeMae(Registro* registro) { return registro->cidadeMae; }
void registro_setCidadeMae(Registro* registro, char* cidadeMae) { registro->cidadeMae = cidadeMae; }

char* registro_getCidadeBebe(Registro* registro) { return registro->cidadeBebe; }
void registro_setCidadeBebe(Registro* registro, char* cidadeBebe) { registro->cidadeBebe = cidadeBebe; }
