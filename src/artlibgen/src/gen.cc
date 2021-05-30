#include "gen.h"

int Domaintoid(CTemplate tpl, string domname, size_t *out_id) {
    size_t i, l;
    for(i = 0, l = tpl.domains.size(); i < l; i++) {
        if(tpl.domains[i].name == domname) {
            *out_id = i;
            return 0;
        }
    }

    return -1;
}
