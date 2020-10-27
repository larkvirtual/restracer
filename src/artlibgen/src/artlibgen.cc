//#include <fstream>
#include <string.h> // strcmp()
#include <stdio.h>  // perror()
#include "gen_c.h"
#include "gen_xml_sax_read.h" // saxparser_initStr2Maps(), saxparser_templateLoader();

void showHelp(void) {
    printf("params:  <template> <header> <source> # generate art.h art.c from template file\n");
    printf("params:  <template>                   # dumps template (DEBUG VERSION ONLY)\n");
    printf("params:  --version                    # prints version\n");
    printf("example: tpl_generic3.xml art.h art.c\n");
    printf("example: tpl_generic3.xml\n");
}

int main(int argc, char **argv) {
    bool dumpOnlyAndExit = false;
    if(2 == argc) {
        if(0 == strcmp(argv[1], "--version")) {
            printf("ART v%s%s%s\n", ART_VERSION_BRANCH, ART_VERSION_SUB,
                   ART_VERSION_EXTRA);
            return 0;
        }
        else dumpOnlyAndExit = true;
    }
    else if(4 == argc) { /* просто ничего не делаем, обработка этой ситуации дальше */ }
    else { showHelp(); return 0; }

    // Фаза 1. Загрузка шаблона в структуру CTemplate
    // Тут раньше было использование cloader'а + своего разборщика
    // Теперь используется libxml++
    CTemplate tpl;

    int retval = saxparser_templateLoader(tpl, argv[1]);
    if(-1 == retval) {
        perror("Unable to parse template file!\n");
        return retval;
    }

    if(dumpOnlyAndExit) {
#ifdef ART_DEBUG
        tpl.dump();
#endif
        return 0;
    }

    // Фаза 2. Вызов генератора для генерации встраиваемых файлов по шаблону
    retval = Generator_C(tpl, argv[1], argv[2], argv[3]);
    if(retval) {
        cerr << "Generation error. See err.log for details." << endl;
        cerr << "error code = " << retval << endl;
    }
    else cout << "Files " << argv[2] << ", " << argv[3] << " are generated." << endl;

    return retval;
}
