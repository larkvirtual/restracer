#include <iostream>
#include <stdlib.h> // abort() // её может не быть в финальной сборке
#include <assert.h> // assert() // её может не быть в финальной сборке
#include <string.h> // strncmp()
#include <string>
#include "tracer.h"
#include "utils_linefetch.h"
#include "structures.h"
#include "core_types.h"
#include "utils_numeric.h"
#include "gen_xml_sax_read.h" // saxparser_templateLoader()

bool readHeader(CVLineFetcher *fetcher, traceInfo &ti) {
    string line;

    /////////////////////////////////////////////////////////////////////////
    fetcher->fetchLine(line);

    if (line != "ART TRACING BEGIN") {
        cout << "Unexpected data: '" << line << "'" << endl;
        return -1;
    }
    /////////////////////////////////////////////////////////////////////////
    fetcher->fetchLine(line);

    if(line != "ART PROTOCOL 01") {
        cout << "Unexpected data: '" << line << "'" << endl;
        return -1;
    }

    /* TODO: FIXME: при таком подходе возможны гонки */
    ofstream filedtpl("template.tmp");

    fetcher->fetchLine(line);
    while (line != "</template>") {
        filedtpl << line << endl;
        fetcher->fetchLine(line);
    }

    filedtpl << line << endl;

    filedtpl.close();

    saxparser_templateLoader(ti.tpl, "template.tmp");

    // Добавляем соответствующее количество пустых объектов
    // типа domainInfo
    ti.domains.resize(ti.tpl.domains.size());

    return true;
}

typedef struct {
    bool   isValid;
    size_t domainID;
} domainBox;

domainBox checkDomainName(string domName, traceInfo &ti) {
    domainBox db;

    size_t i, l;
    for(i = 0, l = ti.tpl.domains.size(); i < l; i++) {
        if(domName == ti.tpl.domains[i].name) {
            db.isValid = true;
            db.domainID = i;
            return db;
        }
    }

    db.isValid = false;
    return db;
}

typedef enum {
    tAllocator, tOperator, tReallocator, tDeallocator, tFail
} torType;

torType checkTorType(string tor, domainBox &dombox, traceInfo &ti) {
    torType tt = tFail;

    CDomain *dominfo = &ti.tpl.domains[dombox.domainID];

    size_t i, l;

    // Исходя из того, что операторы чаще встречаются в коде, поиск по ним
    // производится в первую очередь
    for(i = 0, l = dominfo->operators.size(); i < l; i++) {
        if(tor == dominfo->operators[i].name ) { tt = tOperator; return tt; }
    }

    for(i = 0, l = dominfo->allocators.size(); i < l; i++) {
        if(tor == dominfo->allocators[i].name) { tt = tAllocator; return tt; }
    }

    for(i = 0, l = dominfo->reallocators.size(); i < l; i++) {
        if(tor == dominfo->reallocators[i].name) { tt = tReallocator; return tt; }
    }

    for(i = 0, l = dominfo->deallocators.size(); i < l; i++) {
        if(tor == dominfo->deallocators[i].name) { tt = tDeallocator; return tt; }
    }

    return tt;
}

FileLine getFileLine(CVLineFetcher *fetcher) {
    string line;
    FileLine fl;

    fetcher->fetchLine(line);
    fl.file = line;

    fetcher->fetchLine(line);
    fl.line = line;

    return fl;
}

////////////////////////////////////////////////////////////////////////////

void AllocatorEngine(traceInfo &ti, FileLine &fl, string torName, domainBox &dombox,
                     handleInfo &hInfo) {
    if(0 == hInfo.lenght && ti.tpl.domains[dombox.domainID].float_handle) {
        cout << "Warning: allocation zero bytes by " << ti.tpl.domains[dombox.domainID].name
             << ":" << torName << " at " << showFileLine(fl) << endl;
    }

    /////////////////////////////////////////////////////////////
    // Этого кода можно избежать, т.к. он затормаживать анализ
    // будет к O(n). Возможно нужно указать ключик типа
    // "no wild API checks"
    bool found = false;
    vector <size_t> exactPlaces;
    for(size_t i = 0, l = ti.domains[dombox.domainID].handles.size();
        i < l;
        i++) {
        if(hInfo.handle == ti.domains[dombox.domainID].handles[i].handle &&
                allocated == ti.domains[dombox.domainID].handles[i].evolution) {
            found = true;
            exactPlaces.push_back(i);
        }
    }

    if(found) {
        cout << "Error: allocated handle 0x" << showHandle(hInfo.handle) <<
                " from '" << ti.tpl.domains[dombox.domainID].name <<
                "'" <<
                " at " << showFileLine(hInfo.allocationPlace) <<
                " marked as already allocated at:";
        for(size_t i = 0, l = exactPlaces.size(); i < l; i++) {
            cout << " " << showFileLine(ti.domains[dombox.domainID].handles[i].allocationPlace);
        }
        cout << endl;
#ifdef ART_DEBUG

#endif
        //		return false;
    }
    else ti.domains[dombox.domainID].handles.push_back(hInfo);
    /////////////////////////////////////////////////////////////
}

bool transactionAnalyzerforAllocators(CVLineFetcher *fetcher, traceInfo &ti,
                                      FileLine &fl, string torName, domainBox &dombox, handleInfo &hInfo) {
    string line;
    LineCounter lineno;

    hInfo.allocationPlace = fl;

    if(ti.tpl.domains[dombox.domainID].float_handle) {
        fetcher->fetchLine(line);
        targetSizeT tsizet = string2tsizet(line);
        hInfo.lenght = tsizet;
    }

    // значение retval: OK | BAD или для calloc второй аргумент
    lineno = fetcher->fetchLine(line);

    if(16 == line.size()) {
        // это должно быть calloc()
        if(ti.tpl.domains[dombox.domainID].float_handle) {
            hInfo.lenght *= string2tsizet(line);
            // TODO: тут проверить на переполнение при умножении
        }
        else {
            cerr << "Strange error: domain '" << ti.tpl.domains[dombox.domainID].name
                 << "' isn't float, but allocator " << torName << " has 2nd argument"
                 << endl;
            return false;
        }

        lineno = fetcher->fetchLine(line);
    }

    if("OK" == line) {
        hInfo.retval_state = true;
    }
    else if("BAD" == line) {
        hInfo.retval_state = false;
    }
    else {
        cerr << "Unexpected value '" << line
             << "' instead of OK or BAD at line " << lineno << "."
             << endl;
        return false;
    }

    // значение хэндла: OK | BAD + хэндл
    fetcher->fetchLine(line);
    if(line.length() >= 4) {
        size_t shift;
        if(0 == strncmp("OK ", line.c_str(), 3)) {
            hInfo.handle_state = validHandle;
            shift = 3;
        }
        else if(0 == strncmp("BAD ", line.c_str(), 4)) {
            hInfo.handle_state = badHandle;
            shift = 4;
        }
        else {
            cerr << "Strange values at begging of line '" << line << "'." << endl;
            return false;
        }

        hInfo.handle = string2tsizet(line.c_str() + shift);
        hInfo.evolution = allocated;

        AllocatorEngine(ti, fl, torName, dombox, hInfo);
    } else {
        cerr << "Unexpected number of chars. Must be at least 4." << endl;
        return false;
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////

typedef struct {
    size_t domainID;
    bool isGood; // = not bad handle
    targetSizeT usedHandle;
} handle_extended_t;

typedef struct {
    size_t  index;
    bool    found;
} handleSearchResult;

handleSearchResult transactionHandleFinder(traceInfo &ti, domainBox &dombox,
                                           targetSizeT handle) {
    handleSearchResult hsr;
    hsr.found = false;

    if(0 == ti.domains[dombox.domainID].handles.size()) return hsr;

    // Вот тут, кстати, обратный цикл Ады очень подходит, не надо было бы
    // последний элемент обрабатывать как-то особо
    for (size_t i = ti.domains[dombox.domainID].handles.size() - 1;
         i > 0 && !hsr.found;
         i--) {
        if (ti.domains[dombox.domainID].handles[i].handle == handle) {
            hsr.found = true;
            hsr.index = i;
            return hsr;
        }
    }

    if (!hsr.found) { // проверяем самую первую запись
        if (ti.domains[dombox.domainID].handles[0].handle == handle) {
            hsr.found = true;
            hsr.index = 0;
        }
    }

    return hsr;
}

bool transactionAnalyzerforOperatorsPerHandle     (traceInfo &ti, domainBox &dombox,
                                             bool isGoodHandle, targetSizeT usedHandle,
                                             string torName, FileLine &fl) {
    if (isGoodHandle) {
        // это теоретически-верный ресурс
        handleSearchResult hsr;
        hsr = transactionHandleFinder(ti, dombox, usedHandle);
        if(hsr.found) {
            // Обнаружено использование ранее-известного ресурса
            if (allocated
                    == ti.domains[dombox.domainID].handles[hsr.index].evolution) {
                // TODO: тут обрабатываем статистику?
                ti.domains[dombox.domainID].handles[hsr.index].evolution = operated;
            } else if (operated
                       == ti.domains[dombox.domainID].handles[hsr.index].evolution) {
                // TODO: тут обрабатываем статистику?
            } else if (deallocated
                       == ti.domains[dombox.domainID].handles[hsr.index].evolution) {
                // 'Обнаружение использования ресурса после освобождения'
                cout << "Error: resource 0x"
                     << showHandle(usedHandle)
                     << " used by "
                     << ti.tpl.domains[dombox.domainID].name << ":"
                     << torName << " at " << showFileLine(fl)
                     << " was allocated at "
                     << showFileLine(
                            ti.domains[dombox.domainID].handles[hsr.index].allocationPlace)
                     << " and deallocated at "
                     << showFileLine(
                            ti.domains[dombox.domainID].handles[hsr.index].deallocationPlace)
                     << endl;
            }
        } else {
            // Обнаружено использование незахваченного ресурса
            cout << "Error: Unallocated resource usage 0x"
                 << showHandle(usedHandle)
                 << " by " << ti.tpl.domains[dombox.domainID].name
                 << ":" << torName << " at " << showFileLine(fl)
                 << endl;
            return false;
        }
    } else {
        cout << "Error 007: bad handle 0x"
             << showHandle(usedHandle)
             << " usage by " << ti.tpl.domains[dombox.domainID].name
             << ":" << torName << " at " << showFileLine(fl) << endl;
    }

    return true;
}

bool transactionAnalyzerforOperators(CVLineFetcher *fetcher, traceInfo &ti,
                                     FileLine &fl, string torName, domainBox &dombox) {
    size_t nChunks;
    string line;
    splitted_line_t sl;
    //cout << "LINE: '" << line << "' at line number: " << lineno << endl;

    (void)dombox;
    bool running = true;

    while(running) {
        fetcher->peekLine(line);
        if("-" == line) {
            running = false;
            break;
        } else {
            fetcher->fetchLine(line);
        }

        nChunks = splitLine(line, &sl);
        if(3 == nChunks) {
            bool isGoodHandle;
            domainBox handleDom; // домен конкретного значения

            handleDom = checkDomainName(sl.chunk[0], ti);
            if(!handleDom.isValid) {
                cerr << "invalid domain at " <<
                        __FILE__ << ":" << __LINE__ << endl;
                return false;
            }

            if("OK" == sl.chunk[1]) isGoodHandle = true;
            else if("BAD" == sl.chunk[1]) isGoodHandle = false;
            else {
                cerr << "unexpected value at "
                     << __FILE__ << ":" << __LINE__ << endl;
                return false;
            }

            targetSizeT usedHandle = string2tsizet(sl.chunk[2]);

            transactionAnalyzerforOperatorsPerHandle     (ti, handleDom,
                                                          isGoodHandle, usedHandle,
                                                          torName, fl);
        } else {
            cerr << "unexpected/wrong format at "
                 << __FILE__ << ":" << __LINE__ << endl;
            return false;
        }
    }

    fetcher->fetchLine(line);
    if ("-" == line) {}
    else
    fetcher->fetchLine(line);
    if ("-" == line) {
    }
//    cout << "LINE: " << line << endl;
//    exit(-1);
    return true;
}

////////////////////////////////////////////////////////////////////////////

void DeallocatorEngine(traceInfo &ti, FileLine &fl, string torName, domainBox &dombox,
                       handleInfo &hInfo) {
    handleSearchResult hsr;
    hsr = transactionHandleFinder(ti, dombox, hInfo.handle);
    if(!hsr.found) {
        cout << "Error: deallocation of unallocated " << showHandleState(hInfo.handle_state)
             << " handle 0x" << showHandle(hInfo.handle) <<
                " by " << ti.tpl.domains[dombox.domainID].name << ":" << torName <<
                " at " << showFileLine(hInfo.deallocationPlace) << endl;
        return;
    }
    // TODO: стоит рассматривать когда ресурс найден несколько раз?

    //if(hsr.found) {
    if(allocated == ti.domains[dombox.domainID].handles[hsr.index].evolution) {
        cout << "Warning: deallocation of resource 0x"
             << showHandle(hInfo.handle)
             << " by " << ti.tpl.domains[dombox.domainID].name << ":" << torName
             << " at " << showFileLine(fl)
             << " while allocated at "
             << showFileLine(ti.domains[dombox.domainID].handles[hsr.index].allocationPlace)
             << " which never operated." << endl;

        ti.domains[dombox.domainID].handles[hsr.index].evolution = deallocated;
        ti.domains[dombox.domainID].handles[hsr.index].deallocationPlace =
                hInfo.deallocationPlace;
    }
    else if(deallocated == ti.domains[dombox.domainID].handles[hsr.index].evolution) {
        cout << "Error: deallocation of handle 0x"
             << showHandle(hInfo.handle)
             << " by " << ti.tpl.domains[dombox.domainID].name << ":" << torName
             << " at " << showFileLine(hInfo.deallocationPlace) <<
                " while the handle allocated at " <<
                showFileLine(ti.domains[dombox.domainID].handles[hsr.index].allocationPlace)
             << " and freed at " <<
                showFileLine(ti.domains[dombox.domainID].handles[hsr.index].deallocationPlace)
             << "." << endl;
    }
    else if(operated == ti.domains[dombox.domainID].handles[hsr.index].evolution) {
        // Всё нормально, идёт освобождение ресурса, который был задействован в работе
        ti.domains[dombox.domainID].handles[hsr.index].evolution = deallocated;
        ti.domains[dombox.domainID].handles[hsr.index].deallocationPlace =
                hInfo.deallocationPlace;
    }
    else {
        cerr << "Internal error. Please, report to developers." << endl;
        assert(false);
    }
}

bool transactionAnalyzerforDeallocators(CVLineFetcher *fetcher, traceInfo &ti,
                                        FileLine &fl, string torName, domainBox &dombox, handleInfo &hInfo) {
    string line;
    LineCounter lineno;

    hInfo.deallocationPlace = fl;

    // значение хэндла: OK | BAD + хэндл
    fetcher->fetchLine(line);

    splitted_line_t sl;
    size_t nChunks;

    nChunks = splitLine(line, &sl);
    if(3 == nChunks) {
        if("OK" == sl.chunk[1]) {
            hInfo.handle_state = validHandle;
        }
        else if("BAD" == sl.chunk[1]) {
            hInfo.handle_state = badHandle;
        }
        else {
            cerr << "Unexpected value at "
                 << __FILE__ << ":" << __LINE__ << endl;
            return false;
        }

        hInfo.handle = string2tsizet(sl.chunk[2]);

        DeallocatorEngine(ti, fl, torName, dombox, hInfo);
    } else {
        cerr << "Unexpected numer of chunks at "
             << __FILE__ << ":" << __LINE__ << endl;
        return false;
    }

    try {
    lineno = fetcher->fetchLine(line);
    }
    catch(...) {
        cout << "Looks like target code execution died after calling "
             << torName << "(0x" << sl.chunk[2] << ") at " << showFileLine(fl)
             << endl;
        return false;
    }

    if("void" == line) {}
    else if("OK" == line) {}
    else {
        cerr << "Unexpected value '" << line << "' at " << lineno << "." << endl;
        return false;
    }

    lineno = fetcher->fetchLine(line);
    if("-" == line) {}
    else {
        cerr << "Unexpected value '" << line << "' at " << lineno << "." << endl;
        return false;
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////

bool transactionAnalyzerforReallocators(CVLineFetcher *fetcher, traceInfo &ti,
                                        FileLine &fl, string torName, domainBox &dombox, handleInfo &hInfo) {
    string line;
    LineCounter lineno;
    splitted_line_t sl;
    size_t nChunks;

    lineno = fetcher->fetchLine(line);
    nChunks = splitLine(line, &sl);
    // XXX ВПИСАТЬ СЮДА ПРОВЕРКУ НА ВЕРНОЕ ЗНАЧЕНИЕ!!!
    if(nChunks != 3) {
        cerr << "Unexpected value '" << line << "' at " << lineno << "." << endl;
        exit (-1);
    }
    // XXX

    if ("OK" == sl.chunk[1]) {
        // это теоретически-верный ресурс
        targetSizeT usedHandle = string2tsizet(sl.chunk[2]);

        hInfo.handle = usedHandle;

        /////////////////////////////////////////////////////////////
        // Этого кода можно избежать, т.к. он затормаживать анализ
        // будет к O(n). Возможно нужно указать ключик типа
        // "no wild API checks"
        bool found = false;
        vector <size_t> exactPlaces;
        for(size_t i = 0, l = ti.domains[dombox.domainID].handles.size();
            i < l;
            i++) {
            if(hInfo.handle == ti.domains[dombox.domainID].handles[i].handle &&
                    allocated == ti.domains[dombox.domainID].handles[i].evolution) {
                found = true;
                exactPlaces.push_back(i);
            }
        }

        if(found) {
            if(1 == exactPlaces.size()) {
                lineno = fetcher->fetchLine(line);

                if(16 == line.length()) {
                    // должно быть это второй аргумент: размер size_t
                    targetSizeT usedSize = string2tsizet(line);

                    if(0 == usedSize) {
                        // это аналог free(ptr);

                        DeallocatorEngine(ti, fl, torName, dombox, hInfo);

                        // считываем код возврата (обычно NULL => artlib пишет BAD)
                        // но вообще игнорируем
                        lineno = fetcher->fetchLine(line);

                        // также игнорируем сообщение типа BAD 0000000000000000
                        // уже о возвращаемом ресурсе
                        lineno = fetcher->fetchLine(line);

                        // на этом обработка данной ситуации заканчивается

                        return true;
                    } else {
                        // какой-то другой (~нормальный размер)

                        if (usedSize
                                == ti.domains[dombox.domainID].handles[exactPlaces[0]].lenght) {
                            cout << "Warning: reallocation handle 0x"
                                 << showHandle(usedHandle)
                                 << " has the same size (" << usedSize
                                 << " bytes) at" << showFileLine(fl) << endl;
                        }

                        lineno = fetcher->fetchLine(line);
                        if("OK" == line) {
                            // успешное перераспределение памяти

                            // далее считываем строку, в которой должно быть что-то типа
                            // OK 00007F8C8560D010
                            lineno = fetcher->fetchLine(line);

                            if (0 != strncmp("OK ", line.c_str(), 3)) {
                                cout << "Abnormal content \"" << line << "\" at tracefile line "
                                     << lineno << endl;
                                exit(-1);
                            }

                            // считываем значение указателя
                            targetSizeT probablyNewHandle = string2tsizet(line.c_str() + 3);

                            // TODO: подумать, нужна ли здесь какая-то статистика по случаям,
                            // когда возвращаемый указатель такой же, т.е. фактически
                            // почти ничего не изменилось

                            // обновляем информацию о новом размере, указателе и месте перерасределения
                            ti.domains[dombox.domainID].handles[exactPlaces[0]].lenght = usedSize;
                            ti.domains[dombox.domainID].handles[exactPlaces[0]].handle = probablyNewHandle;
                            ti.domains[dombox.domainID].handles[exactPlaces[0]].allocationPlace = fl;

                            // анализ данной ситуации закончен
                        } else if ("BAD") {
                            // неуспешное перераспределение памяти
                            // => и указатель будет NULL
                            // ничего не изменяем

                            // виртуальное считывание BAD 0000000000000000
                            lineno = fetcher->fetchLine(line);
                        } else {
                            cerr << "Unexpected format string \"" << line
                                 << "\" at tracefile line " << lineno
                                 << ". " << __FILE__ << ":" << __LINE__ << endl;
                            exit(-1);
                        }
                    }
                } else {
                    cerr << "Unexpected format string \"" << line
                         << "\" at tracefile line " << lineno
                         << ". " << __FILE__ << ":" << __LINE__ << endl;
                    exit(-1);                }
            } else {
                //exactPlaces.size() > 1
                cout << "NYI at " << __FILE__ << ":" << __LINE__ << endl;
                exit(-2);
            }
        } else {// XXX: 012.c (wild realloc)
            cout << "Error XX7: " << ti.tpl.domains[dombox.domainID].name << ":"
                 << torName << " at " << showFileLine(fl)
                 << " trying to reallocate never allocated (or allocated by "
                 << "untracked function) handle 0x"
                 << showHandle(usedHandle)
                 << endl;

            return false;
        }
    } else if ("BAD" == sl.chunk[1]) {
        // считываем размер запрашиваемой памяти
        lineno = fetcher->fetchLine(line);

        targetSizeT newSize = string2tsizet(line);

        // считываем результат выполнения операции
        lineno = fetcher->fetchLine(line);

        if("OK" != line) {
            // операция завершилась неудачно.
            // TODO: здесь можно записать то место, где неудачно завершилась операция
            // т.к. в отлаживаемом коде далее может быть попытка использования этого (нулевого)
            // указателя

            // виртуальное считывание BAD 0000000000000000
            lineno = fetcher->fetchLine(line);
            return true;
        }

        // тут имеем успешный захват памяти ("OK")

        lineno = fetcher->fetchLine(line);

        nChunks = splitLine(line, &sl);
        if(2 != nChunks) {
            cerr << "Unexpected format string \"" << line
                 << "\" at tracefile line " << lineno
                 << ". " << __FILE__ << ":" << __LINE__ << endl;
            exit(-1);
        }

        if ("OK" == sl.chunk[0]) {
            targetSizeT newPtr = string2tsizet(sl.chunk[1]);

            hInfo.allocationPlace = fl;
            hInfo.lenght = newSize;
            hInfo.handle = newPtr;
            hInfo.evolution = allocated;
            hInfo.handle_state = validHandle;

            AllocatorEngine(ti, fl, torName, dombox, hInfo);
        } else {
            cout << "NYI at " << __FILE__ << ":" << __LINE__ << endl;
            //dump(&sl);
            cout << "LINE: '" << line << "' at line " << lineno << endl;
            exit(-2);
        }

    } else {
        cerr << "Unexpected format string \"" << line
             << "\" at tracefile line " << lineno
             << ". " << __FILE__ << ":" << __LINE__ << endl;
        exit(-1);
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////

bool transactionAnalyzer(CVLineFetcher *fetcher, traceInfo &ti) {
    string line;
    LineCounter lineno;

    bool incomingTransactions = true;
    while (incomingTransactions) {
        lineno = fetcher->fetchLine(line);

        domainBox funcDom = checkDomainName(line, ti);
        if(!funcDom.isValid) {
            if("ART_TRAP") {
                fetcher->fetchLine(line);
                if("fork()" == line) {
                    string fork_file, fork_line;
                    fetcher->fetchLine(fork_file);
                    fetcher->fetchLine(fork_line);
                    cout << "ART detected fork() usage at " << fork_file <<
                            ":" << fork_line << ", meanwhile fork() is " \
                            "forbidden according to template settings. "
                            "Please, disable application fork() usage. " \
                            "If it isn\'t possible, contact ART author." << endl;
                }
            }
            else {
                cerr << "Unexpected domain name '" << line << "' at line " <<
                        lineno << "." << endl;
            }

            return false;
        }

        fetcher->fetchLine(line);

        string torName = line;

        torType tt = checkTorType(line, funcDom, ti);
        if(tFail == tt) {
            cerr << "Unexpected tor name '" << line << "' in domain '" <<
                    ti.tpl.domains[funcDom.domainID].name << "'." << endl;
            return false;
        }

        handleInfo hInfo;
        FileLine fl = getFileLine(fetcher);
        bool torValue = true;

        if (tOperator == tt) {
            torValue =
                    transactionAnalyzerforOperators(fetcher, ti, fl, torName,
                                                    funcDom);
            if(!torValue) return torValue;
        }
        else if(tAllocator == tt) {
            torValue =
                    transactionAnalyzerforAllocators(fetcher, ti, fl, torName,
                                                     funcDom, hInfo);
            if(!torValue) return torValue;
        }
        else if(tDeallocator == tt) {
            torValue = transactionAnalyzerforDeallocators(fetcher, ti, fl,
                                                          torName, funcDom,
                                                          hInfo);
            if(!torValue) return torValue;
        }
        else if(tReallocator == tt) {
            torValue = transactionAnalyzerforReallocators(fetcher, ti, fl,
                                                          torName, funcDom,
                                                          hInfo);
            if(!torValue) return torValue;
        }
        else {
            cerr << "Unexpected tor type." << endl;
            exit(-1);
        }

        fetcher->peekLine(line);
        if("ART TRACING DONE" == line) incomingTransactions = false;
    }

    return true;
}

bool postTransactionAnalyzer(traceInfo &ti) {
    size_t di, dl, hi, hl;
    bool isFloatDomain;

    for(di = 0, dl = ti.domains.size(); di < dl; di++) {
        isFloatDomain = ti.tpl.domains[di].float_handle;

        for(hi = 0, hl = ti.domains[di].handles.size(); hi < hl; hi++) {
            if(allocated == ti.domains[di].handles[hi].evolution) {
                cout << "Error: resource leak; handle 0x" <<
                        showHandle(ti.domains[di].handles[hi].handle) <<
                        " from '" << ti.tpl.domains[di].name << "'"
                     << " allocated at " << showFileLine(ti.domains[di].handles[hi].allocationPlace)
                     << " not deallocated. Never operated";
                if(isFloatDomain) {
                    if(ti.domains[di].handles[hi].lenght)
                        cout << ". Bytes leaked: " << ti.domains[di].handles[hi].lenght << endl;
                    else
                        cout << ". Bytes leaked: 0, plus indirect leakage for pointer "
                             << "(example: malloc(size), when size is 0)" << endl;
                } else cout << endl;
            } else if(operated == ti.domains[di].handles[hi].evolution) {
                cout << "Error: resource leak; handle 0x" <<
                        showHandle(ti.domains[di].handles[hi].handle) <<
                        " from '" << ti.tpl.domains[di].name << "'"
                     << " allocated at " << showFileLine(ti.domains[di].handles[hi].allocationPlace)
                     << " not deallocated";
                if(isFloatDomain) {
                    if(ti.domains[di].handles[hi].lenght)
                        cout << ". Bytes leaked: " << ti.domains[di].handles[hi].lenght << endl;
                    else
                        cout << ". Bytes leaked: 0, plus indirect leakage for pointer "
                             << "(example: malloc(size), when size is 0)" << endl;
                } else cout << endl;
            } else if(deallocated == ti.domains[di].handles[hi].evolution) {
                // просто пропускаем
            }
            else {
                cerr << "Internal error. Please, report to developers." << endl;
                assert(false);
            }
        }
    }

    return true;
}

bool trace(CVLineFetcher *fetcher) {
    traceInfo ti;
    bool retval;

    retval = readHeader(fetcher, ti);
    if(!retval) {
        cerr << "Unable to read header." << endl;
        return false;
    }

    //	ti.dump();
    //	exit(-2);

    transactionAnalyzer(fetcher, ti);
// probably (looks like) this code now obsolete or always was?:
//    retval = transactionAnalyzer(fetcher, ti);
//    if(!retval) {
//        cerr << "Unable to analyze transactions." << endl;
//        return false;
//    }

    retval = postTransactionAnalyzer(ti);
    if(!retval) {
        cerr << "Unable to analyze postTransactions." << endl;
        return false;
    }

    return true;
}
