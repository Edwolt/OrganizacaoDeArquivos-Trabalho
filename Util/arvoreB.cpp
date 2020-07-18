// Utilitario para imprimir o arquivo de indice em forma de arvore

#include <iostream>
#include "stdio.h"

using namespace std;

using namespace std;

struct Chave {
    int id;
    int dado;
};

struct Pagina {
    Chave chaves[5];
    int pont[6];
};

FILE* f;

char lc() {
    char c;
    fread(&c, sizeof(char), 1, f);
    return c;
}

int li() {
    int n;
    fread(&n, sizeof(int), 1, f);
    return n;
}

void seek(int rrn) {
    fseek(f, 72 * (rrn + 1), SEEK_SET);
}

void imprime(int rrn, int d) {
    if (rrn == -1) return;

    int n;

    seek(rrn);
    for (int i = 0; i < d; i++) printf("|");
    cout << rrn << ": [nivel: " << li() << " n: " << (n = li()) << "] ";

    Pagina p;
    for (int i = 0; i < 5; i++) p.chaves[i] = {li(), li()};
    for (int i = 0; i < 6; i++) p.pont[i] = li();

    cout << p.pont[0];
    for (int i = 0; i < n; i++) cout << (i < n ? " {" : " (") << p.chaves[i].id << ", " << p.chaves[i].dado << (i < n ? "} " : ") ") << p.pont[i + 1] << " ";
    cout << endl;

    for (int i = 0; i <= n; i++) {
        imprime(p.pont[i], d + 1);
    }
}
void imprime(int rrn) {
    imprime(rrn, 0);
}

int main(int argc, char const* argv[]) {
    f = fopen(argv[1], "rb");

    char c;
    char lixo[54];

    int raiz;

    cout << "status: " << lc() << ", ";
    cout << "raiz: " << (raiz = li()) << ", ";
    cout << "niveis: " << li() << ", ";
    cout << "prox: " << li() << ", ";
    cout << "chave: " << li() << endl;
    fread(&lixo, 1, 55, f);

    imprime(raiz);

    fclose(f);
    return 0;
}
