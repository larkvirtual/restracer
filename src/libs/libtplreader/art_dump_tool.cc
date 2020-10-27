#include <string>
#include "art_dump_tool.h"

std::string bool2yn(bool value) {
    if(value) return "true";
    else return "false";
}
