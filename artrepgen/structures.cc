#include <iostream>
#include <stdio.h> // printf()
#include "utils_numeric.h"
#include "structures.h"

#ifdef ART_DEBUG
void handleInfo::dump(void) {
    cout << "allocationPlace   = " << showFileLine(allocationPlace) << endl;
    cout << "deallocationPlace = " << showFileLine(deallocationPlace) << endl;

    cout << "handle = 0x" << showHandle(handle) << endl;

    cout << "lenght = " << lenght << " (means only for float domains)" << endl;
    cout << "retval_state = " << retval_state << endl;
    cout << "handle_state = " << showHandleState(handle_state) << endl;
    cout << "evolution    = " << showEvolution(evolution) << endl;
}
#endif

#ifdef ART_DEBUG
void domainInfo::dump(void) {
}
#endif

#ifdef ART_DEBUG
void traceInfo::dump(void) {
    size_t i, l;
    for(i = 0, l = tpl.domains.size(); i < l; i++) {
        cout << "Domain # dump:" << endl;
        domains[i].dump();
    }
}
#endif
