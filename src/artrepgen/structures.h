#ifndef STRUCTURES_H_
#define STRUCTURES_H_

#include <vector>
#include "core_types.h"
#include "extra_types.h"
#include "art_struct.h" // class CTemplate

class handleInfo {
public:
    FileLine allocationPlace, deallocationPlace;
    targetSizeT handle;
    targetSizeT lenght; // имеет смысл только для float-доменов
    bool        retval_state;
    handleState handle_state;
    evolutionState evolution;
#ifdef ART_DEBUG
    void dump(void);
#endif
};

class domainInfo {
public:
    vector <handleInfo> handles;
#ifdef ART_DEBUG
    void dump(void);
#endif
};

class traceInfo {
public:
    vector <domainInfo> domains;
    CTemplate tpl;
#ifdef ART_DEBUG
    void dump(void);
#endif
};

#endif /* STRUCTURES_H_ */
