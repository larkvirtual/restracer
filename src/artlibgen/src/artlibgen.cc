//#include <fstream>
#include <string.h> // strcmp()
#include <stdio.h>  // perror()
#include "gen_c.h"
#include "gen_xml_sax_read.h" // saxparser_initStr2Maps(), saxparser_templateLoader();

void showHelp(void) {
    printf("params:  <template> <instrum|preload> <header> <source> # generate instrumentation or preload files from template\n");
    printf("params:  <template>                   # dumps template (DEBUG VERSION ONLY)\n");
    printf("params:  --version                    # prints version\n");
    printf("example: tpl_generic3.xml instrum restracer.h restracer.c\n");
    printf("example: tpl_generic3.xml preload /dev/null   restracer_preload.c\n");
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
    else if(5 == argc) { /* do nothing right here, will handle after */ }
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

    if(0 == strcmp(argv[2], "instrum")) {
        // Фаза 2. Вызов генератора для генерации встраиваемых файлов по шаблону
        retval = Generator_C(tpl, argv[1], argv[3], argv[4]);
        if(retval) {
            cerr << "Generation error. See err.log for details." << endl;
            cerr << "error code = " << retval << endl;
        }
        else cout << "Files " << argv[3] << ", " << argv[4] << " are generated." << endl;
    } else if(0 == strcmp(argv[2], "preload")){
        retval = Generate_Preload_C(tpl, argv[4]);
        if(retval) {
            cerr << "Preload generation error. See err.log for details." << endl;
            cerr << "error code = " << retval << endl;
        }
        else cout << "Preload file " << argv[4] << " generated." << endl;
    } else {
        fprintf(stderr, "Secord arg ('%s') must be 'instrum' or 'preload'\n", argv[2]);
        return 1;
    }

    return retval;
}
