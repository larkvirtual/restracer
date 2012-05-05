#ifndef __gen_c_h_
#define __gen_c_h_

#include <fcntl.h>	// open()
#include <iostream>
#include "art_struct.h"
#include "gen_c_body.h" // used by Generator_C_Source_USER_Body_Part()

/////////////////////////////
// ART Structure -> ANSI C //
/////////////////////////////

// Generates copyright, etc...
void	Generator_C_StubInfo(CTemplate &tpl, ofstream &file);

// Generates header (DECLARATION) (autocalled by Generator_C_Header())
void	Generator_C_Header_Declaration(CTemplate &tpl, ofstream &head);

// Generates header (REDEFINITION) (autocalled by Generator_C_Header())
void	Generator_C_Header_Redefenition(CTemplate &tpl, ofstream &head);

// Generates header
void	Generator_C_Header(CTemplate &tpl, ofstream &head);

// Generates source (ART things) (autocalled by Generator_C_Source())
// tplfilename на самом деле не используется. см. (void) в начале реализации
int	Generator_C_Source_ART_Part(CTemplate &tpl, char *tplfilename,
                                ofstream &src);

int     Generator_C_Source_ART_Template_Writer(char *tplfilename,
                                               ofstream &src);

// Generates source (USER things) (autocalled by Generator_C_Source_USER_Part())
// err - поток, куда склыдываются сообщения о ошибках в шаблоне.
// кем конкретно он является, это уж зависит от реализации console/gui
// в console можно и cerr использовать, а в gui - что-то другое можно ;)
int	Generator_C_Source_USER_Body_Part(CTemplate &tpl, CDomain &tmpDom,
                                      CTor &tmpTor, ofstream &src);

// Generates source (USER things) (autocalled by Generator_C_Source())
int	Generator_C_Source_USER_Part(CTemplate &tpl, ofstream &src);

// Generates source
int	Generator_C_Source(CTemplate &tpl, char *tplfilename, ofstream &src);

// Generates generic info
int	Generator_C(CTemplate &tpl, char *tplfilename,
                const char *header_file, const char *source_file);

#endif
