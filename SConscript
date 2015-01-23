source_files = Glob('src/*.cpp')


env = Environment(CPPPATH=['include'])
env['STATIC_AND_SHARED_OBJECTS_ARE_THE_SAME']=1


objs = env.Object(source_files)

env.StaticLibrary('libdnest3', objs)
env.SharedLibrary('libdnest3', objs)


env.Program('main', [objs, 'main.cpp'],
	        LIBS=['dnest3', 'gsl', 'gslcblas', 'boost_thread', 'boost_system'])

env.SharedLibrary('librjobject', objs)

