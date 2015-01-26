import sys

source_files = Glob('src/*.cpp')


env = Environment(CCFLAGS='-fPIC -Ofast -flto -funroll-loops -Wall -Wextra -ansi -pedantic -DNDEBUG',
	              CPPPATH=['include'],
	              LIBPATH=['.', '/usr/local/lib'],
	              )
env['STATIC_AND_SHARED_OBJECTS_ARE_THE_SAME']=1

def print_cmd_line(s, target, src, env):
    """s is the original command line, target and src are lists of target
    and source nodes respectively, and env is the environment."""
    sys.stdout.write(" Making %s...\n"% (' and '.join([str(x) for x in target])))

env['PRINT_CMD_LINE_FUNC'] = print_cmd_line



objs = env.Object(source_files)

env.StaticLibrary('libdnest3', objs)
env.SharedLibrary('libdnest3', objs)

env.StaticLibrary('librjobject', objs)
# env.SharedLibrary('librjobject', objs)

env.Program('main', [objs, 'main.cpp'],
	        LIBS=['rjobject', 'dnest3', 'gsl', 'gslcblas', 'boost_thread', 'boost_system'])



