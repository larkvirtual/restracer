#include <iostream> // cerr
#include <stdlib.h> // exit()
#include "gen_xml_sax_read.h" // xml_metachunk
#include "ccloader.h"

int ccload(xml_metachunk chunk, local_record_t *records, size_t nRecords) {
    size_t i, l;

    for(i = 0, l = nRecords; i < l; i++) {
        switch(records[i].type) {
        case INT: {exit(-2); break; } // TODO: implement
        case DBL: {exit(-2); break; } // TODO: implement
        case CSTR: {exit(-2); break; } // TODO: implement
        case SIZET: {
            // TODO: проверить границы правой части
            *(size_t*) records[i].address = atoll(chunk.element.fetchValue(records[i].name).c_str());
            break;
        }
        case CCSTR: {
            *(string*) records[i].address = chunk.element.fetchValue(records[i].name);
            break;
        }
        case CCBOOL: {
            string s = chunk.element.fetchValue(records[i].name);
            if("true" == s) *(bool*) records[i].address = true;
            else if("false" == s) *(bool*) records[i].address = false;
            else {
                cerr << "unexpected value '" << s << "' at " << __FILE__ << ":"
                     << __LINE__ << endl;
            }
            break;
        }
        default: {exit(-2); break; } // TODO: implement
        }
    }

    return 0;
}
