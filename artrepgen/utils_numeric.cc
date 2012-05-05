#include <stdio.h> // sprintf()
#include <assert.h> // assert()
#include "utils_numeric.h"

// Допущение что буквы все большие, т.к. они большими генерируются.
targetSizeT string2tsizet(string str) {
    if(0 == str.length() || str.length() > 16) throw "incoming value is very strange";

    targetSizeT num = 0;

    const char* c_str = str.c_str();
    char c;

    size_t i, l;
    for(i = 0, l = str.length(); i < l; i++) {
        c = c_str[l - (i + 1)];

        if(isdigit(c)) {
            num |= ((c - '0') << (i * 4));
        } else if(c >= 'A' && c <= 'F') {
            num |= (((c - 'A') + 10) << (i * 4));
        } else throw "non xdigit";
    }

    return num;
}

targetSizeT string2tsizet(char *c_str) {
    string s = c_str;
    return string2tsizet(s);
}

/////////////////////////////////////////////////////////////////////////////

string showHandleState(handleState hs) {
    char buf[32];

    if(validHandle == hs) return "valid";
    else if(badHandle == hs) return "bad";
    else {
        string s = "<unexpected state>. Internal error. Report to developers. handleState is ";
        s += hs;
        s += ". at ";
        s += __FILE__;
        s += ":";
        sprintf(buf, "%u", __LINE__);
        s += buf;
        return s;
    }
}

string showFileLine(FileLine fl) {
    if(fl.file != fl.line) {
        string s = fl.file;
        s += ":";
        s += fl.line;
        return s;
    }
    else return "";
}

string showHandle(targetSizeT tsizet) {
    char buf[17];
    string s;
    unsigned char rest;

    size_t i, l;
    for(i = 0, l = 16; i < l; i++) {
        rest = tsizet % 16;
        tsizet /= 16;

        if (rest < 10) buf[15 - i] = '0' + rest;
        else           buf[15 - i] = 'A' + (rest - 10);
    }
    buf[16] = 0;

    s = buf;
    return s;
}

string showEvolution(evolutionState es) {
    if(allocated == es) return "allocated";
    else if(deallocated == es) return "deallocated";
    else {
        assert(false);
    }
    return ""; // dummy step to shutup compiler
}
