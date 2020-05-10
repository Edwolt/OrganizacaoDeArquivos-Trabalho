#include "csv.h"

CSV* csv_open(path) {
    if (path) {
        CSV* csv = fopen(path, "r");
        fscanf(csv, "%*[^\n] ");  // Le uma linha do arquivo sem salvar o valor
        return csv;
    };
    return NULL;
}

void csv_del(CSV** csv) {
    // Verifica se objeto já foi apagado
    if (!csv) return;
    if (!*csv) {
        *csv = NULL;
        return;
    }

    fclose(csv);
    *csv = NULL;
}