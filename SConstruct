def CheckPKGConfig(context, version):
    context.Message('Checking for pkg-config... ')
    ret = context.TryAction('pkg-config --atleast-pkgconfig-version=%s' % version)[0]
    context.Result(ret)
    return ret

def CheckPKG(context, name):
    context.Message('Checking for %s...' % name)
    ret = context.TryAction('pkg-config --exists \'%s\'' % name)[0]
    context.Result(ret)
    return ret

# class Options is for Nexenta/Hardy. scons from Hardy is outdated.
opts = Options('custom.py')
#opts = Variables('custom.py')

#opts.Add('OS', 'Set to current OS: linux, freebsd, nexenta.', 0)
opts.Add('PROFILE', 'Set to 1 to build profiled version', 0)
opts.Add('RELEASE', 'Set to 1 to build release', 0)
opts.Add('STATIC', 'Set to 1 to build static executable', 0)
opts.Add('ARCH', 'Set to target architecture', 'generic')

env = Environment(variables = opts)

Help(opts.GenerateHelpText(env))

conf = Configure(env, custom_tests = {'CheckPKGConfig' : CheckPKGConfig,
                                       'CheckPKG' : CheckPKG})

if not conf.CheckPKGConfig('0.18.1'):
    print 'pkg-config >= 0.18.1 not found.'
    Exit(1)

if not conf.CheckPKG('libxml++-2.6 >= 2.5.0'):
    print 'libxml++-2.6 >= 2.5.0 not found.'
    Exit(1)

env = conf.Finish()

#OS = ARGUMENTS.get('OS', 'linux')
ARCH = ARGUMENTS.get('ARCH', 'generic')
RELEASE = ARGUMENTS.get('RELEASE', 0)
STATIC = ARGUMENTS.get('STATIC', 0)
PROFILE = ARGUMENTS.get('PROFILE', 0)

env.ParseConfig('pkg-config --cflags --libs libxml++-2.6')

#
CANDIDATE_ART_FORCE_CAST_FOR_ARTUINT64TOSTR = \
   ARGUMENTS.get('ART_FORCE_CAST_FOR_ARTUINT64TOSTR', 1)

ART_DEBUG_INSERT_DEVEL_COMMENT = \
   ARGUMENTS.get('ART_DEBUG_INSERT_DEVEL_COMMENT', 0)
#

# -Wno-deprecated-declarations to prevent libxml++-2.6 warnings
env.Append(CCFLAGS = '-Wno-deprecated-declarations')

if int(PROFILE):
    env.Append(CCFLAGS = '-pg')
    env.Append(LINKFLAGS = '-pg')

if int(RELEASE):
    if int(PROFILE):
        env.Append(CCFLAGS = '-O3')
        env.Append(CCFLAGS = '-pipe')
    else:
        env.Append(CCFLAGS = '-O3')
        env.Append(CCFLAGS = '--omit-frame-pointer')
        env.Append(CCFLAGS = '-pipe')

    if ARCH != "generic":
        env.Append(CCFLAGS = '-march=' + ARCH)
    else:
        env.Append(CCFLAGS = '-mtune=generic')

else:
    env.Append(CCFLAGS = '-g')
    env.Append(CCFLAGS = '-ggdb')
    env.Append(CCFLAGS = '-Wall')
    env.Append(CCFLAGS = '-W')
    env.Append(CCFLAGS = '-Wextra')
    env.Append(CCFLAGS = '-Werror')
    env.Append(CCFLAGS = '-O0')
    env.Append(CCFLAGS = '-DART_DEBUG')
# no -pedantic, as long long is used, but it doesn't exist in ISO C++ 1998

if int(STATIC):
    env.Append(LINKFLAGS = '-static')

#
if int(CANDIDATE_ART_FORCE_CAST_FOR_ARTUINT64TOSTR):
    env.Append(CCFLAGS = '-DART_FORCE_CAST_FOR_ARTUINT64TOSTR')

if int(ART_DEBUG_INSERT_DEVEL_COMMENT):
    env.Append(CCFLAGS = '-DART_DEBUG_INSERT_DEVEL_COMMENT')
#

Export('env')

env.SConscript('libs/liblinefetch/SConscript')
env.SConscript('libs/libtplreader/SConscript')
env.SConscript('artlibgen/src/SConscript')
env.SConscript('artrepgen/SConscript')
#env.SConscript('artrepgen/SConscript', 'OS')

SConsignFile("signatures");
