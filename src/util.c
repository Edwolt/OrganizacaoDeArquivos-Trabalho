#include "util.h"

bool ehEscrita(const char* modes) {
    int i;
    for (i = 0; modes[i] != '\0'; i++) {
        if (modes[i] == 'w' ||
            modes[i] == 'a' ||
            modes[i] == '+') {
            return true;
        }
    }
    return false;
}