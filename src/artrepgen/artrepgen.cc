#include <iostream>
#include <string>
#include <string.h> // strcmp()
#include <stdlib.h> // atoi()
#include "tracer.h"
#include "utils_linefetch.h"
#include "art_defs.h"

using namespace std;

void usageexample(string appname) {
    cout << "Usage: " << appname << " --file " << "/path/to/tracefile" << endl;
    cout << "Usage: " << appname << " --sock " << "port" << endl;
    cout << "Usage: " << appname << " --version" << endl;
    cout << "Example: " << appname << " --file " << "tracefile.out" << endl;
    cout << "Example: " << appname << " --sock " << "12345" << endl;
}

int main(int argc, char **argv) {
    if(1 == argc) {
        usageexample(argv[0]);
        return 0;
    }
    else if(2 == argc && (0 == strcmp(argv[1], "--version"))) {
        cout << "ART v" << ART_VERSION_BRANCH << ART_VERSION_SUB <<
                ART_VERSION_EXTRA << endl;
        return 0;
    }
    else if(3 == argc && (0 == strcmp(argv[1], "--file"))) {
        CVLineFetcher *fetcher;

        fetcher = new CFileLineFetcher;
        fetcher->attach(argv[2]); // FIXME: проверить возвращаемое значение


        bool ret_val = false;
        try
        {
            ret_val = trace(fetcher);
        }
        catch(const char *msg)
        {
            cerr << "Received exception: \"" << msg <<"\"\nIncorrect input file\n" 
            << "Perhaps the program is stopped incorrectly.\n";
        }

        return ret_val;
    }
    else if(3 == argc && (0 == strcmp(argv[1], "--sock"))) {
        int listenSocket = ltn_CreateListen_s(atoi(argv[2]));
        if(listenSocket < 0) {
            cerr << "Can't listen on port " << argv[2] << endl;
            return -1;
        }

        int connectedSocket = ltn_Accept_s(listenSocket);
        if(connectedSocket < 0) {
            cerr << "Can't accept socket" << endl;
            return -1;
        }

        CVLineFetcher *fetcher;

        fetcher = new CSockLineFetcher();
        fetcher->attach(connectedSocket);

        return trace(fetcher);
    }
    else usageexample(argv[0]);

    return 0;
}
