#ifndef GEN_XML_SAX_READ_H_
#define GEN_XML_SAX_READ_H_

#include <libxml++/libxml++.h>
#include "art_struct.h"

typedef enum {
    onDocStart        // событие без тела
    ,onDocEnd         // событие без тела
    ,onElementStart   // событие с телом Element element
    ,onElementEnd     // событие без тела
    ,onChars          // событие с телом string
    ,onComment        // событие с телом string
} evenType;

typedef struct {
    string name, value;
} Tuple;

class Element {
public:
    string name;
    vector <Tuple> pairs;

    string fetchValue(string name);
};

class xml_metachunk {
public:
    evenType event;
    Element element;
    string  str;

#ifdef ART_DEBUG
    void dump(void);
#endif
};

class chunkGetter {
public:
    chunkGetter();
    ~chunkGetter();

    void copyChunks(vector <xml_metachunk> &chunks);
    xml_metachunk getChunk(void);
    xml_metachunk getNonCharsChunk(void);
    xml_metachunk peekPrev(void); // даёт подглядеть в предыдущий блок, вставок/выниманий
    // не происходит
    xml_metachunk peekNext(void); // даёт подглядеть в следующий блок, вставок/выниманий
    // не происходит
    void putChunkBack(void); // передвигает указатель на предыдущий кусок
    void seekUntilNonCharsChunk(void); // проматывает до неChars-блока, но не вынимает его


private:
    vector <xml_metachunk> chunks;
    size_t i, l;
};

class artSaxParser : public xmlpp::SaxParser
{
public:
    artSaxParser();
    virtual ~artSaxParser();

protected:
    //overrides:
    virtual void on_start_document();
    virtual void on_end_document();
    virtual void on_start_element(const Glib::ustring& name,
                                  const AttributeList& properties);
    virtual void on_end_element(const Glib::ustring& name);
    virtual void on_characters(const Glib::ustring& characters);
    virtual void on_comment(const Glib::ustring& text);
    virtual void on_warning(const Glib::ustring& text);
    virtual void on_error(const Glib::ustring& text);
    virtual void on_fatal_error(const Glib::ustring& text);

public:
    vector <xml_metachunk> chunks;
};

int saxparser_templateLoader(CTemplate &tpl, const char* filepath);

#ifdef NOTHING_YET
#include "utils_linefetch.h"

int saxparser_templateLoader(CTemplate &tpl, CVLineFetcher *fetcher);
#endif

#endif /*GEN_XML_SAX_READ_H_*/
