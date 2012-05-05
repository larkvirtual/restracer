#include <fstream>

#include "gen_c_body.h"
#include "gen.h"

// TODO: В шаблон добавить указание использовать табуляцию или пробелы и их количество

int     Generator_C_USER_Body_Prolog(CTemplate &tpl, CDomain &tmpDom,
                                     CTor &tmpTor, ofstream &src){
    size_t	arg_idx, tmpTor_args_size /* , art_trace_n_args */;
    CArg	tmpArg;

    if(tmpTor.cpp_alias != ""){
        // Игнорировать в С++
        return(0);
    }

    tmpArg = tmpTor.args[0];
    src << tmpArg.type << "\t" << tpl.prefix << tmpTor.name << "(";
    tmpTor_args_size = tmpTor.args.size();
    for(arg_idx = 1; arg_idx < tmpTor_args_size; arg_idx++){
        tmpArg = tmpTor.args[arg_idx];
        src << tmpArg.type << " " << tmpArg.name << ", ";
    }
    src << "const char *" << tpl.prefix << "file, size_t " <<
           tpl.prefix << "line){" << endl;

    if(tmpTor.args[0].type != "void"){
        src << "\t" << tmpTor.args[0].type << " " <<
               tpl.prefix << "stored_retcode;" << endl;
    }
    if(tmpTor.is_handle_arg_out){
        src << "\t" << tmpDom.handle << " " <<
               tpl.prefix << "stored_handle;" << endl;
    }
    if(tpl.multithreaded){
        if(tpl.threading == "posix"){
            src << "\tint retval;" << endl;
        }
        else if(tpl.threading == "win32"){
            src << "\tDWORD dwWaitObject; BOOL bReleaseMutex;" << endl;
        }
    }
    src << endl;

    if(tpl.multithreaded){
        if(tpl.threading == "posix"){
            src << "\tretval = pthread_mutex_lock(&art_mutex);" << endl;
            src << "\tif(retval){arterrlog(\"pthread_mutex_lock()\", __FILE__, __LINE__); exit(-2);}" << endl;
            if(tpl.art_start_selfinit) {
                src << "\tif(0 == art_start_initialized) {" << endl
                    << "\t\tpthread_mutex_unlock(&art_mutex);" << endl
                    << "\t\tart_start(\"autoinitialized by "
                    << tmpTor.name << "() at __FILE__:__LINE__\"); }"
                    << endl << endl;
                src << "\tretval = pthread_mutex_lock(&art_mutex);" << endl;
                src << "\tif(retval){arterrlog(\"pthread_mutex_lock()\", __FILE__, __LINE__); exit(-2);}" << endl;
            }
        }
        else if(tpl.threading == "win32"){
            src << "\tdwWaitObject = WaitForSingleObject(art_mutex, INFINITE);" << endl;
            src << "\tif(dwWaitObject == WAIT_FAILED){arterrlog(\"WaitForSingleObject()\", __FILE__, __LINE__); exit(-2);}" << endl;
            if(tpl.art_start_selfinit) {
                src << "#error NYI! art_start_selfinit for win32-threading model" << endl;
            }
        }
    } else if(tpl.art_start_selfinit){
        src << "\tif(0 == art_start_initialized) {" << endl
            << "\t\tart_start(\"autoinitialized by "
            << tmpTor.name << "() at __FILE__:__LINE__\"); }"
            << endl << endl;
    }

    // PROLOG. FORMAT: DOMAIN TOR'NAME FILE LINE
#ifdef ART_DEBUG_INSERT_DEVEL_COMMENT
    src << "\t/* PROLOG */" << endl;
#endif
    src << "\tart_trace(1, \"" << tmpDom.name << "\");" << endl;
    src << "\tart_trace(1, \"" << tmpTor.name << "\");" << endl;
    src << "\tart_trace(1, " << tpl.prefix << "file);" << endl;
    src << "\tart_trace(1, artsize_t2str(" << tpl.prefix << "line));" << endl;

    return (0);
}

int     Generator_C_USER_Body_Validation(CTemplate &tpl, CDomain &tmpDom,
                                         CTor &tmpTor, ofstream &src){
    size_t	domid,	// Domain ID (just index in vector of Domains)
            t,	// arg counter
            tmpTor_validateDom_before_size;
    string	domname;
    CArg	tmpArg;

    if(tmpTor.cpp_alias != ""){
        // Игнорировать в С++
        return(0);
    }

    // VALIDATION-IN.
#ifdef ART_DEBUG_INSERT_DEVEL_COMMENT
    src << "\t/* VALIDATION-IN */" << endl;
#endif
    tmpTor_validateDom_before_size = tmpTor.validateDom_before.size();
    for(t = 1/* coz 0 is retval, ignoring */; t < tmpTor_validateDom_before_size;
        t++){
        domname = tmpTor.validateDom_before[t];
        if(domname != ""){
            domid = Domaintoid(tpl, domname);
            if(((size_t)-1) != domid) { // found ;)
                CExpr	validationExpr;
                string	type;
                validationExpr = tpl.domains[domid].bad_handle;
                type = tpl.domains[domid].handle;

                src << "\tif(" << tmpTor.args[t].name << " "
                    << validationExpr.oper << " "
                    << validationExpr.value << "){" << endl
                    << "\t\tart_trace(3, \"" << domname << "\", \"BAD\", artuint64tostr1("
       #ifdef ART_FORCE_CAST_FOR_ARTUINT64TOSTR
                    << "(artuint64)"
       #endif
                    << tmpTor.args[t].name << "));" << endl
                    << "\t}" << endl;
                src << "\telse\t{" << endl
                    << "\t\tart_trace(3, \"" << domname << "\", \"OK\", artuint64tostr1("
       #ifdef ART_FORCE_CAST_FOR_ARTUINT64TOSTR
                    << "(artuint64)"
       #endif
                    << tmpTor.args[t].name << "));" << endl
                    << "\t}" << endl;

            }
            else {/* фиктивный домен */}
        }
        // else domname == "", it's ok
    }

    // float_arg_n1; float_arg_n2;
    if(tmpDom.float_handle){
        if(tmpTor.float_arg_n1){
            src << "\tart_trace(1, artuint64tostr1("
       #ifdef ART_FORCE_CAST_FOR_ARTUINT64TOSTR
                << "(artuint64)"
       #endif
                << tmpTor.args[tmpTor.float_arg_n1].name << "));" << endl;

            if(tmpTor.float_arg_n2){
                src << "\tart_trace(1, artuint64tostr1("
       #ifdef ART_FORCE_CAST_FOR_ARTUINT64TOSTR
                    << "(artuint64)"
       #endif
                    << tmpTor.args[tmpTor.float_arg_n2].name
                    << "));" << endl;
            }
        }
    }
    else {/* просто не имеет значения */}
    return (0);
}

int     Generator_C_USER_Body_Action(CTemplate &tpl, CDomain &tmpDom,
                                     CTor &tmpTor, ofstream &src){
    size_t	arg_idx, tmpTor_args_size;
    CArg	tmpArg;

    (void)tmpDom;

    if(tmpTor.cpp_alias != ""){
        // Игнорировать в С++
        return(0);
    }

    // action
    // Если у функции нет возвращаемого значения, то просто
    // вызываем оригинальную функцию, в противном случае сохраняем
    // это значение
#ifdef ART_DEBUG_INSERT_DEVEL_COMMENT
    src << "\t/* ACTION */" << endl;
#endif

    //#ifdef ART_DEBUG
    //cout << "tmpTor.name = " << tmpTor.name << endl;
    //cout << "tmpTor.args[0].type = " << tmpTor.args[0].type << endl;
    //#endif

    if(tmpTor.args[0].type != "void"){
        src << "\t" << tpl.prefix << "stored_retcode = ";
    }
    if(tmpTor.is_handle_arg_out && tmpTor.handle_arg_out == 0 && tmpTor.args[0].type != "void"){
        if(tmpTor.args[0].type != "void"){
            // "\t" already printed, nothing to do
        }
        else	{
            // "\t" don't printed, so print
            src << "\t";
        }
        src << tpl.prefix << "stored_handle = ";
    }
    else {
        // случай tmpTor.handle_arg_out == 0 && tmpTor.args[0].type == "void"
        // игнорируем, т.к. возвращаемого значения нет (оно void).
    }

    if(tmpTor.args[0].type == "void"){
        // we should print it
        src << "\t";
    }

    // action(args)
    src << tmpTor.name << "(";
    tmpTor_args_size = tmpTor.args.size();
    for(arg_idx = 1; arg_idx < tmpTor_args_size - 1; arg_idx++){
        tmpArg = tmpTor.args[arg_idx];
        src << tmpArg.name << ", ";
    }
    tmpArg = tmpTor.args[arg_idx];
    src << tmpArg.name << ");" << endl;

    if(tmpTor.is_handle_arg_out && tmpTor.handle_arg_out > 0){
        tmpArg = tmpTor.args[tmpTor.handle_arg_out];
        src << "\t" << tpl.prefix << "stored_handle = " <<
               tmpArg.name << ";" << endl;
    }

    return (0);
}

int     Generator_C_USER_Body_Epilog(CTemplate &tpl, CDomain &tmpDom,
                                     CTor &tmpTor, ofstream &src){
    // not used CArg	tmpArg;

    if(tmpTor.cpp_alias != ""){
        // Игнорировать в С++
        return(0);
    }

    // epilog
#ifdef ART_DEBUG_INSERT_DEVEL_COMMENT
    src << "\t/* EPILOG */" << endl;
#endif

    //#ifdef ART_DEBUG
    //cout << "tmpTor.name = " << tmpTor.name << endl;
    //cout << "tmpTor.badRetCode.type = " << tmpTor.badRetCode.type << endl;
    //#endif

    // retcode processing
    if(tmpTor.badRetCode.type == "void"){
        src << "\tart_trace(1, \"void\");" << endl;
    }
    else if(tmpTor.badRetCode.type == ""){
        src << "\tart_trace(1, " << "\"-\");" << endl;
    }
    else	{
        src << "\tif(" << tpl.prefix << "stored_retcode "
            << tmpTor.badRetCode.oper << " " << tmpTor.badRetCode.value
            << "){" << endl
            << "\t\tart_trace(1, " << "\"BAD\");" << endl
            << "\t}" << endl
            << "\telse\t{" << endl
            << "\t\tart_trace(1, " << "\"OK\");" << endl
            << "\t}" << endl;
    }

    // TODO: интегрировать ART_FORCE_CAST_FOR_ARTUINT64TOSTR в код навсегда

    // HANDLE-OUT processing
    // пологаем что HANDLE-OUT из того же домена
    if(tmpTor.is_handle_arg_out && tmpTor.handle_arg_out == 0){
        src << "\tif(" << tpl.prefix << "stored_handle "
            << tmpDom.bad_handle.oper << " " << tmpDom.bad_handle.value
            << "){" << endl
            << "\t\tart_trace(2, " << "\"BAD\", artuint64tostr1("
       #ifdef ART_FORCE_CAST_FOR_ARTUINT64TOSTR
            << "(artuint64)"
       #endif
            << tpl.prefix << "stored_handle" << "));" << endl
            << "\t}" << endl
            << "\telse\t{" << endl
            << "\t\tart_trace(2, " << "\"OK\", artuint64tostr1("
       #ifdef ART_FORCE_CAST_FOR_ARTUINT64TOSTR
            << "(artuint64)"
       #endif
            << tpl.prefix << "stored_handle" << "));" << endl
            << "\t}" << endl;
    }
    else if(!tmpTor.is_handle_arg_out){
        src << "\tart_trace(1, \"-\");" << endl;
    }
    else	{
        src << "\tif(" << tmpTor.args[tmpTor.handle_arg_out].name
            << tmpDom.bad_handle.oper << tmpDom.bad_handle.value << "){"<<endl
            << "\t\tart_trace(2, " << "\"BAD\", artuint64tostr1("
       #ifdef ART_FORCE_CAST_FOR_ARTUINT64TOSTR
            << "(artuint64)"
       #endif
            << tmpTor.args[tmpTor.handle_arg_out].name << "));" << endl
            << "\t}" << endl
            << "\telse\t{" << endl
            << "\t\tart_trace(2, " << "\"OK\", artuint64tostr1("
       #ifdef ART_FORCE_CAST_FOR_ARTUINT64TOSTR
            << "(artuint64)"
       #endif
            << tmpTor.args[tmpTor.handle_arg_out].name << "));" << endl
            << "\t}" << endl;
    }

#ifdef ART_PUT_EXTRA_NL_IN_TRACEFILE
    if(!tpl.remote_mode){
        src << "\tfwrite(\"\\n\", 1, 1, art_tracefile);" << endl;
    }
    else	{src << "\tsend(art_socket, \"\\n\", 1, 0);" << endl;}
#endif

    if(tpl.multithreaded){
        if(tpl.threading == "posix"){
            src << "\tretval = pthread_mutex_unlock(&art_mutex);" << endl;
            src << "\tif(retval){arterrlog(\"pthread_mutex_unlock()\", __FILE__, __LINE__); exit(-2);}" << endl;
        }
        else if(tpl.threading == "win32"){
            src << "\tbReleaseMutex = ReleaseMutex(art_mutex);" << endl;
            src << "\tif(!bReleaseMutex){arterrlog(\"ReleaseMutex()\", __FILE__, __LINE__); exit(-2);}" << endl;
        }
    }
    if(tmpTor.args[0].type != "void"){
        src << "\t" << "return (" << tpl.prefix << "stored_retcode);" << endl;
    }
    src << "}" << endl << endl;

    return (0);
}
