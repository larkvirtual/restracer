#ifndef _gen_c_body_h__
#define _gen_c_body_h__

#include <iostream>
#include "art_struct.h"

int     Generator_C_USER_Body_Prolog(CTemplate &tpl, CDomain &tmpDom,
                                     CTor &tmpTor, ofstream &src);

int     Generator_C_USER_Body_Validation(CTemplate &tpl, CDomain &tmpDom,
                                         CTor &tmpTor, ofstream &src);

int     Generator_C_USER_Body_Action(CTemplate &tpl, CDomain &tmpDom,
                                     CTor &tmpTor, ofstream &src);

int     Generator_C_USER_Body_Epilog(CTemplate &tpl, CDomain &tmpDom,
                                     CTor &tmpTor, ofstream &src);

#endif
