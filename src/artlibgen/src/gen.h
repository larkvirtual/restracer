#ifndef GEN_H_
#define GEN_H_

#include <stddef.h> // size_t
#include "art_struct.h"

// -1 if not found, out_id undefined
//  0 if not found, out_id defined
int Domaintoid(CTemplate tpl, string domname, size_t *out_id);

#endif /*GEN_H_*/
