import os
from os.path import join

re_env = Environment(CPPPATH = ['#/include', '#.',
                                '/usr/include/freetype2', '/usr/include/local'],
                     CPPDEFINES='TIXML_USE_STL',
                     CPPFLAGS='-Wall -g')

re_subdir = ['audio',
             'input',
             'interface',
             'misc',
             'network',
             'physics',
             'resource',
             'server',
             'session',
             'util',
             'video']
objs = []
for d in re_subdir:
    o = SConscript(os.path.join(d, 'SConscript'), exports=['re_env'])
    objs.append(o)

demo = SConscript(dirs=['reprize'], exports=['re_env', 'objs'])
objs.append(demo)

baks = []
def get_emacs_bak(name_):
    if name_[-1] == '~':
        baks.append(join(root, name_))

dots = []
def get_dotfiles(name_):
    if name_[0] == '.':
        dots.append(join(root, name_))

for root, dirs, files in os.walk('.'):
    map(get_emacs_bak, files)
    map(get_dotfiles, files)

Clean(objs, baks)
