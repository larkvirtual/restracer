#include "art_defs.h"
#ifdef ART_DEBUG
#include "art_struct.h"
#include "art_dump_tool.h" // bool2yn()

#ifdef _WIN32
#include <stddef.h> /* size_t */
#else
#include <unistd.h> /* size_t, ssize_t */
#endif

#include <iostream>

void CExpr::dump(size_t ntabs){
    size_t t;
    string tabs;
    for(t = 0; t < ntabs; t++) tabs += '\t';

    cout << tabs << "oper = " << oper << endl;
    cout << tabs << "type = " << type << endl;
    cout << tabs << "value = " << value << endl;
}

void CArg::dump(size_t ntabs){
    size_t t;
    string tabs;
    for(t = 0; t < ntabs; t++) tabs += '\t';

    cout << tabs << "type = " << type << endl;
    cout << tabs << "name = " << name << endl;
}

void CTor::dump(size_t ntabs){
    size_t t, size;
    string tabs;
    for(t = 0; t < ntabs; t++) tabs += '\t';

    cout << tabs << "name = " << name << endl;
    for(t = 0, size = args.size(); t < size; t++){
        cout << tabs << "(CArg)args[" << t << "]:" << endl;
        args[t].dump(ntabs + 1);
    }
    cout << tabs << "float_arg_n1 = " << float_arg_n1 << endl;
    cout << tabs << "float_arg_n2 = " << float_arg_n2 << endl;
    cout << tabs << "validateDom_before:" << endl;
    for(t = 0, size = validateDom_before.size(); t < size; t++){
        cout << tabs << "[" << t << "] ";
        cout << validateDom_before[t] << endl;
    }
    if(!is_handle_arg_out){
        cout << tabs << "handle_arg_out = n/a" << endl;
    }
    else    {
        cout << tabs << "handle_arg_out = " <<  handle_arg_out << endl;
    }
    cout << tabs << "badRetCode:" << endl;
    badRetCode.dump(ntabs + 1);
    cout << tabs << "cpp_alias: " << cpp_alias << endl;
}

void CDomain::dump(size_t ntabs){
    size_t t, size;
    string tabs;
    for(t = 0; t < ntabs; t++) tabs += '\t';

    cout << tabs << "name = " << handle << endl;
    cout << tabs << "float_handle = " << bool2yn(float_handle) << endl;
    cout << tabs << "handle = " << handle << endl;
    cout << tabs << "(CExpr)bad_handle:" << endl;
    bad_handle.dump(ntabs + 1);
    cout << tabs << "includes:" << endl;
    for(t = 0, size = includes.size(); t < size; t++){
        cout << tabs << "[" << t << "] ";
        cout << includes[t] << endl;
    }
    for(t = 0, size = allocators.size(); t < size; t++){
        cout << tabs << "allocators[" << t << "]:" << endl;
        allocators[t].dump(ntabs + 1);
    }
    for(t = 0, size = deallocators.size(); t < size; t++){
        cout << tabs << "deallocators[" << t << "]:" << endl;
        deallocators[t].dump(ntabs + 1);
    }
    for(t = 0, size = reallocators.size(); t < size; t++){
        cout << tabs << "reallocators[" << t << "]:" << endl;
        reallocators[t].dump(ntabs + 1);
    }
    for(t = 0, size = operators.size(); t < size; t++){
        cout << tabs << "operators[" << t << "]:" << endl;
        operators[t].dump(ntabs + 1);
    }
}

void CTemplate::dump(void){
    size_t t, size;
    cout << "name = " << name << endl;
    cout << "art_prefix = " << prefix << endl;
    cout << "errlogmode = " << errlogmode << endl;
    cout << "trap_on_io_err = " << bool2yn(trap_on_io_err) << endl;
    cout << "remote_mode = " << bool2yn(remote_mode) << endl;
    cout << "force_flush = " << bool2yn(force_flush) << endl;
    cout << "multithreaded = " << bool2yn(multithreaded) << endl;
    cout << "threading = " << threading << endl;
    cout << "compiler_type = " << compiler_type << endl;
    cout << "trace_target = " << trace_target << endl;
    for(t = 0, size = domains.size(); t < size; t++){
        cout << "domains[" << t << "]:" << endl;
        domains[t].dump(1);
    }
    for(t = 0, size = specialTors.size(); t < size; t++){
        cout << "specialTor[" << t << "]:" << endl;
        specialTors[t].dump(1);
    }
}
#endif
