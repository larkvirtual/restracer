#ifndef CCLOADER_H_
#define CCLOADER_H_

#include <string> // std::string в local_record_t

typedef enum {
    INT = 0 /* int */
    ,DBL    /* double */
    ,CSTR   /* char[] */
    ,SIZET  /* size_t */
    ,CCSTR  /* std::string */
    ,CCBOOL /* bool */
} record_t;

typedef struct {
    void*    address;
    std::string   name; // тут заменить(добавить?) на char*
    record_t type;
} local_record_t;

int ccload(xml_metachunk chunk, local_record_t *records, size_t nRecords);

#endif /* CCLOADER_H_ */
