#ifndef UTILS_NUMERIC_H_
#define UTILS_NUMERIC_H_

#include <string>
#include "core_types.h"
#include "extra_types.h"

using namespace std;

targetSizeT string2tsizet(string str);
targetSizeT string2tsizet(char *c_str); // оболочка над targetSizeT string2tsizet(string str);

string showHandleState(handleState hs);
string showFileLine(FileLine fl);
string showHandle(targetSizeT tsizet);
string showEvolution(evolutionState es);

#endif /* UTILS_NUMERIC_H_ */
