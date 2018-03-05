#!/usr/bin/python2

import re
pattern = re.compile('GLAPI.*emscripten_(\w*)')

for i, line in enumerate(open('gl.c', 'r')):
    for match in re.finditer(pattern, line):
        func_name = match.groups()[0]
        print('  else if (!strcmp(name, "%s")) return emscripten_%s;' % (func_name, func_name))
