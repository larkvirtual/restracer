#ifndef UTILS_LINEFETCH_H_
#define UTILS_LINEFETCH_H_

#include <string>
#include <fstream>

using namespace std;

#define c_line_len 256
const streamsize clinefetcher_ss = c_line_len - 1;

typedef unsigned long long LineCounter;

class CVLineFetcher {
public:
    CVLineFetcher();
    virtual ~CVLineFetcher();

    virtual int attach(char* file) = 0;
    virtual int attach(int sock) = 0;

    // Возвращает очередную строку
    virtual LineCounter fetchLine(string &str) = 0; // throw "eof" | "i/o error"

    // Позволяет заглянуть на следующую строку. Строка не извлекается.
    // Два раза подряд эту функцию вызывать нельзя, иначе будет вызвано исключение "malfunction"
    // В остальном ведёт себя также как fetchLine
    virtual void peekLine(string &str) = 0;
};

class CFileLineFetcher : public CVLineFetcher {
public:
    CFileLineFetcher();

    int attach(char* file);
    int attach(int sock);

    // Возвращает очередную строку
    LineCounter fetchLine(string &str); // throw "eof" | "i/o error"

    // Позволяет заглянуть на следующую строку. Строка не извлекается.
    // Два раза подряд эту функцию вызывать нельзя, иначе будет вызвано исключение "malfunction"
    // В остальном ведёт себя также как fetchLine
    void peekLine(string &str);

private:

    bool c_lineIsPeekedLine;
    char c_line[c_line_len];
    ifstream trace;
    LineCounter lineCounter;
};

class CSockLineFetcher : public CVLineFetcher {
public:
    CSockLineFetcher();

    int attach(char* file);
    int attach(int sock);

    // Возвращает очередную строку
    LineCounter fetchLine(string &str); // throw "eof" | "i/o error"

    // Позволяет заглянуть на следующую строку. Строка не извлекается.
    // Два раза подряд эту функцию вызывать нельзя, иначе будет вызвано исключение "malfunction"
    // В остальном ведёт себя также как fetchLine
    void peekLine(string &str);

private:

    bool c_lineIsPeekedLine;
    char c_line[c_line_len];
    int m_sock;
    LineCounter lineCounter;
};

int ltn_CreateListen_s(unsigned short port);
int ltn_Accept_s(int listen_socket);
//////////////////////////////////////////////////////////////////////////////
#include <vector>

typedef struct {
    vector<string> chunk;
}splitted_line_t;

// returns number of chunks
// separator for chunks is ' ' or \t
// dst will be cleared before operating
size_t splitLine(string src, splitted_line_t *dst);

//#ifdef ART_DEBUG
void dump(splitted_line_t *splitted_line);
//#endif
//////////////////////////////////////////////////////////////////////////////

#endif /* UTILS_LINEFETCH_H_ */
