#include <fstream>
#include "utils_linefetch.h"

CVLineFetcher::CVLineFetcher() {}
CVLineFetcher::~CVLineFetcher() {}

///////////////////////////////////////////

CFileLineFetcher::CFileLineFetcher() {
    c_lineIsPeekedLine = false;
    lineCounter = 0;
}

int CFileLineFetcher::attach(char* file) {
    trace.open(file);

    if(!trace.good()) return -1;

    return 0;
}

int CFileLineFetcher::attach(int s) {
    (void)s;
    throw "CFileLineFetcher::attach(int s); called !!!";
}

LineCounter CFileLineFetcher::fetchLine(string &str) {
    if(c_lineIsPeekedLine) {
        str = c_line;
        c_lineIsPeekedLine = false;
        return lineCounter;
    }

    trace.getline(c_line, clinefetcher_ss);

    if (trace.eof()) {
        throw "eof";
    }

    if (!trace.good()) throw "i/o error";

    lineCounter++;

    str = c_line;

    return lineCounter;
}

void CFileLineFetcher::peekLine(string &str) {
    if(c_lineIsPeekedLine) throw "malfunction"; // см. описание CVLineFetcher::peekLine()
    fetchLine(str);
    c_lineIsPeekedLine = true;
}

//////////////////////////////////////////////////////////////////////////////
#include <sys/types.h>  /* recv() */
#include <sys/socket.h>
#include <netdb.h>      /* struct sockaddr_in */
#include <netinet/tcp.h> /* TCP_NODELAY */
#include <unistd.h>     /* close() */

/* No more receptions. */
#define LTN_SHUT_RD (int)0

/* No more transmissions. */
#define LTN_SHUT_WR (int)1

/* No more receptions or transmissions. */
#define LTN_SHUT_RDWR   (int)2

int ltn_CreateListen_s(unsigned short port) {
    struct sockaddr_in ssin;

    int on = 1; /* Осторожно: используется в разных функциях */

    static const size_t sizeof_on = sizeof(on);
    static const size_t sizeof_ssin = sizeof(ssin);

    int s;

    if ((s = socket (AF_INET, SOCK_STREAM, 0)) < 0) {
        perror ("socket()");
        return -1;
    }

#ifndef LTN_FORBID_SETSOCKOPT
    if (setsockopt (s, SOL_SOCKET, SO_REUSEADDR, &on, sizeof_on) < 0) {
        perror ("setsockopt()");

        /* сокет выделен, но опция не проканала, но сокет надо закрыть все-же... */
        shutdown (s, LTN_SHUT_RDWR);
        close (s);

        return -1;
    }
    if (setsockopt (s, IPPROTO_TCP, TCP_NODELAY, &on, sizeof_on) < 0) {
        perror ("setsockopt()");

        /* сокет выделен, но опция не проканала, но сокет надо закрыть все-же... */
        shutdown (s, LTN_SHUT_RDWR);
        close (s);

        return -1;
    }
#endif

    ssin.sin_family = AF_INET;/* AF_INET OR AF_LOCAL... */
    ssin.sin_addr.s_addr = INADDR_ANY;/* INADDR_ANY OR INADDR_LOOPBACK... */
    ssin.sin_port = htons (port);

    if (bind (s, (struct sockaddr *) &ssin, sizeof_ssin) < 0) {
        perror ("bind()");
        /* сокет выделен, но foo() не проканала, но сокет надо закрыть все-же... */
        shutdown (s, LTN_SHUT_RDWR);
        close (s);

        return -1;
    }

    if (listen (s, 5) < 0) {
        perror ("listen()");
        shutdown (s, LTN_SHUT_RDWR);
        close (s);

        return -1;
    }
    else return s;
}

int ltn_Accept_s(int listen_socket) {
    struct sockaddr_in ssin;

    socklen_t sizeof_ssin = sizeof (ssin);
    int retval;

    retval = accept (listen_socket, (struct sockaddr *) &ssin,
                     (socklen_t *) &sizeof_ssin);
    if(retval == -1){
        perror("accept()");
        return retval;
    }
    else return retval;
}

int ltn_Readline(void *buff, size_t buff_len, int sock){
    size_t	i;
    ssize_t sign_left_i;
    char*	byte_buff;

    byte_buff = (char*)buff; i = 0;

    while(1){
        if(recv(sock, &byte_buff[i], 1, 0) < 1) return -1;
        if((byte_buff[i] == '\n') || (buff_len == i + 1))
        {
            byte_buff[i] = 0;

            /* Отлов старорежимного досовского 0x0d перед 0x0a
             * ловим \r перед \n и перебиваем нулём \r */
            sign_left_i = i - 1;
            if(sign_left_i >= 0)
            {
                if (byte_buff[sign_left_i] == '\r')
                {
                    byte_buff[sign_left_i] = 0; return sign_left_i;
                }
                else {/* игнорировать отсутствие 0x0d */ }
            }
            else
            { /* есть только '\n'; игнорируем */ }

            return i;
        }
        i++;
    }
}

CSockLineFetcher::CSockLineFetcher() {
    c_lineIsPeekedLine = false;
    lineCounter = 0;
}

int CSockLineFetcher::attach(char* file) {
    (void)file;
    throw "CSockLineFetcher::attach(char* file); called !!!";
}

int CSockLineFetcher::attach(int sock) {
    if(sock) { m_sock = sock; return 0; }
    else return sock;
}

LineCounter CSockLineFetcher::fetchLine(string &str) {
    if(c_lineIsPeekedLine) {
        str = c_line;
        c_lineIsPeekedLine = false;
        return lineCounter;
    }

    int nCharsRead = ltn_Readline(c_line, clinefetcher_ss, m_sock);
    //fprintf(stderr, "nCharsRead = %d\n", nCharsRead);

    if(-1 == nCharsRead) throw "i/o error";

    lineCounter++;

    str = c_line;

    return lineCounter;
}

void CSockLineFetcher::peekLine(string &str) {
    if(c_lineIsPeekedLine) throw "malfunction"; // см. описание CVLineFetcher::peekLine()
    fetchLine(str);
    c_lineIsPeekedLine = true;
}

//////////////////////////////////////////////////////////////////////////////
#include <string.h> // strdup()
#include <stdlib.h> // free()

// TODO: добавить возможность использования проблов так:
// слово1 "второе, да" слово3.
// TODO: неразличать несколько разделительных символов подряд.
size_t splitLine(string src, splitted_line_t *dst) {
    size_t i, src_len = src.length();
    char c;

    dst->chunk.clear();

    if(0 == src_len) return 0; // nothing to split

    vector<size_t> spacePlace;

    for(i = 0; i < src_len; i++) {
        c = src.at(i);
        if(' ' == c || '\t' == c) spacePlace.push_back(i);
    }

    size_t spaces_len = spacePlace.size();
    if(0 == spaces_len) { // src contains only one word
        string tmp_s(src);
        dst->chunk.push_back(tmp_s);
        return 1;
    }

    char *copy_of_string;
    copy_of_string = strdup(src.c_str());
    if(!copy_of_string) return 0; // no mem - no result

    for(i = 0; i < spaces_len; i++) {
        string tmp_s;
        //c = copy_of_string[spacePlace[i]];
        copy_of_string[spacePlace[i]] = 0;
        tmp_s = copy_of_string + (i == 0 ? 0 : spacePlace[i - 1] + 1);
        // copy_of_string[spacePlace[i]] = c;// восстанавливать не обязательно
        dst->chunk.push_back(tmp_s);
    }

    // добавляем последний сегмент
    string tmp_s;
    tmp_s = copy_of_string + spacePlace[spacePlace.size() - 1] + 1;
    dst->chunk.push_back(tmp_s);

    free(copy_of_string);

    return dst->chunk.size(); // а тут можно было бы возвращать i(-1?)
    //return i;
}

//#ifdef ART_DEBUG
#include <iostream>

void dump(splitted_line_t *splitted_line) {
    size_t i, l = splitted_line->chunk.size();

    cout << "Splitted line in " << l << " chunks:" << endl;
    for(i = 0; i < l; i++) {
        cout << '[' << i << "] = '" << splitted_line->chunk[i] << '\'' << endl;
    }
}
//#endif
