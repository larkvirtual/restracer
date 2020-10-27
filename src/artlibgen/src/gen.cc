#include "gen.h"

// <<Новая>> реализация
size_t Domaintoid(CTemplate tpl, string domname) {
    size_t i, l;
    for(i = 0, l = tpl.domains.size(); i < l; i++) {
        if(tpl.domains[i].name == domname) return i;
    }
    
    return -1; // Сигнал игнорировать домен
}
